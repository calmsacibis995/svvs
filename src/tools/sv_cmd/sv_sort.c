#ifdef	UNDEF
#ident	"@(#)sv_sort.c	1.10"
#endif

/*LINTLIBRARY*/
#include <fcntl.h>

/*
** Definitions and initializations used in sort.c
*/

#define	DESCRIPTION	0
#define	FUNCTION	1
#define	UNASSIGNED	2
#define	MESSAGE		4
#define	ERROR		5


struct	sort {
	struct	sort	*next;
	struct	sort	**children;
	char			*lineno;
	int				nchild;
	char			type;
	char			supp;
	long			address;
};

struct	sort	root={
	(struct sort *)0,
	(struct sort **)0,
	"",
	-1,
	UNASSIGNED,
	1,
	-1,
};

struct	class {
	char	*key;
	int	type;
};

struct	class	types[] = {
	{	"SEVERR",ERROR	},
	{	"ERROR",ERROR	},
	{	"WARNING",MESSAGE	},
	{	"MESSAGE",MESSAGE	},
	{	"UNSUPPORTED",MESSAGE	},
	{	"No Match",UNASSIGNED		},
};

#define	NKEYS	((sizeof(types)/sizeof(struct class))-1)

int	errfound = 0;		/*Flag indicating that an error was found*/
static	char	outbuff[512];

extern	void	exit(),free();
extern	char	*malloc(),*realloc(),*getfield(),*nxtok(),*strcpy();
extern	int	whitesp();
extern	long	lseek(),svseek();
struct	sort	*addkid(),*testchild(),*findnumber();
long	getspecial();
static	int	fgetln();

/*
** End of initializations and definitions.
*/

int	sort(journ,tmp)
int	journ,tmp;
{
	long	pos;
	int	type;
	char	line[512],number[512];
	int		pid,sts;

	errfound = 0;
	if(pid = fork()){
		if(wait(&sts) == -1){
			return(-1);
		}
		if(sts){
			if(((sts >> 8) & 0377) == 1){
				errfound = 1;
				return(0);
			}
			return(-1);
		}
		return(0);
	}
	pos = getspecial(tmp,(long)0,line);
	while(pos >= 0){
		type = classify(line);
		getnumber(number,line,type);
		place(number,pos,type);
		pos = getspecial(tmp,pos,line);
	}
	if(svseek(journ,(long)0,2) < (long)0){
		foutf(2,"Error seeking to end of output file.\n");
		exit(2);
	}
	dump(journ,tmp,root.next);
	release(root.next);
	root.next = (struct sort *)0;
	exit(errfound);
}

place(number,line,type)
char	*number;
long	line;
int		type;
{
	struct	sort	*q,*p;
	char	field[512];

	number = getfield(field,number);
	if(!root.next){
/*
* lint will complain about alignment problems, malloc returns a
* generic ptr
*/ 
		root.next = (struct sort *)malloc(sizeof(struct sort));
		if(!root.next){
			foutf(2,"Could not allocate memory, verify failed\n");
			exit(-1);
		}
		p = root.next;
		p->lineno = malloc(sizeof(char));
		if(!p->lineno){
			foutf(2,"Could not allocate memory, verify failed\n");
			exit(-1);
		}
		*p->lineno = (char)0;
		p->next = (struct sort *)0;
		p->children = (struct sort **)0;
		p->nchild = -1;
		p->supp = 1;
		p->address = (long)-1;
	}
	if(*field){
		enter(root.next,field,number,type,line);
	}
 	else {
		q = p = &root;
		while(p->next && !(*p->next->lineno)){
			q = p;
			p = p->next;
		}
		insert(q,field,type,line);
	}
}

