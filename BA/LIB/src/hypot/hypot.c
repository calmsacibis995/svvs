#include	<sv_base.h> /*replacement macros */
#include	<math.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)hypot.c	1.3"



/*
** hypot
**	Test the hypot routine (x^2 + y^2)^1/2
*/
main()
{
setup("hypot");
funct("hypot");

	t_hypot();
done();
/*NOTREACHED*/
}
/*
** do the actual tests
*/
t_hypot()
{
	double	ret_val;
	extern	double	hypot();

	char	*err_lookup ();

testing("hypot(0,0) should return 0.");
start
	break_on (SEVERR | ERROR);
	expecting (VOID);
	ret_val = hypot(0.0,0.0);
	if ( !DCMP(ret_val +1.0,1.0) )
	{
		error ("hypot(0, 0) returned %f",ret_val);
	}
finis


testing("hypot(0,4) should return 4.");
start
	break_on (SEVERR | ERROR);
	expecting (4.0);
	hypot(0.0,4.0);
finis

		

testing("hypot(4,0) should return 4.");
start
	break_on (SEVERR | ERROR);
	expecting (4.0);
	hypot(4.0,0.0);
finis


testing("hypot(2,2) should return sqrt(8).");
start
	break_on (SEVERR | ERROR);
	ret_val = hypot(2.0,2.0);

	if ( !DCMP(ret_val,sqrt(8.0)) )
	{
		error ("hypot(2,2) returned %f",ret_val);
	}
finis

		
testing("hypot(23,32) should return sqrt(1553).");
start
	break_on (SEVERR | ERROR);
	ret_val = hypot(23.0,32.0);

	if ( !DCMP(ret_val,sqrt(1553.0)) )
	{
		error ("hypot(23,32) returned %f", ret_val);
	}
finis


testing("hypot(-4,4) should return sqrt(32).");
start
	break_on (SEVERR | ERROR);
	ret_val = hypot(-4.0,4.0);

	if ( !DCMP(ret_val,sqrt(32.0)) )
	{
		error ("hypot(-4,4) returned %f", ret_val);
	}
finis


testing("hypot(4,-4) should return sqrt(32).");
start
	break_on (SEVERR | ERROR);
	ret_val = hypot(4.0,-4.0);

	if ( !DCMP(ret_val,sqrt(32.0)) )
	{
		error ("hypot(4,-4) returned %f", ret_val);
	}
finis


testing("hypot(-4,-4) should return sqrt(32).");
start
	break_on (SEVERR | ERROR);
	ret_val = hypot(-4.0,-4.0);

	if ( !DCMP(ret_val,sqrt(32.0)) )
	{
		error ("hypot(-4,-4) returned %f", ret_val);
	}
finis


testing("hypot(MAXDOUBLE,MAXDOUBLE) should return HUGE,");
testing ("with errno set to ERANGE.");
start
	break_on (SEVERR | ERROR);
	expecting (VOID);
	ret_val = hypot(MAXDOUBLE,MAXDOUBLE);

	if ( !DCMP(ret_val,HUGE) )
	{
		if ( !DCMP(HUGE, ret_val) )
			error("hypot(MAXDOUBLE,MAXDOUBLE) returned %e",
				ret_val);
		else
			error ("%s/hypot (MAXDOUBLE, MAXDOUBLE) returned %e",
				ADVISORY,ret_val);
	}
	else if ( errno != ERANGE )
		error ("errno = %s", err_lookup (errno));
finis
}
