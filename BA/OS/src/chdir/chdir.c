#include	<sv_base.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)chdir.c	1.5"

# define	BAD_PATH	"data.badpath/tmpdir"
# define	MISSING_DIR	"missing.dir"
# define	PROT_DIR	"noread.dir"
# define	NOOWNER_DIR	"nosearchowner"
# define	NOGROUP_DIR	"nosearchgroup"
# define	NOALL_DIR	"nosearchall"

/*
 * chdir
 *	Test the chdir function.
 *
 *	This test is dependent upon:
 *		getcwd()
 *
 */
main() {
	char 	*start_path;	/* cwd path at invocation 	*/
	char	*curr_path;		/* cwd path during test		*/
	char	*new_path;		/* path to which to chdir()	*/
	char	*getcwd();
	char 	*data_path();
	char	*malloc();

	setup("chdir");
	funct("chdir");

	prep("Allocate path buffers.");
	start
		exit_on( ERROR );
		start_path = malloc( PATH_MAX );
		curr_path = malloc( PATH_MAX );
		new_path = malloc( PATH_MAX );
	finis

	abstract("Chdir to the full path for the current working directory.");
	start
		get_path( start_path );
		do_chdir( start_path );
		chk_path( start_path, curr_path, 0 );
	finis

	abstract("Chdir to '.'.");
	start
		get_path( start_path );
		do_chdir( "." );
		chk_path( start_path, curr_path, 0 );
	finis

	
	abstract("Chdir to an existing directory.");
	start
		get_path( start_path );
		strcpy( new_path, data_path() );
		new_path[ strlen( new_path ) - 1 ] = '\0'; /* rm trailing '/' */
		munch_path( start_path, new_path );
		do_chdir( new_path );
		chk_path( start_path, curr_path, 0 );
	finis

	abstract("Chdir along a path containing a non-directory component.");
	abstract("Verify chdir fails and sets errno to ENOTDIR.");
	start
		get_path( start_path );
		expecting( ENOTDIR );
		do_chdir( BAD_PATH );
		chk_path( start_path, curr_path, 1 );
	finis

	abstract("Chdir to a non-existent directory.");
	abstract("Verify chdir fails and sets errno to ENOENT.");
	start
		get_path( start_path );
		expecting( ENOENT );
		do_chdir( MISSING_DIR );
		chk_path( start_path, curr_path, 1 );
	finis

	testing("Chdir to an access denied directory.");
	testing("directory mode 0.");
	abstract("Verify chdir fails and sets errno to EACCES.");
	start
		get_path( start_path );
		expecting( EACCES );
		do_chdir( PROT_DIR );
		chk_path( start_path, curr_path, 1 );
	finis

	testing("Chdir to an access denied directory.");
	testing("directory mode 0677.");
	abstract("Verify chdir fails and sets errno to EACCES.");
	start
		get_path( start_path );
		expecting( EACCES );
		do_chdir( NOOWNER_DIR );
		chk_path( start_path, curr_path, 1 );
	finis

	testing("Chdir to an access denied directory.");
	testing("directory mode 0767, owner = bin.");
	abstract("Verify chdir fails and sets errno to EACCES.");
	start
		get_path( start_path );
		expecting( EACCES );
		do_chdir( NOGROUP_DIR );
		chk_path( start_path, curr_path, 1 );
	finis

	testing("Chdir to an access denied directory.");
	testing("directory mode 776, owner and group = bin.");
	abstract("Verify chdir fails and sets errno to EACCES.");
	start
		get_path( start_path );
		expecting( EACCES );
		do_chdir( NOALL_DIR );
		chk_path( start_path, curr_path, 1 );
	finis



	cleanup("Free path buffers.");
	start
		free( start_path );
		free( curr_path );
		free( new_path );
	finis

	done();
	/*NOTREACHED*/
}
get_path( pth )
char *pth;
{
	prep("Get the full path for the current working directory.");
	start
		exit_on( ERROR );
		expecting( pth );
		getcwd( pth, PATH_MAX );
	finis
}
chk_path( exp_path, curr_path, flg )
char *exp_path;
char *curr_path;
int flg;
{
	if( flg ) {
		testing("Call getcwd to verify that the chdir failure has not");
		testing("changed the current working directory with getcwd.");
	}
	else {
		testing("Call getcwd to verify that chdir has changed to the");
		testing("correct directory");
	}
	start
		expecting( curr_path );
		getcwd( curr_path, PATH_MAX );
		expecting( VOID );
		if( strcmp( curr_path, exp_path ))
			error("%s\nexpected:	'%s'\nreturned:	'%s'\n",
			  "getcwd returned unexpected path",
			  exp_path,
			  curr_path);
	finis
}
do_chdir( pth )
char *pth;
{
	testing("Call chdir(\"%s\").", pth );
	start
		chdir ( pth );
	finis
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
