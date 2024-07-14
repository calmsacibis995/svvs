#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifdef ACCT
#include	<sys/acct.h>
#endif

# define	WAITTIME	5
# define	USERID		99
# define	ACCFK_EX	0
# define	MAXNUM		1000

#ident	"@(#)acct.c	1.4"


char		PathPointer[STRLEN];

/*
** acct
**	Test the acct function.
*/
main()
{
#ifdef ACCT
	long		t;
	int		ret_val;
	struct	acct	test_acct;
	struct	acct	*acct_ptr;
	int		process_id;
	int		i;
	unsigned short	ret_id;
	FILE		*read_stream;
	FILE		*write_stream;

	extern	char		*temp_path();
	extern	FILE		*temp_fopen();
	extern	unsigned short	getuid();
	extern	unsigned short	getgid();
	extern	long		time();
	extern	char		*err_lookup ();

setup("acct");
funct("acct");

testing("Turn the accounting off.");
start
	expecting (SUCCESS);
	write_stream = temp_fopen("tmpacct", "w");
	fclose(write_stream);

	expecting (VOID);
	acct( (char *)0);
finis


testing("Using acct, turn accounting on when it has already been turned on.");
testing("This should return -1, and an errno of EBUSY.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "tmpacct");

	expecting (SUCCESS);
	acct(PathPointer);

	expecting (-1);
	expecting (EBUSY);
	acct(PathPointer);
finis


testing("acct of a path with a nondirectory element ");
testing("should return -1, and an errno of ENOTDIR.");
start
	expecting (VOID);
	if ( (ret_val = acct( (char *)0)) != 0 )
		error("Could not turn accounting off, acct returned %d",
			ret_val);
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "tmpacct/garbage");

	expecting (-1);
	expecting (ENOTDIR);
	acct(PathPointer);
finis


testing("acct of a path with a nonexistent element");
testing("should return -1, and an errno of ENOENT.");
start
	expecting (VOID);
	if ( (ret_val = acct( (char *)0)) != 0 )
		error("Could not turn accounting off, acct returned %d",
			ret_val);
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "garbage");

	expecting (-1);
	expecting (ENOENT);
	acct(PathPointer);
finis


testing("acct of a path which ends in a special file");
testing("should return -1, and an errno of EACCES.");
start
	expecting (VOID);
	if ( (ret_val = acct( (char *)0)) != 0 )
		error("Could not turn accounting off, acct returned %d",
			ret_val);
	strcpy(PathPointer, "/dev/null");

	expecting (-1);
	expecting (EACCES);
	acct(PathPointer);
finis


