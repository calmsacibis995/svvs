#include	<signal.h>
#include	<fcntl.h>
#include	<svvs.h>

#ident	"@(#)verify.c	1.15"

struct tfail			/* FOR each FAIL TABLE entry */
{
    char	p_name[20];	/* Name of test */
    int		stat;		/* Status of last test */
};

struct	jlnparse {
	char	*key;		/*Keyword to invoke an action*/
	int		(*func)();	/*Action to be invked in respone to keyword*/
};

/*Macro to establish any relationship between user and root */
/*
** Lint will complain of pointer problems because ROOT_[UG]ID are
** defined as "&(int *)object", while object is declared "pointer object".
** ("pointer" is a typdef for "char *")
*/
#define	ANYROOT ((getuid() == ROOT_UID) || (geteuid() == ROOT_UID) || (getgid() == ROOT_GID) || (getegid() == ROOT_GID))

/*Constants*/

#define FTABSZ 300
#define LMAX 256
#define SECTION -2
#define	TEST	-1

#ifndef SUBTMP
#define	SUBTMP	"yatd"
#endif

#ifdef SYSVR_2
#define	DEPFMT	"%s/%s/adm/dependency2"
#else
#define	DEPFMT	"%s/%s/adm/dependency"
#endif

/*external functions */

extern	void	exit();
extern	char *ctime(), *getenv(),*strcpy(),*strcat();
extern	long svseek();
extern	long time();
extern	unsigned	short	getuid();
extern	unsigned	short	geteuid();
extern	unsigned	short	getgid();
extern	unsigned	short	getegid();

char	*getword(), *gttst(), *getdep(),*nxtok();
int		manstart(),runbeg(),problem();

/*Data */

extern	int	errfound;	/*Flag set by sort to indicate that a*/
				/*test reported errors*/

static	long	begtim;		/*Man Page Start time*/
static	long	endtim;		/*Man Page End time*/
static	long	runstart;	/*Run start date*/
static	int		vmode;		/* verify by sect or test */

int	iopt=0;			/* ignore dependencies*/
int	copt=0;			/* continuation run */
int	lopt=0;			/* don't filter */
int	topt=0;			/* correction run */
int	dopt=0;			/* don't cleanup */
int	ropt=0;			/* remove journal before verification*/

static	int	jrndesc = -1;	/*File descriptor of journal file*/
static	int	tmpdesc = -1;	/*File descriptor of temp file*/
static	int	ctldesc = -1;	/*File descriptor of control file*/
static	int	failsz = 0;		/*The actual size of the fail table*/
static	int	lstrun = 1;		/*The run number of the most journal run*/

static	char	species[30];	/*The type of the run, "initial" etc.*/
static	char	section[100];	/*Name of the section to be tested*/
static	char	tmpbuff[LMAX];	/*Temporary buffer for sprintf*/
static	char	control[LMAX];	/*The path to the control file*/
static	char	tmpjourn[LMAX];	/*The path to the temporary file*/
static	char	journal[LMAX];	/*The path to the journal file*/
static	char	tester[LMAX];	/*The name of the tester*/
static	char	testorg[LMAX];	/*The name of the test organiztion*/
static	char	manpage[30] = ""; /*The name of the last manpage in the
									previous run after loadftab runs*/
static	char	tstnam[30];		/*Name of the current test*/
static	char	tstloc[100];	/*Section where current test resides*/

static	struct	tfail	failtab[FTABSZ];

struct	jlnparse	actions[] = {
	{	"BegRun",runbeg		},
	{	"BegMan",manstart	},
	{	"SEVERR",problem	},
	{	"ERROR",problem	},
	{	"WARNING",problem	},
};

#define	NKEYS	(sizeof(actions)/sizeof(struct jlnparse))

