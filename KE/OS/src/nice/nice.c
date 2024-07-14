#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	USERID		99

#ident	"@(#)nice.c	1.3"


/*
** nice
**	Test the nice function.
*/
main()
{
# ifdef NICE
	int		ret_val;
	
	int		incr;
	int		niceNZERO;
	int 		max_nice = (2*NZERO -1);
	int		min_nice=0;
	int		nice_value;

setup("nice");
funct("nice");

testing("nice with incr of 2*NZERO");
testing("should return (2*NZERO - 1) - NZERO.");
start
	expecting (max_nice - NZERO);
	nice(max_nice +1);
finis

testing("nice with all valid ranges of nice_value (upper to lower limit)");
testing("should return (nice_value - NZERO).");
start
	/*set nice value to upper limit */
	expecting (VOID);
	nice(2*NZERO);

	for (incr = -1 ; incr > -2*NZERO; incr--)
	{
		ret_val = nice(-1);
		if ( ret_val != nice(0) )
			error ("nice (-1) returned %d, expecting %d",
				ret_val, nice(0));
/*!!!!! is this correct????? */
	}
finis


testing("nice with all valid ranges of nice_value (lower to upper limit)");
testing("should return (nice_value - NZERO).");
start
	/*set nice value to zero */
	expecting (VOID);
	nice(-2*NZERO);

	for (incr = 1 ; incr < 2*NZERO ; incr++)
	{
		ret_val = nice(1);
		if ( ret_val != nice(0) )
			error ("nice (-1) returned %d, expecting %d",
				ret_val, nice(0));
	}
finis



testing("trying to set nice_value to negative value");
testing("should return -NZERO.");
start
	/*set nice value to one*/
	expecting (VOID);
	nice(-2*NZERO);
	nice(1);
	
	/*Try to decrement nice_value to -2*/
	expecting (-NZERO);
	nice(-2);
finis


done();
/*NOTREACHED*/

# else
	void exit();
	
	unsupported ("Changing process priority is not supported.");
	exit(0);
/*NOTREACHED*/



# endif
}
