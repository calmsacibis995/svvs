#include	<sv_base.h> /*replacement macros */
#include	<signal.h>
#include	<errno.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)strtod.c	1.5"



# ifndef	SYSVR_1
extern	double	strtod();
# endif	/* Not SYSVR_1 */
extern	double	atof();
extern	char	*err_lookup ();


#ifndef	SYSVR_2
	void (*signal())();
#else
	int (*signal())();
#endif

extern	int	fork();
extern	int	errno;
int	handler();

/*
** strtod
**	Test the string to double routines.
*/
main()
{
setup("strtod");
# ifndef	SYSVR_1
funct ("strtod");
start
	t_strtod();
finis

# endif	/* Not SYSVR_1 */
funct ("atof");
start
	t_atof();
finis

done();
}

# ifndef	SYSVR_1
/*
** t_strtod
**	String to Double
*/
t_strtod()
{
	double	ret_val;
	char	*ptr;
	char	*foo;
	char	*exp;
	int		cstat,pid;
testing("strtod(0) should return 0.0.");
start
	expecting (0.0);
	strtod("0.0",(char **) 0);
finis


testing("strtod(123e1) should return 1230.");
start
	expecting (1230.0);
	strtod("123e1",(char **) 0);
finis


testing("strtod(+123e1) should return 1230.");
start
	expecting (1230.0);
	strtod("+123e1",(char **) 0);
finis


testing("strtod( 123e1) should return 1230.");
start
	expecting (1230.0);
	strtod(" 123e1",(char **) 0);
finis


testing("strtod(123E1) should return 1230.");
start
	expecting (1230.0);
	strtod("123E1",(char **) 0);
finis


testing("strtod(-123E1) should return -1230.");
start
	expecting (-1230.0);
	strtod("-123E1",(char **) 0);
finis


testing("strtod(1.23) should return 1.23.");
start
	expecting (1.23);
	strtod("1.23",(char **) 0);
finis


testing("strtod(1.23E1) should return 12.30.");
start
	expecting (12.3);
	strtod("1.23E1",(char **) 0);
finis


testing("strtod(0) should set the pointer to the");
testing("location beyond the parsed portion of string.");
start
	foo="0.0";
	exp=foo+strlen(foo);

	expecting (0.0);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod(123e1) should set the pointer to the");
testing("location beyond parsed portion of string.");
start

	foo="123e1";
	exp=foo+strlen(foo);

	expecting (1230.0);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error("pointer set incorrectly");
finis


testing("strtod(+123e1) should set the pointer to the");
testing("location beyond parsed portion of string.");
start
	foo="+123e1";
	exp=foo+strlen(foo);

	expecting (1230.0);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod( 123e1) should set the pointer to the");
testing("location beyond parsed portion of string");
start
	foo=" 123e1";
	exp=foo+strlen(foo);

	expecting (1230.0);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod(123E1) should set the pointer to the");
testing("location beyond parsed portion of string.");
start
	foo="123E1";
	exp=foo+strlen(foo);

	expecting (1230.0);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod(-123E1) should set the pointer to the");
testing("location beyond parsed portion of string.");
start
	foo="-123E1";
	exp=foo+strlen(foo);

	expecting (-1230.0);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod(1.23) should set the pointer to the");
testing("location beyond parsed portion of string.");
start
	foo="1.23";
	exp=foo+strlen(foo);

	expecting (1.23);
	strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod(1.23E1) should set the pointer to the");
testing("location beyond parsed portion of string.");
start
	foo="1.23E1";
	exp=foo+strlen(foo);

	expecting (12.3);
	  strtod(foo,&ptr);
	if ( ptr < exp )
		error ("pointer set incorrectly");
finis


testing("strtod(^%$#) should return 0, and set ptr to the start.");
start
	foo = "^%$#";
	expecting (0.0);
	strtod(foo,&ptr);

	if ( ptr == 0 )
		error ("strtod changed ptr to 0");

	if ( ptr != foo )
		error ("ptr does not point to the start of the string");
finis


testing("strtod(\"123%\") should return 123,");
testing("with ptr pointing beyond the parsed portion of the string.");
start
	foo = "123%";
	exp= foo + strlen(foo) - 1;

	expecting (123.0);
	strtod(foo,&ptr);

	if ( !ptr )
		error ("strtod changed ptr to 0");

	if ( ptr < exp )
		error ("pointer was set incorrectly");
finis


testing("Trying a string representing a VERY small number.");
testing("should return 0.0, and errno set to ERANGE.");
testing("On some systems this causes a SIGFPE.");
testing("If this occurs, an advisory message will be generated.");
start
	expecting (SUCCESS);  /*  error if -1 is returned  */
	if(!fork()){
/*
** Child will perform this test since it is prone to SIGFPEs
*/
		expecting (VOID);
#ifdef SYSVR_2
		if(signal(SIGFPE,handler) == (siggy (*)())-1)
			error ("Cannot catch SIGFPE in child.");
#else
		signal(SIGFPE,handler);
#endif

		expecting (0.0);
		expecting( ERANGE );
		strtod("1.0e-5000000000000",(char **) 0);

		exit(0);
	}
	wait(&cstat);

	if(cstat & 0xff00){
		warning ("%s\nchild returned status %d", ADVISORY, cstat);
		}
finis


testing("a number larger than can be represented");
testing("should return HUGE, and an errno ERANGE.");
start
	expecting (VOID);
	ret_val = strtod("1.0e5000000000000",(char **) 0);
	if ( !DCMP(ret_val,HUGE) ) /* if values are not close */
	{
		if (ret_val == 0)
			warning ("%s\nreturned 0.", ADVISORY);
		else
		{
			if (!(DCMP(HUGE, ret_val)))
				error ("strtod returned %e, %e was expected",
					ret_val, HUGE);
			else
				warning ("%s\nstrtod returned %e, %e was expected",
					ADVISORY, ret_val, HUGE);
		}
	}
	else
	{
		if ( errno != ERANGE )
		{
			if (errno == 0)
				warning ("%s\nerrno still 0", ADVISORY);
			else
				error ("errno = %s, ERANGE was expected", 
					err_lookup (errno) );
		}
	}
finis


testing("a negative number of larger absolute magnitude");
testing("than can be represented.");
testing("should return -HUGE, and an errno ERANGE.");
start
	expecting (VOID);
	ret_val = strtod("-1.0e5000000000000",(char **) 0);
	if ( !DCMP(ret_val,-HUGE) )
	{
		if (ret_val == 0)
			warning ("%s\nreturned 0", ADVISORY);
		else
		{
			if (!(DCMP(-HUGE, ret_val)))
				error ("strtod returned %e, %e was expected",
					ret_val, -HUGE);
			else
				warning ("%s\nstrtod returned %e, %e was expected",
					ADVISORY, ret_val, -HUGE);
		}
	}
	else
	{
		if ( errno != ERANGE )
		{
			if (errno == 0)
				warning ("%s\nerrno remained 0.", ADVISORY);
			else
				error ("errno = %s, ERANGE was expected",
					err_lookup (errno) );
		}
	}

finis
}

