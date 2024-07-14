#include <fcntl.h>
#include <svvs.h>

#ident	"@(#)report.c	1.14"

struct	flags {
	int	copt;
	int	sopt;
	int	mopt;
	int	topt;
	int	xopt;
	int	hopt;
};

struct	indx {
	struct	indx	*next;
	char	*manpage;
	int	tstrun;
	int	pgno;
	char	grade[10];
};

struct summary {
	struct	summary	*next;
	int	runnum;		/*Which run is this summary for*/
	int	manrun;		/*How Many manpages were run*/
	int	manpassed;	/*How many manpages passed*/
	int	manfailed;	/*How many manpages failed*/
	int	suppressed;	/*How many manpages were suppressed*/
	int	unsup;		/*How many manpages were unsupported*/
	int	totmsgs;	/*How many messages were issued*/
	int	messages;	/*How many msgs. were purely informative*/
	int	warnings;	/*How many msgs. were warnings*/
	int	errors;		/*How many msgs. were errors*/
	int	severe;		/*How many msgs. were severe errors*/
	int	breaks;		/*How many breaks were taken*/
	int	exits;		/*How many exits were taken*/
	int	aborts;		/*How many tests were aborted*/
};

struct	msgonly {
	struct	msgonly	*next;
	long	offset;		/*Offset in file of begining of manpage */
	int	runnum;		/*Run number where the message occurred*/
};

struct	footer	{
	int	size;
	int	(*funct)();
};

#define	SECTION		-2
#define	TEST		-1
#define	DEFJOURN	"./journal"
#define	COVER	3
#define	SUMMARY	2
#define	PAGE	1
#define	INDEX	0
#define	NEWPAGE	1
#define	TSTMAX	512		/*Max number of tests*/
#define	SCTMAX	64		/*Max number of sections*/

extern	long	svseek();
extern	char	*malloc(),*strcpy(),*strcat();
extern	void	exit(),free();

char	*nxtok();
char	*split();
long	dopage();
struct	indx	*insind();
struct	summary	*getsumm();

/*Parsing Actions*/
int	sevmsg();
int	errmsg();
int	warnmsg();
int	infmsg();
int	initial();
int	runstart();
int	manstart();
int	manend();
int	printline();
int	noformat();
int	format();
int	brkact();
int	exitact();
int	abtact();
int	supact();
int	unsupact();
int	child();
int	unchild();

/*Footer Functions*/
int	indfooter();
int	footer();
int	sumfooter();
int	coverfoot();

static	char	*tests[TSTMAX];	/*List of tests to be reported under -t*/
static	char	*sects[SCTMAX];	/*List of sections/files to be reported*/
static	char	fday[3];	/*Day of footer date*/
static	char	fmonth[4];	/*Month of footer date*/
static	char	fyear[5];	/*Year of footer date*/
static	char	ftime[9];
static	int	t_on=0;		/*Flag to indicate -t option is active*/
static	int	ntests=0;	/*Number of tests in test list*/
static	int	nsects=0;	/*Number of sections to be tested*/
static	int	thispage=1;	/*Flag - indicates page is requested*/
static	int	pagelen = 66;	/*User supplied page length, default: 66*/
static	int	linelen = 80;	/*User supplied page width, default: 80*/
static	int	currun = 1;	/*Current Run Number*/
static	int	therun = 1;	/*The run we are interested in*/
static	int	noform = 0;	/*Suppress formatting if TRUE*/
static	int	page = 1;	/*Page Number within current run */
static	int	line = 1;	/*Line number within current page */
static	int	margin = 0;	/*Margin of the page*/
static	int	basindent = 0;	/*Base indentation distance*/
static	int	indent = 0;	/*Indentation from base*/
static	int	chldind = 0;	/*Indentation for Child brackets*/
static	int	suppress = 0;	/*Flag to suppress printing*/
static	int	enable = 1;	/*Flag to enable processing*/
static	int	firstfail = 1;	/*Flag to say no errors yet for manpage*/
static	int	firstmsg = 1;	/*Flag to say no messages yet for manpage*/
static	int	journal = -1;	/*Journal file descriptor*/

static	char	pagename[256];	/*Name of current Manual page*/
static	char	tmpbuf[256];	/*Temporary print buffer*/
static	long	curpos=0;	/*Current position in journal file*/
static	long	manpos=0;	/*Current manpage start position */

static	struct	msgonly	*printlist = (struct msgonly *)0;
static	struct	msgonly	*printbot = (struct msgonly *)0;
static	struct	indx	*index = (struct indx *)0;
static	struct	indx	*curind = (struct indx *)0;
static	struct	summary	*sumlist = (struct summary *)0;
static	struct	summary	*summend = (struct summary *)0;

static	struct	flags	options;


/*Parsing data*/
struct	parse	{
	char	*key;
	int	(*funct)();
};

