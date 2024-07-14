#include <sys/types.h>
#include <sys/stat.h>

#ident	"@(#)instmen.c	1.19"

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#ifndef SEEK_SET
#define	SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR 1
#endif
#ifndef SEEK_END
#define	SEEK_END 2
#endif

#define	NSLIBS	"src/tools/z_nse "
#define	TILIBS	"src/tools/z_tie "

char	*malloc();
char	*nxtok();

int	f_source();
int	f_dest();
int	f_comp();
int	f_options();
int	f_libs();
int	f_release();
int	f_exten();
int	f_sections();
int	f_exit();
int	f_badinput();
int	f_wantlibs();
int	f_wantcmds();
int	f_nslibs();
int	f_tilibs();
int	f_check();
int	f_splibs();
int	f_spsect();
int	f_cursect();
int	f_null();

char	*translate();
char	*strcat(),*strcpy(),*getenv();
void	exit();

#define	RBUFSZ	512	/*Size of a read buffer*/

extern	long	lseek();
long	svseek();

struct	rdbuf {
	long	bufpos;
	int		pos;
	int		len;
	char	buf[RBUFSZ];
};

static	struct	rdbuf	rbuffs[] = {
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
};
#define	MAXFD	((sizeof(rbuffs) -1 )/ sizeof(struct rdbuf))

struct	pstruct	{
	char	*value;
	int	(*funct)();
};

static struct	pstruct	keys[] = {
	{	"source",f_source	},
	{	"destination",f_dest,	},
	{	"compiler",f_comp	},
	{	"options",f_options	},
	{	"libraries",f_libs	},
	{	"sp_libs",f_splibs	},
	{	"spec_sect",f_spsect	},
	{	"cursect",f_cursect	},
	{	"release",f_release	},
	{	"extensions",f_exten	},
	{	"sections",f_sections	},
	{	"exit",f_exit			},
	{	"wantlibs",f_wantlibs	},
	{	"wantcmds",f_wantcmds	},
	{	"ns_libs",f_nslibs		},
	{	"ti_libs",f_tilibs		},
	{	"check",f_check			},
	{	"nothing",f_null		},
	{	"Bad Input",f_badinput	},
};

#define	NUMKEYS	((sizeof(keys) / sizeof(struct pstruct))-1)

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
	{	"NS_LOCAL","NS/LOCAL"	},
};

#define	NXLATE	(sizeof(xlation)/sizeof(struct	xlate))

struct	sp_libs {
	char	*sect;
	char	libs[512];
};

struct	sp_libs	specs[] = {
	{	"BA",""			},
	{	"KE",""			},
	{	"TI",""			},
	{	"NS",""			},
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
	{	"NS_LOCAL",""	},
};

#define	NSECT	(sizeof(specs)/sizeof(struct sp_libs))

int		curspec = 0;

struct	settings{
	char	source[512];
	char	dest[512];
	char	compiler[512];
	char	options[512];
	char	libs[512];
	char	release[512];
	char	extensions[512];
	char	sections[512];
	int		nsl;
	int		til;
	int		bldlibs;
	int		bldcmds;
};

static	struct	settings setup = {
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	0,
	0,
	1,
	1,
};

static	int	reset;
static	char	prevmenu[512];

int	shell;	/*Tell the menu system whether a shell escape happened*/

main(argc,argv)
int	argc;
char	*argv[];
{
	char	mfile[256];
	char	regards[128];
	char	response[128];
	int		i;

	if(argc < 2){
		(void)strcpy(mfile,".");
	}
	else{
		(void)strcpy(mfile,argv[1]);
	}
	(void)strcat(mfile,"/data/install.mn");
	if(loadmenus(mfile) < 0)
		exit(-1);
	while(1){
		domenu(regards,response);
		if(reset = strcmp(prevmenu,regards)){
			(void)strcpy(prevmenu,regards);
		}
		if(!strcmp(response,"quit")){
			foutf(2,"Leaving with no action taken...\n");
			exit(-1);
		}
		act(regards,response);
	}
}

act(routine,value)
char	*routine,*value;
{
	int	i;
	for(i = 0; (i < NUMKEYS) && strcmp(routine,keys[i].value); ++i);
	(*keys[i].funct)(value);
}

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

