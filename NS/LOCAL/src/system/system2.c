#include <sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)system2.c	1.5"

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
# define NENT	(sizeof(stuff)/sizeof(struct foo))

# define BSIZ 2048

# define	isdiggy(z)	(z >= '0' && z <= '9')

char **zenvp;

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

	setup("system");
	funct("system");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	prep("Obtain memory.");
	prep("Initialize temporary files and open data and command files.");
	start
		exit_on ( ERROR | SEVERR );

		expecting ( NON_NULL );
		line = malloc(BSIZ);

		init_mac();

		strcpy(datafile, data_path());
		strcat(datafile, "environ.data");

		expecting ( SUCCESS );
		fd=open(datafile, O_RDONLY);
	finis

	testing ("perform the tests from the datafile");
	start
		sysex(fd, line);

		expecting ( VOID );
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
			severr("Unrecognized line in file.");
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
			severr("Bad expected return value in data file.");
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

	expecting ( VOID );
	if(close(fileno(stderr)))
		severr("Cannot close stderr.");

	expecting ( VOID );
	if((fd=open("/dev/null", O_RDWR)) != fileno(stderr))
		severr("Cannot open /dev/null for stderr.");

	expecting ( VOID );
	ret_val=system(sysbuf);

	expecting ( VOID );
	if(close(fileno(stderr)))
		severr("Cannot close stderr.");

	expecting ( VOID );
	if(dup(fileno(stdout))!=fileno(stderr))
		severr("Cannot dup stdout to stderr.");

	if((ret_val&0x0ffff) != (exp_val&0x0ffff))
	{
		error("Unexpected return status from system.\nThe expected return status was %d.\nThe actual return status was %d.\n", exp_val, ret_val);	/* ERROR */
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
	static char *fbuf = (char *)0;
	static int fsize = 0;
	int fd;
	int i, j;
	int cnt, my_cnt, its_cnt;
	char **myenvp;
	char **itsenvp;
	struct stat st_buf;


	expecting ( VOID );
	if((fd=open(filename, O_RDONLY))<0)
		severr("Can't open output file.");

	expecting ( VOID );
	if(fstat(fd, &st_buf))
		severr("Can't stat output file.");

	if( (long)fsize < st_buf.st_size )
	{
		expecting ( VOID );
		if(fbuf) free(fbuf);

		expecting ( VOID );
		if(!(fbuf = malloc( (unsigned) (st_buf.st_size +1))))
			severr("Out of memory.");
	}
	cnt = 0;

	expecting ( VOID );
	while(read(fd, &fbuf[cnt], 1)>0) cnt++;
	fbuf[cnt]='\0';

	expecting ( VOID );
	(void) close(fd);

	expecting ( VOID );
	for(i=0;zenvp[i];i++) ;
	if(!(myenvp = (char **)malloc( (i + 16) * sizeof(char *) )))
		severr("Out of memory.");

	expecting ( VOID );
	if(!(itsenvp = (char **)malloc( (i + 16) * sizeof(char *) )))
		severr("Out of memory.");
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
		severr("Parent missing environment variable HOME.");

	for(i=0;i<my_cnt;i++)
		if(!strncmp(myenvp[i], "PATH=", 5))
			break;
	if(i == my_cnt)
		severr("Parent missing environment variable PATH.");

	for(i=0;i<its_cnt;i++)
		if(!strncmp(itsenvp[i], "HOME=", 5))
			break;

	if(i == its_cnt)
	{
		error("Command missing environment variable HOME.");	/* ERROR */
		goto done;
	}

	for(i=0;i<its_cnt;i++)
		if(!strncmp(itsenvp[i], "PATH=", 5))
			break;

	if(i == its_cnt)
	{
		error("Command missing environment variable PATH.");	/* ERROR */

		goto done;
	}
	if(my_cnt != its_cnt)
	{
		error("Number of environment parameters differs.\nThe expected count was %d.\nThe actual count was %d.\n", my_cnt, its_cnt);	/* ERROR */
		goto done;
	}

	for(i=0;i<my_cnt;i++)
	{
		for(j=0;j<its_cnt;j++)
			if(itsenvp[j] && !strcmp(myenvp[i], itsenvp[j]))
				break;
		if(j == its_cnt)
		{
			error("Missing environment parameter in child.\nThe expected parameter was %s.", myenvp[i]);	/* ERROR */
			goto done;
		}
		itsenvp[j] = (char *)0;
	}
	for(i=0;itsenvp[i];i++)
	{
		if(itsenvp[i])
		{
			error("Unexpected environment parameter in childThe unexpected parameter was %s.\n", itsenvp[i]);	/* ERROR */
			goto done;
		}
	}
done:
	expecting ( VOID );
	free((char *)itsenvp);
	expecting ( VOID );
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

	expecting ( VOID );
	for(i=0; i < NMACS; i++)
		if(!(mac[i].val = malloc(256)))
			severr("Malloc failed.");

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
		severr("Bad macro call in data file.");
	}

	strcat( line, mac[j].val );
	ungetbyte();
	return( mac[j].siz );
}
