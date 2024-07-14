#include	<sv_base.h> /*replacement macros */
#include	<signal.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
# ifdef SEM
# include	<sys/types.h>
# include	<sys/ipc.h>
# include	<sys/sem.h>
# endif

#ident	"@(#)semop.c	1.7"


# ifdef	SEM
# define	ALLPERM	0666
# define	KEYVAL	2534

union Semun
{
	int			val;
	struct	semid_ds	*buf;
	ushort			*array;
};

typedef	union Semun	SEMUN;

#endif

/*
** semop
**	Test the semop routine.
*/
main()
{
#ifdef SEM
	int			ret_val;
	int			semid;
	int			semaphore;
	int			i;
	int			pid;
	int			wait_val;
	struct	sembuf		sop[1];
	struct	sembuf		*sops;
	SEMUN			arg;
	key_t			key;
	extern	int		handler();
	extern	char		*err_lookup ();
	long			sval = 0l;
	long			oldsval;


setup("semop");
funct("semop");

/*
 *  If the parent fails, then kill all the children
 *  that would otherwise wait forever for the semaphore to unblock
 */
abort_on (ERROR | SEVERR);

testing("Create a legal semaphore to use with semop.");
start
	key = (key_t) KEYVAL;

	expecting (SUCCESS);
	semid = semget(key,1,IPC_CREAT|ALLPERM);

	arg.val = 5;
	semctl(semid,0,SETVAL,arg);
finis

testing("semop with a negative sem_op whose absolute value");
testing("is equal to the semval");
testing("should return 0, with sem_op added to semval.");
start
	sop[0].sem_num = 0;
	sop[0].sem_op = -5;
	sop[0].sem_flg = 0;

	expecting (SUCCESS);
	semop(semid,sop,1);

	ret_val = semctl(semid,0,GETVAL,arg);
	if ( ret_val != 0 )
		error("%s\nsemval = %d, 0 was expected",
			"semop did not set semval correctly",
			ret_val);
finis


testing("semop with a negative sem_op whose absolute value");
testing("is equal to the semval, and has SEM_UNDO set");
testing("should return 0, with semop added to semval, and");
testing("the absolute value of sem_op added to semadj.");
start
	undo_test(semid,5,-5);
finis


testing("semop with -3 (a negative sem_op whose absolute value");
testing("is less than the semval of 5)");
testing("should return 2, with sem_op added to semval.");
start
	arg.val = 5;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);

	sop[0].sem_num = 0;
	sop[0].sem_op = -3;
	sop[0].sem_flg = 0;

	semop(semid,sop,1);

	ret_val = semctl(semid,0,GETVAL,arg);
	if ( ret_val != 2 )
		error("%s\nsemval = %d, 2 was expected",
			"semop did not set semval correctly",
			ret_val);
finis


testing("semop with a negative sem_op whose absolute value");
testing("is less then the semval, with SEM_UNDO set");
testing("should return 0, with semop added to semval,");
testing("and the absolute value of sem_op added to semadj.");
start
	undo_test(semid,5,-3);
finis


testing("semop with a positive sem_op.");
testing("should return 0, with the value of sem_op added to semval.");
start
	arg.val = 5;
	expecting (SUCCESS);
	ret_val = semctl(semid,0,SETVAL,arg);

	sop[0].sem_num = 0;
	sop[0].sem_op = 3;
	sop[0].sem_flg = 0;

	ret_val = semop(semid,sop,1);  /*  success means returning zero  */

	ret_val = semctl(semid,0,GETVAL,arg);
	/*  success means getting the semval  */
	if ( ret_val != 8 )
		error("%s\nsemval = %d, 8 was expected",
			"semval set to the wrong value",
			ret_val);
finis


testing("semop in a child process with a positive sem_op,");
testing("and with (sem_flg & SEM_UNDO) set");
testing("should return 0, with the value of sem_op added to the semadj value.");
start
	undo_test(semid,4,5);
finis


testing("semop with a sem_op of 0, and a semval of 0");
testing("should return 0.");
start
	arg.val = 0;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);
	
	sop[0].sem_num = 0;
	sop[0].sem_op = 0;
	sop[0].sem_flg = 0;

	semop(semid,sop,1);
finis


testing("semop with a sem_op of 0, a semval of !0, and ");
testing("(IPC_NOWAIT & sem_flg) set");
testing("should return -1, with an errno of EAGAIN.");
start
	arg.val = 7;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);
	
	sop[0].sem_num = 0;
	sop[0].sem_op = 0;
	sop[0].sem_flg = IPC_NOWAIT;

	expecting (-1);
	expecting (EAGAIN);
	semop(semid,sop,1);
