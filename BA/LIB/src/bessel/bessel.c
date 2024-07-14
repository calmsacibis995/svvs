#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

extern	FILE	*temp_fopen();

#ident	"@(#)bessel.c	1.1"

/*
 *  These arrays hold the argument and return value pairs
 */
#define	num_j0	17
static	double	j0_args [num_j0] [2]
={
0.0,	1.,
0.5,	0.9384698072,
1.5,	0.5118276717,
1.0,	0.7651976866,
2.0,	0.2238907791,
3.0,	-0.2600519549,
4.0,	-0.3971498099,
5.0,	-0.1775967713,
6.0,	0.1506452573,
7.0,	0.3000792705,
8.0,	0.1716508071,
9.0,	-0.0903336112,
10.0,	-0.2459357645,
11.0,	-0.1711903004,
12.0,	0.0476893108,
13.0,	0.2069261024,
14.0,	0.1710734761,
};

#define	num_j1	17
static	double	j1_args [num_j1] [2]
={
0.0,	0.0,
0.5,	0.2422684577,
1.5,	0.5579365079,
1.0,	0.4400505857,
2.0,	0.5767248078,
3.0,	0.3390589585,
4.0,	-0.0660433280,
5.0,	-0.3275791376,
6.0,	-0.2766838581,
7.0,	-0.0046828235,
8.0,	0.2346363469,
9.0,	0.2453117866,
10.0,	0.0434727462,
11.0,	-0.1767852990,
12.0,	-0.2234471045,
13.0,	-0.0703180521,
14.0,	0.1333751547
};

#define	num_y0	16
static	double	y0_args [num_y0] [2]
={
0.5,	-0.4445187335,
1.5,	0.3824489238,
1.0,	0.0882569642,
2.0,	0.5103756726,
3.0,	0.3768500100,
4.0,	-0.0169407393,
5.0,	-0.3085176252,
6.0,	-0.2881946840,
7.0,	-0.0259497440,
8.0,	0.2235214894,
9.0,	0.2499366983,
10.0,	0.0556711673,
11.0,	-0.1688473239,
12.0,	-0.2252373126,
13.0,	-0.0782078645,
14.0,	0.1271925686
};


#define	num_y1	16
static	double	y1_args [num_y1] [2]
={
0.5,	-1.4714723927,
1.5,	-0.4123086270,
1.0,	-0.7812128213,
2.0,	-0.1070324315,
3.0,	0.3246744248,
4.0,	0.3979257106,
5.0,	0.1478631434,
6.0,	-0.1750103443,
7.0,	-0.3026672370,
8.0,	-0.1580604617,
9.0,	0.1043145752,
10.0,	0.2490154242,
11.0,	0.1637055374,
12.0,	-0.0570992183,
13.0,	-0.2100814084,
14.0,	-0.1666448419
};

typedef	struct
	{
	int	arg1;
	double	arg2;
	double	result;
	} n_args ;

#define	num_jn	28
static	n_args	jn_args [num_jn]
	={
	{3, 1.0, 0.0195633540 },
	{3, 3.6, 0.3987626737 },
	{3, 4.2, 0.4343942764 },
	{3, 9.8, 0.0096999027 },
	{4, 1.0, 0.0024766390 },
	{4, 3.6, 0.2197990574 },
	{4, 4.2, 0.3100285510 },
	{4, 9.8, -0.2452842690 },
	{5, 1.0, 0.0002497577 },
	{5, 3.6, 0.0896796760 },
	{5, 4.2, 0.1561362970 },
	{5, 9.8, -0.2099319591 },
	{6, 1.0, 0.0000209383 },
	{6, 3.6, 0.0293111538 },
	{6, 4.2, 0.0617245370 },
	{6, 9.8, 0.0310679842 },
	{7, 1.0, 0.000001502325817 },
	{7, 3.6, 0.0080241700 },
	{7, 4.2, 0.0202195230 },
	{7, 9.8, 0.2479743887 },
	{8, 1.0, 0.000000094223442 },
	{8, 3.6, 0.0018939518 },
	{8, 4.2, 0.0056738731 },
	{8, 9.8, 0.3231811425 },
	{9, 1.0, 0.000000005249250 },
	{9, 3.6, 0.0003933937 },
	{9, 4.2, 0.0013952316 },
	{9, 9.8, 0.2796682929 }
	};

