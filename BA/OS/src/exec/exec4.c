#include	<sv_base.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)exec4.c	1.4"

/*
 * exec4()
 *
 *	 This is a setgid program.
 */
main() {
	setup("exec");

	testing("Verify the real group id is svvs.");
	start
		expecting( GID );
		getgid();
	finis

	testing("Verify the effective group id is bin.");
	start
		expecting( BIN_GID );
		getegid();
	finis
	done();
	/*NOTREACHED*/
}
