#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)frexp.c	1.7"

extern	double	frexp();
extern char *format();

/*
** frexp
**	Test floating point manipulators
*/
main()
{
setup("frexp");

funct ("frexp");
start
	t_frexp();
finis

funct ("ldexp");
start
	t_ldexp();
finis

funct ("modf");
start
	t_modf();
finis
done();
/*NOTREACHED*/
}

/*
** t_frexp
**	Text the frexp routine.
*/
t_frexp()
{
	double	ret_val;
	int	eptr;

testing("frexp(1, eptr) should return");
testing("a mantissa value of .5, and an exponent of 1.");
start
	exit_on (ERROR);
	expecting (0.5);
	frexp(1.0,&eptr);

	if ( eptr != 1 )
	{
		error ("frexp(1) returned an incorrect exponent of %s",
			format(&eptr));
	}
finis


testing("frexp(2) should return");
testing("a mantissa value of .5, and an exponent of 2.");
start
	exit_on (ERROR);
	expecting (0.5);
	frexp(2.0,&eptr);

	if ( eptr != 2 )
	{
		error ("frexp(2) returned an incorrect exponent of %s",
			format(&eptr));
	}
finis


testing("frexp(9) should return");
testing("a mantissa value of .5625, and an exponent of 4.");
start
	exit_on (ERROR);
	expecting (0.5625);
	frexp(9.0,&eptr);

	if ( eptr != 4 )
	{
		error ("frexp(9) returned an incorrect exponent of %s",
			format(&eptr));
	}
finis


testing("frexp(73.5) should return");
testing("a mantissa value of .574219, and an exponent of 7.");
start
	exit_on (ERROR);
	expecting (0.574219);
	frexp(73.5,&eptr);

	if ( eptr != 7 )
	{
		error ("frexp(73.5) returned an incorrect exponent of %s",
			format(&eptr));
	}
finis


testing("frexp(0) should return");
testing("a mantissa value of 0, and an exponent of 0.");
start
	exit_on (ERROR);
	expecting (0.0);
	ret_val = frexp(0.0,&eptr);

	if ( eptr != 0 )
	{
		error ("frexp(0) returned an incorrect exponent of %s",
			format(&eptr));
	}
finis

}

/*
** t_ldexp
**	Test the ldexp routine.
*/
t_ldexp()
{
	double	ret_val;
	extern	double	ldexp();

testing("ldexp(0,0) should return zero.");
start
	expecting (0.0);
	ldexp(0.0,0);
finis

testing("ldexp(.5,1) should return 1.");
start
	expecting (1.0);
	ldexp(0.5,1);
finis

testing("ldexp(.5625,4) should return 9.");
start
	expecting (9.0);
	ldexp(0.5625,4);
finis

testing("ldexp(.574219,7) should return 73.5.");
start
	expecting (73.5);
	ldexp(0.574219,7);
finis

testing("ldexp(.9999,INT_MAX) should return HUGE, with errno set to ERANGE.");
start
#ifndef	SYSVR_2
	expecting (HUGE);
	expecting (ERANGE);
	ldexp(0.9999,INT_MAX);
#else
	expecting (VOID);
	ret_val = ldexp(0.9999,INT_MAX);
	if (!DCMP (ret_val, HUGE))
		{
		if (DCMP (ret_val, 0.49995))
			warning ("%s\nldexp returned %s",
				ADVISORY, format(&ret_val));
		else
			error ("ldexp returned %s", format(&ret_val));
		}
	else
		{
		if (errno != ERANGE)
			error ("errno = %s, ERANGE was expected",
				err_lookup (errno) );
		}
#endif
finis

testing("ldexp(-.9999,INT_MAX) should return -HUGE, with errno set to ERANGE.");
start
#ifndef	SYSVR_2
	expecting (-HUGE);
	expecting (ERANGE);
	ldexp(-0.9999,INT_MAX);
#else
	expecting (VOID);
	ret_val = ldexp(-0.9999,INT_MAX);
	if (!DCMP (ret_val, -HUGE))
		{
		if (DCMP (ret_val, -0.49995))
			warning ("%s\nldexp returned %s",
				ADVISORY, format(&ret_val));
		else
			error ("ldexp returned %s", format(&ret_val));
		}
	else
		{
		if (errno != ERANGE)
			error ("errno = %s, ERANGE was expected",
				err_lookup (errno) );
		}
#endif
finis

testing("ldexp(.5,-INT_MAX) should return 0, with errno set to ERANGE.");
start
	expecting (0.0);
	expecting (ERANGE);
	ldexp(0.5,-INT_MAX);
finis

}

/*
** t_modf
**	Test the modf function.
*/
t_modf()
{
	double	ret_val;
	double	iptr;

testing("modf(0) should return 0, with iptr set to 0.");
start
	expecting (0.0);
	modf(0.0,&iptr);
	if ( iptr != 0.0 )
	{
	error ("modf(0) returned an incorrect integral part of %s",
		format(&iptr));
	}
finis

testing("modf(14.5) should return .5, iptr set to 14.");
start
	expecting (0.5);
	modf(14.5,&iptr);
	if ( !DCMP(iptr,14.0) )
	{
	error ("modf(14.5) returned an incorrect integral part of %s",
		format(&iptr));
	}
finis

testing("modf(-14.5) should return -.5, iptr set to 14.");
start
	expecting (-0.5);
	modf(-14.5,&iptr);
	if ( !DCMP(iptr,-14.0) )
	{
	error ("modf(-14.5) returned an incorrect integral part of %s",
		format(&iptr));
	}
finis

testing("modf(4331.234) should return .234, iptr set to 4331.");
start
	expecting (0.234);
	modf(4331.234,&iptr);
	if ( !DCMP(iptr,4331.0) )
	{
	error ("modf(4331.234) returned an incorrect integral part of %s",
		format(&iptr));
	}
finis

testing("modf(100) should return 0, iptr set to 100.");
start
	expecting (0.0);
	modf(100.0,&iptr);
	if ( !DCMP(iptr,100.0) )
	{
	error ("modf(100.) returned an incorrect integral part of %s",
		format(&iptr));
	}
finis
}



static char *format( val )
double *val;
{
	static char buf[STRVAL];
	int i,l;

	(void) sprintf( buf, "%g (0x", *val );
	l = strlen( buf );
	for( i = 0; i < sizeof( double ); i++ )
		sprintf( buf+l+(2*i), "%02x", ((int)(*(((char *)val)+i)))&0xff );
	sprintf( buf+l+(2*i), ")" );
	return( buf );
}
