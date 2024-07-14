/*
 *	t_error.c
 *
 *	a test of t_error(NS_LIB)	
 *	
 *	This test is a direct decendent of the SVVS 
 *	test for perror(BA_LIB).
 *
 *	Tabstops should be set to 4 to view this file.
 */

#ident	"@(#)t_error.c	1.3"

#include	<sv_base.h>
#include	<sv_nse.h>
#include	<tiuser.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	MAXSYSNERR	20000

#ident	"@(#)t_error.c	1.3"

extern	FILE	*temp_fopen();
extern	char	*t_errlist[];
extern	int		t_errno;
extern	int		t_nerr;

#define	SYSERR_PRINTS_ERRNO_OK

main() {
	FILE	*stream;
	int		num_error;
	char	ret_buf[STRLEN];
	int		nbyte;
	int		stderr_fd;
	char	*sp;
	char	*bp;

	setup("t_error");

	testing("t_nerr should be a reasonable value.");
	start
		if ( t_nerr < 0 || t_nerr > MAXSYSNERR )
			error("unacceptable size for t_nerr: %d", t_nerr);
	finis

	testing("Check that t_errlist[] contains strings for each");
	testing("value of t_errno between 1 and t_nerr-1 (%d).",t_nerr-1);
	start
		break_on( ERROR );
		for (num_error = 1; num_error < t_nerr; num_error++)
			if ( !t_errlist[num_error] )
				error("t_errlist[%d] contains a NULL entry",num_error);
	finis

	prep("Close stderr, saving the file descriptor so that when");
	prep("the temp file is later opened, its file descriptor may be");
	prep("compared against the file descriptor for stderr.");
	start
		expecting( VOID );
		stderr_fd = fileno( stderr );
		exit_on( ERROR );		/* rest of test would fail anyway */
		expecting( SUCCESS );
		fclose( stderr );
	finis

	abstract("Call t_error for each value of t_errno between");
	abstract("1 and t_nerr-1 (%d).  Check that a string is ",t_nerr-1);
	abstract("printed to stderr for each invocation of t_error.");
	abstract("Verify the printed string is composed of the prefix");
	abstract("passed to t_error, and the error message contained in");
	abstract("t_errlist[].  Check that the string is terminated with");
	abstract("a newline");
	start
		break_on( ERROR );
		nbyte = STRLEN;
		for (num_error = 1; num_error < t_nerr; num_error++) {
			prep ("Open a temp file with the same file descriptor as ");
			prep ("stderr so that error messages are caught and may be");
			prep ("read for comparison.  Verify that the file descriptor");
			prep ("for the temp file is the same as fileno( stderr ).");
			start
				exit_on ( ERROR );	/* rest of test fails if this fails */
				stream = temp_fopen("t_error.tmp", "w");
				expecting( stderr_fd );
				fileno( stream );
			finis
			testing("Call t_error(\"Testing t_error\") with t_errno: %d.",
			  num_error);
			start
				t_errno = num_error;
				t_error("Testing t_error");
			finis
			prep("Close the temp file and re-open it for reading.");
			start
				exit_on( ERROR );
				fclose(stream);
				stream = temp_fopen("t_error.tmp", "r");
			finis
			testing("Check the t_error output for syntax and content");
			start
				ret_buf[0] = '\0';
				expecting (VOID);
				if ( fgets(ret_buf, nbyte, stream) ) {
					sp = "Testing t_error: ";
					if ( strncmp(ret_buf,sp,strlen(sp)) ) {
						ret_buf[strlen(sp)] = '\0';
						fclose( stream );
						error ("%s%s%s%s%s%s%s",
						  "t_error did not prepend the correct string\n",
						  "returned prefix: '",
						  ret_buf,
						  "'\nexpected prefix: '",
						  sp,
						  "'");
					}
					if( strncmp( &ret_buf[strlen(sp)], 
					  t_errlist[num_error], 
					  strlen(t_errlist[num_error]))) {
						fclose( stream );
						error ("%s%s%s%s%s%s%s",
						  "t_error returned an unexpected error string\n",
						  "returned string: '",
						  &ret_buf[strlen(sp)],
						  "'\nexpected string: '",
						  t_errlist[num_error],
						  "'");
					}
					bp = &ret_buf[strlen(sp)+strlen(t_errlist[num_error])];
#ifdef SYSERR_PRINTS_ERRNO_OK
					if ( num_error == TSYSERR )
						while( *bp && *bp != '\n' ) *bp++;
#endif
					if ( *bp != '\n' ) {
						fclose( stream );
						error("%s%s",
						  "t_error did not terminate the string ",
						  "with a newline");
					}
				}
				else {
					fclose( stream );
					error ("t_error did not write to stderr");
				}
			finis
			cleanup("Close the temp file");
			start
				exit_on( ERROR );
				fclose(stream);
			finis
		}
	finis

	abstract("Verify that t_error(\"\") prints a string to stderr.");
	abstract("Check that the printed string matches the corresponding");
	abstract("t_errlist[] entry, and is terminated by a newline.");
	start
		prep ("Open a temp file with the same file descriptor as ");
		prep ("stderr so that error messages are caught and may be");
		prep ("read for comparison.  Verify that the file descriptor");
		prep ("for the temp file is the same as fileno( stderr ).");
		start
			exit_on ( ERROR );	/* rest of test fails if this fails */
			stream = temp_fopen("t_error.tmp", "w");
			expecting( stderr_fd );
			fileno( stream );
		finis
		nbyte = STRLEN;
		num_error = t_errno = 1;
		testing("Call t_error(\"\") with t_errno: %d.", num_error);
		start
			t_errno = num_error;
			t_error("");
		finis
		prep("Close the temp file and re-open it for reading.");
		start
			exit_on( ERROR );
			fclose(stream);
			stream = temp_fopen("t_error.tmp", "r");
		finis
		testing("Check the t_error output for syntax and content.");
		start
			ret_buf[0] = '\0';
			expecting (VOID);
			if ( fgets(ret_buf, nbyte, stream) ) {
				if( strncmp( ret_buf, t_errlist[num_error], 
				  strlen(t_errlist[num_error]))) {
					error ("%s%s%s%s%s%s%s",
					  "t_error returned an unexpected error string\n",
					  "returned string: '",
					  &ret_buf[strlen(sp)],
					  "'\nexpected string: '",
					  t_errlist[num_error],
					  "'");
				}
				bp = &ret_buf[strlen(t_errlist[num_error])];
#ifdef SYSERR_PRINTS_ERRNO_OK
					if ( num_error == TSYSERR )
						while( *bp && *bp != '\n' ) *bp++;
#endif
				if ( *bp != '\n' ) {
					error("%s%s",
					  "t_error did not terminate the string ",
					  "with a newline");
				}
			}
			else {
				error ("t_error did not write to stderr");
			}
		finis
		cleanup("Close the temp file");
		start
			exit_on( ERROR );
			fclose(stream);
		finis
	finis
	done();
/*
 *	NOTREACHED
 */
}
