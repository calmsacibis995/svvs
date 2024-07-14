#include <sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<fcntl.h>
#include	<signal.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)system0.c	1.8"

int	f_testing();
int	f_expecting();
int	f_system();
int	f_retval();
int	f_file();
int	f_eot();

struct foo 
{
	char *token;
	int (*func)();
} stuff[] = 
{
	{	":TESTING:", 	f_testing	},
	{	":EXPECTING:", 	f_expecting	},
	{	":RETVAL:", 	f_retval	},
	{	":SYSTEM:", 	f_system	},
	{	":FILE:", 	f_file		},
	{	":EOT:", 	f_eot		}, 
};
struct	infile 
{
	char	ifile[PATH_MAX];
	FILE	*ifptr;
} infile[3];

struct	outfile 
{
	char	ofile[PATH_MAX];
	FILE	*ofptr;
} outfile[3];

# define NENT	(sizeof(stuff)/sizeof(struct foo))

# define BSIZ 2048


# define	isdiggy(z)	(z >= '0' && z <= '9')

void exit();
void severr();

main()
{
	int fd;
	char *line;
	char *malloc();
	char datafile[PATH_MAX];

	setup("system");

	prep("Obtain memory.");
	prep("Initialize temporary files and open data and command files.");
	start
		/*  give up if initialization fails  */
		exit_on (ERROR);

		if ( !(line = malloc(BSIZ)) )
		{
			severr("out of memory");
		}
		init_files();

		init_mac();

		strcpy(datafile, data_path());
		strcat(datafile, "system.data");

		expecting ( VOID );
		if ((fd=open(datafile, O_RDONLY))<0)
		{
			severr("Can't open data file");
		}
	finis


	testing("perform the tests from the datafile");
	start
		sysex(fd, line);
		(void) close(fd);
	finis

	done();
	/* NOTREACHED */
}
sysex(fd, line)
int fd;
char *line;
{
	int i;

	while ((i=getline(fd, line))>0)
	{

		for(i=0;i<NENT;i++)
		{
			if(!strncmp(line, stuff[i].token, strlen(stuff[i].token)))
			{
				(*stuff[i].func)(fd, line);
				break;
			}
		}
		if( i == NENT )
		{
			severr("Unrecognized line in file = '%s'.", line);
		}
	}
}
static int pushback = 0;
static int lineno = 0;
getline(fd, line)
int fd;
char *line;
{
	int i;
	int sts;

	if (pushback)
	{
		pushback=0;
		return(strlen(line));
	}
	lineno++;
	i = 0;
	while( (sts = getbyte(fd)) > 0 )
	{
		line[i] = sts & 0xff;
		if (line[i] == '\n')
		{
			line[++i]='\0';
			return(i);
		}
		if (line[i] == '%')
		{
			line[i] = '\0';
			i += do_macro(fd, line);
		}
		else if(++i == BSIZ)
			severr("Line too long.");
	}
	if (i)
	{
		line[i++]='\n';
		line[i]='\0';
		return(i);
	}
	return(-1);
}
ungetline()
{
	pushback = 1;
}
static int idx = BUFSIZ;
ungetbyte()
{
	idx--;
}
getbyte(fd)
int fd;
{
	static char buf[BUFSIZ];
	static int cnt = BUFSIZ;


	if (idx == BUFSIZ)
	{
		expecting ( VOID );
		if((cnt = read(fd, buf, BUFSIZ))<=0)
		{
			return(-1);
		}
		idx = 0;
	}
	if (idx < cnt)
	{
		return(buf[idx++]&0xff);
	}
	return(-1);
}
f_testing(fd, line)
int fd;
char *line;
{
	int cnt;

/*
**	If difficulties arise in determining the particular line of
**	the data file which corresponds to a failed test case, add
**	a printf here of the variable "lineno".
*/
	while((cnt=getline(fd, line))>0 && line[0] != ':')
	{
		line[cnt-1]=' ';
		testing("%s", line);
	}
	if(cnt > 0) ungetline();
}
f_expecting(fd, line)
int fd;
char *line;
{
	int cnt;

	while((cnt=getline(fd, line))>0 && line[0] != ':')
	{
		line[cnt-1]=' ';
		testing("%s", line);
	}
	if(cnt > 0) ungetline();
}
static int exp_val;
f_retval(fd, line)
int fd;
char *line;
{
	static char retbuf[BSIZ];
	char *p;
	int sign;
	int cnt;

	retbuf[0] = '\0';
	while((cnt=getline(fd, line))>0 && line[0] != ':')
	{
		line[cnt-1] = '\0';
		strcat(retbuf, line);
	}

	if(cnt > 0) ungetline();
	exp_val = 0;
	p = retbuf;

	if (*p == '-')
	{
		p++;
		sign = -1;
	}
	else
		sign = 1;

	while(*p)
		if(!isdiggy(*p))
			severr("Bad expected ret value in data file.");
		else
			exp_val = exp_val * 10 + *p++ - '0';

	exp_val *= sign;
}
f_system(fd, line)
int fd;
char *line;
{
	static char sysbuf[BSIZ];
	static char tmpfile[PATH_MAX];
	int ret_val;
	int cnt;

	start
	sysbuf[0] = '\0';

	while((cnt=getline(fd, line))>0 && line[0] != ':')
		strcat(sysbuf, line);

	if(cnt > 0) ungetline();
	strcpy(tmpfile, temp_path());
	strcat(tmpfile, "sysout");

	expecting ( VOID );
	(void) unlink(tmpfile);
	rwind_files();

	expecting ( VOID );
	if(close(fileno(stderr)))
		severr("Cannot close stderr.");

	expecting ( VOID );
	if((fd=open("/dev/null", O_RDWR)) != fileno(stderr))
		severr("cannot open /dev/null for stderr");
	ret_val=system(sysbuf);

	expecting ( VOID );
	if(close(fileno(stderr)))
		severr("cannot close stderr");

	expecting ( VOID );
	if(dup(fileno(stdout))!=fileno(stderr))
		severr("cannot dup stdout to stderr");

	if((ret_val&0x0ffff) != (exp_val&0x0ffff))
	{
		error("Unexpected return status from system.\nThe expected return value was %d.\nThe actual return value was %d.\n", exp_val, ret_val);	/* ERROR */
	}

	finis

}
f_file(fd, line)
int fd;
char *line;
{
	static char filbuf[BSIZ];
	static char tmpfile[PATH_MAX];
	int cnt;

	filbuf[0] = '\0';
	while((cnt=getline(fd, line))>0 && line[0] != ':')
		strcat(filbuf, line);

	if (cnt > 0) ungetline();

	strcpy(tmpfile, temp_path());
	strcat(tmpfile, "sysout");
	compare(filbuf, tmpfile);
}
f_eot(fd, line)
int fd;
char *line;
{
	int cnt;
	while((cnt=getline(fd, line))>0 && line[0] != ':') ;

	if (cnt>0) ungetline();
}
compare(buf, filename)
char *buf;
char *filename;
{
	static char fbuf[BSIZ];
	int fd;
	int i;
	int cnt;

	expecting ( VOID );
	if((fd=open(filename, O_RDONLY))<0)
		severr("Can't open output file.");

	cnt = 0;

	while(read(fd, &fbuf[cnt], 1)>0) cnt++;

	fbuf[cnt]='\0';

	for(i=0;i<cnt;i++)
	{
		if (buf[i] != fbuf[i])
		{
			error("unexpected file contents\nexpected value was %s\nactual value was %s:\n", buf, fbuf);	/* ERROR */
			break;
		}
	}

	if (cnt != strlen(buf))
		error("Unexpected file contents.\nThe expected string was %s.\nThe actual string was %s:\n", buf, fbuf);	/* ERROR */

	expecting ( VOID );
	(void) close(fd);

}
struct macro 
{
	char *sym;
	char *val;
	int  siz;
} mac[]= 
{
	{	"DATA", 		NULL, 	0	},
	{	"TEMP", 		NULL, 	0	},
	{	"SIGHUP", 	NULL, 	0	},
	{	"SIGINT", 	NULL, 	0	},
	{	"SIGQUIT", 	NULL, 	0	},
	{	"SIGILL", 	NULL, 	0	},
	{	"SIGTRAP", 	NULL, 	0	},
	{	"SIGFPE", 	NULL, 	0	},
	{	"SIGKILL", 	NULL, 	0	},
	{	"SIGSYS", 	NULL, 	0	},
	{	"SIGPIPE", 	NULL, 	0	},
	{	"SIGALRM", 	NULL, 	0	},
	{	"SIGTERM", 	NULL, 	0	},
	{	"SIGUSR1", 	NULL, 	0	},
	{	"SIGUSR2", 	NULL, 	0	},
	{	"SIGHUP0200", 	NULL, 	0	},
	{	"SIGINT0200", 	NULL, 	0	},
	{	"SIGQUIT0200", 	NULL, 	0	},
	{	"SIGILL0200", 	NULL, 	0	},
	{	"SIGTRAP0200", 	NULL, 	0	},
	{	"SIGFPE0200", 	NULL, 	0	},
	{	"SIGKILL0200", 	NULL, 	0	},
	{	"SIGSYS0200", 	NULL, 	0	},
	{	"SIGPIPE0200", 	NULL, 	0	},
	{	"SIGALRM0200", 	NULL, 	0	},
	{	"SIGTERM0200", 	NULL, 	0	},
	{	"SIGUSR10200", 	NULL, 	0	},
	{	"SIGUSR20200", 	NULL, 	0	},
	{	"SIGHUPEXIT", 	NULL, 	0	},
	{	"SIGINTEXIT", 	NULL, 	0	},
	{	"SIGQUITEXIT", 	NULL, 	0	},
	{	"SIGILLEXIT", 	NULL, 	0	},
	{	"SIGTRAPEXIT", 	NULL, 	0	},
	{	"SIGFPEEXIT", 	NULL, 	0	},
	{	"SIGKILLEXIT", 	NULL, 	0	},
	{	"SIGSYSEXIT", 	NULL, 	0	},
	{	"SIGPIPEEXIT", 	NULL, 	0	},
	{	"SIGALRMEXIT", 	NULL, 	0	},
	{	"SIGTERMEXIT", 	NULL, 	0	},
	{	"SIGUSR1EXIT", 	NULL, 	0	},
	{	"SIGUSR2EXIT", 	NULL, 	0	},
	{	"STDIN", 	NULL, 	0	},
	{	"STDOUT", 	NULL, 	0	},
	{	"STDERR", 	NULL, 	0	},
	{	"IFILE0", 	NULL, 	0	},
	{	"IFILE1", 	NULL, 	0	},
	{	"IFILE2", 	NULL, 	0	},
	{	"OFILE0", 	NULL, 	0	},
	{	"OFILE1", 	NULL, 	0	},
	{	"OFILE2", 	NULL, 	0	}, 
};