enter(node,field,number,type,line)
struct	sort	*node;
char	*field,*number;
int		type;
long	line;
{
	struct	sort	*top,*p;
	char	child[30],*lnum;

	top = node;
	number = getfield(child,number);
	if(!node->lineno){
		node->lineno = malloc(((strlen(field)+1) * sizeof(char)));
		if(!node->lineno){
			foutf(2,"Could not allocate memory, verify failed\n");
			exit(-1);
		}
		(void)strcpy(node->lineno,field);
		node->type = type;
		node->address = line;
		node->supp = 0;
		if(!*child){
			return;
		}
	}
	else if(!(node = findnumber(top,field))){
		if(!(number = getfield(child,number))){
			append(top,field,type,line);	/*No children, append message*/
			return;
		}
/*
** Assumption:  Child cannot get a message out before the parent creates
** 				a test case
*/
		foutf(2,"Child without parent test case.\n");
		return;
	}
	if(!*child){ 
/*
** No further children, put this message at the end of the current test
** case and return.
*/
		while(node && !strcmp(field,node->lineno)){
			p = node;
			node = node->next;
		}
		insert(p,field,type,line);
		return;
	}
/*
** There are children, find the testing node from which the children of
** this test case are hanging, and use the specified child
*/
	lnum = node->lineno;
	while(node && !strcmp(node->lineno,lnum) && node->type)
		node = node->next;
	if(!node || strcmp(node->lineno,lnum)){
/*
** assumption:	Children must be preceded by a testing line if we got
**				here.  If they are not, issue error and return.
*/
		foutf(2,"Orphan\n");
		return;
	}
	if(number = getfield(field,number)){
/*
** Still need to do a child, recurse.
*/
		enter(testchild(node,child),field,number,type,line);
		return;
	}
	foutf(2,"Child without subsequent test case.\n");
	return;
}

struct	sort	*testchild(node,child)
struct	sort	*node;
char	*child;
{
	int	kid;

	kid = childno(child);
	if(kid > node->nchild){
		morekids(node,kid);
		return(addkid(node,kid));
	}
	else{
		if(node->children[kid]){
			return(node->children[kid]);
		}
		else{
			return(addkid(node,kid));
		}
	}
}

dump(journ,tmp,tree)
struct	sort	*tree;
int	journ,tmp;
{
	extern	int	lopt;
	int	i;

	if(!lopt)
		mark(tree);
	while(tree){
		if(!tree->supp)
			extract(journ,tmp,tree->address);
		for(i=0; i <= tree->nchild; ++i){
			if(lopt || searchchld(tree->children[i])){
				(void)sprintf(outbuff,"BegChild %d\n",i+1);
				foutf(journ,"%s",outbuff);
				dump(journ,tmp,tree->children[i]);
				(void)sprintf(outbuff,"EndChild %d\n",i+1);
				foutf(journ,"%s",outbuff);
			}
		}
		tree=tree->next;
	}
}

mark(tree)
struct	sort	*tree;
{
	while(tree){
		tree->supp = !(msgbelow(tree) || (tree->type == FUNCTION));
		tree = tree->next;
	}
}

int	msgbelow(node)
struct	sort	*node;
{
	struct	sort	*top;

	top = node;
	node = node->next;
	while(node &&
	  ((node->type==MESSAGE) || (depth(node->lineno)>depth(top->lineno)))){
		if(node->type == MESSAGE){
			return(1);
		}
		node = node->next;
	}
	return(msginchld(top));
}

int	msginchld(node)
struct	sort	*node;
{
	int	i;

	if(node->type == MESSAGE)
		return(1);
	for(i = 0; i <= node->nchild; ++i){
		if(searchchld(node->children[i]))
			return(1);
	}
	return(0);
}

int	searchchld(node)
struct	sort	*node;
{
	while(node){
		if(msginchld(node))
			return(1);
		node = node->next;
	}
	return(0);
}

extract(to,from,pos)
int		from,to;
long	pos;
{
	char	line[512];

	if(pos < (long)0){	/*In the case of a dummy sort node, pos=-1*/
		return;
	}
	if(svseek(from,pos,0) < (long)0){
		foutf(2,"Error seeking on input file.\n");
		return;
	}
	if(!fgetln(from,line)){
		foutf(2,"Unexpected EOF encountered\n");
		return;
	}
	testcase(line);		/*Strip out all child info.*/
	foutf(to,"%s\n",line);
	while(fgetln(from,line) && *line == '\t'){
		foutf(to,"%s\n",line);
	}
}

