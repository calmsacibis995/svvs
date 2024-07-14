#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	STDERR_FD	2


#ident	"@(#)sinh.c	1.2"


/*
 *  These arrays hold the argument and return value pairs
 */
static	double	cosh_args [] [2]
={
0.0, 1.0,
0.05, 1.001250260,
0.10, 1.005004168,
0.25 ,1.031413100,
0.50 ,1.127625965,
1.0 ,1.543080635,
1.5 ,2.352409615,
2.0 ,3.762195691,
3.0 ,10.067661996,
4.0 ,27.308232836,
5.0 ,74.209948525,
10.0 ,11013.232920103,
};



static	double	sinh_args [] [2]
={
0.0, 0.0,
0.05, 0.050020836,
0.10, 0.100166750,
0.25, 0.252612317,
0.50, 0.521095305,
1.0, 1.175201194,
1.5, 2.129279455,
2.0, 3.626860408,
3.0, 10.017874927,
4.0, 27.289917197,
5.0, 74.203210578,
10.0, 11013.232874703,
};



static	double	tanh_args [] [2]
={
0.0, 0.0,
0.05, 0.04995838,
0.10, .09966800,
0.25, 0.24491866,
0.50, 0.46211716,
1.0, 0.76159416,
1.5, 0.90514825,
2.0, 0.9640275801,
3.0, 0.9950547537,
4.0, 0.9993292997,
5.0, 0.9999092043,
10.0, 0.9999999959,
};
/*
** sinh
**	Test the sinh functions.
*/
main()
{
	double			ret_val;
	char			ret_buf[STRLEN];
	int			nbyte;
	FILE			*stream;
	int			nargs;
	int			i;

	extern	FILE		*temp_fopen();
	extern	double		sinh();
	extern	double		cosh();
	extern	double		tanh();


setup("sinh");

expecting (SUCCESS);
fclose(stderr);

expecting (VOID);
close(STDERR_FD);

expecting (SUCCESS);
stream = temp_fopen("tmpsinh", "w");



funct ("cosh");
start
	nargs = sizeof (cosh_args) / (sizeof (double) * 2);
	for (i=0 ; i<nargs ; i++)
		{
		testing("cosh(%f) should return %f", 
			cosh_args[i][0], cosh_args[i][1]);
		start
			expecting (cosh_args [i][1]);
			cosh (cosh_args [i] [0]);
		finis
		}

	testing("cosh(HUGE) should return HUGE, and an errno of ERANGE");
	testing ("and NO error message should be written to stderr.");
	start
		expecting (VOID);
		ret_val = cosh(HUGE);
		if ( DCMP(ret_val,HUGE) == 0 ) /*  did it return HUGE?  */
			{
			if ( DCMP(HUGE, ret_val) == 0 )  /*  no, was it close?  */
				error ("Incorrect return value = %f",
					ret_val);
			else
				warning ("%s\nreturn value = %f",
					ADVISORY, ret_val);
			}
		else
			{
			if ( errno != ERANGE )
				error ("Incorrect errno = %s",
					err_lookup (errno) );
			}
		nbyte = STRLEN;
		expecting (SUCCESS);
		fclose(stream);

		expecting (SUCCESS);
		stream = temp_fopen("tmpsinh", "r");

		ret_buf[0] = '\0';
		expecting (VOID);
		if ( fgets(ret_buf, nbyte, stream) != NULL )
			error ("error message written = '%s'", ret_buf);

		expecting (SUCCESS);
		fclose(stream);

		stream = temp_fopen("tmpsinh","w");
	finis


	testing("cosh(-HUGE) should return HUGE, and an errno of ERANGE");
	testing ("and NO error message should be written to stderr.");
	start
		expecting (VOID);
		ret_val = cosh(-HUGE);
		if ( DCMP(ret_val,HUGE) == 0 ) /*  did it return HUGE?  */
			{
			if ( DCMP(HUGE, ret_val) == 0 )  /*  no, was it close?  */
				error ("Incorrect return value = %f",
					ret_val);
			else
				warning ("%s\nreturn value = %f",
					ADVISORY, ret_val);
			}
		else
			{
			if ( errno != ERANGE )
				error ("Incorrect errno = %s",
					err_lookup (errno) );
			}
		nbyte = STRLEN;
		expecting (SUCCESS);
		fclose(stream);

		expecting (SUCCESS);
		stream = temp_fopen("tmpsinh", "r");

		ret_buf[0] = '\0';
		expecting (VOID);
		if ( fgets(ret_buf, nbyte, stream) != NULL )
			error ("error message written = '%s'", ret_buf);

		expecting (SUCCESS);
		fclose(stream);

		stream = temp_fopen("tmpsinh","w");
	finis
finis


funct ("sinh");
start
	nargs = sizeof (sinh_args) / (sizeof (double) * 2);
	for (i=0 ; i<nargs ; i++)
		{
		testing("sinh(%f) should return %f", 
			sinh_args[i][0], sinh_args[i][1]);
		start
			expecting (sinh_args [i][1]);
			sinh (sinh_args [i] [0]);
		finis
		}

	testing("sinh(HUGE) should return HUGE, and an errno of ERANGE");
	testing ("and NO error message should be written to stderr.");
	start
		expecting (VOID);
		ret_val = sinh(HUGE);
		if ( DCMP(ret_val,HUGE) == 0 ) /*  did it return HUGE?  */
			{
			if ( DCMP(HUGE, ret_val) == 0 )  /*  no, was it close?  */
				error ("Incorrect return value = %f",
					ret_val);
			else
				warning ("%s\nreturn value = %f",
					ADVISORY, ret_val);
			}
		else
			{
			if ( errno != ERANGE )
				error ("Incorrect errno = %s",
					err_lookup (errno) );
			}
		nbyte = STRLEN;
		expecting (SUCCESS);
		fclose(stream);

		expecting (SUCCESS);
		stream = temp_fopen("tmpsinh", "r");

		ret_buf[0] = '\0';
		expecting (VOID);
		if ( fgets(ret_buf, nbyte, stream) != NULL )
			error ("error message written = '%s'", ret_buf);

		expecting (SUCCESS);
		fclose(stream);

		stream = temp_fopen("tmpsinh","w");
	finis


	testing("sinh(-HUGE) should return HUGE or -HUGE,");
	testing ("an errno of ERANGE and NO error message to stderr.");
	start
		expecting (VOID);
		ret_val = sinh(-HUGE);
		if ( DCMP(ret_val,-HUGE) == 0 && DCMP(ret_val, HUGE) == 0 )
			error ("Incorrect return value = %f",
				ret_val);
		if ( errno != ERANGE )
			error ("Incorrect errno = %s",
				err_lookup (errno) );
		nbyte = STRLEN;
		expecting (SUCCESS);
		fclose(stream);

		expecting (SUCCESS);
		stream = temp_fopen("tmpsinh", "r");

		ret_buf[0] = '\0';
		expecting (VOID);
		if ( fgets(ret_buf, nbyte, stream) != NULL )
			error ("error message written = '%s'", ret_buf);

		expecting (SUCCESS);
		fclose(stream);

		stream = temp_fopen("tmpsinh","w");
	finis
finis



funct ("tanh");
start
	nargs = sizeof (tanh_args) / (sizeof (double) * 2);
	for (i=0 ; i<nargs ; i++)
		{
		testing("tanh%f) should return %f", 
			tanh_args[i][0], tanh_args[i][1]);
		start
			expecting (tanh_args [i][1]);
			tanh (tanh_args [i] [0]);
		finis
		}
finis


done();
/*NOTREACHED*/

}
