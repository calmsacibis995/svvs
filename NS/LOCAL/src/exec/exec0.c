#include	<sv_base.h>
#include	<sys/types.h>
#include	<sys/times.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	WAIT			5
# define	PARNT_MASK		02
# define	KNOWN_FILDES	7

#ident	"@(#)exec0.c	1.7"

char *dollar_path;
char *os_bin_path;
char *exec1_path;
char *exec2_path;
char *exec3_path;
char *exec4_path;
char *exec5_path;
char *exec7_path;
char *enoent_path;
char *enotdir_path;
char *no_r_path;
char *no_x_path;
char *bad_bin_path;
char *tmp_path;

int execl();
int execv();
int execle();
int execve();
int execlp();
int execvp();

/*
 *  test all the forms of exec.
 *  The calling sequence is as follows:
 *	main
 *		map_modes
 *		do_init
 *		do_test
 *			texec
 *				use_time
 *				do_exec
 *				do_2big_exec
 *		do_free
 */


main(argc, argv, envp)
int	argc;
char	*argv[];
char	*envp[];
{

	setup("exec");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( SEVERR );

	do_init( envp );

	do_test( execl, "execl", envp);
	do_test( execv, "execv", envp);
	do_test( execle, "execle", envp);
	do_test( execve, "execve", envp);
	do_test( execlp, "execlp", envp);
	do_test( execvp, "execvp", envp);

	do_free();

	done();
	/*NOTREACHED*/
}
do_test( fn, xname, envp )
int		(*fn)();
char 	*xname;
char	**envp;
{
	funct( xname );
	abstract("Test %s.", xname);
	start
		texec( fn, xname, envp );
	finis
}
texec(fn, xname, envp)
int		(*fn)();
char 	*xname;
char	**envp;
{

	abstract("Fork a child process.  The child process will consume");
	abstract("some user and system time, to generate values for ");
	abstract("tms_utime and tms_stime.  The child process then forks");
	abstract("a grandchild process, which consumes some user and system");
	abstract("time to generate tms_cutime and tms_cstime values for the");
	abstract("child process.  The child process then calls exec, and the");
	abstract("parent waits.");
	start
		int pid;
		if( !(pid = fork()) ) {

			use_time();
			prep("Fork a grandchild process to generate values for");
			prep("cutime and cstime.  Wait for the grandchild process");
			prep("to complete.");
			start
				exit_on( ERROR );
				if( !(pid = fork()) ) {
					use_time();			
					exit( 0 );
				}
				if ( ( wait_for(pid, WAIT)) )
					error("grand child process terminated abnormally");
			finis
			prep("Set up parent environment.");
			start
				exit_on( ERROR );
				prep("Set an alarm in the parent.  The alarm is expected to");
				prep("remain in effect in the new process after the exec.");
				start
					expecting ( 0 );
					alarm(1000);
				finis
				prep("Call chdir to change to the temp directory.");
				start
					chdir( tmp_path );
				finis
			finis
			testing("Call %s.", xname );
			start
				char *pth = exec1_path;
				exit_on( ERROR );
				if( fn == execlp || fn == execvp )
					pth = "exec1";
				do_exec( fn, pth, envp );
				done();		/* just in case */
			finis
		}
		if ( ( wait_for(pid, WAIT)) )
			error("child process terminated abnormally");
	finis

	testing("Call %s with a path to a setuid root file.  Verify the",
	  xname );
	testing("real and effective user id's of the new process are");
	testing("set to svvs and root respectively.");
	start
		prep("Fork a child process to call %s, and wait for the child",
		  xname);
		prep("to terminate.");
		start
			int pid;
			exit_on( ERROR );
			if( !(pid = fork()) ) {
				do_exec( fn, exec3_path, envp );
				done();		/* just in case */
			}
			if ( ( wait_for(pid, WAIT)) ) /*wait value*/
				error("child process exited abnormally");
		finis
	finis

	testing("Call %s with a path to a setgid bin file.  Verify the",
	  xname );
	testing("real and effective group id's of the new process are");
	testing("set to svvs and bin respectively.");
	start
		prep("Fork a child process to call %s, and wait for the child",
		  xname);
		prep("to terminate.");
		start
			int pid;
			exit_on( ERROR );
			if( !(pid = fork()) ) {
				do_exec( fn, exec4_path, envp );
				done();		/* just in case */
			}
			if ( ( wait_for(pid, WAIT)) ) /*wait value*/
				error("child process exited abnormally");
		finis
	finis

	testing("Call %s with a path that contains a non-existent entry.",
	  xname );
	testing("Verify failure with errno set to ENOENT.");
	start
		expecting ( ENOENT );
		do_exec( fn, enoent_path, envp );
	finis

	testing("Call %s with a path that contains a non-directory component.",
	  xname );
	testing("Verify failure with errno set to ENOTDIR.");
	start
		expecting ( ENOTDIR );
		do_exec( fn, enotdir_path, envp );
	finis

	testing("Call %s with a path that contains an unreadable component.",
	  xname );
	testing("Verify failure with errno set to EACCES.");
	start
		expecting ( EACCES );
		do_exec( fn, no_r_path, envp );
	finis

	testing("Call %s with a path to a special file.", xname );
	testing("Verify failure with errno set to EACCES.");
	start
		expecting ( EACCES );
		do_exec( fn, "/dev/null", envp );
	finis

	testing("Call %s with a path to a file which does not have", xname);
	testing("execute permission.  Verify failure with errno set");
	testing("to EACCES.");
	start
		expecting ( EACCES );
		do_exec( fn, no_x_path, envp );
	finis

#ifndef	SYSVR_2
	testing("Call %s with an argument greater than ARG_MAX bytes.", xname);
	testing("Verify failure with errno set to E2BIG.");
	start
		do_2big_exec( fn, exec7_path, envp );
	finis
#endif

	testing("Call %s with a path to a file which is open for writing.",
	  xname );
	testing("Verify failure with errno set to ETXTBSY.");
	start
		int fd;
		fd = open( exec2_path, O_WRONLY);
		expecting ( ETXTBSY );
		do_exec( fn, exec2_path, envp );
		close( fd );
	finis

	if( fn != execlp && fn != execvp ) {
		testing("Call %s with a path to a file which is not a binary",
		  xname );
		testing("image.  Verify failure with errno set to  ENOEXEC.");
		start
			expecting ( ENOEXEC );
			do_exec( fn, bad_bin_path, envp );
		finis
	}
	else {
		abstract("Call %s when $PATH does not reference the target file.",
		  xname);
		start
			prep("Set the environmental variable $PATH to \"NULL_PATH\".");
			start
				putenv("PATH=NULL_PATH");
			finis
			testing("Call %s.  Verify failure with errno set", xname );
			testing("to ENOENT.");
			start
				expecting ( ENOENT );
				do_exec( fn, "exec2", envp );
			finis
			cleanup("Reset the environmental variable $PATH.");
			start
				putenv( dollar_path );
			finis
		finis
	}

}
siggy signal_func() {
	return;
}

