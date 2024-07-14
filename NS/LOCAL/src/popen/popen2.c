#include <sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)popen2.c	1.5"

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
	{	":POPEN:", 	f_system	},
	{	":FILE:", 	f_file		},
	{	":EOT:", 	f_eot		}, 
};
# define NENT	(sizeof(stuff)/sizeof(struct foo))

#define TEST	0
#define EOT	7

# define BSIZ 2048

# define	isdiggy(z)	(z >= '0' && z <= '9')

char **zenvp;
void exit();

main(argc, argv, envp)
int argc;
char *argv[];
char *envp[];
{
	int fd;
	char *line;
	char *malloc();
	char datafile[PATH_MAX];

	zenvp = envp;

	setup("popen");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( SEVERR | ERROR );

	start

	prep("Obtain memory.");
	prep("Initialize temporary files and open data and command files.");
	start

	expecting ( VOID );
	if ( !(line = malloc(BSIZ)) )
	{

		severr("out of memory");
		exit(-1);
	}

	init_mac();

	strcpy(datafile, data_path());
	strcat(datafile, "environ.data");

	expecting ( SUCCESS );
	fd=open(datafile, O_RDONLY);
	finis

	sysex(fd, line);

	expecting ( SUCCESS );
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
			expecting ( VOID );
			if(!strncmp(line, stuff[i].token, strlen(stuff[i].token)))
			{
				start
				expecting ( VOID );
				(*stuff[i].func)(fd, line);
				finis
				break;
			}
		}

		if ( i == TEST )
		{
			start

				while ((i=getline(fd, line))>0)
				{
			
					for(i=0;i<NENT;i++)
					{
						expecting ( VOID );
						if(!strncmp(line, stuff[i].token, strlen(stuff[i].token)))
						{
							expecting ( VOID );
							(*stuff[i].func)(fd, line);
							break;
						}
					}

					if ( i == EOT )
						break;
					if( i == NENT )
						{
							severr("Unrecognized line in file inner loop");
						}
			}
			finis
		}
		if( i == NENT )


		{
			severr("Unrecognized line in file outer loop");
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
			severr("line too long");
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
**
**	line no. from data file on verify line
**
**	(void) sprintf(Verify, "line: %05d ", lineno);
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
			severr("bad expected ret value in data file");
		else
			exp_val = exp_val * 10 + *p++ - '0';

	exp_val *= sign;
}

FILE *fptr;
f_system(fd, line)
int fd;
char *line;
{
	static char sysbuf[BSIZ];
	static char tmpfile[PATH_MAX];
	int cnt;


	sysbuf[0] = '\0';

	while((cnt=getline(fd, line))>0 && line[0] != ':')
		strcat(sysbuf, line);

	if(cnt > 0) ungetline();
	strcpy(tmpfile, temp_path());
	strcat(tmpfile, "sysout");

	expecting ( VOID );
	(void) unlink(tmpfile);

	expecting ( SUCCESS );
	close(fileno(stderr));

	expecting ( VOID );
	if((fd=open("/dev/null", O_RDWR)) != fileno(stderr))
		severr("cannot open /dev/null for stderr");

	expecting ( SUCCESS );
	fptr=popen(sysbuf, "r");

	expecting ( SUCCESS );
	close(fileno(stderr));

	expecting ( VOID );
	if(dup(fileno(stdout))!=fileno(stderr))
		severr("cannot dup stdout to stderr");

}

f_file(fd, line)
int fd;
char *line;
{
	static char filbuf[BSIZ];
	int cnt;

	filbuf[0] = '\0';

	while((cnt=getline(fd, line))>0 && line[0] != ':')
		strcat(filbuf, line);

	if (cnt > 0) ungetline();
	fcompare(filbuf, fptr);
}

f_eot(fd, line)
int fd;
char *line;
{
	int cnt;
	int ret_val;

	expecting ( VOID );
	ret_val = pclose(fptr);
	if((ret_val&0x0ffff) != (exp_val&0x0ffff))
	{
		error("Unexpected return status from pclose.\nExpected value on close was %d.\nThe actual value on close was %d.\n", exp_val, ret_val);	/* ERROR */
	}

	while((cnt=getline(fd, line))>0 && line[0] != ':') ;
	if (cnt>0) ungetline();
}

fcompare(buf, fptr)
char *buf;
FILE *fptr;
{
	char filename[PATH_MAX];
	char buf2[BUFSIZ];
	int cnt;
	int fd;

	strcpy(filename, temp_path());
	strcat(filename, "popen.env");

	expecting ( VOID );
	if( (fd = open( filename, O_CREAT | O_WRONLY | O_TRUNC, 0664 ) ) < 0 )
		severr("can't open/creat popen.env temp file");

	while(cnt = fread( buf2, 1, BUFSIZ, fptr))
		if( write(fd, buf2, cnt) != cnt)
			severr("can't write popen.env temp file");

	expecting ( VOID );
	close(fd);
	compare(buf, filename);
}