finis


testing("Have semop in a child process, with a sem_op of 0, a semval");
testing("of !0, and (sem_flg & IPC_NOWAIT) not set");
testing("Then have the parent set semval to 0.");
testing("The child should wait until semval becomes 0, semzcnt");
testing("should be incremented until semval becomes 0.");
start
	arg.val = 7;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("could not fork");

	sop[0].sem_num = 0;
	sop[0].sem_op = 0;
	sop[0].sem_flg = 0;

	/*
	** The child process
	*/
	if ( pid == 0 )
	{
		expecting (VOID);  /*  let the parent process errors  */
		ret_val = semop(semid,sop,1);
		if ( ret_val )
			exit(ret_val);
		exit(0);
/*NOTREACHED*/
	}
	finis

	/*
	** The parent process
	*/
	expecting (VOID);
	sleep(5);
	chdir(".");
	chdir(".");

	/*
	** Check the count
	*/
	expecting (SUCCESS);
	ret_val = semctl(semid,0,GETZCNT,arg);

	if ( ret_val != 1 )
		error("%s\nsenzcnt = %d, 1 was expected",
			"semctl(GETZCNT) returned the wrong value",
			ret_val);

	arg.val = 0;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);

	expecting (VOID);  /*  wait returns the child's status */
	wait_val = wait_for(pid,5);
	if ( wait_val )
	{
		if ( (wait_val & 0377) )
			error("the child terminated abnormally, returning %o",
				wait_val);
		else
		{
			wait_val = (wait_val >> 8) & 0377;
			error("semop in the child process returned %d",
				wait_val);
		}
	}

	expecting (SUCCESS);
	ret_val = semctl(semid,0,GETZCNT,arg);
	if ( ret_val )
		error("semctl(GETZCNT) returned %d, 0 was expected",
			ret_val);
finis


testing("semop with a negative sem_op whose absolute value is");
testing("greater than semval, and with sem_flg & IPC_NOWAIT");
testing("should return -1, with an errno of EAGAIN.");
start
	arg.val = 3;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);

	sop[0].sem_num = 0;
	sop[0].sem_op = -5;
	sop[0].sem_flg = IPC_NOWAIT;
	sops = sop;

	expecting (-1);
	expecting (EAGAIN);
	semop(semid,sops,1);
finis


testing("Execute semop in a child process with a negative sem_op whose");
testing("absolute value is greater than semval. Then in the parent,");
testing("increment the semval.");
testing("This should return 0, semncnt should increment, and the process should");
testing("wait until semval becomes greater than the absolute value of sem_op.");
start
	arg.val = 3;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("could not fork");

	if ( pid == 0 )
	{
		sop[0].sem_num = 0;
		sop[0].sem_op = -5;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (VOID);
		ret_val = semop(semid,sops,1);
		if ( ret_val != 0 )
			exit(1);

		ret_val = semctl(semid,0,GETVAL,arg);
		if ( ret_val != 1 )
			exit(ret_val + 10);
		exit(0);
	}
	else
	{
		/*
		** The Parent process.
		** let the child go to sleep on the semaphore.
		*/
		for ( i = 0 ; i < 500 ; i++ )
			i = i;
		expecting (VOID);
		sleep(8);
		chdir(".");
		chdir(".");

		expecting (SUCCESS);
		ret_val = semctl(semid,0,GETVAL,arg);
		if ( ret_val != 3 )
			error("%s\nsemval = %d, 3 was expected",
				"semctl claims that the semval has changed",
				ret_val);

		ret_val = semctl(semid,0,GETNCNT,arg);
		if ( ret_val != 1 )
			error("%s\nsemncnt = %d, 1 was expected",
				"semctl(GETNCNT) did not return the right number",
				ret_val);

		/*
		** This should not cause the child to wake up.
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = 1;
		sop[0].sem_flg = 0;
		sops = sop;

		semop(semid,sops,1);

		/*
		** The child should wake up on this one.
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = 2;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (SUCCESS);
		semop(semid,sops,1);

		expecting (VOID);  /*  wait returns the child's status  */
		ret_val = wait_for(pid,5);
		if ( ret_val )
		{
			i = 0377 & (ret_val >> 8);

			if ( i == 1 )
				error("semop in the child returned -1");
			else
				error("%s\nsemval = %d",
			"child process woke up on the wrong value of semval",
			i - 10);
		}

	}
	finis

