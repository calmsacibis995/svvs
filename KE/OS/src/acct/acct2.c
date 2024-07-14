#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifdef ACCT
#include	<sys/acct.h>
# endif

#ident	"@(#)acct2.c	1.2"


/*
** ACCT2
**	This is the process being exec'd from the test for acct.
**
**	The test for acct is running setuid root, hence this process
**		is also setuid root.  We want to get the ac_flag set
**		to show that we used root privs.
**
** Notes
**	This program doesn't do any testing, everything is assumed to 
**	happen in the parent.
*/
main()
{
# ifdef ACCT
	
	int	owner;
	int	group;
	char	pathpointer[STRLEN];
	ttime 	t;
	ttime 	time();
setup("acct");

testing ("this is the SUID root child");
start

	expecting (VOID);
	t = time((ttime *)0) + 2l;
	while( t > time((ttime *)0))
		;

	strcpy(pathpointer, temp_path());
	strcat(pathpointer, "tmpacct");

	owner = BIN_UID;
	group = BIN_UID;
/*
**	give file away, does not require root privs.
*/
	if ( chown((char *)pathpointer, owner, group) != 0 )
		error ("can not do a #1 chown");
/*
**	give someone else's file away, does require root privs
*/
	if ( chown((char *)pathpointer, owner, group) != 0 )
		error ("can not do a #2 chown");
finis

	exit(2);
/*NOTREACHED*/
#endif
}