static	struct	parse	linekeys[]={
	{	"SEVERR",errmsg	},
	{	"ERROR",errmsg	},
	{	"WARNING",warnmsg },
	{	"MESSAGE",infmsg },
	{	"BREAK",brkact	},
	{	"EXIT",exitact	},
	{	"ABORT",abtact	},
	{	"SUPPRESSED",supact },
	{	"UNSUPPORTED",unsupact },
	{	"BEGIN_SCREEN",noformat	},
	{	"END_SCREEN",format	},
	{	"BegJourn",initial },
	{	"BegRun",runstart  },
	{	"BegMan",manstart  },
	{	"EndMan",manend	   },
	{	"BegChild",child    },
	{	"EndChild",unchild },
	{	"Default",printline},
};

#define	NUMKEYS	((sizeof(linekeys)/sizeof(struct parse))-1)

static	struct	footer	feet[] = {
	{6,indfooter},
	{6,footer},
	{6,sumfooter},
	{0,coverfoot}
};

#define	NUMFEET	(sizeof(feet)/sizeof(struct footer))

main(argc,argv)
int	argc;
char	*argv[];
{
	int	i,fd,opened=0;
	static	char	sect[512];	/*Keep this off of the stack*/
	static	char	fname[512];	/*And this also*/

	sv_config();
	clropt();
	for(i=1; i < argc; ++i){
		if(*argv[i] == '-'){
			if(opt(argv[i]+1))
				usage(argv[0]);
		}
		else if(t_on){
			if(ntests < TSTMAX)
				tests[ntests++] = argv[i];
			else{
				foutf(2,"Too many test names specified\n");
				exit(-1);
			}
		}
		else{
			if(nsects < TSTMAX)
				sects[nsects++] = argv[i];
			else{
				foutf(2,"Too many section/file names specified\n");
				exit(-1);
			}
		}
	}
	for(i = 0; i < nsects; ++i){
		if(setsect(sect,sects[i],SVVSPATH) == SECTION){
			opened = 1;
			while(*sect){
				(void)sprintf(fname,"%s/%s/adm/journal",SVVSPATH,sect);
				if((fd = open(fname,O_RDONLY)) < 0){
					foutf(2,"%s: Cannot open file %s\n",argv[0],fname);
				}
				else{
					report(fd);
					(void)close(fd);
				}
				(void)setsect(sect,sects[i],SVVSPATH);
			}
			rstsect();
		}
		else{
			opened = 1;
			if((fd = open(sects[i],O_RDONLY)) < 0){
				foutf(2,"%s: Cannot open file %s\n",argv[0],sects[i]);
			}
			else{
				report(fd);
				(void)close(fd);
			}
		}
	}
	if(!opened){
		if((fd = open(DEFJOURN,O_RDONLY)) < 0){
			foutf(2,"%s: Cannot open file %s\n",argv[0],DEFJOURN);
			exit(-1);
		}
		report(fd);
		(void)close(fd);
	}
	exit(0);
/*NOTREACHED*/
}

/*
** This function generates the report.
*/
report(fd)
int	fd;
{
	int	s;

	init_parms();
	journal = fd;
	suppress = options.sopt;
	if(options.mopt)
		msgrep(fd);
	else{
		fullrep(fd);
	}
	s = suppress;
	suppress = 0;
	printindex(index);
	printsumm(sumlist);
	suppress = s;
}

/*
** Routine to reset all of the formatting values to the values they should
** have for a new journal file.
*/

init_parms()
{
	noform = 0;
	currun = 1;
	therun = 1;
	page = 1;
	line = 1;
	basindent = 0;
	indent = 0;
	enable = 1;
	curpos = (long)0;
	manpos = (long)0;
	printlist = (struct msgonly *)0;
	printbot = (struct msgonly *)0;
	index = (struct indx *)0;
	curind = (struct indx *)0;
	sumlist = (struct summary *)0;
	summend = (struct summary *)0;
}


/*
**
** Routine to run a full report from the current position in the
** journal file.
**
*/

int	fullrep(fd)
int	fd;
{
	while((curpos = dopage(fd,curpos)) != -1);
}

int	msgrep(fd)
int	fd;
{
	fullrep(fd);
	options.mopt = 0;
	printmsg(fd);
	options.mopt = 1;
}

int	printmsg(fd)
int	fd;
{
	struct	msgonly	*op;

	currun = 0;
	while(printlist){
		if(printlist->runnum != currun){
			(void)getsumm(printlist->runnum);
			newrun(printlist->runnum);
		}
		(void)dopage(fd,printlist->offset);
		op = printlist;
		printlist = printlist->next;
		free(op);
	}
}

/*
** This function parses from 'offset' in the journal to the next "EndMan"
** line.
*/

