#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	STDERR_FD	2


#ident	"@(#)exp.c	1.2"


/*
 *  These arrays hold the argument and return value pairs
 */
#define	exp_num	3
static	double	exp_args [exp_num] [2]
={
0.0, 1.0,
5.0, 148.413,
-5.0, 0.006737947
};

#define	pow_num	9
static	double	pow_args [pow_num] [3]
={
0.0, 10.0, 0.0,
10.0, 0.0, 1.0,
15.0, 0.0, 1.0,
2.0, 8.0, 256.0,
10.0, 4.0, 10000.0,
5.0, 3.0, 125.0,
17.0, 5.0, 1419857.0,
10.0, 2.2, 158.489319,
-2.0, 3.0, -8.0
};

#define	log10_num	4
static	double	log10_args [log10_num] [2]
={
1.0, 0.0,
10.0, 1.0,
1000.0, 3.0,
158.489319, 2.2
};

#define	sqrt_num	3
static	double	sqrt_args [sqrt_num] [2]
={
0.0, 0.0,
1.0, 1.0,
4.0, 2.0
};

/*
** exp
**	Test the exp function.
*/
main()
{
	double			ret_val;
	FILE			*fp;
	int			nbyte;
	int			i;
	char			buffer[STRLEN];

	extern	double		exp();
	extern	double		log();
	extern	double		log10();
	extern	double		pow();
	extern	double		sqrt();
	extern	FILE		*temp_fopen();


setup  ("exp");

expecting (SUCCESS);
fclose(stderr);

/*
** Ensure stderr is closed.
*/
expecting (VOID);
close(STDERR_FD);

funct ("exp");
start

	for (i=0 ; i<exp_num ; i++) {
		testing("exp(%f) should return %f", 
			exp_args[i][0], exp_args[i][1]);
		start
			expecting (exp_args [i][1]);
			exp (exp_args [i] [0]);
		finis
	}

	testing("exp (HUGE) should return HUGE with errno set to ERANGE.");
	start
		expecting (ERANGE);
		expecting (HUGE);
		exp (HUGE);
	finis

	testing("exp (-HUGE) should return 0 with errno set to ERANGE.");
	start
		expecting (ERANGE);
		expecting (0.0);
		exp (-HUGE);
	finis
finis

funct ("log");
start
	testing("log(1.0) should return 0.0");
	start
		expecting (0.0);
		log (1.0);
	finis

	testing("log(exp(5.0)) should return 5.0");
	start
		expecting (VOID);
		ret_val = exp(5.0);
		expecting (5.0);
		log(ret_val);
	finis

	testing("log(exp(-5.0)) should return -5.0");
	start
		expecting (VOID);
		ret_val = exp(-5.0);
		expecting (-5.0);
		log(ret_val);
	finis

	testing("log (-1.0) should return -HUGE with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmplog' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmplog","w");
		
		expecting (EDOM);
		expecting (-HUGE);
		log (-1.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmplog","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("log (-1.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis


	testing("log (0.0) should return -HUGE with errno set to EDOM,");
	testing("and a message about SING printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmplog' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmplog","w");
		
		expecting (EDOM);
		expecting (-HUGE);
		log (0.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmplog","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"SING") )
			error ("log (0.0) did not print %s\n%s'%s'",
			    "an error message about SING on stderr",
			    "actual message was",
			    buffer);
	finis
finis


funct ("pow");
start
	for (i=0 ; i<pow_num ; i++) {
		testing("pow (%f, %f) should return %f", 
			pow_args[i][0], pow_args[i][1], pow_args[i][2]);
		start
			expecting (pow_args [i][2]);
			pow (pow_args [i] [0], pow_args [i] [1]);
		finis
	}

	testing("pow (0.0, 0.0) should return 0 with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmppow' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmppow","w");
		
		expecting (EDOM);
		expecting (0.0);
		pow (0.0, 0.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmppow","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("pow (0.0, 0.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis


	testing("pow (0.0, -1.0) should return 0 with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmppow' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmppow","w");
		
		expecting (EDOM);
		expecting (0.0);
		pow (0.0, -1.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmppow","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("pow (0.0, -1.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis


	testing("pow (-1.0, 2.2) should return 0 with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmppow' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmppow","w");
		
		expecting (EDOM);
		expecting (0.0);
		pow (-1.0, 2.2);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmppow","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("pow (-1.0, 2.2) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis

	testing("pow(10.0,HUGE) should return HUGE, with errno set to ERANGE.");
	start
		expecting (ERANGE);
		expecting (HUGE);
		pow(10.0,HUGE);
	finis


	testing("pow(10.0,-HUGE) should return 0.0, and errno set to ERANGE.");
	start
		expecting (ERANGE);
		expecting (0.0);
		pow(10.0,-HUGE);
	finis
finis


funct ("log10");
start
	for (i=0 ; i<log10_num ; i++) {
		testing("log10 (%f) should return %f", 
			log10_args[i][0], log10_args[i][1]);
		start
			expecting (log10_args [i][1]);
			log10 (log10_args [i] [0]);
		finis
	}

	testing("pow(10.0, log10(7.6)) should return 7.6.");
	start
		expecting (VOID);
		ret_val = log10(7.6);
		expecting (7.6);
		pow(10.0, ret_val);
	finis

	testing("log10 (-1.0) should return -HUGE with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmplog10' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmplog10","w");
		
		expecting( EDOM );
		expecting (-HUGE);
		log10 (-1.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmplog10","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("log10 (-1.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis


	testing("log10 (0.0) should return -HUGE with errno set to EDOM,");
	testing("and a message about SING printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmplog10' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmplog10","w");
		
		expecting( EDOM );
		expecting (-HUGE);
		log10 (0.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmplog10","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"SING") )
			error ("log10 (-1.0) did not print %s\n%s'%s'",
			    "an error message about SING on stderr",
			    "actual message was",
			    buffer);
	finis
finis


funct ("sqrt");
start
	for (i=0 ; i<sqrt_num ; i++) {
		testing("sqrt (%f) should return %f", 
			sqrt_args[i][0], sqrt_args[i][1]);
		start
			expecting (sqrt_args [i][1]);
			sqrt (sqrt_args [i] [0]);
		finis
	}

	testing("sqrt(all numbers between 5 and 111) should yield");
	testing("return values of square root of 5 to 111.");
	start
		break_on (ERROR);

		for (i = 5; i < 112; i++)
		{
			ret_val = sqrt((double)i);
			if ( DCMP(ret_val*ret_val, (double)i) == 0)
				error ("sqrt(%d) returned '%f'",
				    i, ret_val);
		}
	finis

	testing("sqrt (-3.0) should return 0 with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpsqrt' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpsqrt","w");
		
		expecting (EDOM);
		expecting (0.0);
		sqrt (-3.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpsqrt","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("sqrt (-3.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis
finis

done();
/*NOTREACHED*/
}
