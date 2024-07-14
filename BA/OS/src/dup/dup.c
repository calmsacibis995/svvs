/*
 *	dup.c - test the dup function.
 *
 */
#include	<sv_base.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)dup.c	1.4"

#ifndef	SYSVR_2
#define	PROT	( S_IRUSR | S_IWUSR | S_IXUSR | \
		  S_IRGRP | S_IWGRP | S_IXGRP | \
		  S_IROTH | S_IWOTH | S_IXOTH )
#else
#define	PROT		0777
#endif

main()
{

	char	*path = "dup.tmp";
	int		fildes;
	int		duped;
	int		duped2;

	int		dup();
	int		fstat();
	int		close();

	setup("dup");
	funct( "dup" );
	abstract("Normal cases of dup with a file descriptor from creat.");
	start
		expecting( SUCCESS );				/* default	*/
		error_on( E_FAILURE | E_UNEXP );	/* default	*/
		continue_on( ERROR );				/* default	*/
		prep("Set umask to zero.");
		start
			expecting( VOID );
			umask(0);
			expecting( 0 );
			umask(0);
		finis
		break_on( ERROR );					/* quit this block on error	*/
		prep("Creat a new file");
		start
			fildes = temp_creat(path, PROT);
		finis
		testing("Dup file descriptor obtained from creat,");
		testing("should return lowest available file descriptor.");
		start
			expecting( fildes + 1 );
			duped = dup(fildes);
		finis
		testing("Dup again to get the next lowest file descriptor");
		testing("should return lowest available file descriptor.");
		start
			expecting( duped + 1 );
			duped2 = dup(fildes);
		finis
		testing("File descriptors returned from creat and dup should");
		testing("refer to the same file, checking inode and modes"); 
		testing("with fstat.");
		start
			struct	stat	f_buf;
			struct	stat	d_buf;

			fstat(fildes, &f_buf);
			fstat(duped, &d_buf);

			if ( f_buf.st_ino != d_buf.st_ino )
				error("fstat reports st_ino mismatch");
			if ( f_buf.st_mode != d_buf.st_mode )
				error("fstat reports fildes st_mode mismatch");
		finis
		cleanup("Close the file descriptors used in this test.");
		start
			expecting( VOID );			/* in case previous error */
			close(fildes);
			close(duped);
			close(duped2);
		finis
	finis
	abstract("Abnormal cases of dup.");
	start
		expecting( FAILURE );
		error_on( E_SUCCESS | E_ERRNO | E_UNEXP );	/* default	*/
		continue_on( ERROR );						/* default	*/
		testing("Dup with a bad file descriptor for an argument.");
		start
			expecting( EBADF );
			dup( BAD_FILDES );
		finis
		abstract("Dup repetitively to consume OPEN_MAX+1 file");
		abstract("descriptors and then return error of EMFILE.");
		start
			int i;
			prep("Creat a file, then call dup to consume OPEN_MAX");
			prep("file descriptors.  Dup should return the lowest");
			prep("available file descriptor until the OPEN_MAXth");
			prep("invocation.");
			start
				expecting( SUCCESS );
	 			break_on( ERROR );
				fildes = temp_creat(path, PROT);
				for (i = fildes+1; i < OPEN_MAX; i++) {
					expecting( i );
					dup(fildes);
				}
			finis
			testing("Dup the (OPEN_MAX+1)th file descriptor.");
			start
				expecting( EMFILE );
				dup(fildes);
			finis
			cleanup("Close the file descriptors opened by this test.");
			start
				expecting( SUCCESS );
				break_on( ERROR );
				for( i=3; i < OPEN_MAX; i++ )
					close( i );
			finis
		finis
	finis
}
