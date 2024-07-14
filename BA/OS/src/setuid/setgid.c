#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)setgid.c	1.4"

/*
** setgid
**	Test the setgid function.
**
**	NOTE: Each test case
**	sets up the effective and real gid for the next test case.
**	
**	Make sure that this binary is setgid root.
**	And setuid root.
**
**	Since the effective UID is ROOT,
**	setgrp will set both the effective and real GID to anything.
**
*/

main()
{
	unsigned short	getgid();
	unsigned short	getegid();
	int	setgid();

	setup("setuid");

	funct("setgid");

	exit_on ( ERROR );


	testing ("check that the real GID is svvs, effective GID = root");
	start
		expecting (GID);
		getgid ();

		expecting (ROOT_EGID);
		getegid ();
	finis



	abstract("Step through various combinations of setgid calls.");
	abstract("Each test sets up the environment for the next test.");
	start
		testing("Setgid(root) current egid = root, rgid= svvs.");
		testing("Expect rgid = ROOT_GID, egid = ROOT_EGID, return value of 0.");

		start

			expecting ( 0 );
			setgid(ROOT_GID);

			expecting( ROOT_GID );
			(int) getgid();

			expecting( ROOT_EGID );
			(int) getegid();


		finis

		testing("Setgid(svvs) current egid = ROOT_EGID, rgid = ROOT_GID.");
		testing("Expect rgid = GID, egid = EGID, return value of 0.");
		start

			expecting ( 0 );
			setgid(GID);

			expecting( GID );
			(int) getgid();

			expecting( EGID );
			(int) getegid();

		finis



		testing("Setgid(bin) current egid = EGID, rgid = GID.");
		testing("Expect rgid = BIN_GID, egid = BIN_EGID, return value of 0.");
		start

			expecting ( 0 );
			setgid(BIN_GID);

			expecting( BIN_GID );
			(int) getgid();

			expecting( BIN_EGID );
			(int) getegid();

		finis
	finis
	done();
}
