#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)getopt.c	1.5"



extern	char	*optarg;
extern	int	optind, opterr;
FILE		*temp_fopen();

# define	MAXARGV		9	

/*
** getopt
**	Test the getopt command
*/
main()
{

setup("getopt");
funct("getopt");

expecting (SUCCESS);
fclose(stderr);
	
expecting (SUCCESS);
temp_fopen("tmpgetopt","w");

abstract ("testing legal cases.");
start
	test_legal();
finis

abstract ("testing illegal cases.");
start
	test_illegal();
finis

abstract ("testing double-dashcases.");
start
	test_doubledash();
finis


done();
/*NOTREACHED*/
}

char	*
output_test()
{
	static	char	buffer[STRLEN];
	FILE		*fp;
	int		number;

	expecting (SUCCESS);
	fclose(stderr);
	
	fp = temp_fopen("tmpgetopt","r");

	buffer[0] = '\0';
	expecting (VOID);  /*  allow the read to read nothing  */
	number = fread(buffer,1,STRLEN,fp);
	expecting (SUCCESS);
	fclose(fp);

	temp_fopen("tmpgetopt","w");

	if ( number )
		return ( buffer );
	else
		return ( NULL );
}
/*
** test_legal
**	Test a legal argv, argc combination.
**
** Level
**	This routine is in the normal level (0).
*/
test_legal()
{
	char	*argv[MAXARGV];
	int	argc;
	short	flags[MAXARGV];
	int	ret_val;
	int	i;

testing ("getopt with no arguments should return EOF");
start
	argc = 0;
	optind = 1;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}

	expecting (EOF);
	getopt(argc,argv,"ab:c:C:x");
finis


testing("getopt should work with a legal argv, argc");
testing("Returning 'a', 'b', 'c', 'C' and 'x'.");
start
	optind = 1;

	argv[0] = "SVVS";
	argv[1] = "-a";
	argv[2] = "-bbflag";
	argv[3] = "-c";
	argv[4] = "cflag";
	argv[5] = "-CCflag";
	argv[6] = "-x";
	argv[7] = "-x";
	argv[8] = "-x";
	argc = 9;

	expecting (VOID);
	while ( (ret_val = getopt(argc,argv,"ab:c:C:x")) != EOF )
	{
		switch ( ret_val )
		{
		  case 'a':
			if ( flags[0] )
			{
				error("getopt returned the 'a' flag twice");
			}
			if ( optind != 2 )
			{
				error("getopt did not set optid correctly,\
optind = %d", optind);
			}
			flags[0] = 1;
			break;

		  case 'b':
			if ( flags[1] )
			{
				error("getopt returned the 'b' flag twice");
			}
			flags[1] = 1;
			expecting (VOID);
			if ( strcmp("bflag",optarg) )
			{
				error("optarg is not set to the right string\
optarg = '%s'",optarg);
			}
			if ( optind != 3 )
			{
				error("getopt did not set optid correctly,\
optind = %d", optind);
			}
			break;

		  case 'c':
			if ( flags[2] )
			{
				error("getopt returned the 'c' flag twice");
			}
			flags[2] = 1;
			expecting (VOID);
			if ( strcmp("cflag",optarg) )
			{
				error("optarg is not set to the right string\
optarg = '%s'",optarg);
			}
			if ( optind != 5 )
			{
				error("getopt did not set optid correctly\
optind = %d",optind);
			}
			break;

		  case 'C':
			if ( flags[3] )
			{
				error("getopt returned the 'C' flag twice");
			}
			flags[3] = 1;
			expecting (VOID);
			if ( strcmp("Cflag",optarg) )
			{
				error("optarg is not set to the right string\
optarg = '%s'",optarg);
			}
			if ( optind != 6 )
			{
				error("getopt did not set optid correctly\
optind = %d",optind);
			}
			break;

		  case 'x':
			if ( flags[4] > 2 )
			{
				error("getopt returned the -x flag four times");
			}
			if ( optind != ( 7 + flags[4] ) )
			{
				error("getopt did not set optid correctly\
optind = %d",optind);
			}
			flags[4]++;
			break;

		  case '?':
			error ("getopt returned a '?' on a legal argv string");

		  default:
			error ("getopt returned an unexpected character of '%c'",
				ret_val);
		}
	expecting (VOID);  /*  there is no error checking for getenv  */
	}

	if ( flags[0] != 1 )
	{
		error("getopt did not return the expected number of 'a's.");
	}
	if ( flags[1] != 1 )
	{
		error("getopt did not return the expected number of 'b's.");
	}
	if ( flags[2] != 1 )
	{
		error("getopt did not return the expected number of 'c's.");
	}
	if ( flags[3] != 1 )
	{
		error("getopt did not return the expected number of 'C's.");
	}
	if ( flags[4] != 3 )
	{
		error("getopt did not return the expected number of 'x's.");
	}