testcase(str)
char	*str;
{
	char	*hold,*p;

	hold = p = str;
	if((*p > '9') || (*p < '0'))
		return;
	while(!whitesp(*p)){
		if(*p == '>')
			hold = p += 2;
		else
			++p;
	}
	while(*str = *hold){
		++str;
		++hold;
	}
}

release(tree)
struct	sort	*tree;
{
	struct	sort	*t;
	int		i;

	while(tree){
		for(i = 0; i <= tree->nchild; ++i){
			release(tree->children[i]);
		}
		if(tree->children){
			free(tree->children);
		}
		if(tree->lineno){
			free(tree->lineno);
		}
		t = tree;
		tree = tree->next;
		free(t);
	}
}

/*
** If line begins with:
** SEVERR	-|
** UNSUPPORTED	 |
** ERROR	 |- Return MESSAGE
** WARNING	 |
** MESSAGE	_|
**
** A number followed by 'Function:' - Return FUNCTION
**
** A number followed by anything else - Return DESCRIPTION (=0)
**
** Anything else UNASSIGNED
*/

int	classify(line)
char	*line;
{
	char	token[512];
	int	i;

	if(!*line)
		return(UNASSIGNED);
	if((*line < ('9'+1)) && (*line > ('0'-1))){
		line = nxtok(token,line);
		line = nxtok(token,line);
		if(strcmp(token,"Function:")){
			return(DESCRIPTION);
		}
		return(FUNCTION);
	}
	line = nxtok(token,line);
	i = 0;
	while(i < NKEYS){
		if(!strcmp(token,types[i].key))
			break;
		++i;
	}
	if(types[i].type == ERROR){
		errfound = 1;
		return(MESSAGE);
	}
	return(types[i].type);
}

/*
** If type is MESSAGE number is the second token.
** If type is FUNCTION number is the first token.
** If type is DESCRIPTION number is the first token.
** If type is UNASSIGNED return, no number defined.
*/

getnumber(output,input,type)
char	*output,*input;
int	type;
{
	char	*p;

	switch(type){
		case FUNCTION:
		case DESCRIPTION:
			(void)nxtok(output,input);
			return;
		case MESSAGE:
			input = nxtok(output,input);
			(void)nxtok(output,input);
			output[strlen(output)-2]=0;/*Remove parentheses*/
			p = output + 1;		/*from the end and the*/
			while(*(output++) = *(p++));/*beginning*/
			return;
	}
}

/*
** Read in lines until one is found that does not have a tab on the front.
** If end of file, return 0.
*/

long	getspecial(tmp,pos,line)
int	tmp;
long	pos;
char	*line;
{
	long	ret=0;
	static	int	first=1;

	if((ret = svseek(tmp,pos,0)) < (long)0){
		foutf(2,"Error seeking on input file.\n");
		return(-1);
	}
	if(!fgetln(tmp,line)){
		first = 1;
		return(-1);
	}
	if(!first){
		ret = svseek(tmp,(long)0,1);
		if(!fgetln(tmp,line)){
			first = 1;
			return(-1);
		}
	}
	while(!nonblank(line) || *line == '\t'){
		ret = svseek(tmp,(long)0,1);
		if(!fgetln(tmp,line)){
			first = 1;
			return(-1);
		}
	}
	first = 0;
	return(ret);
}

/*
** If  next field is a process number, copy the process number not
** including the angle brackets and return a pointer to the next field.
** otherwise copy the section number up to the process number.
*/

char	*getfield(out,in)
char	*out,*in;
{
	if(!in){
		*out = 0;
		return((char *)0);
	}
	if(!*(in)){
		*out = 0;
		return((char *)0);
	}
	if(*in != '<'){
		while(*in && (*in != '<')){
			*(out++) = *(in++);
		}
		*out = 0;
	}
	else {
		++in;
		while(*in && (*in != '>')){
			*(out++) = *(in++);
		}
		++in;
		*out = 0;
	}
	return(in);
}