#define	num_yn	28
static	n_args	yn_args [num_yn]
	={
	{3, 1.0, -5.8215176060 },
	{3, 3.6, -0.3230993304 },
	{3, 4.2, -0.1118267169 },
	{3, 9.8, -0.2607411779 },
	{4, 1.0, -33.2784230290 },
	{4, 3.6, -0.6215620725 },
	{4, 4.2, -0.4287478482 },
	{4, 9.8, -0.1036600131 },
	{5, 1.0, -260.4058666258 },
	{5, 3.6, -1.0581497197 },
	{5, 4.2, -0.7048358511 },
	{5, 9.8, 0.1761207590 },
	{6, 1.0, -2570.7802432291 },
	{6, 3.6, -2.3177427043 },
	{6, 4.2, -1.2494327497 },
	{6, 9.8, 0.2833750734 },
	{7, 1.0, -30588.9570521240 },
	{7, 3.6, -6.6676592948 },
	{7, 4.2, -2.8649720053 },
	{7, 9.8, 0.1708691267 },
	{8, 1.0, -425674.6184865066 },
	{8, 3.6, -23.6120434422 },
	{8, 4.2, -8.3004739345 },
	{8, 9.8, -0.0392763209 },
	{9, 1.0, -6780204.9387319824 },
	{9, 3.6, -98.2747560037 },
	{9, 4.2, -28.7558810785 },
	{9, 9.8, -0.2349937323 }
	};
