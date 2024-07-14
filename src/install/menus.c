/*LINTLIBRARY*/
#ifdef	UNDEF
#ident	"@(#)menus.c	1.17"
#endif

#include <fcntl.h>
#include <ctype.h>
#ifndef SYSVR_2
#include <unistd.h>
#endif

#ifndef	SEEK_SET
#define	SEEK_SET 0
#endif
#ifndef	SEEK_CUR
#define	SEEK_CUR 1
#endif

/*
** The following functions are available to applications which wish to
** use the menu driver system. 
**
*/
int		loadmenus();	/*Load the named menu file and initialize. */
int		domenu();		/*Display the current menu and respond to user*/
void	set_init();		/*Set the init function*/
char	*get_help();	/*Return a pointer to the help information of the
						most recently displayed menu*/
long	svseek();

#define	PROMPT	0
#define	MENU	1
#define DEADEND	2
#define	HEADER	3
#define	LEADER	4
#define	TYPEMIN	PROMPT		/*lowest valued type flag, PROMPT*/
#define	TYPEMAX	LEADER		/*highest valued type flag, HEADER*/
#define	INIT	8		/*Init bit for object type*/
#define	TYPMASK	7		/*Mask to read the type of an object*/

#define	LINK	0		/*action is a link*/
#define	VALUE	1		/*action is a value*/
#define	HELP	2		/*action is display help*/
#define	EXIT	3		/*action is exit*/
#define	CALL	4		/*action is a call (push/link)*/
#define	RETURN	5		/*action is a return (pop/link)*/
#define	PUSH	6		/*Push an address to fool return*/

#define	NOADD	0		/*Do not add symbol, just look it up.*/
#define	ADD		1		/*Add new symbol*/

#define	STACK_DEPTH	10	/*Depth of call stack*/

struct	entry{
	int		action;		/*Flag indicating action to take*/
						/*0 - link to new menu*/
						/*1 - return the value in the 'value' field*/
						/*2 - present help message and repeat item*/
	int		redisp;		/*If this entry is taken, redisplay the menu*/
	char	*entry;		/*Text of the menu entry if this is a menu*/
	char	**resp;		/*List of responses activating this entry*/
	char	*value;		/*If !link return this value when selected*/
	struct	stab	*link; /*If link, activate this menu when selected*/
};

struct	dsp{
	int		repeat;		/*Repeat count for this menu*/
	int		nent;		/*number of entries in this item*/
	char	*prompt;	/*String to be used as a prompt after a menu*/
	char	*regards;	/*Reason this menu exists*/
	char	*description;	/*Menu heading if menu, prompt if prompt*/
	char	*help;			/*Help message for current item*/
	char	*ack;			/*String to use for acknowledgement of value*/
	struct	entry	*entries; /*The entries defined for this item*/
	struct	stab	*next;	  /*Next menu after repeat count runs out*/
};

struct	stab{
	char	*name;		/*Symbol name*/
	int		type;		/*leader = 4, header = 3, prompt = 1, menu = 0*/
	struct	dsp	*obj;	/*Symbol storage*/
	long		pos;	/*Position in menu file*/
	int			lock;	/*flag to prevent purging of the menu currently*/
						/*in use*/
	struct	stab	*next;	/*Next symbol in table*/
};

struct	mparse	{
	char	*key;
	int	(*funct)();
};

/*
** Name and type of an object declaration keyword.
*/
struct	objkey{
	char	*name;
	int		type;
};

static	menload();		/*Load a menu from disk*/
static	load();			/*called by menload()*/
static	killent();		/*Free an entry node*/
static	push();			/*Push a given node to fool return*/
static	struct stab	*call();/*Push current node and link to another*/
static	struct stab	*mreturn();	/*Pop previous node and link to it*/

static	int	start();	/*Set up the starting menu for the menus*/
static	int	prompt();	/*Set up prompt node in symtab under name in str*/
static	int	menu();		/*Set up menu node in symtab under name in str*/
static	int	header();	/*Set up heading node in symtab under name in str*/
static	int	leader();	/*Set up leader node in symtab under name in str*/
static	int	init();		/*Set init bit in object*/
static	int	deadend();	/*Set up dead end node (display message and sit)*/
static	int	descrip();	/*Description follows, attach to current node*/
static	int	helpinfo();	/*Help message follows, attach it to current node*/
static	int	ack();		/*Acknowledgement follows, attach to current node*/
static	int	entry();	/*If menu, text of entry is in str, else error*/
static	int	nonentry();	/*If menu, text of entry is in str, else error*/
static	int	responses();	/*Responses to select this entry are in str*/
static	int	value();	/*If responses are set, this entry returns
						this value, else this is default value for a
						prompt or an error for menu*/
static	int	menlink();	/*If responses are not set, error, else link
						to the menu named in str*/
static	int	mencall();	/*If responses are not set, error, else link
						to the menu named in str with call as action*/
static	int	menpush();	/*If responses are not set, error, else push
						the menu named in str and link to next*/
