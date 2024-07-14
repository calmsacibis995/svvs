#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifdef ACCT
#include	<sys/acct.h>
# endif

#ident	"@(#)acct1.c	1.2"


/*
** ACCT1
**	This is the process being exec'd from the test for acct.
**
** Notes
**	This program doesn't do anything, everything is assumed to happen
**	in the parent.
*/
main()
{
# ifdef ACCT
	ttime t;
	ttime time();
setup("acct");

testing ("the child has successfully spawned.");
start
	expecting (VOID);
	t = time((ttime *)0) + 2l;
	while( t > time((ttime *)0));
finis
	exit(0);
/*NOTREACHED*/
# endif
}

