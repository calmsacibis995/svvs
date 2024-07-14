#include <fcntl.h>
#include <svvs.h>

#ident	"@(#)svvs.c	1.14"

#ifdef SYSVR_2
#define	DEPFMT	"%s/%s/adm/dependency2"
#define MENFMT	"/data/svvs2.mn"
#else
#define	DEPFMT	"%s/%s/adm/dependency"
#define MENFMT	"/data/svvs.mn"
#endif

#define	MAXCMD	80	/*Max size of a line in a system() buffer*/

char	*nxtok(),*translate();
char	*mktemp(),*strcat(),*strcpy();
char	*getenv();
void	exit();

static	int	f_action();
static	int	f_d_option();
static	int	f_do_it();
static	int	f_drive_tests();
static	int	f_drive_tsect();
static	int	f_exit();
static	int	f_nothing();
static	int	f_options();
static	int	f_params();
static	int	f_redirect();
static	int	f_rep_tests();
static	int	f_section();
static	int	f_startup();

static	void	init();
char	*translate();

struct	rparse	{
	char	*key;
	int	(*funct)();
};


static	struct	rparse	keys[]={
	{	"action",f_action			},
	{	"d_option",f_d_option		},
	{	"do_it",f_do_it				},
	{	"drive_tests",f_drive_tests	},
	{	"drive_tsect",f_drive_tsect	},
	{	"exit",f_exit				},
	{	"nothing",f_nothing			},
	{	"options",f_options			},
	{	"params",f_params			},
	{	"redirect",f_redirect		},
	{	"rep_tests",f_rep_tests		},
	{	"section",f_section			},
	{	"startup",f_startup			},
};

#define	NUMKEYS	((sizeof(keys)-1)/sizeof(struct rparse))


#define	SECTION		-2

static	char	action[64] = "";
static	char	command[64] = "";
static	char	options[64] = "";
static	char	tests[5000] = "";
static	char	sections[256] = "";
static	char	sects[256] = "";
static	char	buff[5000] = "";
static	char	cmd[5000] = "";
static	char	redir[512] = "";
static	char	tester[512] = "";
static	char	testorg[512] = "";

struct	stest{
	char	*sect;
	char	val[512];
};

static	int	cursect=0;
static	struct	stest	secttest[]={
	{	"BA_ENV",""		},
	{	"BA_OS",""		},
	{	"BA_LIB",""		},
	{	"KE_OS",""		},
	{	"KE_ENV",""		},
	{	"TI_ENV",""		},
	{	"TI_CMD",""		},
	{	"TI_LIB",""		},
	{	"NS_OS",""		},
	{	"NS_LIB",""		},
	{	"NS_CMD",""		},
	{	"NS_DEV",""		},
	{	"NS_REMOTE",""	},
};

#define	NSECTS	(sizeof(secttest)/sizeof(struct stest))

main()
{
	char	mfile[256];
	char	regards[128];
	char	*response;

	foutf(2,"Please wait...");
	sv_config();
	set_init(init);		/*Set the action for an init menu */
	response = &buff[0];
	(void)strcpy(mfile,(char *)SVVSPATH);
	(void)strcat(mfile,MENFMT);
	if(loadmenus(mfile) < 0)
		exit(-1);
	foutf(2,"\n");
	while(1){
		domenu(regards,response);
		if(!strcmp(regards,"exit")){
			exit(0);
		}
		deploy(regards,response);
	}
}

static	void	init()
{
	int	i;

	redir[0] = 0;
	tests[0] = 0;
	command[0] = 0;
	options[0] = 0;
	sections[0] = 0;
	sects[0] = 0;
	action[0] = 0;
	for(i = 0; i < NSECTS; ++i){
		secttest[i].val[0] = 0;
	}
}

static	deploy(sct,str)
char	*sct;
char	*str;
{
	int	i;

	for(i = 0; i < NUMKEYS; ++i){
		if(!strcmp(sct,keys[i].key)){
			break;
		}
	}
	(*keys[i].funct)(str);
}

/*
** Begin menu handling functions.
*/

/*
** If tester and testorg are not set, prompt for them.
*/

