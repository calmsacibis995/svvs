#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
# ifdef MSG
# include	<sys/types.h>
# include	<sys/ipc.h>
# include	<sys/msg.h>
# endif

# define	TESTKEY		5
# define	MSGSIZE		10000
# define	ALLPERM		0777
# define	SOMEPERM	0666
# define	NOPERM		00

int		handler();
extern	char	*err_lookup ();

#ident	"@(#)msgop.c	1.8"


/*
** msgop
**	Test the msgop function.
*/
main()
{
# ifdef MSG
	int		ret_val;
	int		msqid1;
	int		msgflg;
	int		msgsz;
	int		msglength;
	int		pid;
	int		i;
	long		msgtyp;

	ushort		oldmsg_qnum;
	ushort		save_qbytes;

	key_t		key;

	struct	msqid_ds	buffer;
	struct	msqid_ds	*buf = &buffer;
	struct	message
	{
		long	mtype;
		char	mtext[MSGSIZE];
	};
	struct	message	msg_data;
	struct	message	*msgp = &msg_data;

setup("msgop");
funct("msgsnd");


testing ("create a message queue");
start
	/*
	 *  If this fails then give up
	 */
	exit_on (ERROR);

	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);
finis


testing("Using msgsnd with an illegal msqid");
testing("should return -1 with an errno of EINVAL.");
start
	msgp->mtype = 1;

	expecting (VOID);
	strcpy(msgp->mtext,"");
	msgsz = MSGSIZE;
	msgflg = ALLPERM;

	expecting (-1);
	expecting (EINVAL);
	msgsnd(msqid1+1, msgp, msgsz, msgflg);
finis


testing("Using msgsnd with an illegal mtype");
testing("should return -1 with an errno of EINVAL.");
start
	msgp->mtype = -1;

	expecting (VOID);
	strcpy(msgp->mtext,"");
	msgsz = MSGSIZE;
	msgflg = ALLPERM;

	expecting (-1);
	expecting (EINVAL);
	msgsnd(msqid1, msgp, msgsz, msgflg);
finis


testing("Using msgsnd with an unsendable message(IPC_NOWAIT bit set)");
testing("should return -1 with an errno of EAGAIN.");
start
	msgp->mtype = 1;

	expecting (SUCCESS);
	msgctl(msqid1, IPC_STAT, buf);
	msgsz = (int)buf->msg_qbytes / 4;
	msgsz += 4;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgflg = IPC_NOWAIT;
	for ( i = 0 ; i < 10 ; i++ )
	{
		expecting (VOID);
		if ( (ret_val = msgsnd(msqid1, msgp, msgsz, msgflg)) != 0 )
			break;
	}
	if ( ret_val == 0 )
		error("Msgsnd should have failed with an unsendable message.");
	else
		if ( errno != EAGAIN )
			error ("errno = %s, EAGAIN was expected", 
				err_lookup (errno) );
finis


testing("Using msgsnd with a msgsz of less than zero");
testing("should return -1 with an errno of EINVAL.");
start
	msgp->mtype = 1;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgsz = -1;
	msgflg = ALLPERM | MSG_NOERROR;

	expecting (-1);
	expecting (EINVAL);
	msgsnd(msqid1, msgp, msgsz, msgflg);
finis


