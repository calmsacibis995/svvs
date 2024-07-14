#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)setuid1.c	1.4"

/*
** setuid1
**	Test the setuid function.
**
**	NOTE: Each test case
**	sets up the effective and real uid for the next test case.
**	
**	Make sure that this binary is setuid bin.
**
*/

main()
{
	unsigned short	getuid();
	unsigned short	geteuid();
	int	setuid();

	setup("setuid");

	funct("setuid");

	abstract("Step through various combinations of setuid calls.");
	abstract("Each test sets up the environment for the next test.");
	start
		break_on ( ERROR );

		testing("Setuid(svvs) with current effective uid of bin and real uid of svvs and an executable suid bin.");
		testing("Expect ruid = UID, euid = EUID, return value of 0.");
		start

			expecting ( 0 );
			setuid(UID);

			expecting( UID );
			getuid();

			expecting( EUID );
			geteuid();

		finis

		testing("Setuid(bin) with current effective and real uid of svvs.");
		testing("Expect ruid = UID, euid = BIN_EUID, return value of 0.");
		start

			expecting ( 0 );
			setuid(BIN_UID);

			expecting( UID );
			getuid();

			expecting( BIN_EUID );
			geteuid();

		finis

		testing("Setuid(ROOT_UID)...which is illegal for this process.");
		testing("Return value of -1, and errno of EPERM. Uid's should be unchanged.");
		start
			expecting( EPERM );
			setuid(ROOT_UID);

			expecting( UID );
			getuid();

			expecting( BIN_EUID );
			geteuid();

		finis


	finis

	testing ("setuid (UID_MAX) should fail with EINVAL");
	start
		expecting (EINVAL);
		setuid (UID_MAX);
	finis



	testing ("setuid (-1) should fail with EINVAL");
	start
		expecting (EINVAL);
		setuid (-1);
	finis



	done();
	/*NOTREACHED*/
}