long	dopage(fd,offset)
int	fd;
long	offset;
{
	static	char	inbuff[512];	/*Keep this off of the stack*/

	if((curpos = svseek(fd,offset,0)) < 0){
		foutf(2,"Error seeking on journal file.\n");
		return(-1);
	}
	while(fgetln(fd,inbuff)){
		if(parserep(inbuff) == NEWPAGE){
			return(svseek(fd,(long)0,1));
		}
		curpos = svseek(fd,(long)0,1);
	}
	return((long)-1);		
}

/*
** This routine interprets and acts on the current line.
*/

parserep(str)
char	*str;
{
	char	token[100],*p;
	int	i;
	static	int	wasfunc=0;

	if(wasfunc){
		wasfunc = 0;
		(void)nxtok(token,str);
		(void)functline(token);
		return(0);
	}
	p = str;
	str = nxtok(token,str);
	if((*token < ('9'+1)) && (*token > ('0'-1))){
		str = nxtok(token,str);
		if(!strcmp(token,"Function:")){
			wasfunc = 1;
		}
		else{
			(void)sectline(p);
		}
		return(0);
	}
	str = nxtok(token,p);
	for(i = 0; i < NUMKEYS; ++i){
		if(!strcmp(token,linekeys[i].key)){
			i = (linekeys[i].funct)(str);
			return(i);
		}
	}
	i = (linekeys[i].funct)(p);
	return(i);
}

/*
**
** Routine to print the summaries, one summary is printed per page.
** The summaries are a linked list of summary data.
**
*/

printsumm(sums)
struct	summary	*sums;
{
	int	startline;
	struct	summary	*oldsums;
	int	tmarg;

	startline = 1 + ((pagelen - 12) / 3);
	indent = 0;
	while(line > 1)
		print("",INDEX);
	page = 1;
	while(sums){
		basindent = 0;
		indent = 0;
		center("Summary of Run # %d",sums->runnum);
		++line;
		tmarg = margin;
		margin = linelen / 4;
		while(line < startline)
			print("",SUMMARY);
		(void)sprintf(tmpbuf,"Passed:  %4d  Not Supported:%4d",sums->manpassed
			,sums->unsup);
		print(tmpbuf,SUMMARY);
		(void)sprintf(tmpbuf,"Failed:  %4d  Not Executed:%5d",sums->manfailed
			,sums->suppressed);
		print(tmpbuf,SUMMARY);
		(void)sprintf(tmpbuf,"Executed:%4d  Omitted:     %5d  Total Tests:%4d"
			,(sums->manpassed + sums->manfailed)
			,(sums->suppressed + sums->unsup)
			,sums->manrun);
		print(tmpbuf,SUMMARY);
		print("",SUMMARY);
		print("Itemization of diagnostics",SUMMARY);
		indent = 4;
		(void)sprintf(tmpbuf,"Errors:                 %d",sums->errors);
		print(tmpbuf,SUMMARY);
		(void)sprintf(tmpbuf,"Warnings:               %d",sums->warnings);
		print(tmpbuf,SUMMARY);
		(void)sprintf(tmpbuf,"Informational Messages: %d",sums->messages);
		print(tmpbuf,SUMMARY);
		indent = 0;
		(void)sprintf(tmpbuf,"Total diagnostics:          %d",sums->totmsgs);
		print(tmpbuf,SUMMARY);
		print("",SUMMARY);
		(void)sprintf(tmpbuf,"Error responses:");
		print(tmpbuf,SUMMARY);
		indent = 4;
		(void)sprintf(tmpbuf,"Aborts:     %d",sums->aborts);
		print(tmpbuf,SUMMARY);
		(void)sprintf(tmpbuf,"Exits:      %d",sums->exits);
		print(tmpbuf,SUMMARY);		
		(void)sprintf(tmpbuf,"Breaks:     %d",sums->breaks);
		print(tmpbuf,SUMMARY);
		margin = tmarg;
		indent = 0;
		while(line > 1)
			print("",SUMMARY);
		oldsums = sums;
		sums = sums->next;
		free(oldsums);
	}
}

/*
** Routine to print the index data.  The index entries are one per line.
** A marker is inserted at the beginning of the index for each test run.
** The data is stored in a sorted linked list, and traversed straight
** through.
*/

int	printindex(ind)
struct	indx	*ind;
{
	struct	indx	*itmp;
	int	i,sp;
	int	oldrun = 0;
	int	first = 1;
	static	char	buff[512];	/*Keep this off of the stack*/

