#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# include	<malloc.h>

#ident	"@(#)mallinfo.c	1.3"

/*
** mallinfo
**	Test the mallinfo function.
*/

main()
{
	struct mallinfo ret_val1, ret_val2;
	struct mallinfo mallinfo();
	char *malloc();

	setup("malloc");
	funct("mallinfo");

	abstract("An invocation of mallinfo after a call to malloc.");
	abstract("Expect a return of struct mallinfo.");
	start

	prep("Issue a call to malloc to prime the arena.");
	start
		expecting ( NON_NULL );
		malloc(17);
	finis

	testing("Issue the call to mallinfo.");
	start
		expecting ( VOID );
		ret_val1 = mallinfo();

	finis
	finis

	abstract("A second invocation of mallinfo after more space has been allocated.");
	abstract("Expect a return of struct mallinfo different from previous test case.");
	start

		prep("Issue the second call to malloc.");
		start
		expecting ( NON_NULL );
		malloc(23);
		finis

		testing("Issue the second call to mallinfo.");
		start
		expecting ( SUCCESS );
		ret_val2 = mallinfo();
		finis

	testing("Determine that the info for the two arenas differs.");
	start

		if ( ret_val1.arena == ret_val2.arena && ret_val1.ordblks == ret_val2.ordblks && ret_val1.smblks == ret_val2.smblks && ret_val1.hblkhd == ret_val2.hblkhd && ret_val1.usmblks == ret_val2.usmblks && ret_val1.fsmblks == ret_val2.fsmblks && ret_val1.uordblks == ret_val2.uordblks && ret_val1.fordblks == ret_val2.fordblks && ret_val1.keepcost == ret_val2.keepcost )
	{
warning("Ret val from mallinfo not changed by malloc call.");	/* WARNING */
	}

	finis
		finis
		done();
}
		/*NOTREACHED*/

