#include <sv_base.h> /*replacement macros */
# define INIT		register char *regptr = instring;
# define GETC()		(*regptr++)
# define PEEKC()	(*regptr)
# define UNGETC(c)	(--regptr)
# define RETURN(c)	return ((char *)0)
# define ERROR(zz)	fexcept("internal error in pattern matching test")
#include	<regexp.h>
# undef ERROR
#include	<fcntl.h>
#include	<stdio.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)popen1.c	1.5"


char *l = "*?[!]-.bevy/";
char *lf = ".Bdes";
char *ltemp = "sB.ed";

char temp_string[32];
char xcept[256];

char fstring[5];

struct files 
{
	char name[5];
	char hit;
} files[153];

struct dirs 
{
	char dname[5];
	char dhit;
} dirs[3] = 
{
	{	".", 	0	},
	{	"..", 	0	},
	{	"ze", 	0	}, 
};

int fcnt = 0;

char string[11];

# define	isletter(zz)	((zz>='a'&&zz<='z')||(zz>='A'&&zz<='Z'))

int elength;
char here[PATH_MAX];

FILE *fptr;
char *getcwd();

main()
{
	char	PathPointer[PATH_MAX];

	setup("popen");
	exit_on ( ERROR | SEVERR );

	start

	prep("Chdir into correct environment and initialize files.");
	start

	expecting ( VOID );
	if(!getcwd(here, PATH_MAX))
	{
		severr("cannot getcwd");
	}
	strcpy(PathPointer, temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;
	if(chdir(PathPointer))
	{
		severr("cannot chdir to temp directory");
	}
	init_temp();
	init_files();
	string[0] = '\0';
	finis

	testing("Matching file names against:");
	for(elength = 1; elength < 7 ;elength++){
		do_exp(&string[1], elength);
	}
	finis
	done();
	/*NOTREACHED*/
};

char range;
int state = 0;
int flag = -1;
int letcnt = 0;
int starcnt = 0;
int bracecnt = 0;
int dashcnt = 0;
int quescnt = 0;
int dotcnt = 0;
int slashcnt = 0;

do_exp(str, len)
char *str;
int len;
{
	char *p;
	char sbuf[256];
	int ret_val;

	char b_range = range;
	int b_state = state;
	int b_flag = flag;
	int b_letcnt = letcnt;
	int b_starcnt = starcnt;
	int b_bracecnt = bracecnt;
	int b_dashcnt = dashcnt;
	int b_quescnt = quescnt;
	int b_dotcnt = dotcnt;
	int b_slashcnt = slashcnt;


	if (!len)
	{
		*str = '\0';
		if (!bad_exp(str, 0))
		{
			testing("pattern match: %s", &string[1]);
			start
			switch (match(&string[1]))
			{
			case -1:
				break;
			case 0:
				expecting ( VOID );
				(void) unlink("sysout");
				strcpy(sbuf, here);
				strcat(sbuf, "/pop1 ");

				strcat(sbuf, &string[1]);

				expecting ( SUCCESS );
				fptr = popen(sbuf, "r");

				(void) check_results(0, fptr);

				expecting ( SUCCESS );
				pclose(fptr);

				break;
			default:
				expecting ( VOID );
				(void) unlink("sysout");
				strcpy(sbuf, here);
				strcat(sbuf, "/pop1 ");

				strcat(sbuf, &string[1]);

				expecting ( SUCCESS );
				fptr = popen(sbuf, "r");

				(void) check_results(1, fptr);

				expecting ( SUCCESS );
				pclose(fptr);
			}
			finis
		}
		return;
	}

	for(p=l;*p;p++)
	{
		*str = *p;
		if (!bad_exp(str, len))
			do_exp((str+1), (len-1));
		range = b_range;
		state = b_state;
		flag = b_flag;
		letcnt = b_letcnt;
		starcnt = b_starcnt;
		bracecnt = b_bracecnt;
		dashcnt = b_dashcnt;
		quescnt = b_quescnt;
		dotcnt = b_dotcnt;
		slashcnt = b_slashcnt;
	}
}
bad_exp(exp, len)
char *exp;
int len;
{
	char *p;


	if (string[1]=='/') return(-1);
	if (len < 3 && elength > 4 && !bracecnt) return(-1);
	if (len < 3 && elength > 5 && !dashcnt) return(-1);

	switch(*exp)
	{
	case 0:
		if (*(exp-1) == '/')
			return(-1);
		return(flag | state);

	case '/':
		if (slashcnt++ || state)
			return(-1);
		break;

	case '*':
		if (starcnt++>=2 || state  || *(exp-1) == '*') return(-1);
		flag = 0;
		break;
	case '?':
		if (quescnt++ >= 3 || state) return(-1);
		flag = 0;
		break;
	case '!':
		if (state != 1) return(-1);
		state = 2;
		break;
	case '[':
		if (bracecnt++ >= 2 || state) return(-1);
		state = 1;
		flag = 0;
		break;
	case ']':
		if (state != 3 && state != 5) return(-1);
		if (*(exp-2)=='[')
			return(-1);
		state = 0;
		break;
	case '-':
		if (dashcnt++>=2 || state != 3) return(-1);
		range = *(exp-1);
		state = 4;
		break;
	case '.':
		if ( dotcnt++ ) return(-1);
		/* fall through */
	default:
		if (letcnt++ >= 4) return(-1);

		if (state && *exp == '.') return(-1);

		switch(state)
		{
		case 0:
			if (*exp != 'b' && *exp != 'e' && *exp != '.')
				return(-1);
			if (*exp == 'b' && elength > 3)
				return(-1);
			if (isletter( *(exp-1) ))
				return(-1);
			break;
		case 1:
		case 2:
			state = 3;
			break;
		case 3:
			p = exp-1;
			while(*p != '[')
				if(*p--==*exp)
				{
					return(-1);
				}
			break;
		case 4:
			if (range >= *exp)
				return(-1);
			state = 5;
			break;
		case 5:
			state=3;
			break;
		}
	}
	return(0);
}

clr_files()
{
	int i;

	for(i=0;i<153;i++)
		files[i].hit=0;
	dirs[0].dhit = 0;
	dirs[1].dhit = 0;
	dirs[2].dhit = 0;
}

init_files()
{
	char *p;

	for(p=lf;*p;p++)
	{
		fstring[0] = *p;
		do_file(&fstring[1], 2);
	}
}

do_file(str, len)
char *str;
int len;
{
	char *p;

	*str = '\0';

	expecting ( VOID );
	if(strcmp(".", fstring)&&strcmp("..", fstring))
		strcpy(files[fcnt++].name, fstring);
	if (!len) return;

	for(p=lf;*p;p++)
	{
		*str = *p;
		do_file((str+1), (len-1));
	}
}

char *pcomp(exp)
char *exp;
{
	char re[1024];
	char *rp = re;
	static char rbuf[1024];
	char *compile();
	int flag = 0;

	*rp++ = '^';

	while (*exp)
	{
		switch(*exp)
		{

		case '*':
			*rp++ = '.';
			*rp++ = '*';
			flag = 1;
			break;
		case '?':
			*rp++ = '.';
			flag = 1;
			break;
		case '.':
			*rp++ = '\\';
			*rp++ = '.';
			flag = 1;
			break;
		case '!':
			*rp++ = '^';
			if (!flag) *rp++ = '.';
			break;
		case '[':
			*rp++ = *exp;
			break;
		default:
			*rp++ = *exp;
			flag = 1;
			break;
		}

		exp++;
	}
	*rp++ = '$';
	*rp = '\0';

	if (compile(re, &rbuf[0], &rbuf[1024], 0))
	{
		severr("regexp compile returned unexpected value");
	}
	return(rbuf);
}

pmatch(flag, r, s)
char *r;
char *s;
int flag;
{
	int sts;
	if (flag && *s == '.') return(0);
	sts=advance(s, r);
	return(sts);
}

match(exp)
char *exp;
{
	int i, j;
	char *px;
	int cnt = 0;
	int flag = 0;
	char direxp[32];

	if (*exp == '*' || *exp == '?')
		flag = 1;
	clr_files();
	for(i=1;i<5;i++)
	{
		if(exp[i]=='/')
		{
			strcpy(direxp, exp);
			direxp[i]=0;
			if (!(px=pcomp(direxp))) return(0);
			if(pmatch(flag, px, "..")) return(-1);
			if(pmatch(flag, px, dirs[0].dname)) dirs[0].dhit |= 2;
			if(pmatch(flag, px, dirs[2].dname)) dirs[2].dhit |= 2;
			if(dirs[0].dhit || dirs[2].dhit)
			{
				flag = 0;
				if (exp[i+1] == '*' || exp[i+1] == '?')
					flag = 1;
				for(j=0;j<153;j++)
				{
					if (!(px=pcomp(&exp[i+1])))
						return(0);
					if(pmatch(flag, px, files[j].name))
					{
						cnt++;
						files[j].hit |= 4;
					}
				}
				if(pmatch(flag, px, dirs[0].dname))
				{
					cnt++;
					dirs[0].dhit |= 4;
				}
				if(pmatch(flag, px, dirs[1].dname))
				{
					cnt++;
					dirs[1].dhit |= 4;
				}
			}
			return(cnt);
		}
	}
	if (!(px=pcomp(exp)))
		return(0);
	if(pmatch(flag, px, dirs[0].dname))
	{
		cnt++;
		dirs[0].dhit |= 1;
	}
	if(pmatch(flag, px, dirs[1].dname))
	{
		cnt++;
		dirs[1].dhit |= 1;
	}
	if(pmatch(flag, px, dirs[2].dname))
	{
		cnt++;
		dirs[2].dhit |= 1;
	}
	for(j=0;j<153;j++)
	{
		if(pmatch(flag, px, files[j].name))
		{
			cnt++;
			files[j].hit |= 1;
		}
	}
	return(cnt);
}

check_results(f, fptr)
int f;
FILE *fptr;
{
	int cnt, i;
	char *p;
	static char longname[256];
	static char buf[10240];

	cnt = 0;
	expecting ( VOID );
	while(fread(&buf[cnt], 1, 1, fptr)==1) cnt++;
	buf[cnt]='\0';

	p = buf;
	if (!f)
	{
		if (!smatch(&p, &string[1]))
		{
			return(-1);
		}
	}
	if( dirs[0].dhit & 1 )
		if (!smatch(&p, dirs[0].dname))
		{
			return(-1);
		}
	if( dirs[1].dhit & 1 )
		if (!smatch(&p, dirs[1].dname))
		{
			return(-1);
		}
	for(i=0;i<153;i++)
		if(files[i].hit & 1)
			if (!smatch(&p, files[i].name))
			{
				return(-1);
			}
	if( dirs[2].dhit & 1 )
		if (!smatch(&p, dirs[2].dname))
		{
			return(-1);
		}
	if( dirs[0].dhit & 2 )
	{
		if( dirs[0].dhit & 4 )
		{
			(void) sprintf(longname, "%s/%s", dirs[0].dname, dirs[0].dname);
			if (!smatch(&p, longname))
				return(-1);
		}
		if( dirs[1].dhit & 4 )
		{
			(void) sprintf(longname, "%s/%s", dirs[0].dname, dirs[1].dname);
			if (!smatch(&p, longname))
				return(-1);
		}
		for(i=0;i<153;i++)
		{
			if(files[i].hit & 4)
			{
				(void) sprintf(longname, "%s/%s", dirs[0].dname, files[i].name);
				if (!smatch(&p, longname))
				{
					return(-1);
				}
			}
		}
	}

	if( dirs[2].dhit == 2 )
	{
		if( dirs[0].dhit & 4 )
		{
			(void) sprintf(longname, "%s/%s", dirs[2].dname, dirs[0].dname);
			if (!smatch(&p, longname))
				return(-1);
		}
		if( dirs[1].dhit & 4 )
		{
			(void) sprintf(longname, "%s/%s", dirs[2].dname, dirs[1].dname);
			if (!smatch(&p, longname))
				return(-1);
		}
		for(i=0;i<153;i++)
		{
			if(files[i].hit & 4)
			{
				(void) sprintf(longname, "%s/%s", dirs[2].dname, files[i].name);
				if (!smatch(&p, longname))
				{
					return(-1);
				}
			}
		}
	}
	return(0);
}

smatch(pp, name)
char **pp;
char *name;
{
	register char *p = *pp;
	register char *n = name;
	static int errcnt = 0;

	while( *p == *n )
	{
		p++;
		n++;
	}

	if (*n || *p != '\n')
	{
		while(*p && *p != '\n') p++;
		*p = '\0';
		if (errcnt++ < 10)
	{
error("Pattern matching mismatch.\nThe expected string was %s.\nThe actual string was %s.\n", name, *pp);	/* ERROR */
	}
		else
	{
severr("Too many mismatches.\nThe expected string was %s.\nThe actual string was %s.\n", name, *pp);	/* HALT */
	}
		return(0);
	}
	*pp = (p+1);
	return(1);
}

init_temp()
{
	expecting ( VOID );
	system ("rm -rf *");

	do_temp(temp_string, 3);
	do_temp(temp_string, 1);

	expecting ( VOID );
	if(system("mkdir ze"))
	{
		severr("cannot make temp subdirectory");
	}

	do_temp(temp_string, 2);
	if(chdir("ze"))
	{
		severr("cannot chdir to temp subdirectory");
	}

	do_temp(temp_string, 2);
	do_temp(temp_string, 3);
	do_temp(temp_string, 1);

	expecting ( VOID );
	if(chdir(".."))
	{
		severr("cannot chdir from temp subdir to temp dir");
	}
}

do_temp(str, len)
char *str;
int len;
{
	int fd;
	char *p;

	if (!len)
	{
		*str = '\0';
		expecting ( VOID );
		if(strcmp(".", temp_string) && strcmp("..", temp_string))
		{
			expecting ( SUCCESS );
			fd=open(temp_string, O_CREAT|O_WRONLY, 0664);

			expecting ( SUCCESS );
			(void) close(fd);
		}
		return;
	}

	for(p=ltemp;*p;p++)
	{
		*str = *p;
		do_temp((str+1), (len-1));
	}
}

fexcept(str)
char *str;
{

	severr(str);

}