	indent = 0;
	while(line > 1)
		print("",PAGE);
	page = 1;
	while(ind){
		if(ind->tstrun != oldrun){
			oldrun = ind->tstrun;
			if(!first){
				print("",INDEX);
				print("",INDEX);
			}
			(void)sprintf(tmpbuf,"Test Run %d",oldrun);
			print(tmpbuf,INDEX);
			print("",INDEX);
			first = 0;
		}
		(void)strcpy(tmpbuf,ind->manpage);
		sp = (options.sopt ? linelen-9:linelen/2);
		i = strlen(tmpbuf) + margin;
		while(i++ < sp)
			(void)strcat(tmpbuf,(i > sp - 3)||(i % 3) ? " " : ".");
		(void)strcat(tmpbuf,ind->grade);
		if(!options.sopt){
			sp = (linelen - 9)-sprintf(buff,"%d-%d",ind->tstrun,ind->pgno);
			i = strlen(tmpbuf) + margin;
			while(i++ < sp)
				(void)strcat(tmpbuf,(i > sp - 3)||(i % 3) ? " " : ".");
			(void)strcat(tmpbuf,buff);
		}
		print(tmpbuf,INDEX);
		itmp = ind;
		ind = ind->next;
		free(itmp);
	}
}

/*
**
** routine to print a line, keeping track of page length etc.
**
*/

int	print(str,pgtype)
char	*str;
int	pgtype;
{
	int	whitespace,i;
	char	outline[512],ch;
	static	int	oldtype = COVER;

	if((pgtype < 0) || (pgtype > NUMFEET)){
		pgtype = oldtype;
	}
	oldtype = pgtype;
	if(!suppress){
		if(!noform){
			notabs(str);
			whitespace = margin + basindent + indent;
			do{
				i = 0;
				str = split(outline,str,linelen - whitespace);
				ch = *str;
				*str = 0;
				while(i++ < whitespace)
					outf(" ");
				outf("%s\n",outline);
				*str = ch;
				if(++line > (pagelen - feet[pgtype].size)){
					(feet[pgtype].funct)();
					line = 1;
				}
			}while(*str);
		}
		else{
			outf("%s\n",str);
			if(++line > (pagelen - feet[pgtype].size)){
				(feet[pgtype].funct)();
				line = 1;
			}
		}
	}
}

/*
** Routine to rip tabs out
*/
notabs(str)
char	*str;
{
	char	*p,*st;

	st = p = str;
	while(*p == '\t')
		++p;
	while(*str++ = *p++);
	while(*st){
		if(*st == '\t')
			*st = ' ';
		++st;
	}
}

/*
** Routine to break up lines that are too long.
*/
char	*split(out,in,width)
char	*out,*in;
int	width;
{
	char	*p,*q;
	int	i;

	(void)strcpy(out,in);
	p = out;
	i = 0;
	q = (char *)0;
	while(*p){
		if(++i > width){
			if(q){
				return(q+1);
			}
			else{
				return(p);
			}
		}
		if((*p == ' ') || (*p == '.') || (*p == ',') || (*p == ';')){
			q = p;
		}
		p++;
	}
	return(p);
}

/*
**
** Routine to set up to report on the next run.
**
*/

int	newrun(number)
int	number;
{
	int	s;

	s = suppress;
	suppress = 0;
	while(line > 1)
		print("",PAGE);
	page = 1;
	currun = number;
	suppress = s;
}

/*
**
** Routine to print the footer for a summary page.
**
*/

int	sumfooter()
{
	static	char	bf[50];

	outf("\n");
	center("System V Verification Report");
	sprintf(bf,"%s %s, %s",fmonth,fday,fyear);
	center(bf);
	center("- %d -",page);
	outf("\n\n");
	++page;
}

/*
**
** Routine to print the footer for an index page.
**
*/

int	indfooter()
{
	static	char	bf[50];
	outf("\n");
	center("System V Verification Report");
	sprintf(bf,"%s %s, %s",fmonth,fday,fyear);
	center(bf);
	center("- Contents %d -",page);
	outf("\n\n");
	++page;
}

/*
** Routine to feed a cover page.
*/
int	coverfoot()
{
}

/*
**
** Routine to print the footer for a normal page.
**
*/	

int	footer()
{
	static	char	bf[50];
	outf("\n");
	center("System V Verification Report");
	sprintf(bf,"%s %s, %s",fmonth,fday,fyear);
	center(bf);
	center("%d-%d",currun,page);
	outf("\n\n");
	++page;
}

/*
** Routine to center text on a line.
**
*/
/*VARARGS1*/
int	center(fmt,a,b)
char	*fmt;
char	*a,*b;
{
	int	spaces,i=0;
	static	char	output[256];

	(void)sprintf(output,fmt,a,b);
	spaces = margin + (((linelen-margin) / 2) - (strlen(output) / 2));
	while(i++ < spaces)
		outf(" ");
	outf("%s\n",output);
}

/*
**
** Routine to put a new manual page into the index.
**
*/