testing("With the IPC_NOWAIT bit cleared, msgsnd with msgsz greater than");
testing("msg_qbytes.  Check that the process sleeps until msqid is removed");
testing("from the system.");
testing("This should return -1 from msgsnd with an errno of EIDRM.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/* The child process */
		msgp->mtype = 1;
		expecting (VOID);
		strcpy(msgp->mtext,"This is a test!");

		expecting (SUCCESS);
		msgctl(msqid1, IPC_STAT, buf);
		msgsz = (int)buf->msg_qbytes / 4;
		msgsz += 4;
		msgflg = ALLPERM; 	/* IPC_NOWAIT will clear */

		/* The child should go to sleep here. */
		for ( i = 0 ; i < 10 ; i++ )
		{
			expecting (VOID);
			if ( (ret_val = msgsnd(msqid1, msgp, msgsz, msgflg)) != 0 )
				break;
		}
		if ( ret_val != -1 )
			exit(1);
/*NOTREACHED*/
		else
			if ( errno != EIDRM )
				exit(errno + 2);
		exit(0);
	}
	finis

	/*
	** The Parent process.
	** Wait a few seconds, and let the child
	** go to sleep on the msgsnd.
	*/
	for ( i = 0 ; i < 500 ; i++ )
		i = i;
	expecting (VOID);
	sleep(3);
	chdir(".");
	chdir(".");

	expecting (SUCCESS);
	msgctl(msqid1, IPC_RMID, buf);

	expecting (VOID);
	ret_val = wait_for(pid,5);
	/*
	** the child had some problem.
	*/
	if ( ret_val )
	{
		if ( (ret_val & 0277) )
			error("The child process died abnormally returning %o",
				ret_val);
		else
		{
			ret_val = (ret_val >> 8) & 0277;
			if ( ret_val == 1 )
				error("msgop in the child did not return -1");
			else
					error("msgop in the child set errno \
incorrectly to %s", err_lookup (ret_val - 2));
			}
		}
finis


testing("With the IPC_NOWAIT bit cleared, msgsnd with msgsz greater than");
testing("msg_qbytes.  Check that the process sleeps until a signal is");
testing("received by the child.");
testing("The child should continue upon receipt of a signal.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		expecting (VOID);
		/* The child process */
		if ( signal(SIGINT,handler) == SIG_ERR )
			exit(1);

		msgp->mtype = 1;
		strcpy(msgp->mtext,"This is a test!");

		expecting (SUCCESS);
		msgctl(msqid1, IPC_STAT, buf);
		msgsz = (int)buf->msg_qbytes / 4;
		msgsz += 4;
		msgflg = ALLPERM; 	/* IPC_NOWAIT will clear */

		/* The child should go to sleep here. */
		for ( i = 0 ; i < 20 ; i++ )
		{
			expecting (VOID);
			if ( (ret_val = msgsnd(msqid1, msgp, msgsz, msgflg)) != 0 )
				break;
		}

		if ( ret_val != -1 )
			exit(1);
		else
			if ( errno != EIDRM )
				exit(errno + 2);
		exit(0);
	}
	else
	{
		/*
		** The Parent process.
		** Wait a few seconds, and let the child
		** go to sleep on the msgsnd.
		*/
		for ( i = 0 ; i < 500 ; i++ )
			i = i;
		expecting (VOID);
		sleep(2);
		chdir(".");
		chdir(".");

		/******** SEND SIGNAL *******/
		kill(pid,SIGINT);

		expecting (VOID);
		ret_val = wait_for(pid,5);
		/*
		** the child had some problem.
		*/
		if ( ret_val )
		{
			if ( (ret_val & 0277) )
				error("The child process died abnormally \
returning %o", ret_val);
			else
			{
				ret_val = (ret_val >> 8) & 0277;
				if ( ret_val == 1 )
					error("msgop in the child did not \
return -1");
			}
		}

		expecting (0);
		msgctl(msqid1, IPC_RMID, buf);
	}

	finis
finis


