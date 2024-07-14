/*LINTLIBRARY*/
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#ident	"@(#)instfuncs.c	1.11"

char	*vstrchr();
void	vstrncat(),exit();
char	*nxtok();
char	*getdef();
long	time();

static	char	temp[2048];
static	char	curlin[2048];
static	char	cmd[4096];
static	char	buildfile[100]="build";
extern	int	nopt;
extern	int	sopt;

struct	settings{
	char	source[512];
	char	dest[512];
	char	compiler[512];
	char	options[512];
	char	libs[512];
	char	release[512];
	char	extensions[512];
	char	sections[512];
};

struct	param {
	char	*name;
	char	*value;
};

#define	SECT2	"src/tools src BA KE"
#define	SECT3	"src/tools/z_nse src/tools/z_tie src/tools src BA KE TI NS"
static struct	settings setup = {
	"",
	"",
	"cc",
	"",
	"",
	"-D SYSVR_3",
	"",
	SECT3,
};

static	char	svlibs[512]="";

static struct	param parms[] ={
	{	"SOURCE",&setup.source[0]	},
	{	"DEST",&setup.dest[0]		},
	{	"COMP",&setup.compiler[0]	},
	{	"OPTS",&setup.options[0]	},
	{	"LIBS",&setup.libs[0]		},
	{	"REL",&setup.release[0]		},
	{	"EXT",&setup.extensions[0]	},
	{	"SECT",&setup.sections[0]	},
	{	"SVLIBS",&svlibs[0]		},
	{	"Bad Input",""			},
};

#define	NUMPARMS ((sizeof(parms)/sizeof(struct param))-1)

#define	MAXCMD	80	/*Max size of a line in a system() buffer*/
#define	SIZEFILE "/data/sizes"

int	f_source(str)
char	*str;
{
	(void)nxtok(setup.source,str);
}

int	f_dest(str)
char	*str;
{
	(void)nxtok(setup.dest,str);
}

int	f_comp(str)
char	*str;
{
	(void)nxtok(setup.compiler,str);
}

int	f_options(str)
char	*str;
{
	static	char	first = 1;

	if(first){
		*setup.options = 0;
		first = 0;
	}
	while(*str){
		str = nxtok(temp,str);
		vstrcat(setup.options,temp);
		vstrcat(setup.options," ");
	}
}

int	f_libs(str)
char	*str;
{
	static	char	first = 1;

	if(first){
		*setup.libs = 0;
		first = 0;
	}
	while(*str){
		str = nxtok(temp,str);
		vstrcat(setup.libs,temp);
		vstrcat(setup.libs," ");
	}
}

static	int	sectset=0;

int	f_release(str)
char	*str;
{
	(void)nxtok(temp,str);
	if(!strcmp(temp,"2")){
		vstrcpy(buildfile,"build2");
		if(!sectset){
			vstrcpy(setup.sections,SECT2);
		}
	}
	vstrcpy(setup.release,"-D SYSVR_");
	vstrcat(setup.release,temp);
}

int	f_exten(str)
char	*str;
{
	static	char	first = 1;

	if(first){
		*setup.extensions = 0;
		first = 0;
	}
	while(*str){
		str = nxtok(temp,str);
		if(*temp == ':'){
			vstrcat(setup.extensions,temp);
			vstrcat(setup.extensions," ");
		}
		vstrcat(setup.extensions,"-D ");
		vstrcat(setup.extensions,temp);
		vstrcat(setup.extensions," ");
	}
}

int	f_sections(str)
char	*str;
{
	static	char	first = 1;

	sectset = 1;
	if(first){
		*setup.sections = 0;
		first = 0;
	}
	while(*str){
		str = nxtok(temp,str);
		vstrcat(setup.sections,temp);
		vstrcat(setup.sections," ");
	}
}

f_print(str)
{
	outf("Source directory is: %s\n",setup.source);
	outf("Dest directory is: %s\n",setup.dest);
	outf("Compiler is: %s\n",setup.compiler);
	outf("Options are: %s\n",setup.options);
	outf("libs are: %s\n",setup.libs);
	outf("release is: %s\n",setup.release);
	outf("extensions are: %s\n",setup.extensions);
	outf("sections are: %s\n\n",setup.sections);
}