/*
** Routines called in response to the various menus
*/

int	f_source(str)
char	*str;
{
	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",setup.source);
		return(0);
	}
	if(testfile(str)){
		if(!testdir(str)){
			foutf(2,"Warning: %s is not a directory.\n",str);
		}
	}
	else
		foutf(2,"Warning: %s does not exist on this machine.\n",str);
	(void)strcpy(setup.source,str);
}

int	f_dest(str)
char	*str;
{
	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",setup.dest);
		return(0);
	}
	if(testfile(str)){
		if(!testdir(str)){
			foutf(2,"Warning: %s is not a directory.\n",str);
		}
	}
	else
		foutf(2,"Warning: %s does not exist on this machine.\n",str);
	(void)strcpy(setup.dest,str);
}

int	f_comp(str)
char	*str;
{
	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",*setup.compiler ? setup.compiler : "cc");
		return(0);
	}
	(void)strcpy(setup.compiler,str);
}

int	f_options(str)
char	*str;
{
	char	token[128];

	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",setup.options);
		return(0);
	}
	if(!strcmp(str,"clear")){
		*setup.options = 0;
		return;
	}
	if(reset){
		*setup.options = 0;
		reset = 0;
	}
	while(*str){
		str = nxtok(token,str);
		absorb(setup.options,token);
	}
}

int	f_libs(str)
char	*str;
{
	char	token[128];

	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",setup.libs);
		return(0);
	}
	if(!strcmp(str,"clear")){
		*setup.libs = 0;
		return;
	}
	if(reset){
		*setup.libs = 0;
		reset = 0;
	}
	while(*str){
		str = nxtok(token,str);
		absorb(setup.libs,token);
	}
}

int	f_release(str)
char	*str;
{
	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",*setup.release ? setup.release : "3");
		return(0);
	}
	if(!strcmp(str,"3")){
		setup.nsl = 1;
		setup.til = 1;
	}
	(void)strcpy(setup.release,str);
}

int	f_exten(str)
char	*str;
{
	char	token[128];

	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",setup.extensions);
		return(0);
	}
	if(!strcmp(str,"clear")){
		*setup.extensions = 0;
		return;
	}
	if(reset){
		*setup.extensions = 0;
		reset = 0;
	}
	while(*str){
		str = nxtok(token,str);
		absorb(setup.extensions,token);
	}
}

int	f_sections(str)
char	*str;
{
	char	token[128];

	if(!strcmp(str,"sHoW")){
		foutf(2,"%s\n",setup.sections);
		return(0);
	}
	if(!strcmp(str,"clear")){
		*setup.sections = 0;
		return;
	}
	if(reset){
		*setup.sections = 0;
		reset = 0;
	}
	while(*str){
		str = nxtok(token,str);
		absorb(setup.sections,token);
	}
}

f_wantlibs(str)
char	*str;
{
	if(!strcmp(str,"no")){
		setup.bldlibs = 0;
		foutf(2,"SVVS Libraries will NOT be built\n");
	}
	else if(!strcmp(str,"yes")){
		setup.bldlibs = 1;
		foutf(2,"SVVS libraries will be built\n");
	}
	else{
		stayput();
	}		
}

f_nslibs(str)
char	*str;
{
	if(!strcmp(str,"no")){
		setup.nsl = 0;
		foutf(2,"SVVS NS Libraries will NOT be built\n");
	}
	else if(!strcmp(str,"yes")){
		setup.nsl = 1;
		foutf(2,"SVVS NS libraries will be built\n");
	}		
	else{
		stayput();
	}		
}

f_tilibs(str)
char	*str;
{
	if(!strcmp(str,"no")){
		setup.til = 0;
		foutf(2,"SVVS TI Libraries will NOT be built\n");
	}
	else if(!strcmp(str,"yes")){
		setup.til = 1;
		foutf(2,"SVVS TI libraries will be built\n");
	}		
	else{
		stayput();
	}		
}


f_wantcmds(str)
char	*str;
{
	if(!strcmp(str,"no")){
		setup.bldcmds = 0;
		foutf(2,"SVVS commands will NOT be built\n");
	}
	else if(!strcmp(str,"yes")){
		setup.bldcmds = 1;
		foutf(2,"SVVS commands will be built\n");
	}		
	else{
		stayput();
	}		
}

