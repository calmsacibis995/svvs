#include <sv_env.h>
#ident	"@(#)exec7.c	1.1"

/*
 * exec7() - used to test E2BIG.
 *	under normal conditions, will never be executed.
 */
main() {
error("exec succeeded unexpectedly.");
	exit( 1 );
	/*NOTREACHED*/
}
