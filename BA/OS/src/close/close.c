#include	<sv_base.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)close.c	1.3"

/*
 * close - a test of close(BA_OS).
 */

main() {
	int	fildes;

	setup("close");
	funct("close");

	exit_on( ERROR );
	testing("A legal invocation of close.");
	start
		fildes = temp_creat( "close.tmp", 0664 );
		close( fildes );
	finis

	testing("Close an invalid file descriptor.");
	testing("Verify failure with errno set to EBADF.");
	start
		expecting( EBADF );
		close( BAD_FILDES );
	finis

	testing("Close a file descriptor that is already closed.");
	testing("Verify failure with errno set to EBADF.");
	start
		expecting( EBADF );
		close( fildes );	/* Obtained from previous test */
	finis

	done();
	/*NOTREACHED*/
}
