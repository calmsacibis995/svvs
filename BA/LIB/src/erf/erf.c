#include	<sv_base.h> /*replacement macros */
#include	<math.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)erf.c	1.1"



extern	double	erf();
extern	double	erfc();


/*
 *  These arrays hold the argument and return value pairs
 */
#define	num_erf	9
static	double	erf_args [num_erf] [2]
={
0.0, 0.0,
0.17, 0.189992,
0.25, 0.276326,
0.5, 0.5205,
0.76, 0.717537,
0.83, 0.759524,
1.0, 0.842701,
5.0, 1.0,
100.0, 1.0
};


#define	num_erfc	9
static	double	erfc_args [num_erfc] [2]
={
0.0, 1.0,
0.17, 1 - 0.189992,
0.25, 1 - 0.276326,
0.5, 1 - 0.5205,
0.76, 1 - 0.717537,
0.83, 1 - 0.759524,
1.0, 1 - 0.842701,
5.0, 0.0,
100.0, 0.0,
};

/*
** erf
**	Test the error functions.
*/
main()
{
setup("erf");
	t_erf();
	t_erfc();
done();
/*NOTREACHED*/
}

/*
** t_erf
**	Test the erf function.
**
** Level
**	This routine is in the normal level (0).
*/
t_erf()
{
int	i;

funct ("erf");
start

	for (i=0 ; i<num_erf ; i++) {
		testing("erf(%f) should return %f", 
			erf_args[i][0], erf_args[i][1]);
		start
			if (erf_args [i][1] == 0.0)
				expecting (ZERO);
			else
				expecting (erf_args [i][1]);
			erf (erf_args [i] [0]);
		finis
	}
finis
}

/*
** t_erfc
**	Test the erfc function.
**
** Level
**	This routine is in the normal level (0).
*/
t_erfc()
{
int	i;

funct ("erfc");
start

	for (i=0 ; i<num_erfc ; i++) {
		testing("erfc(%f) should return %f", 
			erfc_args[i][0], erfc_args[i][1]);
		start
			if (erfc_args [i][1] == 0.0)
				expecting (ZERO);
			else
				expecting (erfc_args [i][1]);
			erfc (erfc_args [i] [0]);
		finis
	}
finis
}
