#include <sv_base.h> /*replacement macros */
#include	<signal.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)pop0.c	1.2"

/*
**
** pop0 [-i [infile]] [-t waitval] [-x exitval] [-s signum] [-p procnum] [-e] [-o [outfile]] [args]*
**
**	pop0 			exit(0);
**	pop0 -x exitval		exit(exitval);
**	pop0 -s signum		kill(getpid(),signum);
**	pop0 -o 		copy argv to stdout
**	pop0 -o ofile		copy argv to ofile
**	pop0 -o[d]		copy to fd
**	pop0 -i 		copy stdin instead of argv
**	pop0 -i[d]		copy from fd
**	pop0 -i ifile 		copy ifile instead of argv
**	pop0 -e 		copy envp instead of argv
**	pop0 -t waitval		seconds to sleep before start
**	pop0 -p procnum		process id to kill, default is self
**
**	x and s oppose, i alone is an eater, signum, waitval, and exitval 
**	are decimal, i and e oppose
**	
*/

# define	isdiggy(z)	(z >= '0' && z <= '9')

FILE *inp = stdin;
FILE *outp = stdout;
FILE *fopen();

char *progname = NULL;
char *ifile = NULL;
int iflag = 0;
char *ofile = NULL;
int oflag = 0;
char *exitval = NULL;
int xflag = 0;
int eflag = 0;
char *procval = NULL;
int pflag = 0;
char *signum = NULL;
int sflag = 0;
int xit = 0;
int sig = 0;
int procnum = 0;
int tflag = 0;
char *waitval = NULL;
int tik;

char *myargs[2048];
unsigned sleep();
void exit();

main(argc,argv,envp)
int argc;
char *argv[];
char *envp[];
{

	char *arg;
	int i;
	int sts;
	int sign;
	int mycnt;
	char *p;

	progname = argv[0];
	mycnt = 0;

	expecting ( VOID );
	for( i = 1; i < argc ; i++ )
	{
		arg = argv[i];
		if (*arg++ != '-')
		{
			myargs[mycnt++] = argv[i];
			continue;
		}
		switch( *arg++ )
		{

		case 'i':
			iflag = 1;
			if(*arg)
			{
				if (isdiggy(*arg))
					if(!*(arg+1))
					{
						if(inp = fdopen((*arg-'0'),"r"))
							break;
					}
				oops("bad argument");
			}
			else
			{
				i++;
				if (i < argc)
				{
					if(!*(arg = argv[i]))
						oops("bad infile");
					if (*arg == '-')
						i--;
					else 
						ifile= arg;
				}
			}
			break;

		case 'e':
			eflag = 1;
			if(*arg)
				oops("bad argument");
			break;

		case 'x':
			xflag = 1;
			if(*arg)
				oops("bad argument");
			else
			{
				i++;
				if (i < argc)
				{
					if(!*(arg = argv[i]))
						oops("bad exitval");
					exitval= arg;
				}
			}
			break;

		case 't':
			tflag = 1;
			if(*arg)
				oops("bad argument");
			else
			{
				i++;
				if (i < argc)
				{
					if(!*(arg = argv[i]))
						oops("bad waitval");
					waitval= arg;
				}
			}
			break;

		case 'p':
			pflag = 1;
			if(*arg)
				oops("bad argument");
			else
			{
				i++;
				if (i < argc)
				{
					if(!*(arg = argv[i]))
						oops("bad procnum");
					procval = arg;
				}
			}
			break;

		case 's':
			sflag = 1;
			if(*arg)
				oops("bad argument");
			else
			{
				i++;
				if (i < argc)
				{
					if(!*(arg = argv[i]))
						oops("bad signum");
					signum = arg;
				}
			}
			break;

		case 'o':
			oflag = 1;
			if(*arg)
			{
				if (isdiggy(*arg))
					if(!*(arg+1))
					{
						if(outp = fdopen((*arg-'0'),"w+"))
							break;
					}
				oops("bad argument");
			}
			else
			{
				i++;
				if (i < argc)
				{
					if(!*(arg = argv[i]))
						oops("bad ofile");
					if (*arg == '-')
						i--;
					else 
						ofile= arg;
				}
			}
			break;
		}
	}
	if(xflag && sflag) oops("bad combination of args 'x' and 's'");
	if(xflag)
	{
		if(!(p=exitval)) oops("bad exitval");
		if (*p == '-')
		{
			p++;
			sign = -1;
		}
		else
			sign = 1;
		while(*p)
			if(!isdiggy(*p))
				oops("bad exitval");
			else
				xit = xit * 10 + *p++ - '0';
		xit *= sign;
	}
	if(sflag)
	{
		if(!(p=signum)) oops("bad signum");
		while(*p)
			if(!isdiggy(*p))
				oops("bad signum");
			else
				sig = sig * 10 + *p++ - '0';
	}
	if(pflag)
	{
		if(!(p=procval)) oops("bad procnum");
		while(*p)
			if(!isdiggy(*p))
				oops("bad procnum");
			else
				procnum = procnum * 10 + *p++ - '0';
	}
	if(iflag)
	{
		if (ifile)
			if(!(inp=fopen(ifile,"r")))
				oops("can't open ifile");
	}
	if(oflag)
	{
		if (ofile)
			if(!(outp=fopen(ofile,"a")))
				oops("can't open ofile");
	}
	if(tflag)
	{
		if(!(p=waitval)) oops("bad waitval");
		while(*p)
			if(!isdiggy(*p))
				oops("bad waitval");
			else
				tik = tik * 10 + *p++ - '0';
		(void) sleep((unsigned)tik);
	}

	if(oflag)
	{
		if(iflag)
			while((sts=fgetc(inp))!=EOF)
				(void) fputc(sts & 0x00ff,outp);
		else if (eflag)
			for(i=0;envp[i];i++)
				(void) fprintf(outp,"%s\n",envp[i]);
		else
		{
			for(i=0;i<mycnt;i++)
				(void) fprintf(outp,"%s\n",myargs[i]);
		}
		if (ofile) (void) fclose(outp);
	}
	else
		if(iflag)
			while(fgetc(inp)!=EOF);
	if(xflag)
		exit(xit);

	(void) fflush(stdout);

	if(sflag)
		if (pflag)
			(void) kill( procnum, sig );
		else
			(void) kill(getpid(),sig);
	else
		if (pflag)
			(void) kill( procnum, SIGKILL );

	exit(0);
}
oops(str)
char *str;
{
	severr(str);
	exit(-1);
}
