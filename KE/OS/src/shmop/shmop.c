#include	<sv_base.h> /*replacement macros */
#include	<fcntl.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
# ifdef	SHM
# include	<sys/types.h>
# include	<sys/stat.h>

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

# include	<sys/ipc.h>
# include	<sys/shm.h>
# endif

#ident	"@(#)shmop.c	1.8"


# define	ALLPERM		0666
# define	KEYVAL		4321
# define	SIZE		1025
# define	UID1		((UID) + 1)
# define	UID2		((UID) + 2)
# define	BADRET		((char *) -1)

extern	char	*err_lookup ();

/*
** shmop
**	Test the shmat, and shmdt routines.
*/
main()
{
#ifdef SHM
setup("shmop");

funct ("shmat");
start
	t_shmat();
finis

funct ("shmdt");
start
	t_shmdt();
finis



done();
/*NOTREACHED*/
}

/*
** t_shmat
**	Test the shared memory attach routine.
*/
t_shmat()
{
	char		*ret_val;
	char		*shmaddr;
	char		*buffer;
	char		*ptr;
	int		i;
	int		fd;
	int		shmid;
	int		first, second;
	int		wait_val;
	int		val;
	key_t		key;
	char		*str;
	char		errstr[STRLEN];

	extern	char	*shmat();

/*  vstrcpy("Normal level (0).");  */
testing("Create the shared memory segment we will need to use.");
/*  testing("A return != -1 from shmget.");  */
start
	/*  abort if this fails  */
	exit_on (ERROR);

	key = (key_t) KEYVAL;
	expecting (SUCCESS);
	shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);
finis


testing("Attach to the segment.");
/*  testing("A return != -1, and a segment to suddenly appear.");  */
start
	expecting (SUCCESS);
	ret_val = shmat(shmid,(char *) 0,0);

	shmaddr = ret_val;
finis


testing("Accessing the segment");
testing("should allow read/write on the segment.");
start
	ptr = shmaddr;
	for ( i = 0 ; i < SIZE ; i++ )
		ptr[i] = 'a';
	for ( i = 0 ; i < SIZE ; i++ )
		if ( ptr[i] != 'a' )
		{
			error ("incorrect value in the segment, ptr [%d] = %c",
				i, ptr[i]);
		}

	expecting (SUCCESS);
	shmdt(shmaddr);		/*  detach the segment */

	shmctl(shmid,IPC_RMID,0);  /*  remove the segment  */
finis