static	int	f_startup(str)
char	*str;
{
	char	*p;

	if(!(p=getenv("TESTER"))){
		while(!tester[0]){
			foutf(2,"Enter the name of the tester: ");
			if(!getln(tester))
				exit(0);
		}
	}
	else{
		strcpy(tester,p);
	}
	if(!(p=getenv("TESTORG"))){
		while(!testorg[0]){
			foutf(2,"Enter the name of the test organization: ");
			if(!getln(testorg))
				exit(0);
		}
	}
	else{
		strcpy(testorg,p);
	}
}

/*
** Set the command to execute and the menu to return to if the command
** cannot be executed.
*/

static	int	f_action(str)
char	*str;
{
	if((!strcmp(str,"run")) || (!strcmp(str,"verify"))){
		strcpy(command,"verify");
	}
	else{
		strcpy(command,"report");
	}
	strcpy(action,str);
}

/*
** If they have answered yes, then use the -d option to verify, otherwise
** use no options at all.
*/

static	int	f_d_option(str)
char	*str;
{
	if(!strcmp(str,"show")){
		foutf(2,"Temporary files %s be preserved\n",(options[0] ? "will" : "will not"));
	}
	else if(!strcmp(str,"yes")){
		strcpy(options,"-d");
	}
	else if(!strcmp(str,"no")){
		options[0] = 0;
	}
	else{
		stayput();
	}
}

/*
** if the appropriate parameters have been set, execute the command
** currently specified.  Otherwise, go back to the main menu for the
** currently active command.  The menu's name is contained in 'action'.
*/
static	int	f_do_it(str)
char	*str;
{
	int		i;

	if((!strcmp(action,"verify")) || (!strcmp(action,"report"))){
		if(!sections[0]){
			foutf(2,"No sections specified for %s.\n",action);
			chmenu(action);
		}
		else{
			normal_cmd();
		}
	}
	else{
		for(i = 0; i < NSECTS; ++i){
			if(secttest[i].val[0]){
				run_cmd();
				return;
			}
		}
		foutf(2,"No tests specified for individual testing.\n");
		chmenu(action);
	}
}

/*
** Set cursect to the index into the section test list they have specified
*/

static	int	f_drive_tsect(str)
char	*str;
{
	int	i;

	if(!strcmp(str,"clear")){
		for(i = 0; i < NSECTS; ++i){
			secttest[i].val[0] = 0;
		}
	}
	else if(!strcmp(str,"show")){
		for(i = 0; i < NSECTS; ++i){
			if(secttest[i].val[0]){
				foutf(2,"%s: %s\n",secttest[i].sect,secttest[i].val);
			}
		}
		sit();
	}
	else{
		for(cursect = 0; cursect < NSECTS; ++cursect){
			if(!strcmp(secttest[cursect].sect,str)){
				sectionis(secttest[cursect].sect);
				return;
			}
		}
		foutf(2,"Unrecognized section: %s\n",str);
		stayput();
	}
}

/*
** Add a test they have selected to the list of tests to be used for
** the current section.
*/

static	int	f_drive_tests(str)
char	*str;
{
	if(!strcmp(str,"help")){
		showtests();
	}
	else if(!strcmp(str,"clear")){
		secttest[cursect].val[0] = 0;
	}
	else if(!strcmp(str,"show")){
		foutf(2,"%s: %s\n",secttest[cursect].sect,secttest[cursect].val);
	}
	else{
		strcat(secttest[cursect].val,str);
		strcat(secttest[cursect].val," ");
	}
	return(0);
}

/*
** In case a menu regarding nothing actually returns a value, ignore
** it.
*/

static	int	f_nothing(str)
char	*str;
{
	return;
}

/*
** absorb options into the list of active options.
*/

static	int	f_options(str)
char	*str;
{
	char	*p;

	if(!strcmp(str,"show")){
		if(*options){
			foutf(2,"%s\n",options);
		}
		return;
	}
	if(!strcmp(str,"clear")){
		*options = 0;
		return;
	}
	p = &options[0];
	if(!*p){
		*(p++) = '-';
		*p = 0;
	}
	while(*p){
		if(*(p++) == *str)
			return;
	}
	(void) strcat(options,str);
}

/*
** This function is only called with 'show'.  Show all of the currently
** active parameters.
*/

static	int	f_params(str)
char	*str;
{
	if(!strcmp(action,"report")){
		rep_show();
	}
	else if(!strcmp(action,"run")){
		run_show();
	}
	else{
		vfy_show();
	}
}

