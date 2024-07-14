#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)sync.c	1.3"

/*
** sync
**	Test the sync function.
**
*/

main()
{

	void	sync();

	setup("sync");
	funct("sync");

	testing("A typical call to sync.");
	testing("Expecting nothing bad to happen.");
	start
		sync();
	finis

	done();
	/*NOTREACHED*/
}
