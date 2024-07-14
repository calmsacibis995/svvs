#include	<sv_base.h> /*replacement macros */
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)putenv.c	1.2"



/*
** putenv
**	Test the putenv function.
*/
main()
{
# ifndef SYSVR_1

	char	*ret_buf;
	char	*new_buf;
	char	*old_buf;
	int	pid;
	int	wait_val;
	extern	char	*getenv();

setup("putenv");
funct("putenv");

testing("putenv of the guaranteed variables (PATH, TZ, TERM, and HOME)");
testing("should cause Getenv() to return the changed value.");
start
	/*
	** change PATH
	*/
	new_buf = "testpath";
	expecting (SUCCESS);
	putenv("PATH=testpath");

	ret_buf = getenv("PATH");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv did not set PATH correctly,\n\
returning '%s' instead of 'testpath'", ret_buf);

	/*
	** change TZ now
	*/
	new_buf = "testtz";
	expecting (SUCCESS);
	putenv("TZ=testtz");

	ret_buf = getenv("TZ");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv did not set TZ correctly,\n\
returning '%s' instead of 'testtz'", ret_buf);

	/*
	** change TERM now
	*/
	new_buf = "testterm";
	expecting (SUCCESS);
	putenv("TERM=testterm");
	ret_buf = getenv("TERM");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv did not set TERM correctly,\n\
returning '%s' instead of 'testterm'", ret_buf);

	/*
	** change HOME now
	*/
	new_buf = "testhome";
	expecting (SUCCESS);
	putenv("HOME=testhome");
	ret_buf = getenv("HOME");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv did not set HOME correctly,\n\
returning '%s' instead of 'testhome'", ret_buf);
finis


testing("putenv(PAT, T, TERMI and Home) should cause");
testing("no change in PATH, TZ, TERM, or HOME.");
start
	/*
	** test PAT
	*/
	expecting (SUCCESS);
	old_buf = getenv("PATH");
	putenv("PAT=badpath");
	new_buf = getenv("PATH");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(old_buf, new_buf) != 0 )
		error ("putenv set PATH for PAT incorrectly\n\
getenv returned '%s', then '%s'", old_buf, new_buf);


	/*
	** test T
	*/
	expecting (SUCCESS);
	old_buf = getenv("TZ");
	putenv("T=badtz");
	new_buf = getenv("TZ");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(old_buf, new_buf) != 0 )
		error ("putenv set TZ for T incorrectly\n\
getenv returned '%s', then '%s'", old_buf, new_buf);


	/*
	** test TERMI
	*/
	expecting (SUCCESS);
	old_buf = getenv("TERM");
	putenv("TERMI=badtermi");
	new_buf = getenv("TERM");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(old_buf, new_buf) != 0 )
		error ("putenv set TERM for TERMI incorrectly\n\
getenv returned '%s', then '%s'", old_buf, new_buf);


	/*
	** test Home 
	*/
	expecting (SUCCESS);
	old_buf = getenv("HOME");
	putenv("Home=badhome");
	new_buf = getenv("HOME");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(old_buf, new_buf) != 0 )
		error ("putenv set HOME for Home incorrectly\n\
getenv returned '%s', then '%s'", old_buf, new_buf);
finis


testing("putenv of a new variable should cause");
testing("getenv of this new variable to return the correct value.");
start
	/*
	** test new variable NEWENV
	*/
	new_buf = "newvarvalue";
	expecting (SUCCESS);
	putenv("NEWENV=newvarvalue");
	ret_buf = getenv("NEWENV");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv set the new environment variable NEWVAR to \n\
'%s', 'newvarvalue' was expected", ret_buf);
finis


testing("putenv of the guaranteed variables (PATH, TZ, TERM, and HOME)");
testing("when a child process is run should cause the");
testing("getenv() to return the changed value for the child process.");
start
	/*
	** change PATH
	*/
	new_buf = "testpath";

	expecting (SUCCESS);
	putenv("PATH=testpath");
	ret_buf = getenv("PATH");

	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv returned '%s', '%s' was expected",
			ret_buf, new_buf);
	
	/*
	** now run a child process 
	*/
	expecting (SUCCESS);
	pid = fork();
	if ( pid != 0 )  /*  parent waits for child, gets exit status  */
		wait_val = wait_for(pid, 5);
	else
	{
		/* child process */
		expecting (SUCCESS);
		ret_buf = getenv("PATH");
		expecting (VOID); /*  SUCCESS is not defined for strcmp  */
		if ( strcmp(ret_buf, new_buf) != 0 )
		{
			exit(1);
		}
		exit(0);
	}
	if ( wait_val )
		error ("putenv did not set PATH correctly for child");

	/*
	** change TZ now
	*/
	new_buf = "testtz";

	expecting (SUCCESS);
	putenv("TZ=testtz");
	ret_buf = getenv("TZ");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv returned '%s', '%s' was expected",
			ret_buf, new_buf);
		
	/*
	** now run a child process 
	*/
	expecting (SUCCESS);
	pid = fork();
	if ( pid != 0 )  /*  parent waits for child, gets exit status  */
		wait_val = wait_for(pid, 5);
	else
	{
		/* child process */
		expecting (SUCCESS);
		ret_buf = getenv("TZ");
		expecting (VOID);  /*  SUCCESS is not defined for strcmp  */
		if ( strcmp(ret_buf, new_buf) != 0 )
		{
			exit(1);
		}
		exit(0);
	}
	if ( wait_val )
		error ("putenv did not set TZ correctly for child");

	/*
	** change TERM now
	*/
	new_buf = "testterm";

	expecting (SUCCESS);
	putenv("TERM=testterm");
	ret_buf = getenv("TERM");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv returned '%s', '%s' was expected",
			ret_buf, new_buf);
	
	/*
	** now run a child process 
	*/
	expecting (SUCCESS);
	pid = fork();
	if ( pid != 0 )
		wait_val = wait_for(pid, 5);
	else
	{
		/* child process */
		expecting (SUCCESS);
		ret_buf = getenv("TERM");
		expecting (VOID);  /*  SUCCESS is not defined for strcmp  */
		if ( strcmp(ret_buf, new_buf) != 0 )
		{
			exit(1);
		}
		exit(0);
	}
	if ( wait_val )
		error ("putenv did not set TERM correctly for child");

	/*
	** change HOME now
	*/
	new_buf = "testhome";

	expecting (SUCCESS);
	putenv("HOME=testhome");
	ret_buf = getenv("HOME");
	expecting (VOID);	/*  SUCCESS is not defined for strcmp  */
	if ( strcmp(ret_buf, new_buf) != 0 )
		error ("putenv returned '%s', '%s' was expected",
			ret_buf, new_buf);
	
	/*
	** now run a child process 
	*/
	expecting (SUCCESS);
	pid = fork();
	if ( pid != 0 )
		wait_val = wait_for(pid, 5);
	else
	{
		/* child process */
		expecting (SUCCESS);
		ret_buf = getenv("HOME");
		expecting (VOID);  /*  SUCCESS is not defined for strcmp  */
		if ( strcmp(ret_buf, new_buf) != 0 )
		{
			exit(1);
		}
		exit(0);

	}
	if ( wait_val )
		error ("putenv did not set HOME correctly for child");
finis


done();
/*NOTREACHED*/
# else
	str_print("VOID:  putenv is an update included in System V Release 2.\n");
	exit(0);
# endif
}