finis


testing("Execute semop in a child process with a negative sem_op whose");
testing("absolute value is greater than semval. Then in the parent,");
testing("increment the semval.");
testing("This should return 0, with the process to wait until semval");
testing("becomes equal than the absolute value of sem_op.");
start
	arg.val = 3;
	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("could not fork");

	if ( pid == 0 )
	{
		sop[0].sem_num = 0;
		sop[0].sem_op = -5;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (VOID);  /*  let the parent log the errors  */
		ret_val = semop(semid,sops,1);

		if ( ret_val != 0 )
			exit(1);

		ret_val = semctl(semid,0,GETVAL,arg);
		if ( ret_val != 0 )
			exit(ret_val + 10);
		exit(0);
	}
	else
	{
		/*
		** The Parent process.
		** let the child go to sleep on the semaphore.
		*/
		for ( i = 0 ; i < 500 ; i++ )
			i = i;
		expecting (VOID);
		sleep(8);
		chdir(".");
		chdir(".");

		expecting (SUCCESS);
		ret_val = semctl(semid,0,GETVAL,arg);
		if ( ret_val != 3 )
			error("%s\nsemval = %d, 3 was expected",
				"semctl claims that the semval has changed",
				ret_val);

		ret_val = semctl(semid,0,GETNCNT,arg);
		if ( ret_val != 1 )
			error("%s\nsemncnt = %d, 1 was expected",
				"semctl(GETNCNT) did not return the right number",
				ret_val);

		/*
		** This should not cause the child to wake up.
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = 1;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (SUCCESS);
		semop(semid,sops,1);

		/*
		** The child should wake up on this one.
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = 1;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (SUCCESS);
		semop(semid,sops,1);

		expecting (VOID);  /*  wait returns the child's status  */
		ret_val = wait_for(pid,5);
		if ( ret_val )
		{
			i = 0377 & (ret_val >> 8);

			if ( i == 1 )
				error("semop in the child returned -1");
			else
				error("%s\nsemval = %d",
			"child process woke up on the wrong value of semval",
			i - 10);
		}

	}
	finis
finis


/*  vstrcpy("Robustness level (2).");  */
testing("Passing an illegal semid to semop");
testing("should return -1, with an errno of EINVAL.");
start
	sop[0].sem_num = 0;
	sop[0].sem_op = 1;
	sop[0].sem_flg = 0;
	sops = sop;

	expecting (-1);
	expecting (EINVAL);
	semop(-1,sops,1);
finis


testing("Passing a sem_num less then 0");
testing("should return -1, with an errno of EFBIG.");
start
	sop[0].sem_num = -1;
	sop[0].sem_op = 1;
	sop[0].sem_flg = 0;
	sops = sop;

	expecting (-1);
	expecting (EFBIG);
	semop(semid,sops,1);
finis


testing("Passing a sem_num greater than the number in the set");
testing("should return -1, with an errno of EFBIG.");
start
	sop[0].sem_num = 55;
	sop[0].sem_op = 1;
	sop[0].sem_flg = 0;
	sops = sop;

	expecting (-1);
	expecting (EFBIG);
	semop(semid,sops,1);
finis


testing("In a child process, have semop with a negative sem_op");
testing("whose absolute value is greater than semval.");
testing("The parent process removing the semaphore");
testing("should return -1 from the child's semop, with errno set to EIDRM.");
start
	key = (key_t) (KEYVAL * 5);
	expecting (SUCCESS);
	semaphore = semget(key,1,IPC_CREAT|ALLPERM);

	arg.val = 3;
	semctl(semaphore,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/*
		** The child process
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = -5;
		sop[0].sem_flg = 0;
		sops = sop;

		/*
		** The child should go to sleep here.
		*/
		expecting (-1);
		expecting (EIDRM);
		semop(semaphore,sops,1);
		exit(0);
	}

	/*
	** The Parent process.
	** let the child go to sleep on the semaphore.
	*/
	for ( i = 0 ; i < 500 ; i++ )
		i = i;
	expecting (VOID);
	sleep(8);
	chdir(".");
	chdir(".");

	expecting (SUCCESS);
	semctl(semaphore,0,IPC_RMID,arg);

	expecting (SUCCESS);  /*  wait returns the child's status should be 0  */
	wait_for(pid,5);
	finis
finis