finis


testing ("getopt with no arguments beginning with - should return EOF");
start

	argc = MAXARGV;
	optind = 1;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}
	argv[0] = "SVVS";
	argv[1] = "a";
	argv[2] = "bbflag";
	argv[3] = "c";
	argv[4] = "cflag";
	argv[5] = "CCflag";

	expecting (EOF);

	getopt(argc,argv,"ab:c:C:x");
finis



testing ("getopt should stop scanning for options when a non-option is found");
start

	argc = MAXARGV;
	optind = 1;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}
	argv[0] = "SVVS";
	argv[1] = "-a";
	argv[2] = "a";
	argv[3] = "-a";

	expecting ('a');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 2)
		error ("optind = %d, should be 2", optind);

	expecting (EOF);
	getopt(argc,argv,"ab:c:C:x");
finis



testing ("getopt should stop scanning for options when a '-' is found");
start

	argc = MAXARGV;
	optind = 1;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}
	argv[0] = "SVVS";
	argv[1] = "-ax";
	argv[2] = "-a";
	argv[3] = "-";
	argv[4] = "-a";

	expecting ('a');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 1)
		error ("optind = %d, should be 1", optind);

	expecting ('x');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 2)
		error ("optind = %d, should be 2", optind);

	expecting ('a');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 3)
		error ("optind = %d, should be 3", optind);

	expecting (EOF);
	getopt(argc,argv,"ab:c:C:x");
finis



testing ("testing rule 5:");
testing ("options with no arguments may be grouped behind one delimiter");
testing ("due to backwards compatibilities, rule 6");
testing ("(the first option-argument following an option must be");
testing ("preceded by white space) may not apply");
start

	argc = 2;
	optind = 1;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}
	argv[0] = "SVVS";
	argv[1] = "-axcxa";

	expecting ('a');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 1)
		error ("optind = %d, should be 1", optind);

	expecting ('x');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 1)
		error ("optind = %d, should be 1", optind);

	expecting ('c');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 2)
		error ("optind = %d, should be 2", optind);
	expecting (VOID);
	if (strcmp (optarg, "xa") != 0)
		error ("optarg = '%s', 'xa' was expected", optarg);

	expecting (EOF);
	getopt(argc,argv,"ab:c:C:x");
finis



testing ("testing rule 8:");
testing ("groups of option arguments following an option");
testing ("must be separated by commas or separated by white space and quoted");
start
	optind = 1;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}
	argv[0] = "SVVS";
	argv[1] = "-xC";
	argv[2] = "clist,has,four,words";
	argv[3] = "-b";
	argv[4] = "this is for b";
	argv[5] = "-";
	argv[6] = "-c ignored";
	argc = 7;

	expecting ('x');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 1)
		error ("optind = %d, should be 1", optind);

	expecting ('C');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 3)
		error ("optind = %d, should be 3", optind);
	expecting (VOID);
	if (strcmp (optarg, "clist,has,four,words") != 0)
		error ("optarg = '%s', '%s' was expected",
			optarg, "clist,has,four,words");

	expecting ('b');
	getopt(argc,argv,"ab:c:C:x");
	if (optind != 5)
		error ("optind = %d, should be 5", optind);
	expecting (VOID);
	if (strcmp (optarg, "this is for b") != 0)
		error ("optarg = '%s', '%s' was expected",
			optarg, "this is for b");

	expecting (EOF);
	getopt(argc,argv,"ab:c:C:x");