int	f_check()
{
	int		i;

	if(*setup.source) foutf(2,"source %s\n",setup.source);
	if(*setup.dest) foutf(2,"destination %s\n",setup.dest);
	if(*setup.compiler) foutf(2,"compiler %s\n",setup.compiler);
	if(*setup.options) foutf(2,"options %s\n",setup.options);
	if(*setup.libs) foutf(2,"libraries %s\n",setup.libs);
	for(i = 0; i < NSECT; ++i){
		if(specs[i].libs[0]){
			foutf(2,"libraries :%s %s\n",specs[i].sect,specs[i].libs);
		}
	}
	if(*setup.release) foutf(2,"release %s\n",setup.release);
	if(*setup.extensions) foutf(2,"extensions %s\n",setup.extensions);
	if(setup.nsl || setup.til ||setup.bldlibs ||
	  setup.bldcmds || *setup.sections)
		foutf(2,"sections ");
	if(setup.nsl) foutf(2,NSLIBS);
	if(setup.til) foutf(2,TILIBS);
	if(setup.bldlibs) foutf(2,"src/tools ");
	if(setup.bldcmds) foutf(2,"src ");
	if(*setup.sections) foutf(2,"%s\n",setup.sections);
	else foutf(2,"\n");
	sit();
}

int	f_exit()
{
	int		i;
	char	tsect[512],token[50],*p;

	if(*setup.source) outf("source %s\n",setup.source);
	else foutf(2,"No source directory specified!\n");
	if(*setup.dest) outf("destination %s\n",setup.dest);
	else foutf(2,"No destination directory specified!\n");
	if(*setup.compiler) outf("compiler %s\n",setup.compiler);
	else outf("compiler cc\n");
	outf("options %s\n",setup.options);
	outf("libraries %s\n",setup.libs);
	for(i = 0; i < NSECT; ++i){
		if(specs[i].libs[0]){
			outf("libraries :%s %s\n",translate(specs[i].sect),specs[i].libs);
		}
	}
	if(!*setup.release){
		strcpy(setup.release,"3");
	}
	outf("release %s\n",setup.release);
	outf("extensions %s\n",setup.extensions);
	outf("sections ");
	if(setup.nsl) outf(NSLIBS);
	if(setup.til) outf(TILIBS);
	if(setup.bldlibs) outf("src/tools ");
	if(setup.bldcmds) outf("src ");
	p = &setup.sections[0];
	tsect[0] = 0;
	while(*p){
		p = nxtok(token,p);
		strcat(tsect,translate(token));
		strcat(tsect," ");
	}
	if(*tsect) outf("%s\n",tsect);
	else outf("\n");
	exit(0);
}

int	f_spsect(str)
char	*str;
{
	int		i;

	if(!strcmp(str,"clear")){
		for(i = 0; i < NSECT; ++i){
			specs[i].libs[0] = 0;
		}
	}
	else if(!strcmp(str,"sHoW")){
		for(i = 0; i < NSECT; ++i){
			if(specs[i].libs[0] == ' '){
				foutf(2,"%s: none\n",specs[i].sect);
			}
			else if(specs[i].libs[0]){
				foutf(2,"%s: %s\n",specs[i].sect,specs[i].libs);
			}
		}
		sit();
	}
	else{
		for(curspec = 0; curspec < NSECT; ++curspec){
			if(!strcmp(specs[curspec].sect,str)){
				return;
			}
		}
		foutf(2,"Unrecognized section: %s\n",str);
		stayput();
	}
}

int	f_splibs(str)
char	*str;
{
	if(!strcmp(str,"clear")){
		specs[curspec].libs[0] = 0;
	}
	else if(!strcmp(str,"sHoW")){
		if(specs[curspec].libs[0] == ' '){
			foutf(2,"%s: none\n",specs[curspec].sect);
		}
		else{
			foutf(2,"%s\n",specs[curspec].libs);
		}
	}
	else if(!strcmp(str,"none")){
		strcpy(specs[curspec].libs," ");
		return;
	}
	else if(specs[curspec].libs[0] == ' '){
		strcpy(specs[curspec].libs,str);
		strcat(specs[curspec].libs," ");
	}
	else{
		strcat(specs[curspec].libs,str);
		strcat(specs[curspec].libs," ");
	}
	return(0);
}