testing("In a child process, have semop with a sem_op equal to 0,");
testing("but have semval not equal to 0.");
testing("Having the parent process remove the semaphore");
testing("should return -1 from the child's semop, with errno set to EIDRM.");
start
	expecting (SUCCESS);
	key = (key_t) (KEYVAL * 5);
	semaphore = semget(key,1,IPC_CREAT|ALLPERM);

	arg.val = 3;
	semctl(semaphore,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/*
		** The child process
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = 0;
		sop[0].sem_flg = 0;
		sops = sop;

		/*
		** The child should go to sleep here.
		*/
		expecting (-1);
		expecting (EIDRM);
		semop(semaphore,sops,1);
		exit(0);
	}
	finis

	/*
	** The Parent process.
	** let the child go to sleep on the semaphore.
	*/
	for ( i = 0 ; i < 500 ; i++ )
		i = i;
	expecting (VOID);
	sleep(8);
	chdir(".");
	chdir(".");

	expecting (1);
	semctl(semaphore,0,GETZCNT,arg);

	expecting (SUCCESS);
	semctl(semaphore,0,IPC_RMID,arg);

	expecting (0);  /*  the child should return 0  */
	wait_for(pid,5);
finis


testing("In a child process, have semop with a negative sem_op whose");
testing("absolute value is greater than semval. Then have the parent");
testing("process send a signal to the process.");
testing("The value of semncnt should be incremented before the");
testing("the signal, the signal should be received by the child");
testing("in a normal fashion, and then the semncnt should be decremented.");
start
	expecting (SUCCESS);
	key = (key_t) (KEYVAL * 5);
	semaphore = semget(key,1,IPC_CREAT|ALLPERM);

	arg.val = 3;
	semctl(semaphore,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/*
		** The child process
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = -5;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (VOID);
		if ( signal(SIGINT,handler) == SIG_ERR )
			exit(1);

		/*
		** The child should go to sleep here.
		*/
		ret_val = semop(semaphore,sops,1);
		exit(2);
	}
	finis

	/*
	** The Parent process.
	** let the child go to sleep on the semaphore.
	*/
	for ( i = 0 ; i < 500 ; i++ )
		i = i;
	expecting (VOID);
	sleep(8);
	chdir(".");
	chdir(".");

	expecting (1);
	semctl(semaphore,0,GETNCNT,arg);

	expecting (SUCCESS);
	kill(pid,SIGINT);

	expecting (VOID);
	ret_val = wait_for(pid,5);
	/*
	** the child had some problem.
	*/
	if ( ret_val )
	{
		if ( (ret_val & 0377) )
			error("The child process died abnormally, stat = %o",
				ret_val);
		else
		{
			ret_val = (ret_val >> 8) & 0377;
			switch ( ret_val )
			{
			  case 1:
				error("Child process could not perform a signal");
				break;

			  case 2:
				error("signal not sent to child process, but semop returned");
				break;

			  default:
				ret_val -= 2;
				error("Child received the wrong signal of %d",
					ret_val);
				break;
			}
		}
	}

	expecting (SUCCESS);
	semctl(semaphore,0,IPC_RMID,arg);
finis


testing("In a child process, call semop with a sem_op equal to 0,");
testing("and have semval not equal to 0. Then have the parent");
testing("process send a signal to the process.");
testing("The value of semzcnt should be incremented before the");
testing("the signal, the signal should be received by the child");
testing("in a normal fashion, and then the semzcnt should be decremented.");
start
	key = (key_t) (KEYVAL * 5);

	expecting (SUCCESS);
	semaphore = semget(key,1,IPC_CREAT|ALLPERM);

	arg.val = 3;
	semctl(semaphore,0,SETVAL,arg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/*
		** The child process
		*/
		sop[0].sem_num = 0;
		sop[0].sem_op = 0;
		sop[0].sem_flg = 0;
		sops = sop;

		expecting (VOID);
		if ( signal(SIGINT,handler) == SIG_ERR )
			exit(1);

		/*
		** The child should go to sleep here.
		*/
		ret_val = semop(semaphore,sops,1);
		exit(2);
	}
	finis

	/*
	** The Parent process.
	** let the child go to sleep on the semaphore.
	*/
	for ( i = 0 ; i < 500 ; i++ )
		i = i;
	expecting (VOID);
	sleep(8);
	chdir(".");
	chdir(".");

	expecting (1);
	semctl(semaphore,0,GETZCNT,arg);

	expecting (SUCCESS);
	kill(pid,SIGINT);

	ret_val = wait_for(pid,5);
	/*
	** the child had some problem.
	*/
	if ( ret_val )
	{
		if ( (ret_val & 0377) )
			error("The child process died abnormally stat = %o",
				ret_val);
		else
		{
			ret_val = (ret_val >> 8) & 0377;
			switch ( ret_val )
			{
			  case 1:
				error("Child process could not perform a signal");
				break;

			  case 2:
				error("signal not sent to child process,\
 but semop returned");
				break;

			  default:
				ret_val -= 2;
				error("Child received the wrong signal of %d",
					ret_val);
				break;
			}
		}
	}

	expecting (SUCCESS);
	semctl(semaphore,0,IPC_RMID,arg);