int	f_badinput(str)
char	*str;
{
	foutf(2,"Bad Input: %s\n",str);
}

static	char	cursect[512];
static	char	jfile[512];
static	int	journal;

install()
{
	long	tm;
	int	fd,ln=0;
	char	*p,*q;
	static	char	curfile[512]; /*Conserve stack*/
	static	char	opath[512],path[512];

	if(!((*setup.source) && (*setup.dest))){
		foutf(2,"Source and Destination not specified, cannot build\n");
		exit(-1);
	}
	if(sopt){
		dosizes();
		exit(0);
	}
	if(!nopt){
		(void)sprintf(jfile,"%s/journal",setup.dest);
		if((journal = open(jfile,O_WRONLY|O_TRUNC|O_CREAT,0666))<0){
			foutf(2,"build: cannot create journal file, no record of build\n");
			foutf(2,"problems will be kept.");
		}
		begjourn();
		begrun();
	}
	else{
		journal = -1;
	}
	p = &setup.sections[0];
	while((p = nxtok(cursect,p)),*cursect) {
		outf("Building section: %s\n",cursect);
		(void)sprintf(opath,"%s/%s",setup.source,cursect);
		if(chdir(opath)){
			foutf(journal,"\tERROR\n");
			foutf(journal,"\tchdir() failed unexpectedly\n");
			foutf(journal,"\tABORT\n");
			foutf(2,"Can not chdir to %s\n",opath);
			exit(-1);
		}
		fd = open(buildfile,O_RDONLY);
		if(fd < 0){
			foutf(2,"Can not open build file: %s/%s\n",opath,buildfile);
			foutf(journal,"\tERROR\n");
			foutf(journal,"\tCan not open build file: %s/%s\n",opath,buildfile);
			foutf(journal,"\tABORT\n");	
			exit(-1);
		}
		getlibs(fd,svlibs);
		while(fgetln(fd,curfile)){
			q = nxtok(path,curfile);
			foutf(journal,"BegMan %s/%s/%s\n",cursect,path,q);
			tm = time((long *)0);
			(void)sprintf(temp,"1.\tABSTRACT\n");
			foutf(journal,"%s",temp);
			foutf(journal,"\tExecute commands in %s/%s/%s\n",opath,path,q);
			if(chdir(path)){
				foutf(2,"Can not chdir to %s/%s\n",opath,path);
				foutf(journal,"\tERROR\n");
				foutf(journal,"\tCan not chdir to %s/%s\n",opath,path);
				foutf(journal,"\tABORT\n");
				exit(-1);
			}
			build(q,1);
			if(chdir(opath)){
				foutf(2,"Can not chdir to %s\n",opath);
				foutf(journal,"\tERROR\n");
				foutf(journal,"\tCan not chdir to %s\n",opath);
				foutf(journal,"\tABORT\n");
				exit(-1);
			}
			tm = time((long *)0) - tm;
			(void)sprintf(temp,"EndMan %d\n",(int)tm);
			foutf(journal,"%s",temp);
		}
		(void)close(fd);
	}
	if(journal > -1)
		(void)close(journal);
}

/*
** build(file,ln)
** This function reads the file named in the string "file" and
** performs the operations specified in that file using the
** user defined parameters to substitute in the place of keywords
** delimited by % % in the file.
*/

build(file,ln)
char	*file;
int		ln;
{
	int	infile,subno=1;

	if((infile = open(file,O_RDONLY)) == -1){
		foutf(journal,"\tOpening build file: %s\n",file);
		foutf(journal,"\tERROR\n\tFailed to open build file: %s\n",file);
		foutf(2,"build: Failed to open build file: %s\n",file);
		close(journal);
		exit(-1);
	}
	while(fgetln(infile,curlin)){
		clean(cmd,curlin);
		(void)sprintf(temp,"%d.%d.\tPREPARATION\n",ln,subno);
		foutf(journal,"%s",temp);
		foutf(journal,"\t%s\n",cmd);
		if(execute(cmd)){
			foutf(journal,"\tERROR\n");
			foutf(journal,"\tCommand exited with an error.\n");
		}
		subno++;
		*cmd = 0;
	}
	close(infile);
}

