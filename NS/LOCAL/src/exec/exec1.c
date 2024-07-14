#include 	<sv_base.h>
#include	<sys/types.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<sys/times.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	PARNT_MASK		02
# define	KNOWN_FILDES	7

#ident	"@(#)exec1.c	1.7"

siggy (*signal())();

main(argc,argv,envp)
int	argc;
char	**argv;
char	**envp;
{
	char	*envp_string;
	char	*envp2_string;
	char	*exp_path;
	char	*cur_path;
	char	*os_bin_path;

	no_close( 7 );
	no_sig( SIGHUP );
	no_sig( SIGUSR1 );
	no_sig( SIGUSR2 );
	setup("exec");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	prep("Allocate some buffer space for the environment and paths.");
	start
		int	envp_cnt;
		int i;
		char *malloc();

		envp_cnt = 1024;	/* a little extra room */
		expecting ( VOID );
		for(i = 0; envp[i]; i++) envp_cnt += strlen(envp[i]);
		expecting ( SUCCESS );
		exit_on( ERROR );
		envp_string= malloc( envp_cnt );
		envp2_string= malloc( envp_cnt );
		exp_path = malloc( PATH_MAX );
		cur_path = malloc( PATH_MAX );
		os_bin_path	= malloc( PATH_MAX );
		sprintf( os_bin_path, "%s%s", SVVSPATH, OS_BIN );
	finis

	abstract("Verify the process id, parent process id, process");
	abstract("group id, current working directory, umask, file size");
	abstract("limit, tms_utime, tms_stime, tms_cutime, tms_cstime,");
	abstract("user id, group id, alarm clock setting, signal");
	abstract("handling settings, and open file descriptors.");
	start
		testing("Verify the current working directory is the same as the");
		testing("current working directory for the parent process.");
		start
			char *getcwd();

			sprintf( exp_path, "%s%s", SVVSPATH, OS_BIN );
			munch_path( exp_path, temp_path() );
			getcwd( cur_path, PATH_MAX );
			exp_path[ strlen( exp_path ) - 1 ] = '\0';
			expecting( VOID );
			if( strcmp( exp_path, cur_path ) ) 
				error("%s\nexpected:	'%s'\nreturned:	'%s'\n",
				  "getcwd returned unexpected path",
				  exp_path,
				  cur_path);
		finis

		prep("Chdir back to the bin directory.");
		start
			chdir( os_bin_path );
		finis

		testing("Verify that a file descriptor opened in the parent");
		testing("process was not closed by closing it.");
		start
			close(7);
		finis

		testing("Verify the environment matches the environment the");
		testing("process wrote to a temp file.");
		start
			int i;
			int fd;
			int	cnt;

			exit_on( ERROR );
			i = 0;
			envp_string[0] = '\0';
			while ( *(envp+i) != NULL ) {
				strcat(envp_string, *(envp+i));
				i++;
			}
			fd = temp_open("exec.env", O_RDONLY);
			i = 0;
			do {
				expecting ( VOID );
				if ( (cnt = read(fd, &envp2_string[i], 1)) < 0) {
					error("could not read the tmp file.");
					break;
				}
				else if (cnt > 0) i++;
			} while ( cnt == 1 );
			envp2_string[i]='\0';
			expecting ( VOID );
			if ( strcmp(envp_string, envp2_string) )
				error("The child did not inherit the parent environment.");
			expecting ( SUCCESS );
			close( fd );
		finis

		testing("Verify by calling close, that the file opened in the");
		testing("parent process which had the close on exec flag set,");
		testing("was indeed closed.");
		start
			break_on(ERROR);
			expecting ( EBADF );
			close( KNOWN_FILDES + 1 );
		finis

		testing("Verify signal handling settings.");
		start
			expecting ( SIG_DFL );
			signal(SIGHUP, SIG_IGN);
			expecting ( SIG_IGN );
			signal(SIGUSR1, SIG_DFL);
			expecting ( SIG_DFL );
			signal(SIGUSR2, SIG_IGN);
		finis

		testing("Verify umask setting.");
		start
			expecting ( PARNT_MASK );
			umask(01);
		finis

		testing("Verify ulimit file size limit setting.");
		start
			expecting( PROC_LIMIT - 10 );
			ulimit(1, 0l);
		finis

		testing("Verify alarm setting.");
		start
			expecting ( VOID );
			if( alarm( 0 ) < 100 )
				error("Time remaining on alarm was not inherited.");
		finis
		testing("Verify Utime, stime, cstime, and cutime setting.");
		start
			struct	tms	buf;
			struct	tms	*times_buf = &buf;

			times(times_buf);
			if ( !times_buf->tms_utime )
				error("The utime was not inherited in the child process.");
			if ( !times_buf->tms_stime )
				error("The stime was not inherited in the child process.");
			if ( !times_buf->tms_cutime )
				error("The cutime was not inherited in the child process.");
			if ( !times_buf->tms_cstime )
				error("The cstime was not inherited in the child process.");
		finis

		testing("Verify pid and ppid.");
		start
			FILE *fstr;
			FILE *temp_fopen();
			int	child_pid, child_ppid, parent_pid;

			expecting ( VOID );
			child_ppid = getppid();
			child_pid = getpid();
			if ( child_ppid == child_pid )
				warning("%s\nparent and child pid's match",
				  ADVISORY);
			expecting( SUCCESS );
			fstr = temp_fopen("exec.pid", "r");
			expecting ( VOID );
			if ( !fread(&parent_pid, sizeof(size_t), 1, fstr) )
				warning("%s\nfread returned 0", ADVISORY);
			if ( child_ppid != (int) parent_pid )
				warning("%s\nparent and child pid's match",
				  ADVISORY);
			expecting ( SUCCESS );
			fclose( fstr );
		finis

		cleanup("Free the allocated space.");
		start	
		    free( envp_string );
			free( envp2_string );
			free( exp_path );
			free( cur_path );
			free( os_bin_path );

		finis
	finis
	done();
	/*NOTREACHED*/
}
munch_path( spath, np )
char *spath;	/* start_path	*/
char *np;		/* new_path		*/
{
	char *sp = spath;
	prep("Determine the expected path.");
	start
		expecting( VOID );
		if( *np == '/' ) {				/* full path in new_path 	*/
			strcpy( spath, np );
			return;
		}
		while( *sp ) sp++;				/* get to end of start_path	*/
		while( !strncmp( np, "../", 3 ) ) {
			np += 3;
			while( sp > spath && *sp != '/' ) sp--;
		}
		*(sp+1) = '\0';
		strcat( spath, np );
	finis
}