testing("Using msgsnd, send a legal message to a legal queue.");
testing("The associated variables should be appropriately updated.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);
	msgp->mtype = 1;

	expecting (SUCCESS);
	msgctl(msqid1, IPC_STAT, buf);
	save_qbytes = buf->msg_qbytes;
	oldmsg_qnum = buf->msg_qnum;
	msgsz = (int)buf->msg_qbytes / 4;
	msgsz += 4;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgflg = SOMEPERM;

	expecting (0);
	msgsnd(msqid1, msgp, msgsz, msgflg);

	msgctl(msqid1, IPC_STAT, buf);

	if ( buf->msg_qnum != (oldmsg_qnum + 1) )
		error("%s\nmsg_qnum = %d, %d was expected",
			"Upon a msgsnd the msg_qnum did not get incremented.",
			buf->msg_qnum, (oldmsg_qnum+1));

	if ( buf->msg_lspid != getpid() )
		warning ("%s\nmsg_lpid = %d, %d was exepected",
			ADVISORY, buf->msg_lspid, getpid());

	expecting (0);
	msgctl(msqid1, IPC_RMID, buf);
finis


funct("msgrcv");
testing("Using msgrcv with the IPC_NOWAIT set and with an illegal msqid");
testing("should return -1 with an errno of EINVAL.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	msgp->mtype = 1;
	msgsz = MSGSIZE;
	msgtyp = 0;
	msgflg = IPC_NOWAIT | ALLPERM;

	expecting (-1);
	expecting (EINVAL);
	msgrcv(msqid1+1, msgp, msgsz, msgtyp, msgflg);

	expecting (0);
	msgctl(msqid1, IPC_RMID, buf);
finis


testing("Using msgrcv with the IPC_NOWAIT set try to read a message");
testing("with no read permission");
testing("should return -1 with an errno of EACCES");
start
	key = TESTKEY;
	msgflg = IPC_CREAT | ALLPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	expecting (SUCCESS);
	msgctl(msqid1, IPC_STAT, buf);
	msgsz = (int)buf->msg_qbytes / 4;
	msgsz += 4;

	msgp->mtype = 1;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgflg = NOPERM;

	expecting (SUCCESS);
	msgsnd(msqid1, msgp, msgsz, msgflg);

	buf->msg_perm.mode = NOPERM;

	expecting (SUCCESS);
	msgctl(msqid1, IPC_SET, buf);
	msgp->mtype = 1;
	msgsz = MSGSIZE;
	msgtyp = 0;
	msgflg = IPC_NOWAIT;

	expecting (-1);
	expecting (EACCES);
	msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);

	expecting (SUCCESS);
	msgctl(msqid1, IPC_RMID, buf);
finis


testing("Using msgrcv with the IPC_NOWAIT set and with an illegal msgsz");
testing("should return -1 with an errno of EINVAL.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (VOID);
	if ( (msqid1 = msgget(key, msgflg)) == -1 )
		error("Can not create a message queue.");

	expecting (SUCCESS);
	msgctl(msqid1, IPC_STAT, buf);
	msgsz = (int)buf->msg_qbytes / 4;
	msgsz += 4;

	msgp->mtype = 1;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgflg = SOMEPERM | MSG_NOERROR; 

	expecting (SUCCESS);
	msgsnd(msqid1, msgp, msgsz, msgflg);

	msgp->mtype = 1;
	msgsz = -1;
	msgtyp = 0;
	msgflg = MSG_NOERROR | IPC_NOWAIT | SOMEPERM;

	expecting (-1);
	expecting (EINVAL);
	msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);

	expecting (SUCCESS);
	msgctl(msqid1, IPC_RMID, buf);
finis


testing("Using msgrcv with the IPC_NOWAIT set try to read a message");
testing("which is longer than msgsz");
testing("should return -1 with an errno of E2BIG");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|ALLPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);
	msgp->mtype = 1;
	msgsz = save_qbytes / 4;
	msgsz += 4;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgflg = ALLPERM;

	expecting (SUCCESS);
	msgsnd(msqid1, msgp, msgsz, msgflg);

	msgp->mtype = 1;
	msgsz = 5;
	msgtyp = 0;
	msgflg = IPC_NOWAIT | ALLPERM;

	expecting (-1);
	expecting (E2BIG);
	msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);

	expecting (SUCCESS);
	msgctl(msqid1, IPC_RMID, buf);
finis


testing("Using msgrcv with the IPC_NOWAIT set try to read a message");
testing("when there are none in the message queue");
testing("should return -1 with an errno of ENOMSG.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|ALLPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);
	msgp->mtype = 1;
	msgsz = MSGSIZE;
	msgtyp = 0;
	msgflg = IPC_NOWAIT | ALLPERM;

	expecting (-1);
	expecting (ENOMSG);
	msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);

	expecting (SUCCESS);
	msgctl(msqid1, IPC_RMID, buf);