char	*nxtok(dst,src)
char	*src,*dst;
{

	while(*src && (whitespace(*(src)))) ++src;
	while(*src && (!whitespace(*(dst) = *(src++)))) ++dst;
	while(*src && (whitespace(*(src)))) ++src;
	*dst = 0;
	return(src);
}

/*VARARGS1*/
outf(a,b,c,d,e,f)
char	*a,*b,*c,*d,*e,*f;
{
	static	char	buff[2048];
	int	bsiz;

	bsiz = sprintf(buff,a,b,c,d,e,f);
	if(write(1,buff,(unsigned)bsiz) != bsiz)
		wrerr("stdout");
}

/*VARARGS2*/
foutf(fd,a,b,c,d,e,f)
int	fd;
char	*a,*b,*c,*d,*e,*f;
{
	static	char	buff[2048];
	int	bsiz;

	if(fd < 0)
		return;
	bsiz = sprintf(buff,a,b,c,d,e,f);
	if(write(fd,buff,(unsigned)bsiz) != bsiz)
		wrerr("output file");
}

wrerr(name)
char	*name;
{
	char	buff[100];
	int		bsiz;

	bsiz = sprintf(buff,"Error writing to %s.",name);
	(void)write(2,buff,(unsigned)bsiz);
}

/*
** clean(out,in)
** This function takes the input string specified in "in" and
** performs the substitutions indicated by calls to getdef().
** The result is placed in the output string "out".
*/

clean(out,in)
char	*out,*in;
{
	char	tmp[20],*t;

	while(*in){
		t = &tmp[0];
		while(*in && *in != '%'){
			vstrncat(out,in++,1);
		}
		if(*in){
			++in;
			while((*in) && *(in) != '%'){
				*(t++) = *(in++);
			}
			*t = 0;
		}
		if(*in){
			vstrcat(out,getdef(tmp));
			++in;
		}
	}
}

/*
** getdef(ident)
** This function determines the substitution to use for the
** keyword given in the string "ident".  It returns a pointer
** to the string to be substituted.
*/

char	*getdef(ident)
char	*ident;
{
	int	i;
	static	char	tmp[512];

	for(i=0; i<NUMPARMS; ++i){
		if(!vstrcmp(ident,parms[i].name)){
			break;
		}
	}
	chunk(tmp,parms[i].value);
	return(tmp);
}

chunk(out,in)
char	*out,*in;
{
	char	*p,*e,ch;
	char	tmp[128];
	int		got=0;

	p = in;
	while(*p){
		while(*p && (*p != ':')){
			p++;
		}
		if(*p){
			p = nxtok(tmp,p+1);
			if(!vstrcmp(tmp,cursect)){
				got = 1;
				break;
			}
		}
	}
	if(!got){
		p = in;
	}
	e = p;
	while(*e && (*e != ':'))
		++e;
	ch = *e;
	*e = '\0';
	strcpy(out,p);
	*e = ch;
}
		
execute(c)
char	*c;
{
	static	char	command[2048];

	outf("%s\n",c);
	breakup(command,c);
	if(nopt){
		return(0);
	}
	return(mysystem(command));
}

int	mysystem(c)
char	*c;
{
	FILE	*cmdout,*popen();
	int	ch;
	char	*p;

	p = &temp[0];
	vstrcat(c," 2>&1");
	cmdout = popen(c,"r");
	if((ch = getc(cmdout)) != EOF) {
		foutf(journal,"\tMESSAGE\n");
		foutf(2,"\tMESSAGE\n");
		while(ch != EOF) {
			*(p++) = (char)ch;
			if(ch == '\n'){
				*p = 0;
				foutf(2,"\t%s",temp);
				foutf(journal,"\t%s",temp);
				p = &temp[0];
			}
			ch = getc(cmdout);
			
		}
		*p = 0;
		foutf(journal,"\t%s\n",temp);
		foutf(2,"\t%s\n",temp);
	}
	return(pclose(cmdout));
}