testing("Use two children of different uids to access a segment.");
testing("Note that the segment will not be available to the parent");
testing("until after the children have all returned.");
testing("Both children should see the changes.");
start
	prep ("Fork a subtest.");
	start

	expecting (SUCCESS);
	first = fork();
	if ( first == -1 )
		error ("can not fork and make the first child");

	/*
	** The child process
	*/
	if ( first == 0 )
	{
		expecting (SUCCESS);
		setuid(UID1);

		key = (key_t) KEYVAL;
		shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);

		shmaddr = shmat(shmid,(char *) 0,0);

		shmaddr[0] = 'H';
		shmaddr[1] = 'i';
		shmaddr[2] = '\0';

		expecting (VOID);
		sleep(2);

		i = 0;
		for (;;)
		{
			expecting (VOID);  /* strcmp never fails!   */
			if ( strcmp(shmaddr,"Hi") != 0 )
				break;
			i++;
			if ( i > 500 )
				break;
			sleep(2);
		}
		if ( i > 500 )
		{
			error ("the shared memory never changed from 'Hi'");
			exit(3);
		}

		expecting (VOID);
		if ( strcmp(shmaddr,"Bye") )
		{
			error ("%s\nshmaddr = '%s', 'Bye' was expected",
"the shared memory was not changed to the correct string", shmaddr);
		}
		exit(0);
	}  /*  end of child  */

	finis


	prep ("Fork a subtest.");
	start

	expecting (SUCCESS);
	second = fork();
	if ( second == -1 )
		error ("can not fork and make the second child");

	/*
	** The second child process
	*/
	if ( second == 0 )
	{
		expecting (SUCCESS);
		setuid(UID2);

		key = (key_t) KEYVAL;
		shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);

		shmaddr = shmat(shmid,(char *) 0,0);

		expecting (VOID);
		sleep(2);
		i = 0;
		for (;;)
		{
			expecting (VOID);	/*  strcmp never fails! */
			if ( strcmp(shmaddr,"Hi") == 0 )
				break;
			i++;
			if ( i > 500 )
				break;
			sleep(2);
		}
		if ( i > 500 )
		{
			error ("the shared memory never changed from 'Hi'");
		}
		shmaddr[0] = 'B';
		shmaddr[1] = 'y';
		shmaddr[2] = 'e';
		shmaddr[3] = '\0';

		expecting (VOID);
		shmdt(shmaddr);
		exit(0);
	}  /*  end of child  */

	finis

	/*
	** wait for a child to finish.
	*/
	expecting (VOID);
	wait_val = wait(&val);
	if ( wait_val == -1 )
		error ("wait returned -1");

	if ( wait_val == first )
		str = "first child";
	else
		if ( wait_val == second )
			str = "second child";
		else
			error ("The first wait returned an unexpected child");

	if ( val )
	{
		if ( (val & 0277) )
		{
			error ("%s died abnormally, status 0%o",str, val);
		}
		/*****  this is no longer needed  *****
		 * else
		 *	set_exit(ERROR);
		 *****/
	}

	/*
	** wait for the other child
	*/
	expecting (VOID);
	wait_val = wait(&val);
	if ( wait_val == -1 )
		error ("wait returned -1");

	if ( wait_val == first )
		str = "first child";
	else
		if ( wait_val == second )
			str = "second child";
		else
			error ("The second wait returned an unexpected child");

	if ( val )
	{
		if ( (val & 0277) )
		{
			error ("%s died abnormally, status 0%o",str, val);
		}
		/*****  this is no longer needed  *****
		 * else
		 *	set_exit(ERROR);
		 *****/
	}

	expecting (VOID);  /*  use a more descriptive error message  */
	key = (key_t) KEYVAL;
	shmid = shmget(key,SIZE,ALLPERM);
	if ( shmid == -1 )
	{
		error("%s\n%s\nerrno = %s",
			"shmget returned -1 when the parent tried to get the",
			"segment the children were working with", err_lookup (errno));
	}

	shmaddr = shmat(shmid,(char *) 0, 0);
	if ( shmaddr == (char *) -1 )
	{
		error ("%s\n%s\nerrno = %s",
"shmat returned -1 when the parent tried to attach to the",
"segment the children were working with", err_lookup(errno));
	}

	expecting (VOID);  /*  strcmp never fails  */
	if ( strcmp(shmaddr,"Bye") )
		error ("shmaddr = '%s', 'Bye' was expected", shmaddr);

	expecting (SUCCESS);
	shmdt(shmaddr);
	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
finis


testing("attach to a given shmaddr.");
/*  testing("A return of shmaddr.");  */
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);
	key = (key_t) (KEYVAL+1);
	shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);

	buffer = shmat(shmid,(char *)0,0);
	/*  explicit error checking
	 *  if (buffer == BADRET) 
	 *	error("shmat for buffer returned -1",ERROR,0,0,0);
	 */
	shmdt( buffer );
	shmaddr = shmat(shmid,buffer,0);
	/*  explicit error checking
	 *  if ( shmaddr == BADRET )
	 *	errorp("shmat returned -1",ERROR,buffer,shmaddr,0);
	 */
	if ( shmaddr != buffer )
		error ("shmaddr returned the wrong pointer");

	expecting (VOID);
	shmdt(shmaddr);

	expecting (SUCCESS);  /*  remove the segment  */
	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
finis


testing("attach to a given (shmaddr - (shmaddr modulos SHMLBA)).");
/*  testing("A return of shmaddr.");  */
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);

	key = (key_t) (KEYVAL+1);
	shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);

	shmaddr = shmat(shmid,buffer,SHM_RND);
	/*  explicit error checking
	 *  if ( shmaddr == BADRET )
	 *	errorp("shmaddr returned -1",ERROR,ptr,shmaddr,0);
	 */

	expecting (VOID);
	shmdt(shmaddr);

	expecting (SUCCESS);  /*  remove the segment  */
	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
finis


/*  vstrcpy("Robustness level (2).");  */
testing("shmat on an invalid identifier should fail with EINVAL.");
start
	expecting (BADRET);
	expecting (EINVAL);
	shmat(-1,(char *) 0,0);
finis


