/*
 *	chown[012].c - a test of chown(BA_OS).
 *
 *	chown0, chown1, and chown2 are linked and setuid root.
 *
 *	chown0:	executes as svvs.
 *	chown1:	executes as bin.	
 *	chown2:	executes as root.	
 *
 */

#ident	"@(#)chown0.c	1.11"

#include	<sv_base.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	PROT		0777
# define	BITS_PROT	( PROT | S_ISUID | S_ISGID )
# define	ROOT		( uid == ROOT_UID )
# define	READ_ONLY	1
# define	READ_WRITE	0

char	*strcpy();
char	*strcat();
char	path[PATH_MAX + 10];

main( argc, argv ) 
int argc;
char *argv[];
{
	char	*usr;
	char	*name;
	int		uid;

	setup( "chown" );
	exit_on( ERROR | SEVERR );

	for( name = argv[0]; *name; name++ );
	while( name > argv[0] && *(name-1) != '/' ) name--;

	expecting( VOID );
	if( !strcmp( name, "chown0" ) ) {
		usr = "svvs";
		uid = UID;
	}
	else if( !strcmp( name, "chown1" ) ) {
		usr = "bin";
		uid = BIN_UID;
	}
	else if( !strcmp( name, "chown2" ) ) {
		usr = "root";
		uid = ROOT_UID;
	}
	else
		severr("%s'%s'\n%s",
		  "Unknown name: ",
		  name,
		  "expected:	chown[012].");

	expecting( SUCCESS );
	abstract("Test cases of chown while executing as %s.", usr );
	start
		prep("Initialize variables, clear umask,");
		prep("and call setuid( %d ).", uid);
		start
			char	*malloc();
			strcat( strcpy( path, temp_path() ), "chown.tmp" );
			expecting( VOID );
			umask( 0 );
			setuid( uid );
		finis

		abstract("Test cases of files with no setuid/setgid bits set.");
		abstract("Verify ownership change with no mode changes.");
		start
			do_four( PROT, PROT );
		finis

		abstract("Test cases of files with setuid/setgid bits set.");
		abstract("Verify ownership change with the setuid/setgid");
		abstract("bits of the mode%scleared.", ROOT ? " not " : " " );
		start
			do_four( BITS_PROT, ROOT ? BITS_PROT : PROT );
		finis

		abstract("Test cases of chown which should fail.");
		start
			abstract("Chown for a file which does not exist.");
			abstract("Verify failure with errno set to ENOENT.");
			start
				strcat( strcpy( path, temp_path() ), "non_existent" );
				expecting( ENOENT );
				do_chown( path, UID, GID );
			finis

			if( !ROOT ) {
				abstract("Chown for a path along which search permission");
				abstract("is denied.  Verify failure with errno set to");
				abstract("EACCES.");
				start
					strcat( strcpy( path, data_path() ),
					  "noperm.dir/chown_tryme" );
					expecting( EACCES );
					do_chown( path, UID, GID );
				finis

				abstract("Chown on /etc/passwd should fail with errno = EPERM");
				abstract("since that is owned by ROOT.");
				start
					expecting (EPERM);
					do_chown  ("/etc/passwd", UID, GID);
				finis
			}

			abstract("Chown for a path which contains a non-directory");
			abstract("component.  Verify failure with errno set to");
			abstract("ENOTDIR.");
			start
				int fd;

				strcat( strcpy( path, temp_path() ), "chown.bad_dir" );
				fd = do_creat( path, PROT );
				strcat( path, "/dummy_point" );
				expecting( ENOTDIR );
				do_chown( path, UID, GID );
				expecting( SUCCESS );
				do_close( fd );
			finis

			if ( ROOT ) {
				abstract("Chown a file which is part of a read only");
				abstract("file system.  Verify failure with errno set");
				abstract("to EROFS.");
				start
					exit_on( ERROR );
#ifdef SV_RFS
					dmnt_erofs();
#else
					mnt_erofs();
#endif
					continue_on( ERROR );
					strcpy(path,MOUNT_POINT);
					strcat(path, "/" );
					strcat(path, EROFS_FILE );
#ifdef	SV_RFS
					testing("Call chown( \"%s\", %d, %d ).",path,UID,GID);
					start
						expecting( VOID );
						if( !chown( path, UID, GID ) )
							warning( "%s\n%s", ADVISORY,
							  "chmod succeeded unexpectedly");
						else {
							expecting( EROFS );
							chown( path, UID, GID );
						}
					finis
#else
					expecting( EROFS );
					do_chown( path, UID, GID );
#endif
					exit_on( ERROR );
#ifdef SV_RFS
					dumnt_erofs();
#else
					umnt_erofs();
#endif
				finis
			}
		finis



	finis

	done();
	/*NOTREACHED*/
}
do_chown( pth, uid, gid )
char *pth;
int	uid;
int gid;
{
	testing("Call chown( \"%s\", %d, %d ).",pth,uid,gid);
	start
		chown( pth, uid, gid );
	finis
}
do_close( fd )
int fd;
{
	cleanup("Close the temp file.");
	start
		close( fd );
	finis
}
do_unlink( pth )
char *pth;
{
	cleanup("Unlink the temp file.");
	start
		unlink( pth );	
	finis
}
do_creat( pth, mode )
char *pth;
int mode;
{
	int fd;

	prep("Create a temp file with mode 0%o.", mode);
	start
		fd = creat( pth, mode );
	finis
	return( fd );
}
do_fstat( fd, uid, gid, mode )
int fd;
int uid;
int gid;
int mode;
{
	struct	stat buf;
	
	testing("Call fstat to verify the results.");
	start
		fstat( fd, &buf );
		if ( buf.st_uid != (ushort) uid )
			error("fstat indicates unexpected uid: %d.", buf.st_uid );
		if ( buf.st_gid != (ushort) gid )
			error("fstat indicates unexpected gid: %d.", buf.st_gid );
		if ( ( buf.st_mode & BITS_PROT ) != mode )
			error("fstat indicates unexpected mode: 0%o.", buf.st_mode );
	finis
}
do_four( beg_mode, exp_mode )
int beg_mode;
int exp_mode;
{
	do_one( "svvs", UID,     "svvs", GID,     beg_mode, exp_mode );
	do_one( "bin",  BIN_UID, "svvs", GID,     beg_mode, exp_mode );
	do_one( "svvs", UID,     "bin",  BIN_GID, beg_mode, exp_mode );
	do_one( "bin",  BIN_UID, "bin",  BIN_GID, beg_mode, exp_mode );
}
do_one( usrname, uid, grpname, gid, beg_mode, exp_mode )
char *usrname;
int	uid;
char *grpname;
int gid;
int beg_mode;
int exp_mode;
{
	abstract("Chown( \"%s\", %s, %s ).", path, usrname, grpname );
	start
		int fd;

		fd = do_creat( path, beg_mode );
		do_chown( path, uid, gid );
		do_fstat( fd, uid, gid, exp_mode );
		do_close( fd );
		do_unlink( path );
	finis
}