static	int	menreturn();/*Action for response is return*/
static	int	help();		/*If responses are not set, error, set help flag*/
static	int	exitact();	/*If responses are not set, error, set exit flag*/
static	int	regards();	/*Set regarding field in current menu to str*/
static	int	mrepeat();	/*Set repeat count for current menu*/
static	int	mredisp();	/*Redisplay if this entry is selected*/
static	int	next();		/*Enter str in symtab and connect this node's next
						field to the symtab entry*/
static	int	badinput();	/*Routine to process bad input*/
static	int	psitmsg();	/*Routine to prepare the sit() message*/
static	int	psitrsp();	/*Routine to prepare the sit() response*/
static	int	pexitrg();	/*Routine to prepare the exit context*/
static	int	pexitrsp();	/*Routine to prepare the exit response*/
static	int	setprompt();/*Routine to prepare the prompt for a menu*/
static	int	endtxt();	/*Routine to handle %end% lines*/

static	struct	mparse	language[]={
	{ "%start%",  start},
	{ "%prompt%", prompt},
	{ "%menu%", menu},
	{ "%header%", header},
	{ "%leader%", leader},
	{ "%init%", init},
	{ "%deadend%", deadend},
	{ "%description%", descrip},
	{ "%information%", helpinfo},
	{ "%acknowledge%", ack},
	{ "%entry%", entry},
	{ "%nonentry%", nonentry},
	{ "%responses%", responses},
	{ "%value%", value},
	{ "%link%", menlink},
	{ "%call%", mencall},
	{ "%push%", menpush},
	{ "%return%", menreturn},
	{ "%help%", help},
	{ "%exit%", exitact},
	{ "%regarding%", regards},
	{ "%repeat%", mrepeat},
	{ "%redisplay%", mredisp},
	{ "%promptstr%",setprompt },
	{ "%next%", next},
	{ "%sitmsg%",psitmsg	},
	{ "%sitresp%",psitrsp	},
	{ "%exitreg%",pexitrg	},
	{ "%exitresp%",pexitrsp	},
	{ "%end%",endtxt		},
	{ "Bad input",badinput},
};
#define	NUMTOKS	((sizeof(language)/sizeof(struct mparse)) - 1)

char	*nxtok();
char	*malloc(),*realloc();
char	*strcpy(),*strcat();
static	char	*allocate(),*getmore();
static	struct	stab	*respond();
void	free();

static	int	menufile;
static	struct	stab	*curnode;
static	struct	dsp		*curobj;
static	struct	stab	*startnode;
static	char	exitreg[512];	/*Context of an exit after final menu*/
static	char	exitresp[512];	/*Response for an exit after final menu*/
static	char	sitmsg[512]="Please enter 'c' to continue ";
							/*Text of the message used by sit()*/
static	char	sitresp[128]="c";/*Response needed to continue after sit*/
static	void	(*initfunc)() = (void (*)())0;
								/*Function to be called for init, if set
								by main program call to set_init()*/
/*
**The following routines make up the interface to the menu system:
**			loadmenus(name)
**			char	*name;
**						loads the named menu file and initializes
**						the menu system.  This should be called once
**						at the beginning of a menu application.
**
**			domenu(regarding,response);			
**			char	*regarding,*response;
**						Displays the currently active menu and processes
**						responses.  When a value is found, it is placed
**						in response and regarding gets set to the
**						regarding value of the menu in which the value
**						was found.
**
**			char	*get_help()
**						Returns a pointer to the help message for
**						the currently active menu.  This allows an
**						application to handle help its own way if it
**						wants to, and still have all of the information
**						in the menu file.
**
**			int	sit()
**						This routine displays the sit-message and will not
**						continue until the sit response is typed.  This is
**						useful when displaying information that the user
**						will need to see before it scrolls off of the
**						screen.
**
**			set_init(func)
**			void	(*func)();
**						Sets the init action for the calling program.
**						"func" will be called any time a menu which has
**						the %init% attribute is displayed.
**
**			chmenu(name)
**			char	*name;
**						Changes the next menu to be displayed to the
**						named menu.  If the menu does not exist, -1
**						is returned, otherwise 0 is returned.  In order
**						to preserve easy modification of menus, 'name'
**						should be obtained from a response value in some
**						way.
*/

/*
** Routine to load up the menus  for the menu system.  Returns non zero
** if it fails.
*/

static	int	pass1=0;

int	loadmenus(file)
char	*file;
{
	static	char	*line = (char *)0;

	if(!line){
		purge();
		line = malloc(512 * sizeof(char));
		if(!line)
			foutf(2,"Out of memory.\n");
	}
	pass1 = 1;		/*First pass flag for parse routines*/
	*exitreg = *exitresp = *sitmsg = *sitresp = 0;
	menufile = open(file,O_RDONLY);
	if(menufile < 0){
		foutf(2,"Could not open file: %s\n",file);
		pass1 = 0;
		return(-1);
	}
	while(fgetln(menufile,line)){
		if(nonblank(line)){
			parse(line);
		}
	}
	if(!startnode || (startnode->pos < (long)0)){
		foutf(2,"Start node undefined or unspecified\n");
	}
	checkstab();
	pass1 = 0;
	return(0);
}