finis
}


/*
** test_illegal
**	Test that getopt returns '?' with an illegal option.
**
** Level
**	This routine is in the normal level (0).
*/
test_illegal()
{
	char	*argv[MAXARGV];
	int	argc;
	short	flags[MAXARGV];
	int	ret_val;
	int	i;
	char	*sp;
	int	errflag;

testing("getopt should return '?' on an illegal option, ");
testing("setting opterr to 0 should suppress any output, returning '?'.");
start

	argc = MAXARGV;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}

	argv[0] = "SVVS";
	argv[1] = "-a";
	argv[2] = "-bbflag";
	argv[3] = "-c";
	argv[4] = "cflag";
	argv[5] = "-CCflag";
	argv[6] = "-x";
	argv[7] = "-x";
	argv[8] = "-x";
	errflag = 0;
	opterr = 0;
	optind = 1;

	expecting (VOID);
	while ( (ret_val = getopt(argc,argv,"b:c:C:x")) != EOF )
	{
		switch(ret_val)
		{
		  case '?':
			if ( errflag )
			{
				error("getopt returned '?' twice, on one illegal argument");
			}
			if ( (sp = output_test()) != NULL )
			{
				error ("getopt printed on stderr, even with \
opterr flag set to 0");
			}
			errflag++;
			break;

		  case 'b':
		  case 'c':
		  case 'C':
		  case 'x':
			if ( !errflag )
			{
				error ("getopt did not return a '?' on an \
illegal flag previous to '%c'",ret_val);
			}
			break;

		  default:
			error ("Unexpected return from getopt of '%c'",ret_val);
		}
	expecting (VOID);  /*  there is no error checking for getenv  */
	}

	if ( !errflag )
	{
		error("getopt returned EOF without returning '?' for on an \
illegal argument");
	}
finis


testing("getopt should return '?' on an illegal option, ");
testing("and setting opterr to 1 should cause output on stderr,");
testing("returning '?', and output on stderr.");
start
	errflag = 0;
	opterr = 1;
	optind = 1;

	expecting (VOID);
	while ( (ret_val = getopt(argc,argv,"b:c:C:x")) != EOF )
	{
		switch(ret_val)
		{
		  case '?':
			if ( errflag )
			{
				error("getopt returned '?' twice, on one illegal argument");
			}
			if ( (sp = output_test()) == NULL )
			{
				error ("getopt did not print on stderr, \
even with opterr flag set to 1");
			}
			errflag++;
			break;

		  case 'b':
		  case 'c':
		  case 'C':
		  case 'x':
			if ( !errflag )
			{
				error ("getopt did not return a '?' on an \
illegal flag, previous to '%c'",ret_val);
			}

			if ( (sp = output_test()) != NULL )
			{
				error ("getopt printed '%s' on stderr,\n\
no error was expected");
			}
			break;

		  default:
			error ("Unexpected return from getopt of '%c'",ret_val);
		}
	expecting (VOID);  /*  there is no error checking for getenv  */
	}

	if ( !errflag )
	{
		error("getopt returned EOF without returning '?' for on an \
illegal argument");
	}
finis



testing ("a missing argument should generate an error");
testing ("(rule 7: option arguments cannot be optional)");
testing ("both as a return of '?' and on stderr since opterr = 1");
start


	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}

	argv[0] = "SVVS";
	argv[1] = "-wxc";
	argc = 2;
	errflag = 0;
	opterr = 1;
	optind = 1;

	expecting ('?');
	getopt(argc,argv,"b:c:C:x");
	if ( (sp = output_test()) == NULL )
	{
		error ("getopt did not print on stderr, \
with opterr flag set to 1");
	}

	expecting ('x');
	getopt(argc,argv,"b:c:C:x");
	if ( (sp = output_test()) != NULL )
	{
		error ("getopt printed '%s' on stderr,\n\
no error was expected");
	}

	expecting ('?');
	getopt(argc,argv,"b:c:C:x");
	if ( (sp = output_test()) == NULL )
	{
		error ("getopt did not print on stderr, \
even with opterr flag set to 1");
	}