struct	indx	*insind(name)
char	*name;
{
	struct	indx	*pos,*t;

	t = pos = index;
	while(pos&&(pos->tstrun < currun)){
		t = pos;
		pos = pos->next;
	}
	while(pos&&(pos->tstrun == currun)&&(strcmp(pos->manpage,name) < 0)){
		t = pos;
		pos = pos->next;
	}
	if(!pos){
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
		pos = (struct indx *)malloc(sizeof(struct indx));
		if(!pos){
			foutf(2,"Could not allocate index node.\n");
			exit(-1);
		}
		if(t){
			t->next = pos;
		}
		else{
			index = pos;	/*if pos was ever non-null then*/
					/*'t' will be non-null.  If not,*/
					/*Index needs to be initialized*/
		}
		pos->next = (struct indx *)0;
	}
	else{
		if(pos == index){
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
			index = (struct indx *)malloc(sizeof(struct indx));
			if(!index){
				foutf(2,"Could not allocate index node.\n");
				exit(-1);
			}
			index->next = pos;
			pos = index;
		}
		else{
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
			t->next = (struct indx *)malloc(sizeof(struct indx));
			if(!t->next){
				foutf(2,"Could not allocate index node.\n");
				exit(-1);
			}
			t = t->next;
			t->next = pos;
			pos = t;
		}
	}
	pos->tstrun = currun;
	pos->pgno = page;
	pos->manpage = malloc((unsigned)strlen(name) + 1);
	if(!pos->manpage){
		foutf(2,"Could not allocate space for SVID page name.\n");
		exit(-1);
	}
	(void)strcpy(pos->manpage,name);
	(void)strcpy(pos->grade,"passed");
	return(pos);
}

insmsg(off)
long	off;
{
	if(!printbot){
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
		printlist=printbot=(struct msgonly *)malloc(sizeof(struct msgonly));
		if(!printlist){
			foutf(2,"Could not allocate space for message list node.\n");
			exit(-1);
		}
	}
	else {
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
		printbot->next=(struct msgonly *)malloc(sizeof(struct msgonly));
		if(!printbot->next){
			foutf(2,"Could not allocate space for message list node.\n");
			exit(-1);
		}
		printbot = printbot->next;
	}
	printbot->next = (struct msgonly *)0;
	printbot->offset = off;
	printbot->runnum = currun;
}

/*
**
**Routine to add a new run to the summary list.
**
*/

struct	summary	*getsumm(run)
int	run;
{
	if(!sumlist){
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
		sumlist=(struct summary *)malloc(sizeof(struct summary));
		if(!sumlist){
			foutf(2,"Could not allocate space for summary node.\n");
			exit(-1);
		}
		summend=sumlist;
	}
	else{
/*Lint will complain of alignment problems, malloc returns a GENERAL ptr.*/
		summend->next=(struct summary *)malloc(sizeof(struct summary));
		if(!summend->next){
			foutf(2,"Could not allocate space for summary node.\n");
			exit(-1);
		}
		summend = summend->next;
	}
	summend->next  = (struct summary *)0;
	summend->runnum = run;
	summend->manrun = 0;
	summend->manpassed = 0;
	summend->manfailed = 0;
	summend->totmsgs = 0;
	summend->messages = 0;
	summend->warnings = 0;
	summend->errors = 0;
	summend->severe = 0;
	summend->breaks = 0;
	summend->exits = 0;
	summend->aborts = 0;
	summend->suppressed = 0;
	summend->unsup = 0;
	return(summend);
}

/*
** This function clears out the options structure.
*/
clropt()
{
	ntests = 0;
	options.topt = 0;
	options.sopt = 0;
	options.mopt = 0;
	options.hopt = 0;
	options.xopt = 1;
}

/*
** This routine processes the options.
*/

