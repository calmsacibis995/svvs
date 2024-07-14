#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifdef PLOCK
#include	<sys/lock.h>
# endif

#ident	"@(#)plock.c	1.4"


/*
** plock
**	Test the plock function.
*/
main()
{
# ifdef PLOCK
	int		ret_val;
	int		op;
	int		procid1;

setup("plock");
funct("plock");


testing("plock with op of text lock should return 0.");
start
	op = TXTLOCK;
	expecting (0);
	plock(op);
finis


testing("plock with op of data lock should return 0.");
start
	op = DATLOCK;
	expecting (0);
	plock(op);
finis



testing("plock with op of text lock for a second time");
testing("should return -1 with an errno of EINVAL.");
start
	op = TXTLOCK;
	expecting (-1);
	expecting (EINVAL);
	plock(op);
finis


testing("plock with op of data lock for a second time");
testing("should return -1 with an errno of EINVAL.");
start
	op = DATLOCK;
	expecting (-1);
	expecting (EINVAL);
	plock(op);
finis


testing("plock with op of process lock");
testing("should return -1 with an errno of EINVAL.");
start
	op = PROCLOCK;
	expecting (-1);
	expecting (EINVAL);
	plock(op);
finis


testing("plock with op of release all locks should return 0.");
start
	op = UNLOCK;
	expecting (0);
	plock(op);

finis


testing("plock with op of release all locks for a second time");
testing("should return -1 with an errno of EINVAL.");
start
	op = UNLOCK;
	expecting (-1);
	expecting (EINVAL);
	plock(op);
finis


testing("plock with op of process lock.");
testing("should return 0.");
start
	op = PROCLOCK;
	expecting (0);
	plock(op);
finis


testing("plock with op of text lock");
testing("should return -1 with an errno of EINVAL.");
start
	op = TXTLOCK;
	expecting (-1);
	expecting (EINVAL);
	plock(op);
finis


testing("plock with op of data lock");
testing("should return -1 with an errno of EINVAL.");
start
	op = DATLOCK;
	expecting (-1);
	expecting (EINVAL);
	plock(op);
finis


testing("Using plock, run a child after locking the parent process.");
testing("In the child process unlocking all locks");
testing("should return -1 with an errno of EINVAL.");
start
	expecting (VOID);
	plock(UNLOCK);

	expecting (SUCCESS);
	plock(PROCLOCK);

	prep ("Fork a subtest.");
	start

	expecting (SUCCESS);
	procid1 = fork();
	if ( procid1 == -1 )
		error ("Could not fork from plock()");

	if ( procid1 == 0 )
	{
		/* CHILD process */
		expecting (-1);
		expecting (EINVAL);
		plock(UNLOCK);

		done();
/*NOTREACHED*/
	}
	else
	{
		if ( wait_for(procid1, 5) )
			error ("The child terminated abnormally.");
	}
	finis
finis

done();
/*NOTREACHED*/

# else
	void exit();
	
	unsupported("Process locking is not supported.");
	exit(0);

# endif

}