finis


testing("Passing a sem_nops that is greater then");
testing("the number the system imposed limit");
testing("should return -1, with an errno of E2BIG.");
start
	sop[0].sem_num = 0;
	sop[0].sem_op = 4;
	sop[0].sem_flg = 0;
	sops = sop;

	expecting (-1);
	expecting (E2BIG);
	semop(semid,sops,INT_MAX);
finis

		
testing("overflowing the system imposed limit on semval");
testing("should return -1, with an errno of ERANGE.");
start
	sop[0].sem_num = 0;
	sop[0].sem_op = 0;
	sop[0].sem_flg = 0;

	expecting (VOID);
	oldsval = (long)semctl(semid,0,GETVAL,arg);
	ret_val = 0;

	expecting (VOID);
	for ( i = 0 ; i < 1000 && !ret_val ; i++ )
	{
		errno = 0;
		sop[0].sem_op = 1;
		if( i>3 && !(i%2) ) sop[0].sem_op = sval-1;
		ret_val = semop(semid,sop,1);
		if (ret_val) break;

		sval = (long) semctl(semid,0,GETVAL,arg);
		if(sval != (oldsval+(long)sop[0].sem_op))
		{
			continue_on (ERROR);  /*  continue for cleanup  */
			error ("%s\nsemval = %ld, %ld was exepcted",
				"semop set semval incorrectly",
				sval, (oldsval+(long)sop[0].sem_op));
			goto rolled_over;
		}
		oldsval = sval;
	}

	if ( ret_val != -1 )
		error("semop never returned -1, test ended with ret_val = %d",
			ret_val);
	else
		if ( errno != ERANGE )
			error ("%s\nerrno = %s, ERANGE was expected",
				"semop set errno wrong",
				err_lookup (errno));

rolled_over:
	/*
	** Attempt to remove the semaphore.
	*/
	expecting (SUCCESS);
	semctl(semid,0,IPC_RMID,arg);
finis


done();
/*NOTREACHED*/
}

/*
** undo_test
**	Create a child process, have the child do a semop,
**	see if undo worked.
*/
undo_test(semid,initial,sem_op)
int	semid;
int	initial;
int	sem_op;
{
	int			ret_val;
	int			pid;
	struct	sembuf		sop[1];
	struct	sembuf		*sops;
	SEMUN			arg;


	arg.val = initial;

	expecting (SUCCESS);
	semctl(semid,0,SETVAL,arg);
		
	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("could not fork");

	if ( pid != 0 )
		{
		expecting (VOID);
		ret_val = wait_for(pid,5);
		}
	else
	{
		sop[0].sem_num = 0;
		sop[0].sem_op = sem_op;
		sop[0].sem_flg = SEM_UNDO;
		sops = sop;

		expecting (SUCCESS);
		semop(semid,sops,1);

		ret_val = semctl(semid,0,GETVAL,arg);
		if ( ret_val != (sem_op + initial) )
			error("%s\nsemval = %d, %d was exepcted",
				"semop did not set semval correctly",
				ret_val, (sem_op + initial));
		exit (0);
/*NOTREACHED*/

	}
	finis

	if ( (ret_val & 0177) )
		error("child died abnormally, ret_val = %o",ret_val);

	if ( ret_val )
		error("child did not work correctly, returning %o",ret_val);
	else
	{
		expecting (SUCCESS);
		ret_val = semctl(semid,0,GETVAL,arg);
		if ( ret_val != initial )
			error("s\nsemval = %d, %d was exepcted",
			"the semadj value was not correctly added to semval",
			ret_val,initial);
	}

		
}

/*
** handler
**	Simple handler for signals. We just exit with the signal
**	value, if it is not SIGINT.
*/
int
handler(sig)
int	sig;
{
	if ( sig == SIGINT )
		exit(0);

	exit(sig + 2);

#else

	void exit();
	
	unsupported ("Semaphore subsystem not a supported kernel extension.");
	exit(0);


#endif
}