finis



testing ("a missing argument should generate an error");
testing ("as a return of '?' but not on stderr since opterr = 0");
start
	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}

	argv[0] = "SVVS";
	argv[1] = "-wxc";
	argc = 2;
	errflag = 0;
	opterr = 0;
	optind = 1;

	expecting ('?');
	getopt(argc,argv,"b:c:C:x");
	if ( (sp = output_test()) != NULL )
	{
		error ("getopt printed '%s' on stderr,\n\
no message was expected");
	}

	expecting ('x');
	getopt(argc,argv,"b:c:C:x");
	if ( (sp = output_test()) != NULL )
	{
		error ("getopt printed '%s' on stderr,\n\
no error was expected");
	}

	expecting ('?');
	getopt(argc,argv,"b:c:C:x");
	if ( (sp = output_test()) != NULL )
	{
		error ("getopt printed '%s' on stderr,\n\
no message was expected");
	}
finis

}

/*
** test_doubledash
**	Check that "--" works.
**
** Level
**	This routine is in the normal level (0).
*/
test_doubledash()
{
	char	*argv[MAXARGV];
	int	argc;
	short	flags[MAXARGV];
	int	ret_val;
	int	i;

testing("getopt should stop processing at '--', returning EOF at '--'.");
start

	argc = MAXARGV;

	for ( i = 0 ; i < MAXARGV ; i++ )
	{
		argv[i] = NULL;
		flags[i] = 0;
	}

	argv[0] = "SVVS";
	argv[1] = "-a";
	argv[2] = "-bbflag";
	argv[3] = "-c";
	argv[4] = "cflag";
	argv[5] = "--";
	argv[6] = "-x";
	argv[7] = "-x";
	argv[8] = "-x";
	optind = 1;
	opterr = 1;

	expecting (VOID);
	while ( (ret_val = getopt(argc,argv,"ab:c:C:x")) != EOF )
	{
		switch ( ret_val )
		{
		  case 'a':
			if ( flags[0] )
			{
				error("getopt returned the 'a' flag twice");
			}
			if ( optind != 2 )
			{
				error("getopt did not set optid correctly,\
optind = %d", optind);
			}
			flags[0] = 1;
			break;

		  case 'b':
			if ( flags[1] )
			{
				error("getopt returned the 'b' flag twice");
			}
			flags[1] = 1;
			expecting (VOID);
			if ( strcmp("bflag",optarg) )
			{
				error("optarg is not set to the right string\
optarg = '%s'",optarg);
			}
			if ( optind != 3 )
			{
				error("getopt did not set optid correctly\
optind = %d",optind);
			}
			break;

		  case 'c':
			if ( flags[2] )
			{
				error("getopt returned the 'c' flag twice");
				return;
			}
			flags[2] = 1;
			expecting (VOID);
			if ( strcmp("cflag",optarg) )
			{
				error("optarg is not set to the right string,\
optarg = '%s'",optarg);
			}
			if ( optind != 5 )
			{
				error("getopt did not set optid correctly, \
optid = %d",optind);
			}
			break;

		  case 'x':
			error ("getopt returned an 'x' after the '--'");

		  case '?':
			error ("getopt returned a '?' on a legal argv string");

		  default:
			error ("getopt returned an unexpected character of '%c'",
				ret_val);
		}
	expecting (VOID);  /*  there is no error checking for getenv  */
	}

	if ( flags[0] != 1 )
	{
		error ("getopt did not return the expected number of 'a's.");
	}
	if ( flags[1] != 1 )
	{
		error("getopt did not return the expected number of 'b's.");
	}
	if ( flags[2] != 1 )
	{
		error("getopt did not return the expected number of 'c's.");
	}

finis
}