int	opt(str)
char	*str;
{
	char	*num;

	if(t_on)
		t_on = 0;
	while(*str){
		switch(*(str++)){
			case '-':
				break;
			case 's':
				options.sopt = 1;
				break;
			case 'm':
				options.mopt = 1;
				break;
			case 't':
				options.topt = 1;
				t_on = 1;
				if(*str){
					if(ntests < TSTMAX){
						tests[ntests++] = str;
					}
					else {
						foutf(2,"Too many test names specified\n");
						exit(-1);
					}
				}
				return(0);
			case 'h':
				options.hopt = 1;
				break;
			case 'x':
				options.xopt = 0;
				break;
			case 'l':
				num = tmpbuf;
				if(*(str++) != '='){
					return(-1);
				}
				if((*str < '0') || (*str >'9')){
					return(-1);
				}
				while(*num = *str){
					if((*num < '0') || (*num > '9'))
						break;
					++str;
					++num;
				}
				*num = 0;
				(void)sscanf(tmpbuf,"%d",&pagelen);
				if(pagelen < 20){
					foutf(2,"Page must be at least 20 lines long\n");
					exit(-1);
				}
				break;
			case 'w':
				num = tmpbuf;
				if(*(str++) != '=')
					return(-1);
				if((*str < '0') || (*str >'9'))
					return(-1);
				while(*num = *str){
					if((*num < '0') || (*num > '9'))
						break;
					++num;
					++str;
				}
				*num = 0;
				(void)sscanf(tmpbuf,"%d",&linelen);
				if((linelen < 40) || (linelen > 255)){
					foutf(2,"Page must be between 40 and 255 columns wide\n");
					exit(-1);
				}
				break;
			case 'p':
				num = tmpbuf;
				if(*(str++) != '=')
					return(-1);
				if((*str < '0') || (*str >'9'))
					return(-1);
				while(*num = *str){
					if((*num < '0') || (*num > '9'))
						break;
					++num;
					++str;
				}
				*num = 0;
				(void)sscanf(tmpbuf,"%d",&margin);
				if(margin > (linelen - 40)){
					foutf(2,"Margin must allow at least 40 columns of text\n");
					exit(-1);
				}
				break;
			default:
				return(-1);
		}
	}
	return(0);
}

/*
** This function prints out the usage message and exits.
*/

usage(name)
char	*name;
{
	foutf(2,"Usage: %s [-hmsx] [filename]\n",name);
	foutf(2,"       %s [-t test1 test2 ... testn]\n",name);
	exit(-1);
}

/*
** This function scans the list of requested tests for the supplied
** test name. If the name is found it returns true, otherwise, false
*/

int	requested(test)
char	*test;
{
	int	i;

	for(i = 0; i < ntests; ++i){
		if(!strcmp(tests[i],test))
			return(1);
	}
	return(0);
}

/*
** This function handles "SEVERR" and "ERROR" lines, called indirectly
** from parserep()
*/

int	errmsg()
{
	noform = 0;
	if(enable &&  thispage){
		if(options.mopt){
			if(firstmsg){
				insmsg(manpos);
				firstmsg = 0;
			}
			return(0);
		}
		if(summend){
			++summend->errors;
			++summend->totmsgs;
			if(firstfail){
				firstfail = 0;
				++summend->manfailed;
			}
		}
		if(!suppress){
			indent = 12;
			print("",PAGE);
			print("Error:",PAGE);
		}
		if(curind){
			(void)strcpy(curind->grade,"FAILED");
		}
	}
	return(0);
}

/*
** This function handles "WARNING" lines, called indirectly from
** parserep().
*/

int	warnmsg()
{
	noform = 0;
	if(enable &&  thispage){
		if(options.mopt){
			if(firstmsg){
				insmsg(manpos);
				firstmsg = 0;
			}
			return(0);
		}
		if(summend){
			++summend->warnings;
			++summend->totmsgs;
		}
		if(!suppress){
			indent = 12;
			print("",PAGE);
			print("Warning:",PAGE);
		}
	}
	return(0);
}

/*
** This function handles "MESSAGE" lines, called indirectly from
** parserep().
*/

int	infmsg()
{
	noform = 0;
	if(enable &&  thispage){
		if(options.mopt){
			if(firstmsg){
				insmsg(manpos);
				firstmsg = 0;
			}
			return(0);
		}
		if(summend){
			++summend->messages;
			++summend->totmsgs;
		}
		if(!suppress){
			indent = 12;
			print("",PAGE);
			print("Message:",PAGE);
		}
	}
	return(0);
}
/*
** The following functions turn off and on the indent and fill feature of
** the report generator.  For TI output, it is difficult to read the screen
** information once it has been formatted.
*/

noformat(str)
char	*str;
{
	noform = 1;	/*Turn off formatting*/
}

format(str)
char	*str;
{
	noform = 0;	/*Turn on formatting*/
}

/*
** This function handles "BegJourn" lines, it sets up the run number we
** are interested in and prints a report header.  Called indirectly from
** parserep().
*/

int	initial(str)
char	*str;
{
	int	s;

	init_parms();
	noform = 0;
	if(!options.hopt){
		therun = runno(str);
	}
	if(options.mopt){
		options.mopt = 0;
		if(journal >= 0){
			printmsg(journal);
		}
		options.mopt = 1;
	}
	s = suppress;
	suppress = 0;
	if(index){
		printindex(index);
	}
	if(sumlist){
		printsumm(sumlist);
	}
	while(line > 1)
		print("",-1);
	headpg(str);
	suppress = s;
	return(0);
}

/*
** This function handles "BegRun" lines.  If we are not in a history run,
** it decides whether we are interested in this run, and if we are,
** adds a summary page.
*/

