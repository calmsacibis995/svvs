#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<stdio.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)chroot.c	1.3"


char		PathPointer[STRLEN];

/*
** chroot
**	Test the chroot function.
*/
main()
{
# ifdef CHROOT
	int		ret_val;
	FILE		*write_stream;

	struct stat	st_buf1, st_buf2;

	extern	char		*temp_path();
	extern	FILE		*temp_fopen();

setup("chroot");
funct("chroot");
/*
**	Open a file in the temp dir, which we can use to determine
**	location of current working dir, etc.
**	If cannot creat file, complain and exit.
**	File name:
**		../tmp/chroot.dir/tmpchroot 
**	Current working dir is:
**		<SVVSPATH>/KEXT/bin
*/
	if ( (write_stream = temp_fopen("tmpchroot", "w")) == NULL )
		error ("can not open the temp file for chroot");
	if ( fclose(write_stream) != 0 )
		error ("can not close the temp file for chroot");

testing("chroot of a path with a nondirectory element");
testing("should return -1, and an errno of ENOTDIR.");
start
/*
**	chroot("../tmp/chroot.dir/tmpchroot/garbage") should
**	fail with ENOTDIR.
*/
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "tmpchroot/garbage");

	expecting (-1);
	expecting (ENOTDIR);
	chroot(PathPointer);
finis

testing("chroot of a path with a nonexistent element");
testing("should return -1, and an errno of ENOENT.");
start
/*
**	chroot("../tmp/chroot.dir/garbage") should fail with ENOENT
*/
	strcpy(PathPointer,temp_path());
	strcat (PathPointer, "garbage");

	expecting (-1);
	expecting (ENOENT);
	chroot(PathPointer);
finis


testing("chroot to temp directory.");
testing("Stat of current working directory should not be affected.");
start
	expecting (VOID);
	if( stat( ".", &st_buf1 ) != 0 ) 
	{
		error ( "stat of '.' before chroot failed" );
	}
/*
**	chroot("../tmp/chroot.dir"), should work
*/
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;

	expecting (SUCCESS);
	chroot(PathPointer);
/*
**	if chroot to temp dir failed, error function called exit
**	otherwise, check that cwd has not changed by calling stat
*/
	expecting (VOID);
	if( stat( ".", &st_buf2 ) != 0 ) 
	{
		error ( "stat of '.' after chroot failed" );
	}
	if ( cmp_stat( &st_buf1, &st_buf2 ) )
		error("Chroot altered the current working directory.");
finis


testing("chroot to tmp directory allows access of tmp dir as '/'.");
testing("stat of relative path to tmp directory should match the stat of '/'.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;

	expecting (SUCCESS);
	stat( PathPointer, &st_buf1 );
	stat( "/", &st_buf2 );

	expecting (VOID);
	if ( cmp_stat( &st_buf1, &st_buf2 ) )
		error("Stats of '/' and relative path to tmp dir differ");
finis


testing("chroot to tmp directory allows access of tmp file as '/tmpfile'.");
testing("stat of tmp file from '/tmpfile' to be the same as stat of tmp");
testing("file from path relative to current working dir.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	strcat (PathPointer,"tmpchroot");

	expecting (SUCCESS);
	stat( PathPointer, &st_buf1 );
	stat( "/tmpchroot", &st_buf2 );
	if ( cmp_stat( &st_buf1, &st_buf2 ) )
		error("Absolute and relative paths reference different files");
finis


testing("chroot to tmp directory allows access of tmp dir as '/..'.");
testing("stat of tmp dir from '/..' should be the same as stat of tmp");
testing("dir from path relative to current working dir.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;

	expecting (SUCCESS);
	stat( PathPointer, &st_buf1 );
	stat( "/..", &st_buf2 );
	if ( cmp_stat( &st_buf1, &st_buf2 ) )
		error("Stats of relative paths to tmp dir and '/..' differ");
finis


testing("Chdir to tmp dir, check that '.' and '/' are the same file.");
testing("Stat of '.' and '/' should match.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;

	expecting (SUCCESS);
	chdir(PathPointer);

	stat( ".", &st_buf1 );
	stat( "/", &st_buf2 );
	if ( cmp_stat( &st_buf1, &st_buf2 ) )
		error("Stats of '/' and '.' differ");
finis


testing("'.' and '..' are the same file.");
testing("Stat of '.' and '..' should match.");
start
	expecting (SUCCESS);
	stat( ".", &st_buf1 );
	stat( "..", &st_buf2 );
	if ( cmp_stat( &st_buf1, &st_buf2 ) )
		error("Stats of '..' and '.' differ");
finis


testing("Using chroot, when not a super user.");
testing("should return -1 and errno of EPERM.");
start
	expecting (0);
	setuid(UID);

	expecting (-1);
	expecting (EPERM);
	chroot(".");
finis


done();
/*NOTREACHED*/

#else
	
	unsupported ("Change root directory is not supported on this system.");
	exit(0);
/*NOTREACHED*/

#endif
}
# ifdef CHROOT
cmp_stat( st1, st2 )
struct stat *st1;
struct stat *st2;
{
	if (st2->st_mode != st1->st_mode )
		return(-1);
	if (st2->st_ino != st1->st_ino )
		return(-1);
	if (st2->st_dev != st1->st_dev )
		return(-1);
	if (st2->st_nlink != st1->st_nlink )
		return(-1);
	if (st2->st_uid != st1->st_uid )
		return(-1);
	if (st2->st_gid != st1->st_gid )
		return(-1);
	if (st2->st_size != st1->st_size )
		return(-1);
	if (st2->st_atime != st1->st_atime )
		return(-1);
	if (st2->st_mtime != st1->st_mtime )
		return(-1);
	if (st2->st_ctime != st1->st_ctime )
		return(-1);
	return(0);
}
#endif