f_cursect(str)
char	*str;
{
	char	*p;

	p = str;	/* suppress lint message for unused argument */
	foutf(2,"Section: %s\n",specs[curspec].sect);
}

int	f_null(str)
char	*str;
{
	return(0);
}

int	f_badinput(str)
char	*str;
{
	foutf(2,"Bad Input: %s\n",str);
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

int	testfile(path)
char	*path;
{
	return(!access(path,0));
}

int	testdir(path)
char	*path;
{
	struct	stat	buff;

	return((!stat(path,&buff)) && (buff.st_mode & 040000));
}


/*
** nxtok() gets the next token (delimited by white space) from src
** and places it in dst. It then returns a pointer to the next token
** after the one it got.  If no more tokens, it returns a pointer to
** the end of src.
*/

char	*nxtok(dst,src)
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

int	whitesp(c)
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
	(void)strcat(dst,src);
	(void)strcat(dst," ");
}

int	nonblank(str)
char	*str;
{
	while(*str && whitesp(*str)){
		++str;
	}
	return(*str);
}

int	fgetln(fd,str)
int	fd;
char	*str;
{
	int	rval;
	char	*p;

	p = str;
	while((rval = bytread(fd,str)) && (*str != '\n'))
		++str;
	*str = 0;
	return(rval || !(p == str));
}

int	getln(str)
char	*str;
{
	int	rval;
	char	*p,*q;

	shell = 0;	/*Reset shell escape flag*/
	q = p = str;
	while((rval = read(0,str,(unsigned)1)) && (*str != '\n'))
		++str;
	*str = 0;
	while(*q && ((*q == ' ') || (*q == '	')))
		q++;
	if(*q == '!'){
		shell = 1;	/*Set shell escape flag*/
		doshell(++q);
		*p = 0;
	}
	return((rval || (p != str)) ? 1 : 0);
}


static	bytread(fd,ptr)
int	fd;
char	*ptr;
{
	if(fd < 0 ||  fd > MAXFD){
		foutf(2,"Invalid file descriptor for input file\n");
		return(0);
	}
	if(rbuffs[fd].pos < rbuffs[fd].len){
		*ptr = rbuffs[fd].buf[rbuffs[fd].pos++];
	}
	else{
		rbuffs[fd].bufpos += rbuffs[fd].len;
		if(!(rbuffs[fd].len = read(fd,&rbuffs[fd].buf[0],RBUFSZ))){
			return(0);
		}
		rbuffs[fd].pos = 0;
		*ptr = rbuffs[fd].buf[rbuffs[fd].pos++];
	}
	return(1);
}

long	svseek(fd,offset,whence)
int	fd;	/*File descriptor*/
long	offset;	/*Offset in file*/
int	whence;	/*Offset from where?*/
{
	if((fd < 0) || (fd > MAXFD)){
		return((long)-1);
	}
	if(whence == SEEK_CUR){
		offset += ((long)rbuffs[fd].pos + rbuffs[fd].bufpos);
		whence = SEEK_SET;
	}
	rbuffs[fd].len = 0;
	rbuffs[fd].pos = RBUFSZ;
	rbuffs[fd].bufpos = lseek(fd,offset,whence);
	return(rbuffs[fd].bufpos);
}

rstcache(fd)
int	fd;
{
	rbuffs[fd].bufpos = (long)0;
	rbuffs[fd].pos = RBUFSZ;
	rbuffs[fd].len = 0;
	*rbuffs[fd].buf = 0;
}

doshell(str)
char	*str;
{
	int	oldsout;

	oldsout = dup(1);
	close(1);
	if(dup(2) != 1){
		foutf(2,"Dup failed, standard output was lost!\n");
		foutf(2,"Do not use shell escape until dup is fixed!\n");
		exit(-1);
	}
	(void)system(str);
	close(1);
	if(dup(oldsout) != 1){
		foutf(2,"Dup failed, standard output was lost!\n");
		foutf(2,"Do not use shell escape until dup is fixed!\n");
		exit(-1);
	}
	close(oldsout);
}