compare(buf, filename)
char *buf;
char *filename;
{
	static char *fbuf = (char *)0;
	static int fsize = 0;
	int fd;
	int i, j;
	int cnt, my_cnt, its_cnt;
	char **myenvp;
	char **itsenvp;
	struct stat st_buf;

	expecting ( SUCCESS );
	fd=open(filename, O_RDONLY);

	expecting ( SUCCESS );
	fstat(fd, &st_buf);

	if( (long)fsize < st_buf.st_size )
	{
		if(fbuf) free(fbuf);
		expecting ( VOID );
		if(!(fbuf = malloc( (unsigned) (st_buf.st_size +1) )))
			severr("out of memory");
	}

	cnt = 0;

	while(read(fd, &fbuf[cnt], 1)>0) cnt++;

	fbuf[cnt]='\0';

	expecting ( VOID );
	(void) close(fd);

	for(i=0;zenvp[i];i++) ;

	expecting ( VOID );
	if(!(myenvp = (char **)malloc( (i + 16) * sizeof(char *) )))
		severr("out of memory");


	expecting ( VOID );
	if(!(itsenvp = (char **)malloc( (i + 16) * sizeof(char *) )))
		severr("out of memory");
	i = 0;
	while (zenvp[i])
	{
		myenvp[i] = zenvp[i];
		i++;
	}
	myenvp[i] = (char *)0;
	my_cnt = buf2env(myenvp, buf);
	itsenvp[0] = (char *)0;
	its_cnt = buf2env(itsenvp, fbuf);

	for(i=0;i<my_cnt;i++)
		if(!strncmp(myenvp[i], "HOME=", 5))
			break;
	if(i == my_cnt)
		severr("parent missing environment variable HOME");

	for(i=0;i<my_cnt;i++)
		if(!strncmp(myenvp[i], "PATH=", 5))
			break;

	if(i == my_cnt)
		severr("Parent missing environment variable PATH");

	for(i=0;i<its_cnt;i++)
		if(!strncmp(itsenvp[i], "HOME=", 5))
			break;

	if(i == its_cnt)
	{
		error("Command missing environment variable HOME");	/* ERROR */
	goto done;
}

for(i=0;i<its_cnt;i++)
if(!strncmp(itsenvp[i], "PATH=", 5))
break;

if(i == its_cnt)
{
	error("Command missing environment variable PATH");	/* ERROR */

goto done;
}
if(my_cnt != its_cnt)
{
	error("Number of environment parameters differs.The expected number of paramaters was %d.\nThe actual number of parameters was %d.\n", my_cnt, its_cnt);	/* ERROR */
goto done;
}

for(i=0;i<my_cnt;i++)
{
	for(j=0;j<its_cnt;j++)
		if(itsenvp[j] && !strcmp(myenvp[i], itsenvp[j]))
			break;
	if(j == its_cnt)
	{
		error("Missing environment parameter in child.\nCould not find %s.\n", myenvp[i]);	/*  ERROR */
	goto done;
}
itsenvp[j] = (char *)0;
}
for(i=0;itsenvp[i];i++)
{
	if(itsenvp[i])
	{
		error("Unexpected environment parameter in child.\nUnexpectedly found %s.\n", itsenvp[i]);	/*  ERROR */
	goto done;
}
}
done:
free((char *)itsenvp);
free((char *)myenvp);
}

buf2env(envp, buf)
char *envp[];
char *buf;
{
	int i, j;
	char *l;
	char *e;

	i = 0;
	l = buf;

	while(envp[i]) i++;

	while(*l)
	{
		e = l+1;
		while(*e && *e != '\n') e++;
		if ((j = find_env(envp, l))>=0) envp[j] = l;
		else envp[i++] = l;
		envp[i] = (char *)0;
		if(!*e) break;
		l = e+1;
		*e = '\0';
	}
	return(i);
}

find_env(envp, buf)
char *envp[];
char *buf;
{
	static char sym[512];
	int i, j;

	i = 0;

	while(*buf && *buf != '=' && *buf != '\n') sym[i++] = *buf++;
	if(*buf != '=')
		error("Missing '=' in environment list.");	/* ERROR */
	sym[i] = '=';
	sym[i+1] = '\0';

	for(j=0 ; envp[j] ; j++)
		if(!strncmp(envp[j], sym, i+1))
			return(j);
	return(-1);
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
};

# define NMACS	2

init_mac()
{
	int i;

	for(i=0; i < NMACS; i++)
		if(!(mac[i].val = malloc(256)))
			severr("malloc failed");

	strcpy( mac[0].val, data_path() );
	mac[0].siz = strlen( mac[0].val );
	strcpy( mac[1].val, temp_path() );
	mac[1].siz = strlen( mac[1].val );
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
		severr("Bad macro call in data file");
	}

	strcat( line, mac[j].val );
	ungetbyte();
	return( mac[j].siz );
}