rep_show()
{
	foutf(2,"Command:      report\n");
	foutf(2,"Options:      %s\n",(*options ? options : "none selected"));
	foutf(2,"Sections:     %s\n",(*sections ? sections : "none selected"));
	foutf(2,"Single Tests: %s\n",(*tests ? tests : "none selected"));
	foutf(2,"Redirection:  %s\n",(*redir ? redir : "none"));
}

run_show()
{
	int		i,found=0;

	foutf(2,"Command:      verify\n");
	foutf(2,"Tests:        ");
	for(i = 0; i < NSECTS; ++i){
		if(secttest[i].val[0]){
			foutf(2,"\n    %s: %s",secttest[i].sect,secttest[i].val);
			found = 1;
		}
	}
	if(!found){
		foutf(2,"none specified\n");
	}
	else{
		foutf(2,"\n");
	}
	foutf(2,"Redirection:  %s\n",(*redir ? redir : "none"));
	foutf(2,"Temporary files %s be preserved\n",(*options ? "will" : "will not"));
	sit();
}

vfy_show()
{
	foutf(2,"Command:      verify\n");
	foutf(2,"Options:      %s\n",(*options ? options : "none selected"));
	foutf(2,"Sections:     %s\n",(*sections ? sections : "none selected"));
}

/*
** Set up a temporary file for redirection of output if the answer was
** yes, otherwise do not redirect output.
*/

static	int	f_redirect(str)
char	*str;
{
	if(!strcmp(str,"yes")){
		(void) sprintf(buff, "%s/tmp/svvs_XXXXXX",SVVSPATH);
		(void) sprintf(redir, "%s", mktemp(buff));
		foutf(2,"Output will be directed to: %s\n",redir);
	}
	else if(!strcmp(str,"no")){
		redir[0] = 0;
	}
	else{
		stayput();
	}
}

/*
** Report on individual tests.
*/

static	int	f_rep_tests(str)
char	*str;
{
	if(!strcmp(str,"help")){
		showtests();
		return;
	}
	else if(!strcmp(str,"show")){
		if(*tests){
			foutf(2,"%s\n",tests);
		}
	}
	else if(!strcmp(str,"clear")){
		*tests = 0;
	}
	else{
		strcat(tests,str);
		strcat(tests," ");
	}
}

/*
** Absorb the sections into the list of sections to be used.
*/

static	int	f_section(str)
char	*str;
{
	static	char	token[100];

	if(!strcmp(str,"show")){
		if(*sections){
			foutf(2,"%s\n",sections);
		}
	}
	else if(!strcmp(str,"clear")){
		*sections = 0;
		*sects = 0;
	}
	else{
		while(*str){
			str = nxtok(token,str);
			absorb(sections,token);
		}
		sectionis(sections);
	}
}

static	f_exit(str)
char	*str;
{
	char	*p;

	p = str;
	exit(0);
}

/*
** Begin utility functions used by menu handling functions.
*/

/*
** function to set the list of sections to be used by the showtests()
** function to show what tests are available in the sections in question.
*/
sectionis(sect)
char	*sect;
{
	strcpy(sects,sect);
}

/*
** Function to run normal section oriented commands like verify <section>
** or report.
*/
normal_cmd()
{
	strcpy(buff,command);
	if(*options){
		strcat(buff," ");
		strcat(buff,options);
	}
	strcat(buff," ");
	strcat(buff,sections);
	if(*tests){
		strcat(buff," -t ");
		strcat(buff,tests);
	}
	if(*redir){
		strcat(buff," > ");
		strcat(buff,redir);
	}
	breakup(cmd,buff);
	foutf(2,"%s\n",buff);
	system(cmd);
}

