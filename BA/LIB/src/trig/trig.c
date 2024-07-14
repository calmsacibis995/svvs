#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef M_PI
# define M_PI	3.14159265358979323846
# endif

# define	PI		M_PI
# define	STDERR_FD	2
/*
 *  disable sqrt error checking
 */
#undef	sqrt


#ident	"@(#)trig.c	1.2"




/*
** trig
**	Test the trig functions.
*/
main()
{
	double			ret_val;
	char			ret_buf[STRLEN];
	int			nbyte;
	FILE			*stream;

	extern	FILE		*temp_fopen();
	extern	double		sin();
	extern	double		cos();
	extern	double		tan();
	extern	double		asin();
	extern	double		acos();
	extern	double		atan();
	extern	double		atan2();
	extern	double		sqrt();


setup("trig");
prep ("redirect STDERR output into a the temp file 'tmptrig'");
start
	expecting (SUCCESS);
	fclose(stderr);

	expecting (VOID);
	close(STDERR_FD);

	expecting (SUCCESS);
	stream = temp_fopen("tmptrig", "w");
finis


funct ("cos");
start


testing("cos(0.0) should return 1.0");
start
	expecting (1.0);
	cos (0.0);
finis


testing("cos(PI/2) should return 0.0");
start
	expecting (ZERO);
	cos (PI/2);
finis


testing("cos(-PI/2) should return 0.0");
start
	expecting (ZERO);
	cos (-PI/2);
finis



testing ("cos (PI/4) should return sqrt(2.0)/2.0.");
start
	expecting ( (sqrt(2.0)/2.0) );
	cos(PI/4);
finis


testing("cos(-PI/4) should return sqrt(2.0)/2.0.");
start
	expecting (sqrt(2.0)/2.0);
	cos(-PI/4);
finis


testing("cos(PI) should return -1.0.");
start
	expecting (-1.0);
	cos(PI);
finis


testing("cos(-PI) should return -1.0.");
start
	expecting (-1.0);
	cos(-PI);
finis


testing("cos(3.0*PI/4.0) should return -sqrt(2.0)/2.0.");
start
	expecting (-sqrt(2.0)/2.0);
	cos(3.0*PI/4.0);
finis


testing("cos(-3.0*PI/4.0) should return -sqrt(2.0)/2.0.");
start
	expecting (-sqrt(2.0)/2.0);
	cos(-3.0*PI/4.0);
finis


testing("cos(1.5*PI) should return 0.0.");
start
	expecting (ZERO);
	cos(1.5*PI);
finis


testing("cos(-1.5*PI) should return 0.0.");
start
	expecting (ZERO);
	cos(-1.5*PI);
finis


testing("cos(HUGE).");
testing("should return 0.0, and an errno of ERANGE and error message");
testing("to stderr about TLOSS.");
start
	expecting (ERANGE);
	expecting (0.0);
	cos(HUGE);

	nbyte = STRLEN;
	expecting (SUCCESS);
	fclose(stream);

	stream = temp_fopen("tmptrig", "r");
	ret_buf[0] = '\0';
	expecting (VOID);
	if ( fgets(ret_buf, nbyte, stream) != NULL )
	{
		if ( find(ret_buf, "TLOSS") != 0 )
			error ("'%s' was written to stderr,\n%s",
				ret_buf,
				"a TLOSS error was expected");
	}
	else
	{
		error ("cos(HUGE) did not write anything onto stderr");
	}

	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig","w");
finis
finis  /*  end of cos function  */



funct ("sin");
start
testing("sin(0.0) should return 0.0.");
start
	expecting (ZERO);
	sin(0.0);
finis


testing("sin(PI/2) should return 1.0.");
start
	expecting (1.0);
	sin(PI/2);
finis


testing("sin(-PI/2) should return -1.0.");
start
	expecting (-1.0);
	sin(-PI/2);
finis


testing("sin(PI/4) should return sqrt(2.0)/2.0.");
start
	expecting (sqrt(2.0)/2.0);
	sin(PI/4);
finis


testing("sin(-PI/4) should return -sqrt(2.0)/2.0.");
start
	expecting (-sqrt(2.0)/2.0);
	sin(-PI/4);
finis


testing("sin(PI) should return 0.0.");
start
	expecting (ZERO);
	sin(PI);
finis


testing("sin(-PI) should return 0.0.");
start
	expecting (ZERO);
	sin(-PI);
finis


testing("sin(3.0*PI/4.0) should return sqrt(2.0)/2.0.");
start
	expecting (sqrt(2.0)/2.0);
	sin(3.0*PI/4.0);
finis


testing("sin(-3.0*PI/4.0) should return -sqrt(2.0)/2.0.");
start
	expecting (-sqrt(2.0)/2.0);
	sin(-3.0*PI/4.0);
finis


testing("sin(1.5*PI) should return -1.0.");
start
	expecting (-1.0);
	sin(1.5*PI);
finis


testing("sin(-1.5*PI) should return 1.0.");
start
	expecting (1.0);
	sin(-1.5*PI);
finis


testing("sin(HUGE) should return 0.0,");
testing("with an errno of ERANGE and error message");
testing("to stderr about TLOSS.");
start
	expecting (ERANGE);
	expecting (0.0);
	sin(HUGE);

	nbyte = STRLEN;
	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig", "r");

	ret_buf[0] = '\0';
	expecting (VOID);
	if ( fgets(ret_buf, nbyte, stream) != NULL )
	{
		if ( find(ret_buf, "TLOSS") != 0 )
			error ("'%s' was written to stderr\n%s",
				ret_buf,
				"a TLOSS error was expected");
	}
	else
	{
		error ("Nothing was written to stderr.");
	}

	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig","w");
finis
finis  /*  end of sin function  */



funct ("acos");
start
testing("acos(0.0) should return PI/2.");
start
	expecting (PI/2.0);
	acos(0.0);
finis


testing("acos(-1.0) should return PI.");
start
	expecting (PI);
	acos(-1.0);
finis


testing("acos(1.0) should return 0.0.");
start
	expecting (ZERO);
	acos(1.0);
finis


testing("acos(sqrt(2.0)/2.0) should return PI/4.0.");
start
	expecting (PI/4.0);
	acos(sqrt(2.0)/2.0);
finis


testing("acos(-sqrt(2.0)/2.0) should return 3.0*PI/4.0.");
start
	expecting (3.0*PI/4.0);
	acos(-sqrt(2.0)/2.0);
finis


testing("acos(0.5) should return PI/3.0.");
start
	expecting (PI/3.0);
	acos(0.5);
finis


testing("acos(-0.5) should return 2.0*PI/3.0.");
start
	expecting (2.0*PI/3.0);
	acos(-0.5);
finis


testing("acos(-sqrt(3.0)/2.0) should return 5.0*PI/6.0.");
start
	expecting (5.0*PI/6.0);
	acos(-sqrt(3.0)/2.0);
finis


testing("acos(1.1).");
testing("should return 0.0, and an errno of EDOM and error message");
testing("to stderr about DOMAIN.");
start
	expecting (EDOM);
	expecting (0.0);
	acos(1.1);

	nbyte = STRLEN;
	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig", "r");

	ret_buf[0] = '\0';
	expecting (VOID);
	if ( fgets(ret_buf, nbyte, stream) != NULL )
	{
		if ( find(ret_buf, "DOMAIN") != 0 )
			error ("'%s' was written to stdout\n%s",
				ret_buf,
				"expecting an error about DOMAIN");
	}
	else
	{
		error ("Nothing was written to stdout.");
	}

	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig","w");
finis
finis  /*  end of acos function  */


funct ("asin");
start

testing("asin(0.0) should return 0.0.");
start
	expecting (ZERO);
	asin(0.0);
finis


testing("asin(-1.0) should return -PI/2.0.");
start
	expecting (-PI/2.0);
	asin(-1.0);
finis


testing("asin(1.0) should return PI/2.0.");
start
	expecting (PI/2.0);
	asin(1.0);
finis


testing("asin(sqrt(2.0)/2.0) should return PI/4.0.");
start
	expecting (PI/4.0);
	asin(sqrt(2.0)/2.0);
finis


testing("asin(-sqrt(2.0)/2.0) should return -PI/4.0.");
start
	expecting (-PI/4.0);
	asin(-sqrt(2.0)/2.0);
finis


testing("asin(0.5) should return PI/6.0.");
start
	expecting (PI/6.0);
	asin(0.5);
finis


testing("asin(-0.5) should return -PI/6.0.");
start
	expecting (-PI/6.0);
	asin(-0.5);
finis


testing("asin(sqrt(3.0)/2.0) should return PI/3.0.");
start
	expecting (PI/3.0);
	asin(sqrt(3.0)/2.0);
finis


testing("asin(-sqrt(3.0)/2.0) should return -PI/3.0.");
start
	expecting (-PI/3.0);
	asin(-sqrt(3.0)/2.0);
finis


testing("asin(1.1) should return 0.0,");
testing("and an errno of EDOM and error message to stderr about DOMAIN.");
start
	expecting (EDOM);
	expecting (0.0);
	asin(1.1);

	nbyte = STRLEN;
	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig", "r");

	ret_buf[0] = '\0';
	expecting (VOID);
	if ( fgets(ret_buf, nbyte, stream) != NULL )
	{
		if ( find(ret_buf, "DOMAIN") != 0 )
			error ("%s was written to stdout\n%s",
				ret_buf,
				"expecting an error about DOMAIN");

	}
	else
	{
		error ("Nothing was written to stderr.");
	}

	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig","w");
finis
finis  /*  end of asin function  */



funct ("tan");
start

testing("tan(0.0) should return 0.0.");
start
	expecting (ZERO);
	tan(0.0);
finis


testing("tan(PI/6.0) should return sqrt(3.0)/3.0.");
start
	expecting (sqrt(3.0)/3.0);
	tan(PI/6.0);
finis


testing("tan(PI/4.0) should return 1.0.");
start
	expecting (1.0);
	tan(PI/4.0);
finis


testing("tan(PI/3.0) should return sqrt(3.0).");
start
	expecting (sqrt(3.0));
	tan(PI/3.0);
finis


testing("tan(5.0*PI/12.0) should return (2.0+sqrt(3.0)).");
start
	expecting (2.0+sqrt(3.0));
	tan(5.0*PI/12.0);
finis


testing("tan(2.0*PI/3.0) should return -sqrt(3.0).");
start
	expecting (-sqrt(3.0));
	tan(2.0*PI/3.0);
finis


testing("tan(3.0*PI/4.0) should return -1.0.");
start
	expecting (-1.0);
	tan(3.0*PI/4.0);
finis


testing("tan(5.0*PI/6.0) should return (-sqrt(3.0)/3.0).");
start
	expecting (-sqrt(3.0)/3.0);
	tan(5.0*PI/6.0);
finis


testing("tan(PI) should return 0.0.");
start
	expecting (ZERO);
	tan(PI);
finis
finis  /*  end of tan function  */


funct ("atan");
start

testing("atan(0.0) should return 0.0.");
start
	expecting (ZERO);
	atan(0.0);
finis


testing("atan(sqrt(3.0)/3.0) should return PI/6.0.");
start
	expecting (PI/6.0);
	atan(sqrt(3.0)/3.0);
finis


testing("atan(1.0) should return PI/4.0.");
start
	expecting (PI/4.0);
	atan(1.0);
finis


testing("atan(sqrt(3.0)) should return PI/3.0.");
start
	expecting (PI/3.0);
	atan(sqrt(3.0));
finis
finis  /*  end of atan function  */


funct ("atan2");
start

testing("atan(2.0+sqrt(3.0)) should return 5.0*PI/12.0.");
start
	expecting (5.0*PI/12.0);
	atan(2.0+sqrt(3.0));
finis


testing("atan(-sqrt(3.0)) should return -PI/3.0.");
start
	expecting (-PI/3.0);
	atan(-sqrt(3.0));
finis


testing("atan(-1.0) should return -PI/4.0.");
start
	expecting (-PI/4.0);
	atan(-1.0);
finis


testing("atan(-sqrt(3.0)/3.0) should return -PI/6.0.");
start
	expecting (-PI/6.0);
	atan(-sqrt(3.0)/3.0);
finis


testing("atan2(0.0,PI) should return 0.0.");
start
	expecting (ZERO);
	atan2(0.0,PI);
finis


testing("atan2(sqrt(3.0),3.0) should return PI/6.0.");
start
	expecting (PI/6.0);
	atan2(sqrt(3.0),3.0);
finis


testing("atan2(-sqrt(3.0),3.0) should return -PI/6.0.");
start
	expecting (-PI/6.0);
	atan2(-sqrt(3.0),3.0);
finis


testing("atan2(-sqrt(3.0),-3.0) should return -5.0*PI/6.0.");
start
	expecting (-5.0*PI/6.0);
	atan2(-sqrt(3.0),-3.0);
finis


testing("atan2(sqrt(3.0),-3.0) should return 5.0*PI/6.0.");
start
	expecting (5.0*PI/6.0);
	atan2(sqrt(3.0),-3.0);
finis


testing("atan2(1.0,1.0) should return PI/4.0.");
start
	expecting (PI/4.0);
	atan2(1.0,1.0);
finis


testing("atan2(-1.0,1.0) should return -PI/4.0.");
start
	expecting (-PI/4.0);
	atan2(-1.0,1.0);
finis


testing("atan2(-1.0,-1.0) should return -3.0*PI/4.0.");
start
	expecting (-3.0*PI/4.0);
	atan2(-1.0,-1.0);
finis


testing("atan2(1.0,-1.0) should return 3.0*PI/4.0.");
start
	expecting (3.0*PI/4.0);
	atan2(1.0,-1.0);
finis


testing("atan2(0.0,0.0) should return 0.0,");
testing("with an errno of EDOM and error message to stderr about DOMAIN.");
start
	expecting (EDOM);
	expecting (0.0);
	atan2(0.0,0.0);

	nbyte = STRLEN;
	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig", "r");

	ret_buf[0] = '\0';
	expecting (VOID);
	if ( fgets(ret_buf, nbyte, stream) != NULL )
	{
		if ( find(ret_buf, "DOMAIN") != 0 )
			error ("%s was written to stderr\n%s",
			ret_buf,
			"expecting an error about DOMAIN");
	}
	else
	{
		error ("Nothing was written to stderr.");
	}

	expecting (SUCCESS);
	fclose(stream);
	stream = temp_fopen("tmptrig","w");
finis
finis  /*  end of function atan2  */


done();
/*NOTREACHED*/

}
