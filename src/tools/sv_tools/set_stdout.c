
/*LINTLIBRARY*/
#include <stdio.h>
#include <svvs.h>
#include <sv_env.h>

#ident "@(#)set_stdout.c	1.2"

/*
 *	set_stdout
 *
 *	dup stdout to a new fd, so that the TI tests can 
 *	use stdout for testing, while the error reporting
 *	facilities can still send to the file previsouly 
 *	associated with stdout.
 */
set_stdout() {
	int outfd;
/*
 *	Check if redirection has already been done,
 *	perhaps by a parent process
 */
	if( sv_get_outfd() != 1) 
		return;
	if ( (outfd = dup(fileno(stdout))) == -1 )
		severr("dup of fileno(stdout) failed");

	sv_set_outfd( outfd );
}