/*
** bessel
**	Test the bessel functions.
*/
main()
{
	setup("bessel");
	/*
	 *  close STDERR so all tests can open a temp file in its place
	 */
	fclose (stderr);

	funct("j0");
	start
		t_j0();
	finis
	funct("j1");
	start
		t_j1();
	finis
	funct("jn");
	start
		t_jn();
	finis
	funct("y0");
	start
		t_y0();
	finis
	funct("y1");
	start
		t_y1();
	finis
	funct("yn");
	start
		t_yn();
	finis

	done();
	/*NOTREACHED*/
}
/*
** t_j0
**	Test the j0 function.
**/
t_j0()
{
	int	i;
	char	buffer[STRLEN];
	FILE	*fp;
	extern	double	j0();

	for (i=0 ; i<num_j0 ; i++) {
		testing("j0(%f) should return %f", 
			j0_args[i][0], j0_args[i][1]);
		start
			expecting (j0_args [i][1]);
			j0(j0_args [i] [0]);
		finis
	}

	testing("j0(HUGE) should return 0 with errno set to ERANGE,");
	testing("and a message about TLOSS printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpj0' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpj0","w");
		
		expecting( ERANGE );
		expecting (0.0);
		j0(HUGE);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpj0","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"TLOSS") )
			error ("j0(HUGE) did not print %s\n%s'%s'",
			    "an error message about TLOSS on stderr",
			    "actual message was",
			    buffer);
	finis
}
/*
** t_j1
**	Test the j1 function.
*/
t_j1()
{
	int	i;
	char	buffer[STRLEN];
	FILE	*fp;
	extern	double	j1();

	for (i=0 ; i<num_j1 ; i++) {
		testing("j1(%f) should return %f", 
			j1_args[i][0], j1_args[i][1]);
		start
			expecting (j1_args [i][1]);
			j1(j1_args [i] [0]);
		finis
	}

	testing("j1(HUGE) should return 0 with errno set to ERANGE,");
	testing("and a message about TLOSS printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpj0' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpj1","w");
			
		expecting (ERANGE);
		expecting (0.0);
		j1(HUGE);

		expecting (SUCCESS);
		fclose(fp);

		fp = temp_fopen("tmpj1","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"TLOSS") )
			error ("j1(HUGE) did not print %s\n%s'%s'",
			    "an error message about TLOSS on stderr",
			    "actual message was",
			    buffer);
		
	finis
}
/*
** t_y0
**	Test the y0 function.
*/
t_y0()
{
	int	i;
	char	buffer[STRLEN];
	FILE	*fp;
	extern	double	y0();

	for (i=0 ; i<num_y0 ; i++)
	{
		testing("y0(%f) should return %f", 
			y0_args[i][0], y0_args[i][1]);
		start
			expecting (y0_args [i][1]);
			y0(y0_args [i] [0]);
		finis
	}

	testing("y0(HUGE) should return 0 with errno set to ERANGE,");
	testing("and a message about TLOSS printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpy0' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpy0","w");

		expecting (ERANGE);
		expecting (0.0);
		y0(HUGE);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpy0","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"TLOSS") )
			error ("y0(HUGE) did not print %s\n%s'%s'",
			    "an error message about TLOSS on stderr",
			    "actual message was",
			    buffer);
	finis

	testing("y0(-1) should return -HUGE with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpy0' is opened, then closed and");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpy0","w");

		expecting (-HUGE);
		expecting (EDOM);
		y0(-1.0);

		expecting (SUCCESS);
		fclose(fp);

		fp = temp_fopen("tmpy0","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("y0(-1.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis

}
/*
** t_y1
**	Test the y1 function.
*/
t_y1()
{
	int	i;
	char	buffer[STRLEN];
	FILE	*fp;
	extern	double	y1();

	for (i=0 ; i<num_y1 ; i++) {
		testing("y1(%f) should return %f", y1_args[i][0], y1_args[i][1]);
		start
			expecting (y1_args [i][1]);
			y1(y1_args [i] [0]);
		finis
	}


	testing("y1(HUGE) should return 0 with errno set to ERANGE,");
	testing("and a message about TLOSS printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpy1' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpy1","w");

		expecting(ERANGE);
		expecting(0.0);

		y1(HUGE);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpy1","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"TLOSS") )
			error ("y1(HUGE) did not print %s\n%s'%s'",
			    "an error message about TLOSS on stderr",
			    "actual message was",
			    buffer);
	finis

	testing("y1(-1.0) should return -HUGE with errno set to EDOM");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpy1' is opened, then closed and reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpy1","w");
			
		expecting (-HUGE);
		expecting (EDOM);
		y1(-1.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpy1","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("y1(-1.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	
	finis
}
/*
** t_jn
**	Test the jn routine.
*/
t_jn()
{
	int	i;
	char	buffer[STRLEN];
	FILE	*fp;
	extern	double	jn();

	for (i=0 ; i<num_jn ; i++) {
		testing("jn(%d, %f) should return %f",
		     jn_args[i].arg1, jn_args[i].arg2, jn_args[i].result);
		start
			expecting (jn_args [i].result);
			jn(jn_args [i].arg1, jn_args[i].arg2 );
		finis
	}

	testing("jn(4, HUGE) should return 0 with errno set to ERANGE,");
	testing("and a message about TLOSS printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpjn' is opened, then closed and ");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpjn","w");

		expecting (ERANGE);
		expecting (0.0);
		jn(4, HUGE);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpjn","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"TLOSS") )
			error ("jn(4, HUGE) did not print %s\n%s'%s'",
			    "an error message about TLOSS on stderr",
			    "actual message was",
			    buffer);
	finis
}
/*
** t_yn
**	Test the yn routine
*/
t_yn()
{
	int	i;
	char	buffer[STRLEN];
	FILE	*fp;
	extern	double	yn();


	for (i=0 ; i<num_yn ; i++) {
		testing("yn(%d, %f) should return %f",
		     yn_args[i].arg1, yn_args[i].arg2, yn_args[i].result);
		start
			expecting (yn_args [i].result);
			yn(yn_args [i].arg1, yn_args[i].arg2 );
		finis
	}

	testing("yn(9, HUGE) should return 0 with errno set to ERANGE,");
	testing("and a message about TLOSS printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpyn' is opened, then closed and");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpyn","w");

		expecting(ERANGE);
		expecting(0.0);
		yn(9, HUGE);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpyn","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"TLOSS") )
			error ("yn(9, HUGE) did not print %s\n%s'%s'",
			    "an error message about TLOSS on stderr",
			    "actual message was",
			    buffer);
	finis

	testing("yn(9, -1.0) should return -HUGE with errno set to EDOM,");
	testing("and a message about DOMAIN printed on stderr.");
	testing("To test STDERR for the error message, STDERR is closed");
	testing("and temp file 'tmpyn' is opened, then closed and");
	testing("reopened for reading");
	start
		expecting (SUCCESS);
		fp = temp_fopen("tmpyn","w");

		expecting (-HUGE);		
		expecting (EDOM);
		yn (9, -1.0);

		expecting (SUCCESS);
		fclose(fp);
		fp = temp_fopen("tmpyn","r");
		fgets(buffer,STRLEN,fp);
		fclose(fp);

		if ( find(buffer,"DOMAIN") )
			error ("yn(9, -1.0) did not print %s\n%s'%s'",
			    "an error message about DOMAIN on stderr",
			    "actual message was",
			    buffer);
	finis
}
