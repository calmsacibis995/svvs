#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)setuid0.c	1.4"

/*
** setuid0
**	Test the setuid function.
**
**	NOTE: Each test case
**	sets up the effective and real uid for the next test case.
**	
**	Make sure that this binary is setuid root.
**
*/

main()
{
	unsigned short	getuid();
	unsigned short	geteuid();
	int	setuid();

	setup("setuid");

	funct("setuid");
	exit_on (ERROR);


	testing ("check that the real UID is svvs, effective UID = root");
	start
		expecting (UID);
		getuid ();

		expecting (ROOT_EUID);
		geteuid ();
	finis



	abstract("Step through various combinations of setuid calls.");
	abstract("Each test sets up the environment for the next test.");
	start
		testing("Setuid(root) current euid = root, ruid= svvs.");
		testing("Expect ruid = ROOT_UID, euid = ROOT_EUID, return value of 0.");
		start

			expecting ( 0 );
			setuid(ROOT_UID);

			expecting( ROOT_UID );
			getuid();

			expecting( ROOT_EUID );
			geteuid();

		finis

		testing("Setuid(svvs) current euid = ROOT_EUID, ruid = ROOT_UID.");
		testing("Expect ruid = UID, euid = EUID, return value of 0.");
		start

			expecting ( 0 );
			setuid(UID);

			expecting( UID );
			getuid();

			expecting( EUID );
			geteuid();

		finis


		testing ("setuid (bin) should fail");
		testing ("leaving the real and effective UID unchanged.");
		start
			expecting (EPERM);
			setuid (BIN_UID);

			expecting( UID );
			getuid();

			expecting( EUID );
			geteuid();

		finis


		testing ("setuid (root) should fail");
		testing ("leaving the real and effective UID unchanged.");
		start
			/*
			 *  This will succeed if the saved-user-id
			 *  was not reset
			 */
			expecting (EPERM);
			setuid (ROOT_UID);

			expecting( UID );
			getuid();

			expecting( EUID );
			geteuid();

		finis



	finis
	done();
	/*NOTREACHED*/
}
