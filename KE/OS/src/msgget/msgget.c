#include	<sv_base.h> /*replacement macros */
#include	<fcntl.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
# ifdef MSG
# include	<sys/types.h>
# include	<sys/ipc.h>
# include	<sys/msg.h>
# endif

# define	TESTKEY		5
# define 	MAXQUEUE	500
# define	SOMEPERMS	0666

#ident	"@(#)msgget.c	1.6"


/*
** msgget
**	Test the msgget function.
*/
main()
{
# ifdef MSG
	int		ret_val;
	int		i;
	int		msqid1;
	int		msqid2;
	int		msqid3[MAXQUEUE];
	int		reached_limit;
	int		last_queue;
	int		msgflg;
	key_t		key;
	struct	msqid_ds	buffer;
	struct	msqid_ds	*buf = &buffer;

	extern	FILE	*temp_fopen();
	extern	char	*err_lookup ();

setup("msgget");
funct("msgget");

testing("Create a message queue.  Check that all the variables associated");
testing("with this queue are properly set");
testing("by returning a valid msqid_ds structure.");
start
	/*
	 *  if the first msgget fails, expect all others to fail
	 */
	exit_on (ERROR);
	key = (key_t)TESTKEY;
	msgflg = IPC_CREAT | SOMEPERMS;

	expecting (SUCCESS);
	msqid1 = msgget(key, msgflg);

	expecting (0);
	ret_val = msgctl(msqid1, IPC_STAT, buf);

	/*
	 *  report all errors
	 */
	continue_on (ERROR);

	if ( buf->msg_qnum != 0 )
		error ("msgget set msg_qnum incorrectly to %d, 0 was exepected",
			buf->msg_qnum);
	if ( buf->msg_lspid != 0 )
		warning ("%s\nmsgget  set msg_lspid to %d, 0 was expected",
			ADVISORY, buf->msg_lspid);
	if ( buf->msg_lrpid != 0 )
		warning ("%s\nmsgget  set msg_lrpid to %d, 0 was expected",
			ADVISORY, buf->msg_lrpid);
	if ( buf->msg_stime != 0 )
		error ("msgget set msg_stime to %d, 0 was expected",
			buf->msg_stime);
	if ( buf->msg_rtime != 0 )
		error ("msgget set msg_rtime to %d, 0 was expected",
			buf->msg_rtime);
	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	if ( buf->msg_perm.uid != getuid() )
		error ("msgget set msg_perm.uid to %d, %d was expected",
			buf->msg_perm.uid, getuid());
	if ( buf->msg_perm.gid != getgid() )
		error ("msgget set msg_perm.gid to %d, %d was expected",
			buf->msg_perm.gid, getgid());
	if ( buf->msg_perm.cuid != getuid() )
		error ("msgget set msg_perm.cuid to %d, %d was expected",
			buf->msg_perm.cuid, getuid());
	if ( buf->msg_perm.cgid != getgid() )
		error ("msgget set msg_perm.cgid to %d, %d was expected",
			buf->msg_perm.cgid, getgid());
finis


testing("Open all available file descriptors. Create a message queue");
testing("using msgget should be able to create a message queue.");
start
	for ( i = 3; i < OPEN_MAX; i++ )
	{
		expecting (SUCCESS);
		temp_fopen("tmpmsgget", "w");
	}
	key = (key_t)(TESTKEY + 1);
	msgflg = IPC_CREAT;	/* NO read permission */

	expecting (SUCCESS);
	msqid2 = msgget(key, msgflg);

	for ( i = 3; i < OPEN_MAX; i++ )
	{
		close(i);
	}
finis


testing("Doing a msgget on an existing message queue on which we do");
testing("not have read permission");
testing("should return -1 with an errno of EACCES.");
start
	expecting (-1);
	expecting (EACCES);
	ret_val = msgget((key_t)(TESTKEY+1), SOMEPERMS);
finis


testing("Doing a msgget on a nonexisting message queue");
testing("should return -1 with an errno of ENOENT.");
start
	expecting (-1);
	expecting (ENOENT);
	msgget((key_t)(TESTKEY+2), SOMEPERMS);
finis


testing("Asking for an existing message queue but with msgflg &");
testing("(IPC_CREAT | IPC_EXCL) true");
testing("should return -1 with an errno of EEXIST.");
start
	expecting (-1);
	expecting (EEXIST);
	msgget((key_t)(TESTKEY+1), IPC_CREAT | IPC_EXCL | 0777);
finis


testing("Asking for an existing message queue identifier");
testing("should cause all the correct variables to be set.");
start
	msgflg = SOMEPERMS;

	expecting (SUCCESS);
	msgget((key_t)TESTKEY, msgflg);

	expecting (0);
	msgctl(msqid1, IPC_STAT, buf);

	/*
	 *  report all errors
	 */
	continue_on (ERROR);

	if ( buf->msg_qnum != 0 )
		error ("msgget set msg_qnum to %d, 0 was expected",
			buf->msg_qnum);
	if ( buf->msg_lspid != 0 )
		warning ("%s\nmsg_lspid = %d, 0 was expected",
			ADVISORY, buf->msg_lspid);
	if ( buf->msg_lrpid != 0 )
		warning ("%s\nmsg_lrpid = %d, 0 was expected",
			ADVISORY, buf->msg_lrpid);
	if ( buf->msg_stime != 0 )
		error ("msgget set msg_stime to %d, 0 was expected",
			buf->msg_stime);
	if ( buf->msg_rtime != 0 )
		error("msgget set msg_rtime to %d, 0 was expected",
			buf->msg_rtime);
	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	if ( buf->msg_perm.uid != getuid() )
		error ("msgget set msg_perm.uid to %d, %d was expected",
			buf->msg_perm.uid, getuid());
	if ( buf->msg_perm.gid != getgid() )
		error ("msgget set msg_perm.gid to %d, %d was expected",
			buf->msg_perm.gid, getgid());
	if ( buf->msg_perm.cuid != getuid() )
		error ("msgget set msg_perm.cuid to %d, %d was expected",
			buf->msg_perm.cuid, getuid());
	if ( buf->msg_perm.cgid != getgid() )
		error ("msgget set msg_perm.cgid to %d, %d was expected",
			buf->msg_perm.cgid, getgid());
finis


testing("Creating more message queues than the system allows");
testing("should return of -1 with an errno of ENOSPC.");
start
	key = (key_t)(TESTKEY + 2);
	msgflg = IPC_CREAT;	/* NO read permission */
	reached_limit = 0;

	expecting (VOID);
	for ( i = 0; i < MAXQUEUE; i++ )
	{
		key = key + i;
		if ( (msqid3[i] = msgget(key, msgflg)) == -1 )
		{
			reached_limit = 1;
			last_queue = i + 1;
			break;
		}
		last_queue = 500;
	}

	if ( reached_limit != 1 )
	{
		error("Created %d message queues and never reached\n\
the system limit for message queues", MAXQUEUE);
	}
	else
	{
		if ( errno != ENOSPC )
			error ("Incorrect errno from msgget = %s,\
 ENOSPC was expected.", err_lookup (errno));
	}

	expecting (VOID);
	for ( i = 0; i < last_queue; i++ )
		msgctl(msqid3[i], IPC_RMID, buf);

	msgctl(msqid1, IPC_RMID, buf);
	msgctl(msqid2, IPC_RMID, buf);
finis

done();
/*NOTREACHED*/

#else
	void exit();
	
	unsupported ("Message subsystem not a supported kernel extension.");
	exit(0);
/*NOTREACHED*/


# endif
}