finis


testing("With the IPC_NOWAIT bit cleared, msgrcv with no message");
testing("pending in the queue. Then removing the queue in the parent");
testing("should return -1 from msgrcv with an errno of EIDRM.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/* The child process */

		/* The child should go to sleep here. */
		msgtyp = 0;
		msgsz = MSGSIZE;
		msgp->mtype = 1;
		msgflg = ALLPERM; 	/* IPC_NOWAIT will clear */

		expecting (VOID);
		ret_val = msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);
		if ( ret_val != -1 )
			exit(1);
		else
			if ( errno != EIDRM )
				exit(errno + 2);
		exit(0);
	}
	else
	{
		/*
		** The Parent process.
		** Wait a few seconds, and let the child
		** go to sleep on the msgsnd.
		*/
		for ( i = 0 ; i < 500 ; i++ )
			i = i;

		expecting (VOID);
		sleep(3);
		chdir(".");
		chdir(".");

		ret_val = msgctl(msqid1, IPC_RMID, buf);
		if ( ret_val != 0 )
			error("msgctl(IPC_RMID) returned %d, 0 was expected",
				ret_val);

		expecting (VOID);
		ret_val = wait_for(pid,5);
		/*
		** the child had some problem.
		*/
		if ( ret_val )
		{
			if ( (ret_val & 0277) )
				error("The child process died abnormally \
returning %o",ret_val);
			else
			{
				ret_val = (ret_val >> 8) & 0277;
				if ( ret_val == 1 )
					error("msgop in the child did not \
return -1");
				else
					error("msgop in the child set errno \
incorrectly to %s", err_lookup (ret_val - 2));
			}
		}
	}
	finis

finis


testing("With the IPC_NOWAIT bit cleared, msgrcv with no message");
testing("pending.  Check that the process sleeps until a signal is");
testing("received by the child.");
testing("The child should continue upon receipt of a signal.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		expecting (VOID);
		/* The child process */
		if ( signal(SIGINT,handler) == SIG_ERR )
			exit(1);

		/* The child should go to sleep here. */
		msgtyp = 0;
		msgsz = MSGSIZE;
		msgp->mtype = 1;
		msgflg = ALLPERM; 	/* IPC_NOWAIT will clear */

		expecting (VOID);
		ret_val = msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);
		if ( ret_val != -1 )
			exit(1);
		else
			if ( errno != EIDRM )
				exit(errno + 2);
		exit(0);
	}
	else
	{
		/*
		** The Parent process.
		** Wait a few seconds, and let the child
		** go to sleep on the msgsnd.
		*/
		for ( i = 0 ; i < 500 ; i++ )
			i = i;
		expecting (VOID);
		sleep(2);
		chdir(".");
		chdir(".");

		/******** SEND SIGNAL *******/
		expecting (SUCCESS);
		kill(pid,SIGINT);

		expecting (VOID);
		ret_val = wait_for(pid,5);
		/*
		** the child had some problem.
		*/
		if ( ret_val )
		{
			if ( (ret_val & 0277) )
				error("The child process died abnormally \
returning %o", ret_val);
			else
			{
				ret_val = (ret_val >> 8) & 0277;
				if ( ret_val == 1 )
					error("msgop in the child did not \
return -1");
			}
		}

		expecting (SUCCESS);
		msgctl(msqid1, IPC_RMID, buf);
	}
	finis

finis


