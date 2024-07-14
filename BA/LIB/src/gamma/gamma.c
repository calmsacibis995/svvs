#include	<sv_base.h> /*replacement macros */
#include	<math.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)gamma.c	1.3"



extern	FILE	*temp_fopen();

/*
** gamma
**	Test the ln(gamma(x)) function.
*/
main()
{
	double	ret_val;
	char	buffer[STRLEN];
	FILE	*fp;

setup ("gamma");
funct ("gamma");


testing("gamma(1) should return 0.");
start
	expecting (ZERO);
	gamma(1.0);
finis


testing("gamma(5) should return 3.178054.");
start
	expecting (3.178054);
	gamma(5.0);
finis


testing("gamma(10) should return 12.801827.");
start

	expecting (12.801827);
	ret_val = gamma(10.0);
finis


testing("gamma(80) should return 269.291098.");
start
	expecting (269.291098);
	gamma(80.0);
finis


testing("gamma(100) should return 359.134205.");
start
	expecting (359.134205);
	gamma(100.0);
finis


testing("gamma(0) should return HUGE, with errno set to EDOM,");
testing("and a message about SING on stderr.");
start
	expecting (SUCCESS);
	fclose(stderr);

	fp = temp_fopen("tmpgamma","w");

	expecting (HUGE);
	expecting (EDOM);
	gamma(0.0);

	expecting (SUCCESS);
	fclose(fp);
	fp = temp_fopen("tmpgamma","r");

	if ( fgets(buffer,STRLEN,fp) == NULL )
		{
			error ("gamma(0) did not write any message on stderr");
		}
		else
		{
			if ( find(buffer,"SING") )
				error ("gamma(0) wrote '%s' to stderr",
				    buffer);
		}
		fclose(fp);
finis


testing("gamma(-2) should return HUGE, with errno set to EDOM,");
testing("and a message about SING on stderr.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpgamma","w");

	expecting (HUGE);
	expecting (EDOM);
	ret_val = gamma(-2.0);

	expecting (SUCCESS);
	fclose(fp);

	fp = temp_fopen("tmpgamma","r");

	if ( fgets(buffer,STRLEN,fp) == NULL )
	{
		error ("gamma(-2) did not write any message on stderr");
	}
	else
	{
		if ( find(buffer,"SING") )
			error ("gamma(-2) wrote '%s' to stderr",
			    buffer);
	}
	fclose(fp);
finis

done();
/*NOTREACHED*/
}