# define NMACS	50

init_mac()
{
	int i;

	expecting ( VOID );
	for(i=0; i < NMACS; i++)
		if(!(mac[i].val = malloc(256)))
			severr("Malloc failed.");

	strcpy( mac[0].val, data_path() );
	mac[0].siz = strlen( mac[0].val );

	strcpy( mac[1].val, temp_path() );
	mac[1].siz = strlen( mac[1].val );

	(void) sprintf( mac[2].val, "%d", SIGHUP );
	mac[2].siz = strlen( mac[2].val );

	(void) sprintf( mac[3].val, "%d", SIGINT );
	mac[3].siz = strlen( mac[3].val );

	(void) sprintf( mac[4].val, "%d", SIGQUIT );
	mac[4].siz = strlen( mac[4].val );

	(void) sprintf( mac[5].val, "%d", SIGILL );
	mac[5].siz = strlen( mac[5].val );

	(void) sprintf( mac[6].val, "%d", SIGTRAP );
	mac[6].siz = strlen( mac[6].val );

	(void) sprintf( mac[7].val, "%d", SIGFPE );
	mac[7].siz = strlen( mac[7].val );

	(void) sprintf( mac[8].val, "%d", SIGKILL );
	mac[8].siz = strlen( mac[8].val );

	(void) sprintf( mac[9].val, "%d", SIGSYS );
	mac[9].siz = strlen( mac[9].val );

	(void) sprintf( mac[10].val, "%d", SIGPIPE );
	mac[10].siz = strlen( mac[10].val );

	(void) sprintf( mac[11].val, "%d", SIGALRM );
	mac[11].siz = strlen( mac[11].val );

	(void) sprintf( mac[12].val, "%d", SIGTERM );
	mac[12].siz = strlen( mac[12].val );

	(void) sprintf( mac[13].val, "%d", SIGUSR1 );
	mac[13].siz = strlen( mac[13].val );

	(void) sprintf( mac[14].val, "%d", SIGUSR2 );
	mac[14].siz = strlen( mac[14].val );

	(void) sprintf( mac[15].val, "%d", SIGHUP|0200 );
	mac[15].siz = strlen( mac[15].val );

	(void) sprintf( mac[16].val, "%d", SIGINT|0200 );
	mac[16].siz = strlen( mac[16].val );

	(void) sprintf( mac[17].val, "%d", SIGQUIT|0200 );
	mac[17].siz = strlen( mac[17].val );

	(void) sprintf( mac[18].val, "%d", SIGILL|0200 );
	mac[18].siz = strlen( mac[18].val );

	(void) sprintf( mac[19].val, "%d", SIGTRAP|0200 );
	mac[19].siz = strlen( mac[19].val );

	(void) sprintf( mac[20].val, "%d", SIGFPE|0200 );
	mac[20].siz = strlen( mac[20].val );

	(void) sprintf( mac[21].val, "%d", SIGKILL|0200 );
	mac[21].siz = strlen( mac[21].val );

	(void) sprintf( mac[22].val, "%d", SIGSYS|0200 );
	mac[22].siz = strlen( mac[22].val );

	(void) sprintf( mac[23].val, "%d", SIGPIPE|0200 );
	mac[23].siz = strlen( mac[23].val );

	(void) sprintf( mac[24].val, "%d", SIGALRM|0200 );
	mac[24].siz = strlen( mac[24].val );

	(void) sprintf( mac[25].val, "%d", SIGTERM|0200 );
	mac[25].siz = strlen( mac[25].val );

	(void) sprintf( mac[26].val, "%d", SIGUSR1|0200 );
	mac[26].siz = strlen( mac[26].val );

	(void) sprintf( mac[27].val, "%d", SIGUSR2|0200 );
	mac[27].siz = strlen( mac[27].val );

	(void) sprintf( mac[28].val, "%d", ((SIGHUP|0200)<<8)&0x0ffff );
	mac[28].siz = strlen( mac[28].val );

	(void) sprintf( mac[29].val, "%d", ((SIGINT|0200)<<8)&0x0ffff );
	mac[29].siz = strlen( mac[29].val );

	(void) sprintf( mac[30].val, "%d", (short)((SIGQUIT|0200)<<8)&0x0ffff );
	mac[30].siz = strlen( mac[30].val );

	(void) sprintf( mac[31].val, "%d", ((SIGILL|0200)<<8)&0x0ffff );
	mac[31].siz = strlen( mac[31].val );

	(void) sprintf( mac[32].val, "%d", ((SIGTRAP|0200)<<8)&0x0ffff );
	mac[32].siz = strlen( mac[32].val );

	(void) sprintf( mac[33].val, "%d", ((SIGFPE|0200)<<8)&0x0ffff);
	mac[33].siz = strlen( mac[33].val );

	(void) sprintf( mac[34].val, "%d", ((SIGKILL|0200)<<8)&0x0ffff);
	mac[34].siz = strlen( mac[34].val );

	(void) sprintf( mac[35].val, "%d", ((SIGSYS|0200)<<8)&0x0ffff);
	mac[35].siz = strlen( mac[35].val );

	(void) sprintf( mac[36].val, "%d", ((SIGPIPE|0200)<<8)&0x0ffff);
	mac[36].siz = strlen( mac[36].val );

	(void) sprintf( mac[37].val, "%d", ((SIGALRM|0200)<<8)&0x0ffff);
	mac[37].siz = strlen( mac[37].val );

	(void) sprintf( mac[38].val, "%d", ((SIGTERM|0200)<<8)&0x0ffff);
	mac[38].siz = strlen( mac[38].val );

	(void) sprintf( mac[39].val, "%d", ((SIGUSR1|0200)<<8)&0x0ffff);
	mac[39].siz = strlen( mac[39].val );

	(void) sprintf( mac[40].val, "%d", ((SIGUSR2|0200)<<8)&0x0ffff);
	mac[40].siz = strlen( mac[40].val );

	(void) sprintf( mac[41].val, "%d", fileno(stdin));
	mac[41].siz = strlen( mac[41].val );

	(void) sprintf( mac[42].val, "%d", fileno(stdout));
	mac[42].siz = strlen( mac[42].val );

	(void) sprintf( mac[43].val, "%d", fileno(stderr));
	mac[43].siz = strlen( mac[43].val );

	(void) sprintf( mac[44].val, "%d", fileno(infile[0].ifptr));
	mac[44].siz = strlen( mac[44].val );

	(void) sprintf( mac[45].val, "%d", fileno(infile[1].ifptr));
	mac[45].siz = strlen( mac[45].val );

	(void) sprintf( mac[46].val, "%d", fileno(infile[2].ifptr));
	mac[46].siz = strlen( mac[46].val );

	(void) sprintf( mac[47].val, "%d", fileno(outfile[0].ofptr));
	mac[47].siz = strlen( mac[47].val );

	(void) sprintf( mac[48].val, "%d", fileno(outfile[1].ofptr));
	mac[48].siz = strlen( mac[48].val );

	(void) sprintf( mac[49].val, "%d", fileno(outfile[2].ofptr));
	mac[49].siz = strlen( mac[49].val );
}
do_macro(fd, line)
int fd;
char *line;
{
	static char buf[256];
	char ch;
	int j;
	int sts;

	buf[0] = '\0';
	ch = '\0';

	j = 0;
	while( (sts = getbyte(fd)) > 0 )
	{
		ch = sts & 0xff;
		if (ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9')
		{
			buf[j++] = ch;
		}
		else 
		{
			buf[j] = '\0';
			break;
		}
	}

	for(j = 0; j < NMACS && strcmp( mac[j].sym , buf ); j++) ;
	if (j == NMACS)
	{
		severr("Bad macro call in data file.");
	}

	strcat( line, mac[j].val );
	ungetbyte();
	return( mac[j].siz );
}

init_files()
{
	int i;

	for( i=0; i<3 ;i++ )
		strcpy(infile[i].ifile, data_path());

	strcat(infile[0].ifile, "data0");
	strcat(infile[1].ifile, "data1");
	strcat(infile[2].ifile, "data2");

	for( i=0; i<3 ;i++ )
		if( !(infile[i].ifptr = fopen(infile[i].ifile, "r")) )
			severr("Can't open data file.");


	for( i=0; i<3 ;i++ )
		strcpy(outfile[i].ofile, temp_path());

	strcat(outfile[0].ofile, "temp0");
	strcat(outfile[1].ofile, "temp1");
	strcat(outfile[2].ofile, "temp2");

	for( i=0; i<3 ;i++ )
		if( !(outfile[i].ofptr = fopen(outfile[i].ofile, "w+")) )
			severr("Can't open temp file.");

}
rwind_files()
{
	int i;

	for( i=0; i<3 ;i++ )
		rewind(infile[i].ifptr);
	for( i=0; i<3 ;i++ )
		rewind(outfile[i].ofptr);
}