testing("Using msgsnd, send a legal message to a legal queue.");
testing("Then read the message using msgrcv.");
testing("The associated variables should be appropriately updated.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);
	msgp->mtype = 1;
	msgctl(msqid1, IPC_STAT, buf);
	save_qbytes = buf->msg_qbytes;
	oldmsg_qnum = buf->msg_qnum;
	msgsz = (int)buf->msg_qbytes / 4;
	msgsz += 4;
	msglength = msgsz;

	expecting (VOID);
	strcpy(msgp->mtext,"This is a test!");
	msgflg = SOMEPERM;

	expecting (SUCCESS);
	msgsnd(msqid1, msgp, msgsz, msgflg);

	msgctl(msqid1, IPC_STAT, buf);
	if ( buf->msg_qnum != (oldmsg_qnum + 1) )
		error("%s\nmsg_qnum = %d, %d was expected",
			"Upon a msgsnd the msg_qnum did not get incremented.",
			buf->msg_qnum, oldmsg_qnum+1);

	if ( buf->msg_lspid != getpid() )
		warning ("%s\nmsg_lspid = %d, %d was expected",
			ADVISORY, buf->msg_lspid, getpid());
	msgtyp = 0;
	msgsz = MSGSIZE;
	msgp->mtype = 1;
	msgflg = ALLPERM; 	/* IPC_NOWAIT will clear */

	expecting (VOID);
	if ( (ret_val = msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg)) == msglength )
	{
		expecting (SUCCESS);
		msgctl(msqid1, IPC_STAT, buf);
		if ( buf->msg_qnum != oldmsg_qnum )
			error("%s\nmsg_qnum = %d, %d was expected",
			  "Upon a msgrcv the msg_qnum did not get decremented.",
			  buf->msg_qnum, oldmsg_qnum);

		if ( buf->msg_lspid != getpid() )
			warning ("%s\nmsg_lspid = %d, %d was expected",
				ADVISORY, buf->msg_lspid, getpid());
	}
	else
		error("%s\n%s  %d, %d was expected",
		  "Msgrcv failed to read a message from a legal message queue",
		  "with a message pending, returning ",
		  ret_val, msglength);

	expecting (SUCCESS);
	msgctl(msqid1, IPC_RMID, buf);
finis


testing("msgrcv with no message pending.  Check that the");
testing("process sleeps until a message of the correct type is");
testing("in the message queue.");
testing("The child should continue upon receipt of a message.");
start
	key = TESTKEY;
	msgflg = IPC_CREAT|SOMEPERM;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");
	if ( pid == 0 )
	{
		/* The child should go to sleep here. */
		msgtyp = 0;
		msgsz = MSGSIZE;
		msgp->mtype = 1;
		msgflg = ALLPERM; 	/* IPC_NOWAIT will clear */

		expecting (VOID);
		ret_val = msgrcv(msqid1, msgp, msgsz, msgtyp, msgflg);
		if ( ret_val == -1 )
			exit(1);
		exit(0);
	}
	else
	{
		/*
		** The Parent process.
		** Wait a few seconds, and let the child
		** go to sleep on the msgsnd.
		*/
		for ( i = 0 ; i < 500 ; i++ )
			i = i;
		expecting (VOID);
		sleep(3);
		chdir(".");
		chdir(".");

		msgp->mtype = 1;

		expecting (SUCCESS);
		msgctl(msqid1, IPC_STAT, buf);
		msgsz = (int)buf->msg_qbytes / 4;
		msgsz += 4;

		expecting (VOID);
		strcpy(msgp->mtext,"This is a test!");
		msgflg = SOMEPERM;

		expecting (SUCCESS);
		msgsnd(msqid1, msgp, msgsz, msgflg);

		/*
		** the child had some problem.
		*/
		expecting (VOID);
		if ( ((ret_val = wait_for(pid,5)) & 0277 ) )
			error("The child process died abnormally \
returning %o", ret_val);

		expecting (SUCCESS);
		msgctl(msqid1, IPC_RMID, buf);
	}
	finis

finis


done();
/*NOTREACHED*/
}

/*
** handler
**	Simple handler for signals. We just exit with the signal
**	value, if it is not SIGINT.
*/
handler(sig)
int	sig;
{
	if ( sig == SIGINT )
		exit(0);

	exit(sig + 2);
# else
	void exit();
	
	unsupported ("Message subsystem not a supported kernel extension.");
	exit(0);

#endif
}