int	runstart(str)
char	*str;
{
	int	run;

	noform = 0;
	run = runno(str);
	if(!options.hopt){
		enable = (run == therun);
		run = 1;
	}
	else{
		enable = 1;
	}
	if(options.mopt){
		currun = run;
		return(0);
	}
	if(enable){
		newrun(run);
		(void)getsumm(run);
		if(!suppress){
			(void)runhead(str);
		}
	}
	return(0);
}

/*
** This routine handles "BegMan" lines.  It enters the manual page into
** the index and sets the flags that indicate a fresh page.
*/

int	manstart(str)
char	*str;
{
	thispage = 1;
	noform = 0;
	if(enable){
		firstfail = 1;
		firstmsg = 1;
		manpos = curpos;
		(void)nxtok(pagename,str);
		if(options.topt){
			thispage = requested(pagename);
			if(!thispage)
				return;
		}
		if(!options.mopt){
			curind = insind(pagename);
			if(!suppress){
				indent = 0;
				(void)sprintf(tmpbuf,"Test: %s",pagename);
				print("",PAGE);
				print(tmpbuf,PAGE);
			}
		}
	}
	return(0);
}

/*
** This function handles the "EndMan" lines, it tallies another page and
** prints out the execution time.  It then returns NEWPAGE to the parser
** to tell dopage() that the current manual page is finished.
*/

int	manend(str)
char	*str;
{
	noform = 0;
	if(options.mopt)
		return(0);
	if(enable &&  thispage){
		if(!suppress){
			if(!options.xopt){
				print("",PAGE);
				indent = 8;
				(void)sprintf(tmpbuf,"%s execution time: %s sec.",pagename,str);
				print(tmpbuf,PAGE);
			}
		}
		if(summend){
			++summend->manrun;
			if(firstfail){
				++summend->manpassed;
			}
		}
	}
	return(NEWPAGE);
}

/*
** This function prints out any line which was not caught by another
** parsing function.  It uses the current indentation level plus 4.
*/

int	printline(str)
char	*str;
{
	if(enable &&  thispage){
		if(options.mopt)
			return(0);
		if(!suppress){
			indent += 4;
			print(str,PAGE);
			indent -=  4;
		}
	}
	return(0);
}

/*
** This function tallies and reports "BREAK"s in the report.
*/

int	brkact()
{
	noform = 0;
	if(options.mopt)
		return(0);
	if(enable &&  thispage){
		indent = 12;
		print("Skipped to next test case",PAGE);
		if(summend){
			++summend->breaks;
		}
	}
	return(0);
}

/*
** This function tallies and reports "EXIT"s in the  report.
*/

int	exitact()
{
	noform = 0;
	if(options.mopt)
		return(0);
	if(enable &&  thispage){
		indent = 12;
		print("Exited from test or subtest",PAGE);
		if(summend){
			++summend->exits;
		}
	}
	return(0);
}

/*
** This function tallies and reports "ABORT"s in the report.
*/


int	abtact()
{
	noform = 0;
	if(options.mopt)
		return(0);
	if(enable &&  thispage){
		indent = 12;
		print("Aborted test",PAGE);
		if(summend){
			++summend->aborts;
		}
	}
	return(0);
}

/*
** This function tallies and reports "SUPPRESSED"s in the report.
*/

int	supact()
{
	noform = 0;
	if(options.mopt)
		return(0);
	if(enable &&  thispage){
		indent = 12;
		if(!suppress){
			print("Suppressed test",PAGE);
		}
		if(summend){
			if(firstfail){
				firstfail = 0;
				++summend->suppressed;
			}
		}
		if(curind){
			(void)strcpy(curind->grade,"SUPP  ");
		}
	}
	return(0);
}

/*
** This function tallies and reports "UNSUPPORTED"s in the report.
*/

int	unsupact()
{
	noform = 0;
	if(options.mopt)
		return(0);
	if(enable &&  thispage){
		indent = 12;
		if(!suppress){
			print("Unsupported test",PAGE);
		}
		if(summend){
			if(firstfail){
				firstfail = 0;
				++summend->unsup;
			}
		}
		if(curind){
			(void)strcpy(curind->grade,"UNSUPP");
		}
	}
	return(0);
}

/*
** This routine prints out section numbered lines (not function lines).
*/

int	sectline(str)
char	*str;
{
	noform = 0;
	if(enable &&  thispage){
		if(options.mopt)
			return(0);
		if(!suppress){
			indent = 8;
			print("",PAGE);
			print(str,PAGE);
			print("",PAGE);
		}
	}
	return(0);
}

child(str)
char	*str;
{
	noform = 0;
	if(enable && !options.mopt && thispage){
		chldind = indent;
		(void)sprintf(tmpbuf,"Beginning child %s of this testcase",str);
		print("",PAGE);
		print(tmpbuf,PAGE);
	}
}

