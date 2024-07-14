#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	MAXNUM	350
# define	MAXNUM2	100
# define	LARGE	0x7FFFFFFF
# define	SMALL	-0x80000000

#ident	"@(#)drand48.c	1.4"


/*
** drand48
**	Test the drand48 functions.
*/
main()
{
	double			dr_ret[MAXNUM];
	struct
	{
		double		value;
		int		count;
	}dr_ret2[MAXNUM];
	double			er_ret[MAXNUM];

	long			lr_ret[MAXNUM];
	struct
	{
		long		value;
		int		count;
	}lr_ret2[MAXNUM];
	long			nr_ret[MAXNUM];

	long			mr_ret[MAXNUM];
	struct
	{
		long		value;
		int		count;
	}mr_ret2[MAXNUM];
	long			jr_ret[MAXNUM];

	double			dr_ret3[MAXNUM];
	double			dr_ret4[MAXNUM];
	double			dr_ret5[MAXNUM];
	double			dr_ret6[MAXNUM];

	unsigned	short	unshort_parm[3];
	unsigned	short	myseed[3];
	unsigned	short	*uns_ptr;
	unsigned	short	lcong_parm[7];

	int			i;
	int			j;
	int			found;
	int			last;

	extern	double		drand48();
	extern	double		erand48();
	extern	long		lrand48();
	extern	long		nrand48();
	extern	long		mrand48();
	extern	long		jrand48();
	extern	void		srand48();
	extern	unsigned short	*seed48();
	extern	void		lcong48();

setup("drand48");


funct ("drand48");
start

testing("Verify a return value in the range [0.0,1.0), and no more than");
testing("5%% repetition of the return values.");
start
	exit_on (ERROR);

	if ( (uns_ptr = seed48(unshort_parm)) != NULL )
	{
		myseed[0]= *uns_ptr;
		myseed[1] = *(uns_ptr + 1);
		myseed[2] = *(uns_ptr + 2);
		unshort_parm[0] = *uns_ptr;
		unshort_parm[1] = *(uns_ptr + 1);
		unshort_parm[2] = *(uns_ptr + 2);
	}
	else
	{
		error ("seed48 failed to get the seed");
	}

	if ( (uns_ptr = seed48(unshort_parm)) == NULL )
		error ("seed48 failed to set the seed");

	last = 0;

	for ( i = 0; i < MAXNUM; i++ )
	{
		dr_ret[i] = drand48();
		if ( dr_ret[i] >= 1.0 || dr_ret[i] < 0.0 )
			error ("Drand48 returned %f which is a value outside the range [0.0,1).", dr_ret[i]);

		found = 0;
		for ( j = 0; j < last; j++ )
		{
			if ( dr_ret[i] == dr_ret2[j].value )
			{
				dr_ret2[j].count++;
				if ( dr_ret2[j].count > 50 )
					error ("More than 5%% duplication by drand48().");
				found = 1;
				break;
			}
		}
		if ( found == 0 )
		{
			dr_ret2[last].value = dr_ret[i];
			dr_ret2[last].count = 1;
			last++;
		}
	}
finis

finis

funct("erand48");
start


testing("Verify return values in the range [0.0,1.0) and");
testing("equal to the value returned in drand48.");
start

	unshort_parm[0] = myseed[0];
	unshort_parm[1] = myseed[1];
	unshort_parm[2] = myseed[2];

	for ( i = 0; i < MAXNUM; i++ )
	{
		er_ret[i] = erand48(unshort_parm);
		if ( er_ret[i] >= 1.0 || er_ret[i] < 0.0 )
			error ("Erand48 returned %f, a value outside the \
range [0.0,1).", er_ret[i]);

		if ( er_ret[i] != dr_ret[i] )
			error ("Erand48 returned %f, a value not equal to what drand would return", er_ret [i]);

	}
finis

finis

funct("lrand48");

testing("Verify return values in the range [0,2**31) and no more than");
testing("5%% repetition of the return values.");
start
	unshort_parm[0] = myseed[0];
	unshort_parm[1] = myseed[1];
	unshort_parm[2] = myseed[2];

	if ( (uns_ptr = seed48(unshort_parm)) == NULL )
		error ("seed48 failed to set the seed");

	last = 0;

	expecting (VOID);

	for ( i = 0; i < MAXNUM; i++ )
	{
		lr_ret[i] = lrand48();
		if ( lr_ret[i] > LARGE || lr_ret[i] < 0 )
			error ("Lrand48 returned %f, a value outside the range [0,2**31).", lr_ret[i]);

		found = 0;
		for ( j = 0; j < last; j++ )
		{
			if ( lr_ret[i] == lr_ret2[j].value )
			{
				lr_ret2[j].count++;
				if ( lr_ret2[j].count > 50 )
					error("More than 5%% duplication by lrand48().");
				found = 1;
				break;
			}
		}
		if ( found == 0 )
		{
			lr_ret2[last].value = lr_ret[i];
			lr_ret2[last].count = 1;
			last++;
		}
	}
finis


funct("nrand48");

testing("Verify return a value in the range [0,2**31) and");
testing("equal to the value returned in lrand48.");
start
	unshort_parm[0] = myseed[0];
	unshort_parm[1] = myseed[1];
	unshort_parm[2] = myseed[2];

	expecting (VOID);

	for ( i = 0; i < MAXNUM; i++ )
	{
		nr_ret[i] = nrand48(unshort_parm);
		if ( nr_ret[i] > LARGE || nr_ret[i] < 0 )
			error ("Nrand48 returned the value %d, outside the range [0,2**31).", nr_ret[i]);

		if ( nr_ret[i] != lr_ret[i] )
			error ("Nrand48 returned the value %d, not equal to what lrand would return", nr_ret[i]);

	}
finis


funct ("mrand48");

testing("Verify return values in the range [-2**31,2**31) and no more");
testing("than 5%% repetition of the return values.");
start
	exit_on (ERROR);

	unshort_parm[0] = myseed[0];
	unshort_parm[1] = myseed[1];
	unshort_parm[2] = myseed[2];

	if ( (uns_ptr = seed48(unshort_parm)) == NULL )
		error ("seed48 failed to set the seed");

	last = 0;

	for ( i = 0; i < MAXNUM; i++ )
	{
		mr_ret[i] = mrand48();
		if ( mr_ret[i] > LARGE || mr_ret[i] < SMALL )
			error ("Mrand48 returned %f, a value outside the range [-2**31,2**31).", mr_ret[i]);

		found = 0;
		for ( j = 0; j < last; j++ )
		{
			if ( mr_ret[i] == mr_ret2[j].value )
			{
				mr_ret2[j].count++;
				if ( mr_ret2[j].count > 50 )
					error("More than 5%% duplication by mrand48().");
				found = 1;
				break;
			}
		}
		if ( found == 0 )
		{
			mr_ret2[last].value = mr_ret[i];
			mr_ret2[last].count = 1;
			last++;
		}
	}
finis


funct("jrand48");

testing("Verify return values in the range [-2**31,2**31) and");
testing("equal to the value returned in mrand48.");
start
	unshort_parm[0] = myseed[0];
	unshort_parm[1] = myseed[1];
	unshort_parm[2] = myseed[2];

	exit_on (ERROR);

	for ( i = 0; i < MAXNUM; i++ )
	{
		jr_ret[i] = jrand48(unshort_parm);
		if ( jr_ret[i] > LARGE || jr_ret[i] < SMALL )
			error ("Jrand48 returned %f, a value outside the range [-2**31,2**31).", jr_ret[i]);

		if ( jr_ret[i] != mr_ret[i] )
			error ("Jrand48 returned %f, a value not equal to what lrand would return", jr_ret[i]);

	}
finis


funct ("srand48");

testing("The srand48() function should change the seed.");
start
	srand48((long)25);
	for ( i = 0; i < MAXNUM2; i++ )
	{
		dr_ret3[i] = drand48();
		if ( dr_ret3[i] >= 1.0 || dr_ret3[i] < 0.0 )
			error ("Drand48 returned %f, a value outside the range [0.0,1).", dr_ret3[i]);

		if ( dr_ret[i] == dr_ret3[i] )
			error ("Drand48 returned %f, a value equal to what was returned previous to the srand48 call.",dr_ret3[i]);

	}
finis


funct ("seed48");

testing("Verify the seed48() function changes the seed.");
start

	unshort_parm[0] = myseed[0] + 1;
	unshort_parm[1] = myseed[1] + 1;
	unshort_parm[2] = myseed[2] + 1;

	exit_on (ERROR);

	if ( (uns_ptr = seed48(unshort_parm)) == NULL )
		error ("seed48 failed to set the seed");
	for ( i = 0; i < MAXNUM2; i++ )
	{
		dr_ret4[i] = drand48();
		if ( dr_ret4[i] >= 1.0 || dr_ret4[i] < 0.0 )
			error ("Drand48 returned %f, a value outside the range [0.0,1).", dr_ret4[i]);

		if ( dr_ret[i] == dr_ret4[i] )
			error ("Drand48 returned %f, a value equal to what was returned previous to the seed48 call.", dr_ret4[i]);
finis

	}

funct ("lcong48");

testing("The lcong48() function should change the seed.");
start
	break_on (ERROR);

	lcong_parm[0] = myseed[0] + 1;
	lcong_parm[1] = myseed[1] + 1;
	lcong_parm[2] = myseed[2] + 1;
	lcong_parm[3] = myseed[2] + 1;
	lcong_parm[4] = myseed[2] + 1;
	lcong_parm[5] = myseed[2] + 1;
	lcong_parm[6] = myseed[2] + 1;

	lcong48(lcong_parm);
	for ( i = 0; i < MAXNUM2; i++ )
	{
		dr_ret5[i] = drand48();
		if ( dr_ret5[i] >= 1.0 || dr_ret5[i] < 0.0 )
			error ("Drand48 returned %f, a value outside the range [0.0,1).", dr_ret5[i]);

		if ( dr_ret[i] == dr_ret5[i] )
			error ("Drand48 returned %f, a value equal to what was returned previous to the seed48 call.", dr_ret5[i]);

	}
finis


funct ("seed48");
testing("Verfiy the seed48() function resets the addend and multiplier.");
start
	break_on (ERROR);

	unshort_parm[0] = myseed[0];
	unshort_parm[1] = myseed[1];
	unshort_parm[2] = myseed[2];

	if ( (uns_ptr = seed48(unshort_parm)) == NULL )
		error ("seed48 failed to set the seed");
	for ( i = 0; i < MAXNUM2; i++ )
	{
		dr_ret6[i] = drand48();
		if ( dr_ret6[i] >= 1.0 || dr_ret6[i] < 0.0 )
			error ("Drand48 returned %f, a value outside the range [0.0,1).", dr_ret6[i]);

		if ( dr_ret[i] != dr_ret6[i] )
			error ("Drand48 returned %f, a value not equal to what was returned previous to the lcong48 call.", dr_ret6[i]);
finis

	}
done();
/*NOTREACHED*/

}
