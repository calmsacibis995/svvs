#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	STDERR_FD	2


#ident	"@(#)matherr.c	1.2"



int		Seen;
int		Expecting;
double		Returnval;
int		Printmessage;
int		Twoargs;
double		Arg1;
double		Arg2;
char		*Name;
int		Errortype;
int		ErrNo;

FILE		*Write_Stream;

extern	FILE		*temp_fopen();

extern	double		j0();
extern	double		j1();
extern	double		jn();
extern	double		y0();
extern	double		y1();
extern	double		yn();

extern	double		exp();
extern	double		log();
extern	double		log10();
extern	double		pow();
extern	double		sqrt();

extern	double		gamma();

extern	double		hypot();

extern	double		sin();
extern	double		cos();
extern	double		tan();
extern	double		asin();
extern	double		acos();
extern	double		atan();
extern	double		atan2();

/*
** matherr
**	Test that the math functions call the matherr function
**	when appropriate.  The matherr() function being called
**	is supplied by this test.
*/
main()
{
setup("matherr");
funct("matherr");
Printmessage = 0;
testing ("Testing all functions");
testing("to see if matherr() gets called and the structure passed");
testing("is a valid one.  STDERR messages are NOT checked");
start
	testit();
finis

/****************************
** The following tests will check to see that the 
** appropriate message has been written to STDERR.
*****************************/

abstract ("redirecting STDERR output to a temp file");
start
	expecting (SUCCESS);
	fclose(stderr);
	expecting (VOID);  /*  assure the file is really closed  */
	close(STDERR_FD);
	expecting (SUCCESS);
	Write_Stream = temp_fopen("tmpmatherr","w");
finis

testing ("Testing all functions");
testing("to see if matherr() gets called and the structure passed");
testing("is a valid one and the appropriate STDERR messages is created");
start
	Printmessage = 1;
	testit();
finis

done();
/*NOTREACHED*/
}
/*
** testit()
**	This routine tests all the math functions that call
**	matherr when they receive an illegal parameter.
*/
testit()
{
	double			ret_val;

testing("Call the bessel function j0() with HUGE params");
start
	Name = "j0";
	Errortype = TLOSS;
	Returnval = 0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	j0(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("j0(HUGE) did not call matherr");
finis


testing("Call the bessel function j1() with HUGE params");
start
	Name = "j1";
	Errortype = TLOSS;
	Returnval = 0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	j1(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("j1(HUGE) did not call matherr");
finis


testing("Call the bessel function jn() with HUGE params");
start
	Name = "jn";
	Errortype = TLOSS;
	Returnval = 0;
	Arg1 = 1;
	Twoargs = 1;
	Arg2 = HUGE;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	jn(1,HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("jn(1,HUGE) did not call matherr");
finis


testing("Call the bessel function y0() with HUGE params");
start
	Name = "y0";
	Errortype = TLOSS;
	Returnval = 0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	y0(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("y0(HUGE) did not call matherr");
finis


testing("Call the bessel function y1() with HUGE params");
start
	Name = "y1";
	Errortype = TLOSS;
	Returnval = 0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	y1(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("y1(HUGE) did not call matherr");
finis


testing("Call the bessel function y0() with negative params");
start
	Name = "y0";
	Errortype = DOMAIN; 
	Returnval = -HUGE;
	Arg1 = -1.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	y0(-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");
	Expecting = 0;
	if ( Seen != 1 )
		error("y0(-1.0) did not call matherr");
finis


testing("Call the bessel function y1() with negative params");
start
	Name = "y1";
	Errortype = DOMAIN; 
	Returnval = -HUGE;
	Arg1 = -1.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	y1(-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("y1(-1.0) did not call matherr");
finis


testing("Call the bessel function yn() with negative params");
start
	Name = "yn";
	Errortype = DOMAIN;
	Returnval = -HUGE;
	Arg1 = 1;
	Twoargs = 1;
	Arg2 = -1.0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	yn(1,-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("yn(1,-1.0) did not call matherr");
finis


testing("Call the exp function exp() with HUGE params");
start
	Name = "exp";
	Errortype = OVERFLOW;
	Returnval = HUGE;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (VOID);
	ret_val = exp(HUGE);
	if ( DCMP(ret_val, Returnval) != 1 )
	{
		if ( DCMP(Returnval, ret_val) != 1 )
			error ("The exp function exp(HUGE) returned %g, \
%g expected", ret_val, Returnval);
		else
			warning ("%s\nThe exp function exp(HUGE) returned %g, \
%g expected",
				ADVISORY, ret_val, Returnval);
	}
	/* Note that exp(HUGE) does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("exp(HUGE) did not call matherr");
finis


testing("Call the exp function exp() with -HUGE params");
start
	Name = "exp";
	Errortype = UNDERFLOW;
	Returnval = 0;
	Arg1 = -HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	exp(-HUGE);

	/* Note that exp(-HUGE) does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("exp(-HUGE) did not call matherr");
finis


testing("Call the exp function log() with negative params");
start
	Name = "log";
	Errortype = DOMAIN;
	Returnval = -HUGE;
	Arg1 = -1.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	log(-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("log(-1.0) did not call matherr");
finis


testing("Call the exp function log() with zero params.");
start
	Name = "log";
	Errortype = SING;
	Returnval = -HUGE;
	Arg1 = 0.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	log(0.0);

	if ( Printmessage == 1 )
		checkstderr("SING");

	Expecting = 0;
	if ( Seen != 1 )
		error("log(0.0) did not call matherr");
finis


testing("Call the exp function log10() with negative params.");
start
	Name = "log10";
	Errortype = DOMAIN;
	Returnval = -HUGE;
	Arg1 = -1.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	log10(-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("log10(-1.0) did not call matherr");
finis


testing("Call the exp function log10() with zero params.");
start
	Name = "log10";
	Errortype = SING;
	Returnval = -HUGE;
	Arg1 = 0.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	log10(0.0);

	if ( Printmessage == 1 )
		checkstderr("SING");

	Expecting = 0;
	if ( Seen != 1 )
		error("log10(0.0) did not call matherr");
finis


testing("Call the exp function pow() with (10.0,HUGE).");
start
	Name = "pow";
	Errortype = OVERFLOW;
	Returnval = HUGE;
	Arg1 = 10.0;
	Twoargs = 1;
	Arg2 = HUGE;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (VOID);
	ret_val= pow(10.0,HUGE);
	if ( DCMP(ret_val, Returnval) != 1 && DCMP(ret_val, -Returnval) != 1 )
		if ( DCMP(Returnval, ret_val) != 1 && DCMP(-Returnval, ret_val ) != 1 )
			error ("The exp function pow(10.0,HUGE) returned %g, \
%g expected", ret_val, Returnval);
		else
			warning ("%s\nThe exp function pow(10.0,HUGE) returned \
%g, %g expected", ADVISORY,ret_val, Returnval);

	/* Note that pow(10.0, HUGE) does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("pow(10.0,HUGE) did not call matherr");
finis


testing("Call the exp function pow() with (10.0,-HUGE).");
start
	Name = "pow";
	Errortype = UNDERFLOW;
	Returnval = 0.0;
	Arg1 = 10.0;
	Twoargs = 1;
	Arg2 = -HUGE;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	pow(10.0,-HUGE);

	/* Note that pow(10.0, -HUGE) does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("pow(10.0,-HUGE) did not call matherr");
finis


testing("Call the exp function pow() with (-1.0, 2.2).");
start
	Name = "pow";
	Errortype = DOMAIN;
	Returnval = 0.0;
	Arg1 = -1.0;
	Twoargs = 1;
	Arg2 = 2.2;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	pow(-1.0, 2.2);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("pow(-1.0, 2.2) did not call matherr");
finis


testing("Call the exp function pow() with (0.0,-1.0).");
start
	Name = "pow";
	Errortype = DOMAIN;
	Returnval = 0.0;
	Arg1 = 0.0;
	Twoargs = 1;
	Arg2 = -1.0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	pow(0.0,-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("pow(0.0,-1.0) did not call matherr");
finis


testing("Call the exp function sqrt() with (-1.0).");
start
	Name = "sqrt";
	Errortype = DOMAIN;
	Returnval = 0.0;
	Arg1 = -1.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	sqrt(-1.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("sqrt(-1.0) did not call matherr");
finis


testing("Call the function gamma() with (-1.0).");
start
	Name = "gamma";
	Errortype = SING;
	Returnval = HUGE;
	Arg1 = -1.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (VOID);
	ret_val = gamma(-1.0);
	if ( DCMP(ret_val, Returnval) != 1 )
		if ( DCMP(Returnval, ret_val) != 1 )
			error ("The function gamma(-1.0) returned %g, %g \
was expected", ret_val, Returnval);
		else
			warning ("%s\nThe function gamma(-1.0) returned %g, %g \
was expected", ADVISORY, ret_val, Returnval);

	if ( Printmessage == 1 )
		checkstderr("SING");

	Expecting = 0;
	if ( Seen != 1 )
		error("gamma(-1.0) did not call matherr");
finis


testing("Call the function hypot() with (MAXDOUBLE,MAXDOUBLE).");
start
	Name = "hypot";
	Errortype = OVERFLOW;
	Returnval = HUGE;
	Arg1 = MAXDOUBLE;
	Twoargs = 1;
	Arg2 = MAXDOUBLE;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (VOID);
	ret_val = hypot(MAXDOUBLE,MAXDOUBLE);

	if ( DCMP(ret_val, Returnval) != 1 )
		if ( DCMP(Returnval,ret_val) != 1 )
			error ("hypot(MAXDOUBLE,MAXDOUBLE) did not return HUGE");
		else
			
			warning ("%s\nhypot(MAXDOUBLE,MAXDOUBLE) did not \
return HUGE", ADVISORY);

	/* Note that hypot() does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("hypot(MAXDOUBLE, MAXDOUBLE) did not call matherr");
finis


testing("Call the function sinh() with (HUGE).");
start
	Name = "sinh";
	Errortype = OVERFLOW;
	Returnval = HUGE;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (VOID);
	ret_val = sinh(HUGE);
	if ( DCMP(ret_val, Returnval) != 1 )
		if ( DCMP(Returnval,ret_val) != 1 )
			error ("The function sinh(HUGE) returned %g, \
%g was expected", ret_val, Returnval);
		else
			warning ("%s\nThe function sinh(HUGE) returned %g, \
%g was expected", ADVISORY, ret_val, Returnval);

	/* Note that sinh(HUGE) does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("sinh(HUGE) did not call matherr");
finis


testing("Call the function cosh() with (HUGE).");
start
	Name = "cosh";
	Errortype = OVERFLOW;
	Returnval = HUGE;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (VOID);
	ret_val = cosh(HUGE);
	if ( DCMP(ret_val, Returnval) != 1 )
		if ( DCMP(Returnval,ret_val) != 1 )
			error ("The function cosh(HUGE) returned %g, \
%g was expected", ret_val, Returnval);
		else
			warning  ("%s\nThe function cosh(HUGE) returned %g, \
%g was expected", ADVISORY, ret_val, Returnval);
	/* Note that cosh(HUGE) does not write a message to STDERR */

	Expecting = 0;
	if ( Seen != 1 )
		error("cosh(HUGE) did not call matherr");
finis


testing("Call the function sin() with (HUGE).");
start
	Name = "sin";
	Errortype = TLOSS;
	Returnval = 0.0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	sin(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("sin(HUGE) did not call matherr");
finis


testing("Call the function cos() with (HUGE).");
start
	Name = "cos";
	Errortype = TLOSS;
	Returnval = 0.0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	cos(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("cos(HUGE) did not call matherr");
finis


testing("Call the function tan() with (HUGE).");
start
	Name = "tan";
	Errortype = TLOSS;
	Returnval = 0.0;
	Arg1 = HUGE;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	tan(HUGE);

	if ( Printmessage == 1 )
		checkstderr("TLOSS");

	Expecting = 0;
	if ( Seen != 1 )
		error("tan(HUGE) did not call matherr");
finis


testing("Call the function asin() with (2.0).");
start
	Name = "asin";
	Errortype = DOMAIN;
	Returnval = 0.0;
	Arg1 = 2.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	asin(2.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("asin(2.0) did not call matherr");
finis


testing("Call the function acos() with (2.0).");
start
	Name = "acos";
	Errortype = DOMAIN;
	Returnval = 0.0;
	Arg1 = 2.0;
	Twoargs = 0;
	Arg2 = 0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	acos(2.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("acos(2.0) did not call matherr");
finis


testing("Call the function atan2() with (0.0,0.0).");
start
	Name = "atan2";
	Errortype = DOMAIN;
	Returnval = 0.0;
	Arg1 = 0.0;
	Twoargs = 1;
	Arg2 = 0.0;

	Expecting = 1;
	Seen = 0;
	ErrNo = 0;

	expecting (Returnval);
	atan2(0.0,0.0);

	if ( Printmessage == 1 )
		checkstderr("DOMAIN");

	Expecting = 0;
	if ( Seen != 1 )
		error("atan2(0.0,0.0) did not call matherr");
finis

}

/*
**   checkstderr()
**	This function will look for the error message
**	in stderr.
*/
checkstderr(errstring)
char	*errstring;
{
	int	nbyte;
	FILE	*read_stream;
	char	ret_buf[STRLEN];

	nbyte = STRLEN;
	expecting (VOID);
	if ( fclose(Write_Stream) )
		error ("fclose of the matherr temp file failed");
	if ( (read_stream = temp_fopen("tmpmatherr", "r")) == NULL )
		error ("can't open temp file 'matherr'");
	ret_buf[0] = '\0';
	if ( fgets(ret_buf, nbyte, read_stream) != NULL )
	{
		if ( find(ret_buf, errstring) != 0 )
			error ("The error message '%s'\n\
written to stderr for the math function calling matherr is wrong",
ret_buf);
	}
	else
	{
		error ("The calling math function did not write anything \
onto stderr");
	}
	if ( fclose(read_stream) )
		error ("fclose(tempfile = 'matherr') returned non-zero");
	if ( (Write_Stream = temp_fopen("tmpmatherr","w")) == NULL )
		error ("could not fopen temp file 'matherr'");
}

/*
** matherr()
**	This function will check to see if the correct struct 
**	is being passed to it and sets a global variable 
**	indicating that it has been called.  It will indicate
**	that it has been called for no good reason if true.
**
*/
matherr(x)
register	struct	exception	*x;
{
	if ( !Expecting )
	{
		error("Matherr was called for no apparent reason");
		return(0);
	}

	if ( x == (struct exception *) 0 )
	{
		error ("matherr called with a NULL exception pointer");
	}

	expecting (VOID);
	if ( strcmp(x->name, Name) != 0 )
		error ("Matherr was passed the wrong function name of '%s',\n\
'%s' was expected", x->name, Name);

	if ( x->type != Errortype )
		error("Matherr was passed the wrong error type of %d,\n\
%d was exepcted", x->type, Errortype);

	if ( x->arg1 != Arg1 )
		error ("Matherr was passed the wrong first argument of %g,\n\
%g was exepcted", x->arg1, Arg1);
	
	if ( Twoargs == 1 )
	{
		if ( x->arg2 != Arg2 )
		    error ("Matherr was passed the second first argument of \
%g,\n%g was exepcted", x->arg2, Arg2);
	}

	Seen = 1;
	if ( Printmessage == 1 )
		return(0);
	else
		return(1);
}