static	struct	stab	*prevmenu = (struct stab *)0;
static	struct	stab	*curmenu = (struct stab *)0;
static	int		eact=0;	/*Are we an exit action?*/

int	domenu(regard,response)
char	*regard;	/*Regarding return value slot*/
char	*response;	/*Response return value slot*/
{
	static	int		first = 1;
	int	inmenu = 1;
	int	type;

	eact = 0;
	prevmenu = curmenu;
	if(first){
		if(startnode){
			curmenu = startnode;
			first = 0;
		}
		else
			curmenu = (struct stab *)0;
	}
	while(inmenu){
		if(!curmenu){
			(void)strcpy(regard,exitreg);
			(void)strcpy(response,exitresp);
			eact = 1;
		}
		else{
			dspmenu(curmenu);	/*This locks curmenu*/
			type = curmenu->type & TYPMASK;
			if(curmenu->obj->regards)
				(void)strcpy(regard,curmenu->obj->regards);
			if(type != DEADEND && type != HEADER){
				if(prevmenu && (prevmenu != curmenu)){
					prevmenu->lock = 0;	/*unlock previous menu before we*/
										/*throw it away*/
				}
				prevmenu = curmenu;
				curmenu = respond(curmenu,response);
			}
			else{
				if(prevmenu){
					prevmenu->lock = 0;	/*unlock previous menu before we*/
										/*throw it away*/
				}
				prevmenu = curmenu;
				curmenu = curmenu->obj->next;
			}
		}
		if(*response){
			inmenu = 0;
		}
	}
	if(!eact && prevmenu->obj->ack){
		foutf(2,prevmenu->obj->ack,response);
	}
}

char	*get_help()
{
	if(prevmenu && prevmenu->obj->help)
		return(prevmenu->obj->help);
	else
		return("No help available");
}

sit()
{
	static	char	*line=(char *)0;

	if(!line){
		purge();
		line = malloc(512 * sizeof(char));
		if(!line){
			foutf(2,"sit: Out of memory.");
			exit(-1);
		}
	}
	foutf(2,"%s",sitmsg);
	while(getln(line)){
		if(respcmp(line,sitresp))
			foutf(2,"%s",sitmsg);
		else
			return;
	}
}

void	set_init(func)
void	(*func)();
{
	initfunc = func;
}

static	struct	stab	*symtab=(struct stab *)0;

int	chmenu(name)
char	*name;
{
	struct	stab	*p;

	p = symtab;
	while(p){
		if(!strcmp(p->name,name)){
			curmenu->lock = 0;	/* unlock menu so that it can be purged */
			curmenu = p;		/* set new menu pointer					*/
			return(0);
		}
		p = p->next;
	}
	return(-1);
}

/*
** Set curmenu back to prevmenu to prevent advancement in menus. Usually
** used in case of an error in a menu.
*/
int	stayput()
{

	curmenu->lock = 0;	/* unlock menu so that it can be purged */
	curmenu = prevmenu;	/* Reset to previous menu */
	curmenu->lock = 1;	/* lock menu so that it can't be purged */
}
/*
** This ends the interface to the menus.  The rest of the functions are
** used only by the menu system and should all be static.
*/

static	dspmenu(ent)
struct	stab	*ent;
{
	struct	dsp	*mn;
	int	i;

	menload(ent);	/*Load this entry and lock it in memory*/
	mn = ent->obj;
	if(ent->type & INIT){
		if(initfunc)
			(initfunc)();
	}
	if(mn->description)
		foutf(2,"\n\n%s",mn->description);
	if((ent->type & TYPMASK) == MENU){
		foutf(2,"\n");
		for(i=0; i < mn->nent; ++i){
			if(mn->entries && mn->entries[i].entry)
				foutf(2,"%s\n",mn->entries[i].entry);
		}
		if(mn->prompt)
			foutf(2,"\n%s",mn->prompt);
		else
			foutf(2,"--> ");
	}
	else if((ent->type & TYPMASK) == DEADEND){
		foutf(2,"\n");
	}
}