# endif	/* Not SYSVR_1 */

/*
** t_atof
**	String to Double
*/
t_atof()
{
	double	ret_val;
	int	cstat;
testing("atof(0) should return 0.0.");
start
	expecting (0.0);
	atof("0.0");
finis


testing("atof(123e1) should return 1230.");
start
	expecting (1230.0);
	atof("123e1");
finis


testing("atof(+123e1) should return 1230.");
start
	expecting (1230.0);
	atof("+123e1");
finis


testing("atof( 123e1) should return 1230.");
start
	expecting (1230.0);
	atof(" 123e1");
finis


testing("atof(123E1) should return 1230.");
start
	expecting (1230.0);
	atof("123E1");
finis


testing("atof(-123E1) should return -1230.");
start
	expecting (-1230.0);
	atof("-123E1");
finis


testing("atof(1.23) should return 1.23.");
start
	expecting (1.23);
	atof("1.23");
finis


testing("atof(1.23E1) should return 12.30.");
start
	expecting (12.3);
	atof("1.23E1");
finis


testing("atof(^%$#) should return 0.");
start
	expecting (0.0);
	atof("^%$#");
finis



testing("atof(\"123%\") should return 123.");
start
	expecting (123.0);
	atof("123%");
finis


testing("A string representing a VERY small number");
testing("should return 0.0, with errno set to ERANGE.");
testing("On some systems this causes a SIGFPE.");
testing("If this occurs an advisory message will be generated.");
start
	expecting (SUCCESS);
	if(!fork()){

#ifdef SYSVR_2
		if(signal(SIGFPE,handler) == (siggy (*)())-1)
			error ("Cannot catch SIGFPE in child.");
#else
		signal(SIGFPE,handler);
#endif

		/*
		 *  This is an advisory message now
		 *  expecting (ERANGE);
		 */
		expecting (0.0);
		atof("1.0e-5000000000000");

		if (errno != ERANGE)
			warning ("%s\nerrno = %s, ERANGE was expected",
				ADVISORY, err_lookup (errno));
		exit(0);
	}
	wait(&cstat);
	if(cstat & 0xff00){
		warning ("%s\nchild returned %d", ADVISORY, cstat);
	}
finis


testing("a number larger than can be represented.");
testing("should return HUGE, and an errno ERANGE.");
start
	expecting (VOID);
	ret_val = atof("1.0e5000000000000");
	if ( !DCMP(ret_val,HUGE) )
	{
		if (!(DCMP(HUGE, ret_val)))
			error ("atof returned %e, %e was expected",
				ret_val, HUGE);
		else
			warning ("%s\natof returned %e, %e was expected",
				ADVISORY, ret_val, HUGE);
	}
	else
	{
		if ( errno != ERANGE )
		{
			error ("errno = %s, ERANGE was expected", 
				err_lookup (errno) );
		}
	}
finis


testing("a negative number of larger absolute magnitude");
testing("than can be represented.");
testing("should return -HUGE, and an errno ERANGE.");
start
	expecting (VOID);
	ret_val = atof("-1.0e5000000000000");
	if ( !DCMP(ret_val,-HUGE) )
	{
		if ( !DCMP(-HUGE, ret_val) )
			{
			if (DCMP(HUGE, ret_val) )
				{
				warning ("%s\natof returned %e, %e was expected",
					ADVISORY, ret_val, -HUGE);
				}
			else

			error ("atof returned %e, %e was expected", ret_val, -HUGE);
			}
		else 
			warning ("%s\natof returned %e, %e was expected",
				ADVISORY, ret_val, -HUGE);
	}
	else
	{
		if ( errno != ERANGE )
		{
			error("errno = %s, ERANGE was expected", 
				err_lookup (errno) );
		}
	}

finis
}


handler(sig)
int	sig;
{
	if(sig != SIGFPE)
		error ("Signal caught was %d, SIGFPE was expected.", sig);
	else
		warning ("%s\nSignal SIGFPE was caught", ADVISORY);
	exit(0);
}