int	childno(str)
char	*str;
{
	int	ret;

	++str;
	(void)sscanf(str,"%d",&ret);
	return(ret-1);
}

morekids(node,kid)
struct	sort	*node;
int		kid;
{

	if(!node->children){
/*
* lint will complain about alignment problems, malloc returns a
* generic ptr
*/ 
		node->children = (struct sort **)malloc(((kid+1)*sizeof(struct sort *)));
		if(!node->children){
			foutf(2,"Could not allocate memory, verify failed\n");
			exit(-1);
		}
		for(++node->nchild; node->nchild < kid; ++node->nchild)
			node->children[node->nchild]=(struct sort *)0;
		node->children[node->nchild]=(struct sort *)0;
	}
	if(kid > node->nchild){
/*
* lint will complain about alignment problems, malloc returns a
* generic ptr
*/ 
		node->children = (struct sort **)realloc(node->children,(kid+1)*sizeof(struct sort *));
		if(!node->children){
			foutf(2,"Could not reallocate memory, verify failed.\n");
			exit(-1);
		}
		for(++node->nchild; node->nchild < kid; ++node->nchild)
			node->children[node->nchild]=(struct sort *)0;
		node->children[node->nchild]=(struct sort *)0;
	}
}

struct	sort	*addkid(node,kid)
struct	sort	*node;
int		kid;
{
/*
* lint will complain about alignment problems, malloc returns a
* generic ptr
*/ 
	node->children[kid] = (struct sort *)malloc(sizeof(struct sort));
	if(!node->children[kid]){
		foutf(2,"Could not allocate memory, verify failed\n");
		exit(-1);
	}
	node = node->children[kid];
	node->next = (struct sort *)0;
	node->children = (struct sort **)0;
	node->nchild = -1;
	node->lineno = (char *)0;
	node->type = UNASSIGNED;
	return(node);
}

append(tree,field,type,line)
struct	sort	*tree;
char	*field;
int	type;
long	line;
{
	struct	sort	*pos;

	while(tree){
		pos = tree;
		tree = tree->next;
	}
/*
* lint will complain about alignment problems, malloc returns a
* generic ptr
*/ 
	pos->next = (struct sort *)malloc(sizeof(struct sort));
	if(!pos->next){
		foutf(2,"Could not allocate memory, verify failed\n");
		exit(-1);
	}
	pos = pos->next;
	pos->next = (struct sort *)0;
	pos->type = type;
	pos->address = line;
	pos->lineno = malloc((strlen(field)+1) * sizeof(char));
	if(!pos->lineno){
		foutf(2,"Could not allocate memory, verify failed\n");
		exit(-1);
	}
	(void)strcpy(pos->lineno,field);
	pos->children = (struct sort **)0;
	pos->nchild = -1;
	pos->supp = 0;
}

insert(pos,field,type,line)
struct	sort	*pos;
char	*field;
int	type;
long	line;
{
	struct	sort	*tmp;

	tmp = pos->next;
/*
* lint will complain about alignment problems, malloc returns a
* generic ptr
*/ 
	pos->next = (struct sort *)malloc(sizeof(struct sort));
	if(!pos->next){
		foutf(2,"Could not allocate memory, verify failed\n");
		exit(-1);
	}
	pos = pos->next;
	pos->next = tmp;
	pos->type = type;
	pos->address = line;
	pos->lineno = malloc((strlen(field)+1) * sizeof(char));
	if(!pos->lineno){
		foutf(2,"Could not allocate memory, verify failed\n");
		exit(-1);
	}
	(void)strcpy(pos->lineno,field);
	pos->children = (struct sort **)0;
	pos->nchild = -1;
	pos->supp = 0;
}

struct	sort	*findnumber(tree,number)
struct	sort	*tree;
char	*number;
{
	while(tree && strcmp(tree->lineno,number)){
		tree = tree->next;
	}
	return(tree);
}

int	depth(str)
char	*str;
{
	int	ret = 0;

	while(*str){
		if(*(str)++ == '.')
			++ret;
	}
	return(ret);
}