main (argc, argv)
int	argc;
char	*argv[];
{
	static	char	test[LMAX];		/*Keep it off of the stack*/
	short	i = 1;
	int		tested = 0;
	char	*p;

	outf("Please Wait...\n");
	stsigs();
	sv_config();
	for(i = 1; i < argc; i++){		/* get the options*/
		if(*(argv[i]) == '-'){
			p = argv[i];
			while (*++p){
				switch (*p){
				case 'c':
					copt++;
					break;
				case 'd':
					dopt++;
					break;
				case 'i':
					iopt++;
					break;
				case 't':
					topt++;
					break;
				case 'l':
					lopt++;
					break;
				case 'r':
					ropt++;
					break;
				default:
					outf(
					"Usage: verify [ -[c][d][i][t][l][r] ] section\n");
					outf(
					"\t\tOR: verify [ -d ] testnames\n");
					exit(1);
				}
			}
		}
	}
	for(i = 1; i < argc; ++i){
		rstsect();			/*Reset the setsect flags*/
		if(*(argv[i]) != '-'){
			vmode = setsect(section,argv[i],SVVSPATH);
			if (vmode == SECTION){		
				while(*section){
					if(ropt){
						(void)sprintf(tmpbuff,"rm -f %s/%s/adm/journal",
							SVVSPATH,section);
						(void)system(tmpbuff);
					}
					sectest();
					(void)setsect(section,argv[i],SVVSPATH);
				}
				tested = 1;
			}
			else if(vmode == TEST){
				while (i < argc){
					breakup(section,test,argv[i++]);
					(void)extest(test);
					outf("\n");
					tested = 1;
				}
			}
		}
	}
	if(!tested){
		outf("Usage: verify [ -[c][d][i][t][l] ] section\n");
		outf("\t\tOR: verify [ -d ] testnames\n");
		exit(1);
	}
	exit(0);
/*NOTREACHED*/
}

breakup(path,tst,str)
char	*path,*tst,*str;
{
	char	*p,*q;

	q = p = str;
	while(*p){
		if(*p == '/'){
			q = p;
		}
		p++;
	}
	if(q != str){
		*(q++) = 0;
		(void)strcpy(path,str);	/*Don't change path if none was there*/
		(void)strcat(path,"/");
	}
	(void)strcpy(tst,q);
}

didfail (tst)
char	*tst;
{
	struct tfail	*p;

	for (p = failtab; p < &failtab[failsz]; p++){
		if (strcmp (p->p_name, tst) == 0){
			return (1);
		}
	}
	return(0);
}

/**
 **
 **	invoke() will execute an SVVS test based on the assumption
 **	that the test executable is in the current bin.
 **
 **/

#ifndef	X_OK
#define	X_OK 1
#endif

invoke(tst)
char	*tst;
{
	int	stat;
	int	pid, w;

	if(access(tst,X_OK)){
		return(-1);
	}
	if (!(pid = fork()))
	{
		(void)signal(SIGUSR2,SIG_DFL);
		if(vmode == SECTION)		/* redirect output ? */
		{
			(void)close(1);
			(void)close(2);
			(void)dup(tmpdesc);		/* std out */
			(void)dup(tmpdesc);		/* std err */
		}
		(void)execl (tst, tst, (char*) 0);
		kill(getpid(),SIGUSR2);
		while(1);	/*Make sure we don't continue from here*/
	}
	while ((w = wait (&stat)) != pid && w != -1);
	if(w == -1){		/*Wait received a signal*/
		return(-9);
	}
	if((stat & 0177) == 0){		/* Normal Exit*/
		return(0);
	}
	else{				/*Child received a signal*/
		return(((stat&0177) == SIGUSR2) ? -1 :(-(stat&0177)));
	}
}

sectest()
{
	rootcheck();
	gttster();
	if(files())
		return;
	if(begjourn()){
		return;
	}
	if(begrun()){
		return;
	}
	while(gttst(tstnam,tstloc)){
		foutf(jrndesc,"BegMan %s\n", tstnam);
		if(iopt || !deptest()){
			(void)dotest();
		}
		(void)sprintf(tmpbuff,"EndMan %d\n",(int)(endtim-begtim));
		foutf(jrndesc,"%s",tmpbuff);
	}
	close(jrndesc);
	close(ctldesc);
}

rootcheck()
{
	if(ANYROOT){
		foutf(2,"Tests should be run by someone logged in\n");
		foutf(2,"as a non-superuser to prevent severe system\n");
		foutf(2,"damage.\n");
		exit(-1);
	}
}

gttster()
{
	char	*ptr;

	ptr = getenv("TESTER");
	if(!ptr)
		*tester = 0;
	else
		(void)strcpy(tester, ptr);

	ptr = getenv("TESTORG");

	if(!ptr)
		*testorg = 0;
	else
		(void)strcpy(testorg, ptr);
}

files()
{
	(void)sprintf(control,DEPFMT,SVVSPATH, section);
	if((ctldesc = open(control,O_RDONLY)) < 0)
	{
		outf("verify: can't open %s\n",control);
		return(-1);
	}
	(void)sprintf(journal,"%s/%s/adm/journal",SVVSPATH, section);
	if((jrndesc = open(journal,O_RDWR|O_CREAT, 0666)) < 0)
	{
		outf("verify: can't open or create %s\n",journal);
		return(-1);
	}
	return(0);
}