testing("acct of a path which ends in a directory");
testing("should return -1, and an errno of EACCES.");
start
	expecting (VOID);
	if ( (ret_val = acct( (char *)0)) != 0 )
		error("Could not turn accounting off, acct returned %d",
			ret_val);
	strcpy(PathPointer, temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;

	expecting (VOID);
	if ((ret_val = acct(PathPointer)) != (-1))
		error("Acct returned %d, -1 was expected", ret_val);
	else
		if ( errno != EACCES)
			if (errno == EISDIR)
				warning ("%s\nacct returned errno EISDIR",
					ADVISORY);
			else
				error ("Incorrect errno from acct = %s",
					err_lookup (errno));
finis


testing("Using acct, turn on accounting.  Then do a fork() followed by a");
testing("exec() followed by exit(0) followed by fork() by exec() by exit(1).");
testing("Appropriate entries to the acct structure should be written to the ");
testing("accounting file.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "tmpacct");
	if ( (ret_val = acct(PathPointer)) != 0 )
		error("Could not turn accounting on, acct returned %d",
			ret_val);

	process_id = fork();
	if ( process_id == -1 )
		error ("can not fork");

	if ( process_id == 0 )
	{
		execl("./acct1","acct1",(char *)0);
		error ("can not find program acct1");
	}
	if ( (wait_for(process_id, WAITTIME)) )
		error ("child process 1 exited abnormally");

	acct_ptr = &test_acct;
	if ( (read_stream = temp_fopen("tmpacct", "r")) == NULL )
		error ("can not open the temp file acct");

	while ( fread((char *) acct_ptr, 1, sizeof(test_acct), read_stream) == sizeof(test_acct) )
	{
		if (strncmp("acct1",acct_ptr->ac_comm,8))
			continue;

		if ( acct_ptr->ac_flag != ACCFK_EX )
			error("%s\nac_flag was %d, %d was expected",
"Acct did not set the accounting flag correctly after child 1",
acct_ptr->ac_flag, ACCFK_EX);

		if ( acct_ptr->ac_stat != 0 )
			error("%s\nac_stat = %d, 0 was expected",
"Acct did not set the Exit status correctly after child 1",
acct_ptr->ac_stat);

		ret_id = getuid();
		if ( acct_ptr->ac_uid != getuid() )
			error("%s\nac_uid = %d, %d was expected",
"Acct did not set the user id correctly after child 1",
acct_ptr->ac_uid, ret_id);

		ret_id = getgid();
		if ( acct_ptr->ac_gid != ret_id )
			error("%s\nac_gid = %d, %d was expected",
"Acct did not set the group id correctly after child 1",
acct_ptr->ac_gid, ret_id);

		goto happy1;
	}
	if (!feof(read_stream)||ferror(read_stream))
		error ("Fread failed reading from the accounting file after child 1");
	else
		error ("Acct did not enter 'acct1' into acct file");

happy1:
	if ( fclose(read_stream) )
		error ("can not close the temp file acct after child 1");

	/*
	** fork one more time
	*/
	process_id = fork();
	if ( process_id == -1 )
		error ("can not fork");

	if ( process_id != 0 )
	{
		if ( (wait_for(process_id, WAITTIME)) != 0x100 )
			error ("child process 2 exited abnormally");
	}
	else
	{
		t = time((long *)0) + 2l;
		while( t > time((long *)0));
		exit(1);
/*NOTREACHED*/
	}

	expecting (VOID);
	acct_ptr = &test_acct;
	if ( (read_stream = temp_fopen("tmpacct", "r")) == NULL )
		error ("can not open the temp file acct");

	while ( fread((char *)acct_ptr, 1, sizeof(test_acct), read_stream) == sizeof(test_acct) )
	{
		if (strncmp("acct",acct_ptr->ac_comm,8))
			continue;

		if ( acct_ptr->ac_flag != AFORK )
			error("%s\nac_flag = %d, %d was expected",
"Acct did not set the accounting flag correctly after child 2",
acct_ptr->ac_flag, AFORK);
	
		if ( acct_ptr->ac_stat )
			error("%s\nac_stat = %d, 0 was expected",
"Acct did not set the Exit status correctly after child 2",
acct_ptr->ac_stat);

		ret_id = getuid();
		if ( acct_ptr->ac_uid != getuid() )
			error("%s\nac_uid = %d, %d was expected",
"Acct did not set the user id correctly after child 2",
acct_ptr->ac_uid, ret_id);
	
		ret_id = getgid();
		if ( acct_ptr->ac_gid != ret_id )
			error("%s\nac_gid = %d, %d was expected",
"Acct did not set the group id correctly after child 2",
acct_ptr->ac_gid, ret_id);

		goto happy2;
	}
	if(!feof(read_stream)||ferror(read_stream))
		error ("Fread failed reading from the accounting file after child 2");
	else
		error ("Acct did not enter 'acct' into acct file");

happy2:
	if ( fclose(read_stream) )
		error ("can not close the temp file acct");

	/*
	** Do yet another fork
	*/
	process_id = fork();
	if ( process_id == -1 )
		error ("can not fork");

	if ( process_id == 0 )
	{
		execl("./acct2","acct2",(char *)0);
		error ("can not find program acct2");
	}
	if ( (wait_for(process_id, WAITTIME)) != 0x200 )
	{
		error ("child process 3 exited abnormally");
	}
	acct_ptr = &test_acct;
	if ( (read_stream = temp_fopen("tmpacct", "r")) == NULL )
		error ("can not open the temp file acct");
	while ( fread((char *)acct_ptr, 1, sizeof(test_acct), read_stream) == sizeof(test_acct) )
	{
		if (strncmp("acct2",acct_ptr->ac_comm,8))
			continue;
		if ( acct_ptr->ac_flag != ASU )
			error("%s\nac_flag = %d, %d was expected",
"Acct did not set the accounting flag correctly after child 3",
acct_ptr->ac_flag, ASU);

		if ( acct_ptr->ac_stat != 0 )
			error("%s\nac_stat = %d, 0 was expected",
"Acct did not set the Exit status correctly after child 3",
acct_ptr->ac_stat);

		ret_id = getuid();
		if ( acct_ptr->ac_uid != getuid() )
			error("%s\nac_uid = %d, %d was expected",
"Acct did not set the user id correctly after child 3",
acct_ptr->ac_uid, ret_id);

		ret_id = getgid();
		if ( acct_ptr->ac_gid != ret_id )
			error("%s\nac_gid = %d, %d was expected",
"Acct did not set the group id correctly after child 3",
acct_ptr->ac_gid, ret_id);

		goto happy3;
	}
	if(!feof(read_stream)||ferror(read_stream))
		error ("Fread failed reading from the accounting file after child 3");
	else
		error ("Acct did not enter 'acct2' into acct file");

happy3:
	if ( fclose(read_stream) != 0 )
		error ("Can not close the temp file tmpacct");
finis


testing("Using acct, turn on accounting when not a super user.");
testing("should return of -l and errno of EPERM.");
start
	if ( (ret_val = acct( (char *)0)) != 0 )
		error("Could not turn accounting off, acct returned %d",
			ret_val);

	expecting (0);
	setuid(USERID);

	expecting (VOID);
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "tmpacct");

	expecting (-1);
	expecting (EPERM);
	acct(PathPointer);
finis


testing("Using acct, turn off accounting when not a super user");
testing("should return -l and errno of EPERM.");
start
	expecting (-1);
	expecting (EPERM);
	acct( (char *)0);
finis


done();
/*NOTREACHED*/

#else
	char *ptr;
	int i;
	void ioerror();
	void exit();
	
	unsupported("Kernel extension not supported on this system");
	exit(0);

#endif
}
