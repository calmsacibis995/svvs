#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)access0.c	1.10"

# define	OWNER	6
# define	GROUP	3
# define	OTHER	0
# define	I_ACC	0

extern	char	*err_lookup ();

main(argc,argv)
int argc;
char *argv[];
{
	int		fd;
	char		path[PATH_MAX + 10];

	setup("access");

	funct("access");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	abstract("Test all possible values of file mode and access mode, when");
	abstract("the file owner and group match the process uid and gid.");
	start
		do_test(UID,GID,OWNER);
	finis
	abstract("Test all possible values of file mode and access mode, when");
	abstract("the file owner and process uid match, but the file group");
	abstract("and process gid do not.");
	start
		do_test(UID,BIN_GID,OWNER);
	finis
	abstract("Test all possible values of file mode and access mode, when");
	abstract("the file group and process gid match, but the file owner ");
	abstract("and process uid do not.");
	start
		do_test(BIN_UID,GID,GROUP);
	finis
	abstract("Test all possible values of file mode and access mode, when");
	abstract("the file owner and group do not match the process uid and gid.");
	start
		do_test(BIN_UID,BIN_GID,OTHER);
	finis
	testing("access of a non-existent file.");
	start
		strcpy(path, temp_path());
		strcat(path, "_access");

		expecting( ENOENT );
		access(path, I_ACC);
	finis
	testing("access of a file where search permission along a path is denied.");
	start
		strcpy(path, data_path());
		strcat(path, "noperm.dir");
		strcat(path, "/access_tryme");

		expecting( EACCES );
		access(path, I_ACC);

	finis
	abstract("access when the argument has a component");
	abstract("that is not a directory.");
	start
		break_on( ERROR );
		prep("Creat a temp file useable as a non-directory.");
		prep("Component of a path.");
		start
			expecting( SUCCESS );
			fd = temp_creat("accessbad_dir", 0777);
			close( fd );
		finis

		testing("Invoke access with the invalid path");
		start
			strcpy(path, temp_path());
			strcat(path, "access");
			strcat(path, "bad_dir");
			strcat(path, "/dummy_point");

			expecting( ENOTDIR );
			access(path, I_ACC);
		finis
	finis


	done();

/*
 * NOTREACHED 
 */
}

exp_access(amode, mode, owner, group)
int	amode;		/* amode given to access()		*/
int	mode;		/* files permission mode		*/
int	owner;		/* owner of the file			*/
int	group;		/* group of the file			*/
{
	int	mo;
/*
 *	amode has range of 0-7, corresponds to the set { r, w, x }.
 *	mode has 0-777, corresponds to three sets { r, w, x },
 *		one each for owner, group, and other.
 *	mo is the selection of one of the three sets from mode, based
 *		upon whether uid and gid match the owner and group.
 *	access should be permitted if amode is a subset of mo, or
 *		the same set as mo.  Thus if ((amode | mo) == mo)
 *		then access should be permitted.
 */
	if( owner == UID )
		mo = ( mode & 0700 ) >> 6;
	else if ( group == GID )
		mo = ( mode & 0070 ) >> 3;
	else
		mo =  mode & 007;

	if (( amode | mo ) == mo)
		return ( 0 );
	else
		return( -1 );
}

new_mode( mode, tune )
int mode;
int tune;
{
/*
 *	for owner check 0, 0077 -> 0777, increment 0100
 * 	for group check 0, 0707 -> 0777, increment 0010
 *	for other check 0, 0770 -> 0777, increment 0001
 */
	switch( tune )
	{
	case OWNER:
		if (!mode) return(0077);
		return(mode + 0100);
	case GROUP:
		if (!mode) return(0707);
		return(mode + 0010);
	case OTHER:
		if (!mode) return(0770);
		return(mode + 0001);
	default:
		severr("internal error: invalid argument to new_mode");
	}
}

do_test(owner,group,tune)
int owner, group, tune;
{
	char	path[PATH_MAX];
	char	*temp_path();
	int	amode;
	int	mode;
	int	fd;
	int	exp_error;

	prep("Creat temp file 'access_0'.");
	start
		/*  give up if this fails  */
		exit_on (ERROR);

		expecting( SUCCESS );
		strcpy(path, temp_path());
		strcat(path, "access_0");

		expecting( VOID );
		(void) unlink(path);	/*  ensure file is not there	*/

		expecting( SUCCESS );
		fd = temp_creat("access_0", 0777);
		close( fd );
	finis

	testing("Check access for all values of amode (0-07 or R_OK, W_OK, X_OK, and F_OK),");
	testing("and for all values of file mode permissions (rwx)");
	testing("for %s",
	    (tune == OWNER)?"owner.":((tune == GROUP)?"group.":"other.") );
	testing("For each iteration, change the mode/owner of the file,");
	testing("and attempt to access the file.");
	if( tune != OWNER ) {
		testing("Now unlink the file and creat it again for the");
		testing("next iteration.");
	}

	start
		break_on( ERROR );
		for ( amode = 0; amode <= 07; amode++) {
			for ( mode = 0; mode <= 0777; mode = new_mode(mode, tune)) 	{
				expecting( SUCCESS );
				chmod(path, mode);
				chown(path, owner, group);
				if(exp_access( amode, mode, owner, group))
					exp_error = EACCES;
				else	
					exp_error = SUCCESS;
				expecting (VOID);
				if (access(path, amode))
					{
					/*  the access failed.
					 *  Did we expect it to fail?
					 */
					if (exp_error == SUCCESS)
						{
error ("access succeeded in accessing file '%s'\n\
file mode: %o owner %d group %d\n\
for access by %s amode = %o",
path, mode, owner, group,
(tune == OWNER)?"owner.":((tune == GROUP)?"group.":"other."),
amode);
						}
					else
						{
						/*  correct errno?  */
						if (errno != EACCES)
error ("access failed with errno %s\nin accessing file '%s'\n\
file mode: %o owner %d group %d\n\
for access by %s amode = %o",
err_lookup (errno),
path, mode, owner, group,
(tune == OWNER)?"owner.":((tune == GROUP)?"group.":"other."),
amode);
						}
					}
				else
					{
					/*  should it have failed?  */
					if (exp_error == EACCES)
error ("access failed with errno %s\nin accessing file '%s'\n\
file mode: %o owner %d group %d\n\
for access by %s amode = %o",
err_lookup (errno),
path, mode, owner, group,
(tune == OWNER)?"owner.":((tune == GROUP)?"group.":"other."),
amode);
					}


				if( owner != UID ) { /* speed things up if owner	*/
					expecting( SUCCESS );
					unlink(path);
					fd = temp_creat("access_0", 0777);
					close( fd );
				}
			}
		}
	finis
/*????? TEST: delted since verify -d should keep temp files
DELETE THIS if no side-effects
	cleanup("Unlink the temp file.");
	start
		expecting( VOID );
		unlink(path);
	finis
?????*/
}