int	begjourn()
{
	static	char	jline[LMAX];	/*Keep it off of the stack*/
	char	*rundate,*p;
	if(!fgetln(jrndesc,jline)){	/*Set up new journal*/
		if(copt || topt){
			foutf(2,"Cannot correct/continue empty journal\n");
			return(-1);
		}
		(void)time(&runstart);
		rundate = ctime(&runstart);	/* there was a '\n' */
		*(rundate+24) = 0;
		rundate += 4;
		lstrun = 1;
		(void)sprintf(tmpbuff,"%s %4d %s \"%s\" \"%s\" %s %s \"%s\"\n",
			"BegJourn",
			lstrun,
			SVVSVERSION,
			MACHINE,
			RELEASE,
			rundate,
			section,
			testorg);
		foutf(jrndesc,"%s",tmpbuff);
		(void)strcpy(species,"initial");
		return(0);
	}
	if(svseek(jrndesc,(long)0,0) < (long) 0){
		foutf(2,"Error seeking to beginning of journal file\n");
		return(-1);
	}
	lstrun = runno(&jline[9]) + 1;
	p = &jline[14];
	(void)sprintf(tmpbuff,"%s %4d %s\n","BegJourn",lstrun,p);
	foutf(jrndesc,"%s",tmpbuff);
	return(runtype());
}

int	runtype()
{
	if(copt){
		(void)strcpy(species,"continuation ");
		if(loadftab()){
			return(-1);
		}
		skipdone(manpage);
		return(0);
	}
	if(topt)
	{
		(void)strcpy(species,"correction ");
		return(loadftab());
	}
	(void)strcpy(species,"regression ");
	return(0);
}

begrun()
{
	char	*rundate;

	(void)time(&runstart);
	rundate = ctime(&runstart);
	*(rundate+24) = 0;
	rundate += 3;
	if(svseek(jrndesc, (long) 0, (int) 2) < (long)0){
		foutf(2,"Could not seek to end of journal file.\n");
		return(-1);
	}
	(void)sprintf(tmpbuff,"BegRun %d %s %s %s \"%s\"\n",lstrun,species,( lopt ? "long" : "short"),rundate,tester);
	foutf(jrndesc,"%s",tmpbuff);
	return(0);
}

int	deptest()
{
	char	*depnam;

	while(depnam = getdep()){
		if(didfail(depnam)){
			foutf(jrndesc,"MESSAGE\n\t %s depends on %s which failed\n",
											tstnam, depnam);
			foutf(jrndesc,"SUPPRESSED\n");
			outf("suppressed\n");
			return(1);
		}
	}
	return(0);
}