unchild(str)
char	*str;
{
	noform = 0;
	if(enable && !options.mopt && thispage){
		indent = chldind;
		(void)sprintf(tmpbuf,"Ending child %s of this testcase",str);
		print("",PAGE);
		print(tmpbuf,PAGE);
	}
}

/*
** This routine prints out function lines.  It actually uses the line
** following the function line as its input, this is in order to put
** the word function and the name of the function on the same line.
*/

int	functline(str)
char	*str;
{
	noform = 0;
	if(enable &&  thispage){
		if(options.mopt)
			return(0);
		if(!suppress){
			indent = 4;
			(void)sprintf(tmpbuf,"Component: %s",str);
			print("",PAGE);
			print(tmpbuf,PAGE);
		}
	}
	return(0);
}

static	char	numrun[5];
static	char	month[5];
static	char	day[5];
static	char	hour[15];
static	char	year[5];
static	char	tester[512];

headpg(str)
char	*str;
{
	static	char	system[20];
	static	char	release[20];
	static	char	machine[100];
	static	char	svrel[20];
	static	char	section[100];
	extern	char	*ctime();
	extern	long	time();
	char	*p;
	long	rtime;

	int	i,j,tmarg;

	tmarg = margin;
	margin = 0;
	while(line > 1)
		print("",COVER);
	str = nxtok(numrun,str);
	str = nxtok(system,str);
	str = nxtok(release,str);
	str = nxtok(machine,str);
	str = nxtok(svrel,str);
	str = nxtok(month,str);
	str = nxtok(day,str);
	str = nxtok(hour,str);
	str = nxtok(year,str);
	str = nxtok(section,str);
	str = nxtok(tester,str);

	while(line < (pagelen/3)){
		print("",COVER);
	}
	print("",COVER);
	print("",COVER);
	center("System V Verification Suite");
	++line;
	center("Release %s",release);
	++line;
	print("",COVER);
	if(*svrel && *machine){
		center("Verification Report");
		++line;
		center("%s System V %s",machine,svrel);
		++line;
	}
	else{
		center("Report");
		++line;
		print("",COVER);
	}
	print("",COVER);
	center("Section %s",section);
	++line;
	while(line < (pagelen - 5))
		print("",COVER);
	if(*tester){
		i = sprintf(tmpbuf,"Test Organization: %s",tester) + 1;
		indent = 0;
		margin = linelen - i;
		print(tmpbuf,COVER);
	}
	rtime = time((long *)0);
	p = ctime(&rtime);
	strncpy(fday,(p+8),2);
	fday[2] = 0;
	strncpy(fmonth,(p+4),3);
	fmonth[3] = 0;
	strncpy(fyear,(p+20),4);
	fyear[4] = 0;
	strncpy(ftime,(p+11),8);
	ftime[8] = 0;
	j = sprintf(tmpbuf,"Date: %s %s, %s",fmonth,fday,fyear) + 1;
	indent = 0;
	margin = linelen - (*tester ? i : j );
	print(tmpbuf,COVER);
	(void)sprintf(tmpbuf,"Time: %s",ftime);
	print(tmpbuf,COVER);
	while(line > 1)
		print("",COVER);
	margin = tmarg;
}

int	runhead(str)
char	*str;
{
	static	char	runtype[20];
	static	char	runform[20];
	static	char	buff[256];
	int	i,j;

	if(options.mopt)
		return(0);

	str = nxtok(numrun,str);
	str = nxtok(runtype,str);
	str = nxtok(runform,str);
	str = nxtok(month,str);
	str = nxtok(day,str);
	str = nxtok(hour,str);
	str = nxtok(year,str);
	str = nxtok(tester,str);
	indent = 0;

	if(options.hopt){
		i = sprintf(tmpbuf,"Test Run: %s",numrun) + margin + 1;
	}
	else{
		i = sprintf(tmpbuf,"Test Run: 1") + margin + 1;
	}
	j = sprintf(buff,"Date: %s %s, %s",month,day,year);
	for(; i < (linelen - j); ++i){
		(void)strcat(tmpbuf," ");
	}
	(void)strcat(tmpbuf,buff);
	print(tmpbuf,PAGE);
	*tmpbuf = 0;
	if(*tester){
		i = sprintf(tmpbuf,"Tested by: %s",tester)+1+margin;
	}
	else{
		i = sprintf(tmpbuf,"Type: %s",runtype)+1+margin;
	}
	(void)sprintf(buff,"Time: %s",hour);
	for(; i < (linelen - j); ++i){
		(void)strcat(tmpbuf," ");
	}
	(void)strcat(tmpbuf,buff);
	print(tmpbuf,PAGE);
	if(*tester){
		(void)sprintf(tmpbuf,"Type: %s",runtype);
		print(tmpbuf,PAGE);
	}
	(void)sprintf(tmpbuf,"Form: %s",runform);
	print(tmpbuf,PAGE);
	return(0);
}