static	struct	stab	*respond(ent,rsp)
struct	stab	*ent;
char	*rsp;
{
	extern	int	shell;	/*Set by shell escape routine*/
	struct	dsp	*mn;
	static	char	*line = (char *)0;
	static	char	*token = (char *)0;
	static	int	rep = -1;
	int	i=0;

	if(!line){
		purge();
		line = malloc(512 * sizeof(char));
		if(!line)
			foutf(2,"Out of memory.\n");
	}
	if(!token){
		purge();
		token = malloc(512 * sizeof(char));
		if(!token)
			foutf(2,"Out of memory.\n");
	}
	mn = ent->obj;
	*rsp = 0;
	if(rep == -1)
		rep = mn->repeat;
	if((ent->type & TYPMASK) != LEADER){
		if(!getln(line)){
			foutf(2,"\n");
			return((struct stab *)0);
		}
		if(shell){
			return(ent);
		}
		(void)nxtok(token,line);
		for(i = 0; i < mn->nent; ++i){
			if(mn->entries && isresp(&mn->entries[i],token)){
				break;
			}
		}
	}
	if(i == mn->nent){
		if(!nonblank(line)){
			return(ent);
		}
		if((ent->type & TYPMASK) == PROMPT){
			(void)strcpy(rsp,line);
			if(!rep--){
				return(mn->next);
			}
			return(ent);
		}
		rep = -1;
		foutf(2,"Unrecognized response\n");
		return(ent);
	}
	if(mn->entries[i].action == LINK){
		rep = -1;
		return(mn->entries[i].link);
	}
	else if(mn->entries[i].action == HELP){
		rep = -1;
		if(mn->help){
			foutf(2,"\n\n%s\n",mn->help);
			sit();
		}
		return(ent);
	}
	else if(mn->entries[i].action == CALL){
		rep = -1;
		return(call(mn->entries[i].link));
	}
	else if(mn->entries[i].action == PUSH){
		rep = -1;
		push(mn->entries[i].link);
		return(mn->next);
	}
	else if(mn->entries[i].action == RETURN){
		rep = -1;
		return(mreturn());
	}
	else if(mn->entries[i].action == EXIT){
		return((struct stab *)0);
	}
	(void)strcpy(rsp,mn->entries[i].value);
	if(mn->entries[i].redisp){
		rep = -1;
		return(ent);
	}
	if(!rep--){
		rep = -1;
		return(mn->next);
	}
	return(ent);
}

static	int	isresp(ent,resp)
struct	entry	*ent;
char	*resp;
{
	char	**p;

	for(p = ent->resp; *p; ++p){
		if(!respcmp(*p,resp))
			break;
	}
	if(*p)
		return(1);
	return(0);
}

/*
** Routine to load a menu into memory.  Before the menu is loaded it is
** locked.  If there is no space to load this menu, the menus will be
** purged.  Locked menus will remain in memory, all others will be thrown
** out.
*/
static	menload(ent)
struct	stab	*ent;
{
	if(!ent){
		foutf(2,"Null symbol table entry.");
		exit(-1);
	}
	ent->lock = 1;		/*lock the menu, someone else must unlock it*/
	if(!ent->obj){		/*If no menu currently in memory*/
/*
** Lint will complain of pointer problems here, allocate returns a GENERAL
** pointer.
*/
		ent->obj = (struct dsp *)allocate(sizeof(struct dsp));
		if(!ent->obj){
			foutf(2,"Out of memory, cannot load object.");
			exit(-1);
		}
		curnode = ent;
		curobj = ent->obj;
		curobj->repeat = 0;
		curobj->nent = 0;
		curobj->prompt = (char *)0;
		curobj->regards = (char *)0;
		curobj->description = (char *)0;
		curobj->ack = (char *)0;
		curobj->help = (char *)0;
		if((curnode->type & TYPMASK) == LEADER){
			curobj->entries=(struct entry *)allocate(sizeof(struct entry));
			if(!curobj->entries){
				foutf(2,"Out of memory, cannot load.");
				exit(-1);
			}
			curobj->entries[0].action = -1;
			curobj->entries[0].redisp = 0;
			curobj->entries[0].entry = (char *)0;
			curobj->entries[0].resp = (char **)0;
			curobj->entries[0].value = (char *)0;
			curobj->entries[0].link = (struct stab *)0;
			curobj->nent = 1;
		}
		else{
			curobj->entries = (struct entry *)0;
		}
		curobj->next = (struct stab *)0;
		if(ent->pos > (long) -1){
			load(ent->pos);
		}
		checkobj(ent->obj,ent->name,ent->type);
	}
}

static	int	gotmenu=0;

static	load(pos)
long	pos;
{
	static	char	*line = (char *)0;

	if(!line){
		purge();
		line = malloc(512 * sizeof(char));
		if(!line)
			foutf(2,"Out of memory.\n");
	}
	svseek(menufile,pos,SEEK_SET);	
	gotmenu = 0;
	while(!gotmenu && fgetln(menufile,line)){
		if(nonblank(line)){
			parse(line);
		}
	}
}

/*
** routine to purge menus when memory runs out.  This routine only
** eliminates unlocked menus.
*/
static	int		purge()
{
	struct	stab	*p;

	p = symtab;
	while(p){
		if(p->obj && (!p->lock)){
			reldsp(p->obj);
			p->obj = (struct dsp *)0;
		}
		p = p->next;
	}
}

/*
** Parser to build up the menus.
*/

static	int	intext=0;

static	int	parse(str)
char	*str;
{
	static	char	*token = (char *)0;
	int		i;

	if(!token){
		purge();
		token = malloc(512 * sizeof(char));
		if(!token)
			foutf(2,"Out of memory.\n");
	}
	str = nxtok(token,str);
	if(intext && strcmp(token,"%end%")){
		return(0);
	}
	for(i = 0; i < NUMTOKS; ++i){
		if(!strcmp(language[i].key,token))
			break;
	}
	return((language[i].funct)(str));
}

/*
** Routine to manipulate the symbol table.  The arguments are the name of
** the object and ADD or NOADD.  It returns a pointer to the symbol
** table entry.  If the symbol already exists NOADD is specified,
** a pointer to the symbol table entry will be returned with no effect on
** the symbol table.  If the symbol exists but is undefined, and ADD is
** specified, a symbol will be defined using the name provided.  If it
** was previously defined, it will be redefined and a warning issued.
** If the symbol did not exist it is entered in the symbol table and a
** pointer to its entry is returned.
*/