int	dotest()
{
	int	old_umask,st;

	(void)sprintf(tmpjourn,"%s/%s/adm/.tjourn",SVVSPATH, section);
	if((tmpdesc = open(tmpjourn,O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0){
		foutf(2,"verify: can't create temporary file\n");
		exit(-1);
	}
#ifndef	KEEP
	if(unlink(tmpjourn) < 0){
		foutf(2,"Could not unlink temporary file: %s\n",tmpjourn);
		foutf(2,"Temporary file will be left after test run finishes.\n");
	}
#endif
	(void)sprintf(tmpbuff,"%s/%s/tmp",SVVSPATH, tstloc);
	(void)chdir(tmpbuff);
	(void)sprintf(tmpbuff,"rm -rf %s.dir ; mkdir %s.dir",tstnam, tstnam);
	old_umask = umask(0);
	(void)system(tmpbuff);
	(void)sprintf(tmpbuff,"rm -rf %s/tmp/%s; mkdir %s/tmp/%s",SVVSPATH,
		SUBTMP,SVVSPATH,SUBTMP);
	(void)system(tmpbuff);
	(void)umask(old_umask);
	(void)sprintf(tmpbuff,"%s/%s/bin",SVVSPATH, tstloc);
	(void)chdir(tmpbuff);
	outf("%s ",tstnam);
	(void)time(&begtim);
	st = invoke(tstnam);
	(void)time(&endtim);
	if(sort(jrndesc,tmpdesc)){
		foutf(2,"\nTrouble encountered while sorting test output.\n");
		outf("%s lost\n",tstnam);
		foutf(jrndesc,"MESSAGE\nVerify could not process test output, results lost.\n");
		foutf(jrndesc,"SUPPRESSED\n");
	}
	else{
		grade(st);	/*sort() is used to detect failure and sort the*/
					/*output.  Grade should never be called before sort.*/
	}
	(void)close(tmpdesc);
	if(!dopt){
		(void)sprintf(tmpbuff,"%s/%s/tmp",SVVSPATH, tstloc);
		(void)chdir(tmpbuff);
		(void)sprintf(tmpbuff,"rm -rf %s.dir ; mkdir %s.dir",
				tstnam, tstnam);
		old_umask = umask(0);
		(void)system(tmpbuff);
		(void)umask(old_umask);
	}
	(void)sprintf(tmpbuff,"%s/bin",SVVSPATH);
	(void)chdir(tmpbuff);
	return(st);
}

grade(st)
int		st;
{
	if(st < -1){
		foutf(jrndesc,
"ERROR\n\tDriver detected abnormal termination due to signal.\n");
		errfound = 1;
	}
	if(errfound){	/*Typically set by sort on detection of ERROR or */
					/*SEVERR in the test output*/
		setfail(tstnam);
		outf("failed\n");
		return;
	}
	if (st == -1){	/*Suppressed*/
		foutf(jrndesc,"MESSAGE\n\t%s failed to execute.\n",tstnam);
		foutf(jrndesc,"SUPPRESSED\n");
		outf("\nMESSAGE\n\t%s failed to execute.\n",tstnam);
		setfail(tstnam);
		outf("%s suppressed",tstnam);
	}
	outf("\n");
}

extest(tst)
char	*tst;
{
	int		i, old_umask;
	char	sct[100];

	i = 4;
	outf("%s%s\n",section,tst);
	if(!findtest(tst,sct)){
		foutf(2,"verify: %s is not a test in section %s.\n",tst,section);
		(void)sprintf(tmpbuff,"%s/bin",SVVSPATH);
		(void)chdir(tmpbuff);
		return(4);
	}
	(void)sprintf(tmpbuff,"%s/%s/bin/%s",SVVSPATH,sct,tst);
	if(!access(tmpbuff,1)){
		(void)sprintf(tmpbuff,"%s/%s/tmp",SVVSPATH, sct);
		(void)chdir(tmpbuff);
		(void)sprintf(tmpbuff,"rm -rf %s.dir ; mkdir %s.dir",tst,tst);
		old_umask = umask(0);
		(void)system(tmpbuff);
		(void)umask(old_umask);
		(void)sprintf(tmpbuff,"%s/%s/bin",SVVSPATH, sct);
		(void)chdir(tmpbuff);
		i = invoke(tst);
		if(!dopt){
			(void)sprintf(tmpbuff,"%s/%s/tmp",SVVSPATH, sct);
			(void)chdir(tmpbuff);
			(void)sprintf(tmpbuff,"rm -rf %s.dir ; mkdir %s.dir",tst,tst);
			old_umask = umask(0);
			(void)system(tmpbuff);
			(void)umask(old_umask);
		}
	}
	if(i == 4){
		(void)sprintf(tmpbuff,"Verify: %s/%s failed to execute.\n",sct,tst);
		foutf(2,"%s",tmpbuff);
	}
	if(i < 0)
		foutf(2,"%s terminated abnormally due to sig %d\n",tst, -i);
	(void)sprintf(tmpbuff,"%s/bin",SVVSPATH);
	(void)chdir(tmpbuff);
	return (i);
}

/*
** findtest(tst,section)
** char	*tst,*section;
**
** Find a test in the dependency file and set section to the section
** where it is really found.  Return 0 if you don't find it.
**
*/
int	findtest(tst,sect)
char	*tst,*sect;
{
	char	*p,token[64];
	int		fd;

	sprintf(tmpbuff,"%s/%sadm/dependency",SVVSPATH,section);
	fd = open(tmpbuff,O_RDONLY);
	if(fd < 0){
		return(0);
	}
	while(fgetln(fd,tmpbuff)){
		p = nxtok(sect,tmpbuff);
		p = nxtok(token,p);
		if(!strcmp(token,tst)){
			return(1);
		}
	}
	return(0);
}

/**
 **	loadftab:  scan the previous run in the journal file
 **	and load the fail table "failtab" with names of all the
 **	tests that failed.  If there was no final run, it prints an
 ** error and exits.  Otherwise the global 'manpage' will contain
 ** the name of the last test in the previous run after loadftab() is
 ** done.
 **/
loadftab()
{
	static	char	line[LMAX];	/*Keep it off of the stack*/

	if(svseek(jrndesc,(long)0,0) < (long)0){
		foutf(2,"Cannot seek to beginning of journal.\n");
		return(-1);
	}
	if(lstrun < 2){
		foutf(2,"Cannot correct/continue an empty journal.\n");
		return(-1);
	}
	while(fgetln(jrndesc,line)){
		if(nonblank(line) && (*line != '\t')){
			jlnparse(line);
		}
	}
	return(0);
}

int	jlnparse(str)
char	*str;
{
	char	token[100];
	int		i;

	str = nxtok(token,str);
	for(i = 0; i < NKEYS; ++i){
		if(!strcmp(token,actions[i].key)){
			(actions[i].func)(str);
			return;
		}
	}
	return;
}

/**
 **	skipdone: scan the previous run in the journal file
 **	and find the name of the last test.  Then position
 **	the control file so that the current position points 
 **	to the "last test".
 **/

skipdone(lasttst)
char	*lasttst;
{
	char	*p;

	if(*lasttst){
		while(p = getword(ctldesc, 1)){
			p = getword(ctldesc,0);
			if(strcmp(p, lasttst) == 0){
				return;
			}
		}
	}
}

/**
 **	gttst:  works in two modes:
 **	for Correction runs, return the next entry in 
 **	the previous fail table, removing the entry.
 **	For all other types, return the next test name
 **	in the control file.
 **	Returns NULL string if no more names in either case.
 **/
static	int	fcount;			/* index into fail table*/

char *gttst(name,location)
char	*name,*location;
{
	char	*fp;
	int		i,found=0;

	fp = getword(ctldesc, 1);
	while(fp){
		(void)strcpy(location, fp);
		fp = getword(ctldesc, 0);
		if(fp){
			(void)strcpy(name, fp);
		}
		if(topt){		/* Not a Correction Run */
			for(i = 0; i < failsz; ++i){	/* There are holes in ftab */
				if(failtab[fcount].p_name[0] != '\0'){
					if(!strcmp(name,failtab[i].p_name)){
						failtab[fcount].p_name[0] = '\0';
						return(fp);
					}
				}
			}
		}
		else{				/*Not topt*/
			return(fp);
		}
		fp = getword(ctldesc, 1);
	}
	return((char *)0);
}

void	catch()
{
	foutf(2,"\nSignal received, verify aborted.\n");
	if(jrndesc > -1){
		foutf(jrndesc,"SEVERR\n");
		foutf(jrndesc,"\tSignal received, test run aborted\n");
		foutf(jrndesc,"\tABORT\n");
		foutf(jrndesc,"EndMan 0\n");
	}
	exit(1);
}

int	sigs[12] = {SIGHUP,SIGINT,SIGQUIT,SIGILL,SIGTRAP,SIGFPE,
				SIGSYS,SIGPIPE,SIGALRM,SIGTERM,SIGUSR1,SIGUSR2};

int	stsigs()
{
	int	i;

	for(i = 0; i < 12; ++i){
		(void)signal(sigs[i],catch);
	}
}

/*
** The following are the routines used to parse the journal file to
** load the fail table.
*/

static	int		failed = 0;
static	int		inrun = 0;

problem()
{
	if(inrun && !failed){
		setfail(manpage);
	}
}

runbeg(str)
char	*str;
{
	inrun = (runno(str) == (lstrun-1));
}

manstart(str)
char	*str;
{
	if(inrun){
		(void)nxtok(manpage,str);
		failed = 0;
	}
}


/**
 ** getdep: returns a dependent name from the
 ** control file or NULL string if there are no dependents
 **/
char *getdep()
{
	char *dp;

	dp = getword(ctldesc, 0);
	return((dp) ? dp : (char *)0);
}



/**
 **	getword: returns a string containing
 **	the next word in a given file.
 **	if "whence" is 1, start at a new line,
 **	otherwise, find the next word in the present line.
 **	a NULL string means end of file.
 **/
static	char	linwrd[20];
static	char	linbuf[LMAX];
static	char	*linptr;
static	char	*linmark;

char	*getword(fd, whence)
int	fd;
int	whence;
{
	char	*lp;

	if(whence == 1){
		if(!fgetln(fd, linbuf))
			return((char *)0);
		linmark = linbuf;
	}
	for(lp=linwrd, linptr=linmark; 1; linmark++){
		switch(*linmark){
		case '\0' :
		case '\n' :
			if((long)(linmark-linptr) <= (long)1)
				return((char *)0);

		case ' ' :
			*lp = '\0';
			linmark++;
			return(&linwrd[0]);

		default :
			*lp++ = *linmark;
		}
	}
}


/**
 ** setfail: insert given name in fail table.  There
 ** may be holes in the middle of the table.
 **/
setfail(nam)
char	*nam;
{
	int	k;

	for(k=0; k<failsz; k++){
		if(!strcmp(failtab[k].p_name,nam))
			break;
		if(failtab[k].p_name[0] == '\0')
			break;
	}
	if(k == failsz)
		++failsz;
	(void)strcpy(failtab[k].p_name, nam);
}