use_time() {
	prep("Consume some user and system time.");
	start
		ttime	time();
		ttime	t;
		int		foo;
		int		z;

		/* use some system time */
		expecting ( VOID );
		t = time((ttime *)0) + 10;

		expecting ( VOID );
		while( time((ttime *)0) < t )
			sync();

		/* use some user time */
		t += 5;
		while( time((ttime *)0) < t ) {
			z = 1;	/* a fancy way to count (to fool optimizers) */
			for(foo=0;foo<10000;foo++) {
				z += foo;
				z /= 2;
			}
		}
	finis
}
do_exec( fn, path, envp )
int	(*fn)();
char *path;
char **envp;
{
	char	*argv[2];
	char	name[16];
	char	*p;
	int		i;

	for( p = path; *p; p++ );
	while( p > path && *(p-1) != '/' ) p--;
	for( i = 0; name[i++] = *p++ ; );
	argv[0] = name;
	argv[1] = (char *)0;

	if ( fn == execl )
		execl( path, name, (char *)0 );
	if ( fn == execv )
		execv( path, argv );
	if ( fn == execle )
		execle( path, name, (char *)0, envp );
	if ( fn == execve )
		execve( path, argv, envp );
	if ( fn == execlp )
		execlp( path, name, (char *)0 );
	if ( fn == execvp )
		execvp( path, argv );
}

#ifndef	SYSVR_2

do_2big_exec( fn, path, envp )
int	(*fn)();
char *path;
char **envp;
{
	char	*argv[2];
	char	name[16];
	char	*p;
	char	*bigarg;
	char	*malloc();
	int		i;

	bigarg = malloc(ARG_MAX + 5);

	for( p = path; *p; p++ );
	while( p > path && *(p-1) != '/' ) p--;
	for( i = 0; name[i++] = *p++ ; );
	argv[0] = name;
	argv[1] = bigarg;
	/*  make an argument greater than ARG_MAX bytes  */
	for (p=bigarg ; p < (bigarg + ARG_MAX + 5) ; p++)
		*p = 'z';
	*p = '\0';

	expecting ( E2BIG );

	if ( fn == execl )
		execl( path, name, bigarg, (char *)0  );
	if ( fn == execv )
		execv( path, argv );
	if ( fn == execle )
		execle( path, name, bigarg, (char *)0, envp );
	if ( fn == execve )
		execve( path, argv, envp );
	if ( fn == execlp )
		execlp( path, name, bigarg, (char *)0 );
	if ( fn == execvp )
		execvp( path, argv );
	free(bigarg);
}

#endif