testing("child with a different uid attaching to a segment");
testing("it has no permissions on should fail with EACCES.");
start
	expecting (SUCCESS);
	key = (key_t) KEYVAL;
	shmid = shmget(key,SIZE,IPC_CREAT);

	prep ("Fork a subtest.");
	start

	first = fork();
	if ( first == -1 )
		error ("can not fork");

	if ( first == 0 )
	{
		setuid(UID1);

		expecting (BADRET);
		expecting (EACCES);
		shmaddr = shmat(shmid,(char *) 0,0);
		exit(0);
	}

	finis


	/*
	** The parent
	*/
	val = wait_for(first,5);
	if ( val )
	{
		if ( (val & 0277) )
			error("child died abnormally status = 0%o", val);
		/*****  this is no longer needed  *****
		 *  else
		 *	set_exit(ERROR);
		 *****/
	}

	expecting (SUCCESS);  /*  remove the segment  */
	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
finis


testing("Attaching more than the system imposed limit number of segments");
testing("should at some point fail with errno set to EMFILE.");
start
	expecting (SUCCESS);
	shmid = shmget(key,4,IPC_CREAT|ALLPERM);
	
	/*  make a file of shmids for later removal  */
	fd = temp_open("shmget.id",O_RDWR | O_TRUNC | O_CREAT, 0664);

	ret_val = (char *) 0;
	i = 0;

	expecting (VOID);  /*  the shmat should fail eventually  */
	do {
		ret_val = shmat(shmid,(char *) 0,0);
		if ( ret_val == BADRET )
			break;
		if(write(fd,&ret_val,sizeof(char *))!=sizeof(char *))
			error ("can not write to temp file");
	} while(++i);
	
	continue_on (ERROR);  /*  gotta clean up after errors  */
	if ( ret_val != BADRET )
		error ("shmat never returned -1, last value = %d", ret_val);
	else
		if ( errno != EMFILE )
			error ("shmat set errno = %s, EMFILE was expected",
				err_lookup (errno));

	/*  delete all the segments  */
	expecting (SUCCESS);
	lseek(fd,0L,SEEK_SET);

	/*  break_on (ERROR);  add this if needed  */
	expecting (VOID);  /*  the loop ends when the read fails  */
	while(read(fd,&shmaddr,sizeof(char *))==sizeof(char *))
	{
		expecting (SUCCESS);
		shmdt(shmaddr);

		expecting (VOID);  /*  the loop ends when the read fails  */
	}

	expecting (SUCCESS);  /*  remove the segment  */
	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);

	close(fd);
finis

}

/*
** t_shmdt
**	Test the detach routine.
*/
t_shmdt()
{
	int		ret_val;
	char		*shmaddr;
	int		shmid;
	key_t		key;
	extern	char	*shmat();

/*  vstrcpy("Normal level (0).");  */
testing("Create, attach then detach from a segment.");
/*  testing("Returns of 0, or valid segments.");  */
start
	break_on (ERROR | SEVERR);

	expecting (SUCCESS);
	key = (key_t) KEYVAL;
	shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);

	shmaddr = shmat(shmid,(char *) 0,0);

	ret_val = shmdt(shmaddr);
	/*****  the z_funct should test for this  *****
	 *  if ( ret_val )
	 *	error("shmdt did not return 0",ERROR,0,ret_val,0);
	 *****/

	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
finis


/*  vstrcpy("Robustness level (2).");  */
testing("detaching a segment that is not attached should fail with EINVAL.");
start
	expecting (BADRET);
	expecting (EINVAL);
	shmdt((char *) 0);
finis


testing("Using shmdt to detach a segment, but with a shmaddr that");
testing("is within a segment, not the beginning");
testing("should fail with EINVAL.");
start
	expecting (SUCCESS);
	key = (key_t) KEYVAL;
	shmid = shmget(key,SIZE,IPC_CREAT|ALLPERM);

	shmaddr = shmat(shmid,(char *) 0,0);

	expecting (BADRET);
	expecting (EINVAL);
	shmdt( &shmaddr[4] );

	expecting (SUCCESS);  /*  remove the segment  */
	shmdt( shmaddr );
	shmctl(shmid,IPC_RMID,(struct shmid_ds *) 0);
finis

#else
	void exit();
	
	unsupported ("Shared memory subsystem not supported.");
	exit(0);

#endif
}