static	struct	stab	*syment(str,add)
char	*str;		/*Name of symbol*/
int		add;		/*Flag to determine whether to add object*/
{
	struct	stab	*cur,*prev;
	int		cmp;

	if(symtab){
		cur = prev = symtab;
		while(cur && (cmp = strcmp(str,cur->name))){
			prev = cur;
			cur = cur->next;
		}
		if(!cmp){
			if(cur && add){
				if(cur->pos > (long)-1){
					foutf(2,"Warning: %s multiply defined, old definition superseded\n",cur->name);
				}
				cur->pos = svseek(menufile,0l,SEEK_CUR);
			}
			return(cur);
		}
/*
* Lint will complain about alignment problems here, but allocate
* will always return a GENERAL pointer
*/
		prev->next = (struct stab *)allocate(sizeof(struct stab));
		if(!prev->next){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
		prev = prev->next;
		prev->name = allocate((unsigned)((strlen(str)+1) * sizeof(char)));
		if(!prev->name){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
		(void)strcpy(prev->name,str);
		prev->obj = (struct dsp *)0;
		prev->lock = 0;
		prev->pos = svseek(menufile,0l,SEEK_CUR);
		prev->next = (struct stab *)0;
		return(prev);
	}
/*
* Lint will complain about alignment problems here, but allocate
* will always return a GENERAL pointer
*/
	symtab = (struct stab *)allocate((unsigned)sizeof(struct stab));
	if(!symtab){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	symtab->name = allocate((unsigned)((strlen(str)+1) * sizeof(char)));
	if(!symtab->name){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(symtab->name,str);
	symtab->obj = (struct dsp *)0;
	symtab->lock = 0;
	symtab->pos = svseek(menufile,0l,SEEK_CUR);
	symtab->next = (struct stab *)0;
	return(symtab);
}

/*
** Routine to check the symbol table for undefined or improperly defined
** symbols
*/
static	checkstab()
{
	struct	stab	*p;

	p = symtab;
	while(p){
		if(!p->name){
			foutf(2,"Corrupt symbol table\n");
		}
		else if(!(*p->name)){
			foutf(2,"Null symbol in symbol table\n");
		}
		if(p->obj){
			foutf(2,"Symbol ptr. not initialized.\n");
		}
		if(p->lock){
			foutf(2,"Lock set before activation.");
		}
		if(p->pos < (long)0){
			foutf(2,"Bad file offset.\n");
		}
		p = p->next;
	}
}

/*
** Routine to check consistency of objects in the symbol table
*/

static	checkobj(obj,name,type)
struct	dsp	*obj;
char	*name;
int	type;
{
	int	i;

	type &= TYPMASK;
	if((type < TYPEMIN) || (type > TYPEMAX)){
		foutf(2,"Invalid type of object %s\n",name);
	}
	if(!obj->regards && type != HEADER){
		foutf(2,"No context specified for %s\n",name);
	}
	if(type == MENU){
		if(!obj->nent){
			foutf(2,"No entries for menu %s\n",name);
		}
	}
	for(i=0; i < obj->nent; ++i){
		if(!obj->entries){
			foutf(2,"Unexpected Null list of entries in %s\n",name);
			break;
		}
		else if(type == MENU && !obj->entries[i].resp){
			foutf(2,"Entry in %s has no responses\n",name);
		}
		else if(obj->entries[i].action == HELP && !obj->help){
			foutf(2,"No help available for %s\n",name);
		}
		else if(obj->entries[i].action == LINK
		  && !obj->entries[i].link){
			foutf(2,"No link specified for entry in %s\n",name);
		}
		else if(obj->entries[i].action == CALL
		  && !obj->entries[i].link){
			foutf(2,"No link specified for entry in %s\n",name);
		}
		else if(obj->entries[i].action == PUSH
		  && !obj->entries[i].link){
			foutf(2,"No link specified for entry in %s\n",name);
		}
		else if(obj->entries[i].action == VALUE
		  && !obj->entries[i].value){
			foutf(2,"No value for entry in %s\n",name);
		}
	}
	if(type == PROMPT && !obj->description){
		foutf(2,"No description for prompt %s\n",name);
	}
}

/*
** routine to free a display item node.
*/

static	reldsp(node)
struct	dsp	*node;
{
	int		i;

	if(!node)
		return(0);
	if(node->description){
		free(node->description);
		node->description = (char *)0;
	}
	if(node->help){
		free(node->help);
		node->help = (char *)0;
	}
	for(i = 0; i < node->nent; ++i){
		killent(&node->entries[i]);
	}
	if(node->entries){
		free(node->entries);
		node->entries = (struct entry *)0;
	}
	free(node);
}

static	killent(ent)
struct	entry	*ent;
{
	char	**p;

	if(!ent){
		return(0);
	}
	if(ent->entry){
		free(ent->entry);
		ent->entry = (char *)0;
	}
	if(p = ent->resp){
		while(*p){
			free(*(p++));
		}
		free(ent->resp);
		ent->resp = (char **)0;
	}
	if(ent->value){
		free(ent->value);
		ent->value = (char *)0;
	}
}

/*
** Routine to add an entry to a display item.
*/
static	addentry(node,ent)
struct	dsp	*node;
struct	entry	*ent;
{
	node->nent++;
	if(node->entries){
/*
* Lint will complain about alignment problems here, but allocate
* will always return a GENERAL pointer
*/
		node->entries=(struct entry *)getmore((char *)node->entries,(unsigned)(node->nent*sizeof(struct entry)));
		if(!node->entries){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
	}
	else{
/*
* Lint will complain about alignment problems here, but allocate
* will always return a GENERAL pointer
*/
		node->entries=(struct entry *)allocate((unsigned)sizeof(struct entry));
		if(!node->entries){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
	}
	node->entries[node->nent-1].link = ent->link;
	node->entries[node->nent-1].entry = ent->entry;
	node->entries[node->nent-1].resp = ent->resp;
	node->entries[node->nent-1].value = ent->value;
	node->entries[node->nent-1].action = ent->action;
	node->entries[node->nent-1].redisp = 0;
}

/*
** Routine to add a response to an entry
*/
static	addresp(ent,rsp)
struct	entry	*ent;
char	*rsp;
{
	char	**p;
	int		i=1;

	p = ent->resp;
	while(p && *p){
		i++;
		p++;
	}
	if(ent->resp){
/*
* Lint will complain about alignment problems here, but allocate
* will always return a GENERAL pointer
*/
		ent->resp = (char **)getmore((char *)ent->resp,(unsigned)((i + 1) * sizeof(char *)));
		if(!ent->resp){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
	}
	else{
/*
* Lint will complain about alignment problems here, but allocate
* will always return a GENERAL pointer
*/
		ent->resp = (char **)allocate((unsigned)((i + 1) * sizeof(char *)));
		if(!ent->resp){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
	}
	ent->resp[i-1] = allocate((unsigned)((strlen(rsp) + 1) * sizeof(char)));
	if(!ent->resp[i-1]){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(ent->resp[i-1],rsp);
	ent->resp[i] = (char *)0;
}

/*
** Parsing routines
*/

static	int	prompt(str)
char	*str;
{
	if(pass1){
		curnode = syment(str,ADD);
		curnode->type = PROMPT;
	}
	else{
		gotmenu = 1;
	}
	return(0);
}

static	int	menu(str)
char	*str;
{
	if(pass1){
		curnode = syment(str,ADD);
		curnode->type = MENU;
	}
	else{
		gotmenu = 1;
	}
	return(0);
}

static	int	header(str)
char	*str;
{
	if(pass1){
		curnode = syment(str,ADD);
		curnode->type = HEADER;
	}
	else{
		gotmenu = 1;
	}
	return(0);
}

/*A leader is a valued node in the menu list.  It connects together two*/
/*other nodes.  When it is encountered it returns its value and*/
/*regarding entry to the calling program.  Leaders are used to tell the*/
/*calling program where the menu system is without the need for user*/
/*interaction.*/
static	int	leader(str)
char	*str;
{
	if(pass1){
		curnode = syment(str,ADD);
		curnode->type = LEADER;
	}
	else{
		gotmenu = 1;
	}
	return(0);
}

static	int	deadend(str)
char	*str;
{
	if(pass1){
		curnode = syment(str,ADD);
		curnode->type = DEADEND;
	}
	else{
		gotmenu = 1;
	}
	return(0);
}

static	int	init()
{
	if(pass1){
		curnode->type |= INIT;
	}
}

static	char	buff[4096];
static	char	tmpbuf[256];
static	char	token[100];

static	int	descrip(str)
char	*str;
{
	if(pass1){
		intext = 1;
		return(0);
	}
	*buff = 0;
	while(fgetln(menufile,tmpbuf)){
		(void)nxtok(token,tmpbuf);
		if(!strcmp(token,"%end%"))
			break;
		(void)strcat(buff,tmpbuf);
		(void)strcat(buff,"\n");
	}
	if((curnode->type & TYPMASK) == PROMPT){
		buff[strlen(buff)-1] = 0;
	}
	curobj->description = allocate((unsigned)((strlen(buff)+1) * sizeof(char)));
	if(!curobj->description){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(curobj->description,buff);
	return(0);
}

static	int	ack(str)
char	*str;
{
	if(pass1){
		intext = 1;
		return(0);
	}
	*buff = 0;
	while(fgetln(menufile,tmpbuf)){
		(void)nxtok(token,tmpbuf);
		if(!strcmp(token,"%end%"))
			break;
		(void)strcat(buff,tmpbuf);
		(void)strcat(buff,"\n");
	}
	if((curnode->type & TYPMASK) == PROMPT){
		buff[strlen(buff)-1] = 0;
	}
	curobj->ack = allocate((unsigned)((strlen(buff)+1) * sizeof(char)));
	if(!curobj->ack){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(curobj->ack,buff);
	return(0);
}

static	int	helpinfo()
{
	if(pass1){
		intext = 1;
		return(0);
	}
	*buff = 0;
	if((curnode->type & TYPMASK) == LEADER){
		foutf(2,"Cannot define help information for a 'leader'.\n");
		return(-1);
	}
	while(fgetln(menufile,tmpbuf)){
		(void)nxtok(token,tmpbuf);
		if(!strcmp(token,"%end%"))
			break;
		(void)strcat(buff,tmpbuf);
		(void)strcat(buff,"\n");
	}
	curobj->help=allocate((unsigned)((strlen(buff)+1) * sizeof(char)));
	if(!curobj->help){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(curobj->help,buff);
	return(0);
}

static	int	ent=0;

static	int	entry(str)
char	*str;
{
	struct	entry	e;

	if(pass1){
		return(0);
	}
	ent = 1;
	if((curnode->type & TYPMASK) != MENU){
		foutf(2,"Error: Entry cannot be defined for non-menu\n");
		return(-1);
	}
	if(!*str){
		foutf(2,"Error: No text for menu entry\n");
		return(-1);
	}
	(void)nxtok(buff,str);
	e.entry = allocate((unsigned)(strlen(buff)+1));
	if(!e.entry){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(e.entry,buff);
	e.action = -1;
	e.value = (char *)0;
	e.link = (struct stab *)0;
	e.resp = (char **)0;
	addentry(curobj,&e);
	return(0);
}

static	int	nonentry(str)
char	*str;
{
	struct	entry	e;

	if(pass1){
		return(0);
	}
	ent = 1;
	if((curnode->type & TYPMASK) != MENU){
		foutf(2,"Error: Entry cannot be defined for non-menu\n");
		return(-1);
	}
	(void)nxtok(buff,str);
	e.entry = (char *)0;
	e.action = -1;
	e.value = (char *)0;
	e.link = (struct stab *)0;
	e.resp = (char **)0;
	addentry(curobj,&e);
	return(0);
}

static	int	responses(str)
char	*str;
{
	struct	entry e;

	if(pass1){
		return(0);
	}
	if((curnode->type & TYPMASK) == LEADER){
		foutf(2,"Cannot define responses for a 'leader'.\n");
		return(-1);
	}
	if(!ent){
		e.action = -1;
		e.entry = (char *)0;
		e.value = (char *)0;
		e.link = (struct stab *)0;
		e.resp = (char **)0;
		addentry(curobj,&e);
	}
	while(*str){
		str = nxtok(token,str);
		addresp(&curobj->entries[curobj->nent-1],token);
	}
	ent = 0;
	return(0);
}		

static	int	value(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if(!*str){
		foutf(2,"Error: no token specified for value\n");
		return(-1);
	}
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for value\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = VALUE;
		curobj->entries[curobj->nent-1].link = (struct stab *)0;
		curobj->entries[curobj->nent-1].value = allocate((unsigned)((strlen(str)+1) * sizeof(char)));
		if(!curobj->entries[curobj->nent-1].value){
			foutf(2,"Out of memory\n");
			exit(-1);
		}
		(void)strcpy(curobj->entries[curobj->nent-1].value,str);
	}
	else{
		foutf(2,"Error: Multiple actions defined for one entry\n");
		return(-1);
	}
	return(0);
}

static	int	menlink(str)
char	*str;
{

	if(pass1){
		return(0);
	}
	if(!*str){
		foutf(2,"Error: no symbol specified for link\n");
		return(-1);
	}
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for link\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = LINK;
		curobj->entries[curobj->nent-1].link = syment(str,NOADD);
		return(0);
	}
	foutf(2,"Error: Multiply defined action in menu\n");
	return(-1);
}

static	int	mencall(str)
char	*str;
{

	if(pass1){
		return(0);
	}
	if(!*str){
		foutf(2,"Error: no symbol specified for call\n");
		return(-1);
	}
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for call\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = CALL;
		curobj->entries[curobj->nent-1].link = syment(str,NOADD);
		return(0);
	}
	foutf(2,"Error: Multiply defined action in menu\n");
	return(-1);
}

static	int	menpush(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if(!*str){
		foutf(2,"Error: no symbol specified for push\n");
		return(-1);
	}
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for push\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = PUSH;
		curobj->entries[curobj->nent-1].link = syment(str,NOADD);
		return(0);
	}
	foutf(2,"Error: Multiply defined action in menu\n");
	return(-1);
}

static	int	menreturn(str)
char	*str;
{

	if(pass1){
		return(0);
	}
	if(!*str);			/*Dummy line to use str and prevent lint errors*/
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for link\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = RETURN;
		return(0);
	}
	foutf(2,"Error: Multiply defined action in menu\n");
	return(-1);
}

static	int	help()
{
	if(pass1){
		return(0);
	}
	if((curnode->type & TYPMASK) == LEADER){
		foutf(2,"Cannot use help in a 'leader'.\n");
		return(-1);
	}
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for action 'help'\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = HELP;
		return(0);
	}
	foutf(2,"Error: Multiply defined action in menu\n");
	return(-1);
}

static	int	regards(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if(!*str){
		foutf(2,"Error: no token specified for regards\n");
		return(-1);
	}
	curobj->regards = allocate((unsigned)((strlen(str)+1)*sizeof(char)));
	if(!curobj->regards){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(curobj->regards,str);
	return(0);
}

static	int	mrepeat(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if((curnode->type & TYPMASK) == LEADER){
		foutf(2,"Cannot repeat a 'leader'.\n");
		return(-1);
	}
	if(sscanf(str,"%d",&curobj->repeat) != 1){
		foutf(2,"Error: invalid repeat count: %s\n",str);
		return(-1);
	}
	return(0);
}

static	int	mredisp(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if((curnode->type & TYPMASK) == LEADER){
		foutf(2,"Cannot redisplay a 'leader'.\n");
		return(-1);
	}
	if(curobj && curobj->entries){
		curobj->entries[curobj->nent-1].redisp = 1;
	}
	else{
		foutf(2,"Redisplay must be within an entry\n");
	}
}

static	int	next(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if(!*str){
		foutf(2,"Error: no symbol specified for next\n");
		return(-1);
	}
	curobj->next = syment(str,NOADD);
	return(0);
}

static	int	start(str)
char	*str;
{
	if(!pass1){
		gotmenu = 1;
		return(0);
	}
	if(!*str){
		foutf(2,"Error: No symbol supplied for start\n");
		return(-1);
	}
	startnode = syment(str,NOADD);
	return(0);
}

static	int	pexitrg(str)
char	*str;
{
	if(!pass1){
		gotmenu = 1;
		return(0);
	}
	(void)strcpy(exitreg,str);
	return(0);
}

static	int	pexitrsp(str)
char	*str;
{
	if(!pass1){
		gotmenu = 1;
		return(0);
	}
	(void)strcpy(exitresp,str);
	return(0);
}

static	int	psitmsg(str)
char	*str;
{
	if(!pass1){
		gotmenu = 1;
		return(0);
	}
	if(*str){
		(void)strcpy(sitmsg,str);
		return(0);
	}
	foutf(2,"No sit message specified\n");
	return(-1);
}

static	int	psitrsp(str)
char	*str;
{
	if(!pass1){
		gotmenu = 1;
		return(0);
	}
	if(*str){
		(void)strcpy(sitresp,str);
		return(0);
	}
	foutf(2,"No sit response\n");
	return(-1);
}

static	int	setprompt(str)
char	*str;
{
	if(pass1){
		return(0);
	}
	if((curnode->type & TYPMASK) == LEADER){
		foutf(2,"Cannot set prompt for a 'leader'.\n");
		return(-1);
	}
	curobj->prompt = allocate((unsigned)((strlen(str)+1) * sizeof(char)));
	if(!curobj->prompt){
		foutf(2,"Out of memory\n");
		exit(-1);
	}
	(void)strcpy(curobj->prompt,str);
	return(0);
}

static	int	exitact()
{
	if(pass1){
		return(0);
	}
	if(!curobj->entries){
		foutf(2,"Error: No entry defined for action 'exit'\n");
		return(-1);
	}
	if(curobj->entries[curobj->nent-1].action < 0){
		curobj->entries[curobj->nent-1].action = EXIT;
		return(0);
	}
	foutf(2,"Error: Multiply defined action in menu\n");
	return(-1);
}

static	int	endtxt(str)
char	*str;
{
	intext = 0;
	return(0);
}
	
static	int	badinput()
{
	foutf(2,"Bad line in menu file\n");
	return(-1);
}

static	char	*allocate(size)
unsigned	size;
{
	char	*ret;

	ret = malloc(size);
	if(!ret){
		purge();
		ret = malloc(size);
	}
	return(ret);
}

static	char	*getmore(p,size)
char	*p;
unsigned	size;
{
	char	*ret;

	ret = realloc(p,size);
	if(!ret){
		purge();
		ret = realloc(p,size);
	}
	return(ret);
}

static	struct	stab	*call_stack[STACK_DEPTH];
static	int				stack_point=0;

static	push(addr)
struct stab	*addr;
{
	if(stack_point < (STACK_DEPTH - 1)){
		call_stack[stack_point++] = addr;
		return;
	}
	foutf(2,"Call stack overflow!\n");
	exit(-1);
}

static	struct stab	*call(addr)
struct stab	*addr;
{
	if(stack_point < (STACK_DEPTH - 1)){
		call_stack[stack_point++] = curmenu;
		return(addr);
	}
	foutf(2,"Call stack overflow!\n");
	exit(-1);
}

static	struct stab	*mreturn()
{
	struct	stab	*addr;

	if(stack_point > 0){
		addr = call_stack[--stack_point];
		return(addr);
	}
	foutf(2,"Call stack underflow!\n");
	exit(-1);
}

/*
** Compare a to b ignoring case.  Return 1 if they are different, 0 if
** they are the same.
*/

respcmp(a,b)
char	*a,*b;
{
	while(*a && *b && (tolower(*(a)) == tolower(*(b)))){
		++a;
		++b;
	}
	if(*a || *b){
		return(1);
	}
	return(0);
}
