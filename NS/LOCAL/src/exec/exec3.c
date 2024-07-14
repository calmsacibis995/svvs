#include	<sv_base.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)exec3.c	1.4"

/*
 * exec3()
 *
 *	 This is a setuid program.
 */
main() {
	setup("exec");

	testing("Verify the real user id is svvs.");
	start
		expecting( UID );
		getuid();
	finis

	testing("Verify the effective user id is root.");
	start
		expecting( ROOT_UID );
		geteuid();
	finis
	done();
	/*NOTREACHED*/
}