do_free() {
	cleanup("Free the path buffers.");
	start
		free( dollar_path );
		free( os_bin_path );
		free( exec1_path );
		free( exec2_path );
		free( exec3_path );
		free( exec4_path );
		free( exec5_path );
		free( exec7_path );
		free( enoent_path );
		free( enotdir_path );
		free( no_r_path );
		free( no_x_path );
		free( bad_bin_path );
		free( tmp_path );
	finis
}
do_init( envp ) 
char	**envp;
{
	int i;
	prep("Allocate and initialize buffers, create temp files, etc.");
	prep("for the subsequent tests of the exec family of functions.");
	start
		prep("Allocate and initialize some path buffers.");
		start
			char	*malloc();
			char	*getenv();
			char	*temp_path();
			char	*data_path();
			int		size;

			exit_on( ERROR );
			size = strlen( SVVSPATH ) + strlen( OS_BIN ) + 32;
			os_bin_path = malloc( size );
			sprintf( os_bin_path, "%s%s", SVVSPATH, OS_BIN );
			dollar_path = malloc( size );
			sprintf( dollar_path, "PATH=%s%s", SVVSPATH, OS_BIN );
			exec1_path = malloc( size );
			sprintf( exec1_path, "%s%s", os_bin_path, "/exec1" );
			exec2_path = malloc( size );
			sprintf( exec2_path, "%s%s", os_bin_path, "/exec2" );
			exec3_path = malloc( size );
			sprintf( exec3_path, "%s%s", os_bin_path, "/exec3" );
			exec4_path = malloc( size );
			sprintf( exec4_path, "%s%s", os_bin_path, "/exec4" );
			exec5_path = malloc( size );
			sprintf( exec5_path, "%s%s", os_bin_path, "/exec5" );
			exec7_path = malloc( size );
			sprintf( exec7_path, "%s%s", os_bin_path, "/exec7" );
			enoent_path = malloc( size );
			sprintf( enoent_path, "%s%s", data_path(), "/garbage" );
			enotdir_path = malloc( size );
			sprintf( enotdir_path, "%s%s",data_path(),"not_a_dir/garbage");
			no_r_path = malloc( size );
			sprintf( no_r_path, "%s%s", data_path(),"noreaddir/cant_exec");
			no_x_path = malloc( size );
			sprintf( no_x_path, "%s%s", data_path(), "no_x_perm" );
			bad_bin_path = malloc( size );
			sprintf( bad_bin_path, "%s%s", data_path(), "bad_bin_image" );
			tmp_path = malloc( size );
			i = sprintf( tmp_path, "%s", temp_path() );
			tmp_path[ i - 1 ] = '\0';	/* rm trailing '/' */
		finis

		prep("Initialize the environmental variable \"$PATH\".");
		start
			putenv( dollar_path );
		finis

		prep("Open two temp files with file descriptors %d and %d.",
		  KNOWN_FILDES, KNOWN_FILDES+1 );
		prep("Set the close on exec flag for the second file.  The");
		prep("first file is expected to remain open after calls to exec,");
		prep("while the second file is expected to be closed.");
		start
			int fd;

			for (i = 3; i <= KNOWN_FILDES; i++)
				temp_open("exec.open.file",(O_CREAT|O_RDWR|O_TRUNC), 0666);
			fd = temp_open("exec.clos.file", (O_CREAT|O_WRONLY|O_TRUNC), 0666);
			for ( i = 3; i < KNOWN_FILDES; i++ )
				close(i);
			fcntl( fd, F_SETFD, 0x1 );
		finis

		prep("Write the parent process' environment to a temp file.");
		start
			FILE *fstr, *temp_fopen();
			
			fstr = temp_fopen("exec.env", "w");
			for( i=0; *(envp+i); i++ )
				fwrite((char *)(*(envp+i)), 1, strlen(*(envp+i)), fstr);
			fclose( fstr );
		finis

		prep("Write the parents process id to a temp file.");
		start
			FILE *fstr, *temp_fopen();
			int pid;

			fstr = temp_fopen("exec.pid", "w");
			pid = getpid();
			expecting( VOID );
			if ( !fwrite((char *)&pid, sizeof(size_t), 1, fstr ) )
				warning("%s\nfwrite returned 0",ADVISORY);
			expecting( SUCCESS );
			fclose( fstr );
		finis

		prep("Set signal handling for the parent process.");
		prep("SIGHUP is set to SIG_DFL, SIGUSR1 is set to SIG_IGN,");
		prep("and SIGUSR2 is set to be caught.  SIGUSR2 should be");
		prep("set to SIG_DFL after calls to exec, while the settings");
		prep("for SIGHUP and SIGUSR1 should remain unchanged.");
		start
			siggy	signal_func();
			signal(SIGHUP, SIG_DFL);
			signal(SIGUSR1, SIG_IGN);
			signal(SIGUSR2, signal_func);
		finis

		prep("Call ulimit to set the file size limit.  This limit is");
		prep("expected to remain in effect after calls to exec.");
		start
			long ulimit();

			ulimit(2, (long)PROC_LIMIT - 10 );
		finis

		prep("Call umask to set the file creation mode mask bits.");
		start
			expecting ( VOID );
			umask( PARNT_MASK );
		finis
	finis
}
