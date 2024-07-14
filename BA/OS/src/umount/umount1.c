#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)umount1.c	1.4"

/*
** umount
**	Test the umount function.
**	test for non-root user, try to get back EPERM.
*/

# define	READ_ONLY	1

main()
{
	int	expected;
	int	ret_val;
	int 	pid, wpid, sts;

	setup("umount");

	funct("umount");

	exit_on ( ERROR );

	abstract("An illegal invocation of umount by a non-super-user.");
	abstract("Mount file system, have a non-root user attempt umount,");
	abstract("then umount file system.");
	abstract("Successful mount and umount, failed umount for non-root user");
	abstract("with return value of -1 and errno of EPERM.");
	start


		prep("Unmount SVVS_FS if it's mounted.");
		prep("Mount SVVS_FS.");
		prep("Fork a child who will do the testing.");
		start
			expecting ( VOID );
			umount(SVVS_FS); /* insurance */

			expecting ( SUCCESS );
			ret_val = mount(SVVS_FS, MOUNT_POINT, READ_ONLY);

			expecting ( SUCCESS );
			pid=fork();
		finis

			if ( pid == 0 );
			{
				prep("Change user id to non-root.");
				start
					expecting ( SUCCESS );
					setuid(UID);
				finis

				testing("Child issues the umount.");
				start
				expecting( EPERM );
				umount(SVVS_FS);
				finis

				done();
				/*NOTREACHED*/
			}

			testing("Parent waits for child process to exit.");
			start
				expecting ( VOID );
				wpid = wait(&sts);
			finis

			cleanup("Unmount SVVS_FS.");
			start
				expecting ( SUCCESS );
				umount(SVVS_FS);

			finis

			if ( wpid != pid )
			{
				severr( "umount: wait() terminated abnormally" );
			}

			if ( sts != 0 )
			{
				error( "umount: child process terminated abnormally");
			}

	finis

	done();
	/*NOTREACHED*/
}
