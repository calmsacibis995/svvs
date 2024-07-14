#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)setpgrp.c	1.3"

/*
** setpgrp
**	Test the setpgrp function.
**
** Testing this function is extremely difficult since an ID is a dynamically
** assigned variable issued by the kernel.
**
** Strategy
**	Get the old pid and pgid. Issue a setpgrp and
**	look at the results.
**
**
*/

main()
{
	int	ret_val;
	int	expected;

	int	old_pid, new_pgid;
	int	setpgrp();

	setup("setpgrp");

	funct("setpgrp");

	exit_on ( ERROR );

	testing("Will check that process group ID gets changed to process ID.");
	testing("Setpgrp changes the process group ID.");
	start
		expected = 1;
		expecting ( VOID );
		ret_val = getpid();
		old_pid = ret_val;

		if ((ret_val <= expected) || (ret_val > PID_MAX))
			warning("%s\npid less than 1 or greater than PID_MAX",ADVISORY);

		expected = 1;
		expecting( VOID );
		ret_val = getpgrp();

		if (ret_val <= expected)
			warning("%s\ngetpgrp returned a value less than 1",ADVISORY);

		expected =  old_pid;
		expecting( VOID );
		ret_val = setpgrp();
		new_pgid = ret_val;

		if ( old_pid != ret_val )
			warning("%s\nsetpgrp did not return pid",ADVISORY);

	finis

	testing("Verify new pgid is set by getpgrp");
	testing("Getpgrp will return the correct value.");
	start

		expecting( VOID );
		ret_val = getpgrp();

		if ( new_pgid != ret_val )
			warning("%s\ngetpgrp did not return the same value as setpgrp",ADVISORY);

	finis

	done();
	/*NOTREACHED*/
}