/*
** breakup(out,in)
** Copies the string 'in' to 'out', breaking it up into bite-sized pieces
** for system().
*/
breakup(out,in)
char	*out,*in;
{
	int	br=0;
	int	i=0;

	while(*in){
		if(i++ > MAXCMD - 20){
			br = 1;
		}
		if(br && ((*in == ' ') || (*in == '\t'))){
			*(out++) = '\\';
			*(out++) = '\n';
			br = 0;
			i = 0;
		}
		*(out++) = *(in++);
	}
	*out = 0;
}

/*
** vstrcmp
**
**	Character manipulation procedure for SVVS.
**	This procedure compares character strings.
**
**	We do so in order to avoid a dependency upon C library
**	functions. 
**
**
*/

vstrcmp(s, t)	/* return <0 if s<t, 0 if s==t, >0 if s>t */
char *s, *t;
{
	while (*s == *t)
	{
		if(!*s++) return(0);
		t++;
	}
	return(*s - *t);
}


vstrlen(s)	/*Length of s*/

char	*s;
{
	int	i = 0;

	while(*s++) ++i;
	return(i);
}

vstrcpy(s, t)		/* copy t to s */

char *s, *t;
{
	while (*(s++) = *(t++));
}

vstrcat(s, t)	/* concatenate t to end of s */
char *s, *t;
{
	while (*s) ++s;
	while ( *(s++) = *(t++));	/* copy t to s*/
}

/*
** vstrncat
**
**	Character manipulation procedure for SVVS.
**	This procedure concatenates character strings.
**
**	We do so in order to avoid a dependency upon C library
**	functions. 
**
**	This code is borrowed from K & R p. 44
**
*/
void
vstrncat(s, t, n)/* concatenate t to end of s, copy at most n chars*/
char s[], t[];
int n;
{
	while ( *s )
		s++;
	while (n && ( *s++ = *t++ ))	/* copy t */
		n--;
	if(!n) *s = '\0';
}

char	*ctime();

begjourn()
{
	long	tm;
	char	*yr,*dt	;

	tm = time((long *)0);
	dt = ctime(&tm);
	*(dt + 24) = 0;
	yr = dt + 20;
	*(dt + 19) = 0;
	dt += 4;
	foutf(journal,"BegJourn 1 SVVS 3.0 \"\" \"\" %s %s INSTALL \"\"\n",dt,yr);
}

begrun()
{
	long	tm;
	char	*yr,*dt	;

	tm = time((long *)0);
	dt = ctime(&tm);
	*(dt + 24) = 0;
	yr = dt + 20;
	*(dt + 19) = 0;
	dt += 4;
	foutf(journal,"BegRun  1 initial long %s %s \"\"\n",dt,yr);
}

getlibs(fd,str)
int	fd;
char	*str;
{
	char	*p;

	*str = 0;
	(void)fgetln(fd,curlin);
	p = nxtok(temp,curlin);
	while(*temp){
		vstrcat(str,setup.dest);
		vstrcat(str,"/lib/");
		vstrcat(str,temp);
		vstrcat(str," ");
		p = nxtok(temp,p);
	}
}

int	dosizes()
{
	int		total = 0;
	int		size;
	static	char	sect[512];
	char	*p;

	outf("Approximate Disk Space Requirements in 512 Byte Blocks\n\n");
	p = &setup.sections[0];
	while((p = nxtok(sect,p)),*sect) {
		size = space(sect);
		total += size;
		outf("%d\t\t%s\n",size,sect);
	}
	outf("%d\t\tTotal\n\n",total);
}

int	space(sect)
char	*sect;
{
	char	*p;
	int	size,fd;

	strcpy(temp,setup.source);
	strcat(temp,SIZEFILE);
	if((fd = open(temp,O_RDONLY,0)) < 0){
		foutf(2,"Cannot open size data file.\n");
		exit(-1);
	}
	while(fgetln(fd,curlin)){
		p = nxtok(cmd,curlin);
		if(!strcmp(cmd,sect)){
			p = nxtok(cmd,p);
			sscanf(cmd,"%d",&size);
			close(fd);
			return(size);
		}
		curlin[0] = 0;
	}
	foutf(2,"Unrecognized section %s.\n",sect);
	close(fd);
	return(0);
}
