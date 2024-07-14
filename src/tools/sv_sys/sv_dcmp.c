/*LINTLIBRARY*/
#include	<svvs.h>
#include	<math.h>	/* need HUGE from here */
#include	<sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_dcmp.c	1.3"
#endif

/*
** DCMP
**	Double comparison function.
**	This routine is used to compare doubles, by checking
**	if the arguments are equal to DBL_DIG decimal places.
**	If the second argument is HUGE, a direct comparison is made.
**
** Caveat:
**	This function assumes that MAXDOUBLE and DBL_MIN are correct.
**	If values greater [less] than MAXDOUBLE [DBL_MIN] exist, this
**	function may yield erroneous results.
**
** Return Value:
**	0 if the values are not 'close'
**	1 if they are.
*/
# define	SVVS_DIG	5
# define	EPSILON		1.0e-SVVS_DIG
# define	ZERO_PLUS	(1.0e-10)
# define	ZERO_MINUS	(-1.0e-10)

static int sv_zero = 0;

static double range(x,a,b)
double a,x,b;
{
	if( a < b ) return( a <= x && x <= b );
	else return( a >= x && x >= b );
}
dcmp(a,b)
double	a,b;
{
	static double fp = 1.0+EPSILON;
	static double fm = 1.0-EPSILON;
	int	zero = sv_zero;
	sv_zero = 0;
/*
 *	note that in n digit arithmetic:
 *
 *	( a > M/(1.0+epsilon) ) is equivalent to ( a > M*(1.0-epsilon) )
 *
 *	To check endpoints we need:
 *
 *	( a * (1.0+epsilon) > M )
 *
 *	But cannot represent a value greater than M, so would use:
 *
 *	( a > M/(1.0+epsilon) )
 *
 *	But prefer the equivalent:
 *
 *	( a > M*(1.0-epsilon) )
 *
 *	Because multiplication is preferable to division.
 *
 *
 *	if called from evaluation funct, a is actual ret val, b is expected
 */
	if ( a == b ) 
		return( 1 );
	if ( b == HUGE || b == -HUGE )
		return ( 0 );

	if ( zero ) {
		return( range(a,ZERO_MINUS,ZERO_PLUS) );		
	}

	if ( range(b,MAXDOUBLE*fm,MAXDOUBLE) )	/* b*fp > DBL_MAX */
		return( range(a,b*fm,MAXDOUBLE) );
	if ( range(b,-MAXDOUBLE*fm,-MAXDOUBLE )	)/* b*fp < -DBL_MAX */
		return( range(a,b*fm,-MAXDOUBLE) );
	if ( range(b,DBL_MIN,DBL_MIN*fp) )	/* b*fm < DBL_MIN */
		return( range(a,DBL_MIN,b*fp) );
	if ( range(b,-DBL_MIN,-DBL_MIN*fp) )	/* b*fm > -DBL_MIN */
		return( range(a,-DBL_MIN,b*fp) );

	return(range(a,b*fm,b*fp));
}

sv_setz() {
	if( sv_ltest( sv_xenv ) )
		sv_zero = 1;
}
