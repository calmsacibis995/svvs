#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)setgid1.c	1.6"

/*
** setgid1
**	Test the setgid function.
**
**	NOTE: Each test case
**	sets up the effective and real gid for the next test case.
**	
**	Make sure that this binary is setgid bin.
**
*/

main()
{
	unsigned short	getgid();
	unsigned short	getegid();
	int	setgid();

	setup("setuid");

	funct("setgid");



	testing ("check that the real GID is svvs, effective GID = bin");
	start

		break_on ( ERROR );
		expecting (GID);
		getgid ();

		expecting (BIN_EGID);
		getegid ();
	finis



	abstract("Step through various combinations of setgid calls.");
	abstract("Each test sets up the environment for the next test.");
	start
		testing("Setgid(svvs) with current effective gid of bin and real gid of svvs and an executable sgid bin.");
		testing("Expect rgid = GID, egid = EGID, return value of 0.");

		start

			expecting ( 0 );
			setgid(GID);

			expecting( GID );
			(int) getgid();

			expecting( EGID );
			(int) getegid();

		finis

		testing("Setgid(root)...which should be illegal for this process.");
		testing("Return value of -1 and errno of EPERM. Gid's should remain unchanged.");
		start

			expecting( EPERM );
			setgid(ROOT_GID);

			expecting( GID );
			(int) getgid();

			expecting( EGID );
			(int) getegid();

		finis


#ifndef	SYSVR_2	/*  grandfather clause -- cannot add new tests to SVR2  */

		testing("Setgid(svvs) with current effective gid of bin and real gid of svvs and an executable sgid bin.");
		testing("Expect rgid = GID, egid = EGID, return value of 0.");
		start

			expecting ( 0 );
			setgid(GID);

			expecting( GID );
			(int) getgid();

			expecting( EGID );
			(int) getegid();

		finis


		testing ("setgid (bin) with effective GID = real GID = svvs");
		testing ("should succeed since the saved GID = bin");
		testing ("resulting in the real GID = svvs, effective GID = bin");
		start

			expecting ( 0 );
			setgid(BIN_GID);

			expecting( GID );
			(int) getgid();

			expecting( BIN_EGID );
			(int) getegid();

		finis

#endif


	finis



	testing ("setgid (-1) should fail with EINVAL");
	start
		expecting (EINVAL);
		setgid (-1);
	finis



	done();
}