/*
** Function to run individual tests through verify.
*/
run_cmd()
{
	int	i;

	strcpy(buff,command);
	if(*options){
		strcat(buff," ");
		strcat(buff,options);
	}
	for(i = 0; i < NSECTS; ++i){
		if(*secttest[i].val){
			strcat(buff," ");
			strcat(buff,translate(secttest[i].sect));
			strcat(buff,"/");
			strcat(buff,secttest[i].val);
		}
	}
	if(*tests){
		strcat(buff," -t ");
		strcat(buff,tests);
	}
	if(*redir){
		strcat(buff," > ");
		strcat(buff,redir);
	}
	breakup(cmd,buff);
	foutf(2,"%s\n",buff);
	system(cmd);
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
** Function to display the help message for individual tests and then
** list the tests available.
*/

showtests()
{
	static	char	token[256],token1[256];
	char	*p,*q;
	char	*get_help();
	int		fd;

	foutf(2,"\n%s\n\n",get_help());
	sit();
	p = nxtok(token,sects);
	while(*token){
		testhelp(token);
		while((setsect(token1,token,SVVSPATH) == SECTION) && *token1){
			sprintf(buff,DEPFMT,SVVSPATH,token1);
			if((fd = open(buff,O_RDONLY)) < 0){
				foutf(2,"\nNo tests for section %s\n",token1);
				continue;
			}
			while(fgetln(fd,buff)){
				q = nxtok(token1,buff);
				q = nxtok(token1,q);
				print(token1);
			}
			(void)close(fd);
			(void)rstcache(fd);
		}
		initprint();
		foutf(2,"\n\n");
		sit();
		foutf(2,"\n\n");
		p = nxtok(token,p);
		rstsect();
	}
}

/*
** Function to print a formatted line of test names and, if necessary
** sit() to prevent scolling past the screen.
*/

static	int	line = 0;
static	int	col = 0;

print(str)
char	*str;
{
	int	i,space=0;
	int	pos;

	if(col > 3){
		col = 0;
		foutf(2,"\n");
		if((++line) > 15){
			sit();
			line = 0;
		}
	}
	for(pos=0; pos<20; ++pos){
		if(*str){
			write(2,str++,1);
		}
		else{
			foutf(2," ");
		}
	}
	col++;
}

initprint()
{
	col = 0;
	line = 0;
}

testhelp(sect)
char	*sect;
{
	foutf(2,"\nThe names of the tests in section %s are:\n\n",sect);
}

/*
** nxtok() gets the next token (delimited by white space) from src
** and places it in dst. It then returns a pointer to the next token
** after the one it got.  If no more tokens, it returns a pointer to
** the end of src.
*/

static	char	*nxtok(dst,src)
char	*src,*dst;
{

	while(*src && (whitesp(*(src)))) ++src;
	if(*src != '\"')
		while(*src && (!whitesp(*(dst) = *(src++)))) ++dst;
	else{
		++src;
		while(*src &&( (*dst = *(src++)) != '\"')) ++dst;
	}
	while(*src && (whitesp(*(src)))) ++src;
	*dst = 0;
	return(src);
}

static	int	whitesp(c)
char	c;
{
	return((c == ' ') || (c == '	') || (c == '\n'));
}

static	int	absorb(dst,src)
char	*dst,*src;
{
	char	*p,token[256];

	p = nxtok(token,dst);
	while(*token){
		if(!strcmp(token,src))
			return;
		p = nxtok(token,p);
	}
	(void) strcat(dst,src);
	(void) strcat(dst," ");
}

/*
** char	*translate(svid)
** char	*svid;
**
** Function to translate svid section names into svvs section names.
** args:
**		svid	- string containing the svid section name
**
** return:
**		A pointer to a string containing the svvs section name.
*/

struct	xlate{
	char	*svid;
	char	*svvs;
};

struct	xlate	xlation[]={
	{	"BA_ENV","BA/ENV"		},
	{	"BA_OS","BA/OS"			},
	{	"BA_LIB","BA/LIB"		},
	{	"KE_OS","KE/OS"			},
	{	"KE_ENV","KE/ENV"		},
	{	"TI_ENV","TI/ENV"		},
	{	"TI_CMD","TI/CMD"		},
	{	"TI_LIB","TI/LIB"		},
	{	"NS_OS","NS/OS"			},
	{	"NS_LIB","NS/LIB"		},
	{	"NS_CMD","NS/CMD"		},
	{	"NS_DEV","NS/DEV"		},
	{	"NS_REMOTE","NS/REMOTE"	},
};

#define	NXLATE	(sizeof(xlation)/sizeof(struct	xlate))


char	*translate(section)
char	*section;
{
	int		i;

	for(i = 0; i < NXLATE; ++i){
		if(!strcmp(section,xlation[i].svid)){
			return(xlation[i].svvs);
		}
	}
	return(section);
}
