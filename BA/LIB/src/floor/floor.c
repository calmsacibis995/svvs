#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)floor.c	1.2"


/*
 *  These arrays hold the argument and return value pairs
 */
#define	fabs_num	4
static	double	fabs_args [fabs_num] [2]
={
0.0, 0.0,
1.0, 1.0,
-1.0, 1.0,
HUGE, HUGE
/*  -HUGE, HUGE won't compile  */
};

/*
** floor
**	Test the floor function.
*/
main()
{
	double			ret_val;
	double			variable;
	int			i;

	extern	double		floor();
	extern	double		fmod();
	extern	double		ceil();
	extern	double		fabs();

setup("floor");

funct ("floor");
start


testing("floor(0.0) should return 0.0.");
start
	expecting (0.0);
	floor(0.0);
finis

testing("floor(123.0) should return 123.");
start
	expecting (123.0);
	floor(123.0);
finis

testing("floor(1 thru 100) should return 1 thru 100.");
start
	for (i = 1; i <= 100; i++)
	{
		expecting ((double) i);
		floor((double)i);
	}
finis

testing("floor(6.1 thru 6.9) should return 6.0.");
start
	for (i = 1; i <= 9; i++)
	{
		expecting (6.0);
		floor(6.0+(double)i/10.0);
	}
finis

testing("floor(-5.0) should return -5.0.");
start
	expecting (-5.0);
	floor(-5.0);
finis

testing("floor(-5.5) should return -6.0.");
start
	expecting (-6.0);
	floor(-5.5);
finis
finis


funct (" ceil");
start

testing("ceil(0.0) should return 0.0.");
start
	expecting (0.0);
	ceil(0.0);
finis

testing("ceil(123.0) should return 123.");
start
	expecting (123.0);
	ceil(123.0);
finis

testing("ceil(1 thru 100) should return values of 1 thru 100.");
start
	for (i = 1; i <= 100; i++)
	{
		expecting ((double) i);
		ceil((double)i);
	}
finis

testing("ceil(6.1) should return 7.0.");
start
	expecting (7.0);
	ceil(6.1);
finis
	
testing("ceil(6.2 thru 6.9) should return 7.0.");
start
	for (i = 2; i <= 9; i++)
	{
		expecting (7.0);
		ceil(6.0+(double)i/10.0);
	}
finis

testing("ceil(-5.0) should return -5.0.");
start
	expecting (-5.0);
	ceil(-5.0);
finis
	
testing("ceil(-5.5) should return -5.0.");
start
	expecting (-5.0);
	ceil(-5.5);
finis
finis

	
funct ("fmod");
start

testing("fmod(10.0,5.0) should return 0.0.");
start
	expecting (0.0);
	fmod(10.0,5.0);
finis
	
testing("fmod(2.2,2.0) should return 0.2.");
start
	expecting (0.2);
	fmod(2.2,2.0);
finis
	
testing("fmod(8.6,4.0) should return 0.6.");
start
	expecting (0.6);
	fmod(8.6,4.0);
finis

testing("fmod(27.0,6.0) should return 3.0.");
start
	expecting (3.0);
	fmod(27.0,6.0);
finis

testing("fmod(-27.0,6.0) should return -3.0.");
start
	expecting (-3.0);
	fmod(-27.0,6.0);
finis
	
testing("fmod(27.2,6.0) should return 3.2.");
start
	expecting (3.2);
	fmod(27.2,6.0);
finis

#ifndef	SYSVR_2
testing("fmod(23.0,0.0) should return 23.0.");
start
	expecting (23.0);
	fmod(23.0,0.0);
finis

#else

testing("fmod(23.0,0.0) should return 0.0.");
start
	expecting (VOID);
	ret_val = fmod(23.0,0.0);
	if ( DCMP(ret_val, 23.0) != 0 )
		warning ("%s\nfmod (23.0,0.0) returned 23.0",
			FUTURDIR);
	else if ( DCMP(ret_val+1.0, 1.0) == 0 )
		error ("fmod(23.0,0.0) returned %d", ret_val);
finis
#endif


#ifndef	SYSVR_2

testing("fmod(HUGE,1.0e-10) should return HUGE.");
start
	expecting (HUGE);
	fmod(HUGE,1.0e-10);
finis

#else

testing("fmod(HUGE,1.0e-10) should return 0.0.");
start
	expecting (VOID);
	ret_val = fmod(HUGE,1.0e-10);
	if ( DCMP(ret_val, HUGE) != 0 )
		warning ("%s\nfmod (HUGE,1.0e-10) returned HUGE",
			FUTURDIR);
	else if ( DCMP(ret_val+1.0, 1.0) == 0 )
		error ("fmod(HUGE,1.0e-10) returned %d", ret_val);
finis

#endif

finis
	
funct ("fabs");
start

abstract ("fabs will be called with constant parameters,");
abstract ("then with the parameters in a variable");
testing ("fabs (0.0) should return 0.0");
start
	expecting (0.0);
	fabs (0.0);
finis

testing ("fabs (1.0) should return 1.0");
start
	expecting (1.0);
	fabs (1.0);
finis

testing ("fabs (-1.0) should return 1.0");
start
	expecting (1.0);
	fabs (-1.0);
finis

testing ("fabs (HUGE) should return HUGE");
start
	expecting (HUGE);
	fabs (HUGE);
finis

testing ("fabs (-HUGE) should return HUGE");
start
	expecting (HUGE);
	fabs (-HUGE);
finis


	for (i=0 ; i<fabs_num ; i++) {
		testing("fabs(variable = %g) should return %g", 
			fabs_args[i][0], fabs_args[i][1]);
		start
			expecting (fabs_args [i][1]);
			fabs (fabs_args [i] [0]);
		finis
	}
finis

done();
/*NOTREACHED*/

}
