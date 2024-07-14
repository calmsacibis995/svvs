/*
 *	grp_stdout.c
 *
 *	used by command tests to verify output content to stdout/stderr
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)grp_stdout.c	1.2"

#include <sv_base.h>
#include <stdio.h>	
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

char *sv_stdoutfile();
char *sv_stderrfile();
char *sv_cmdname();

static char buf[1024];
FILE *fp;
FILE *temp_fopen();

static grp_stdfile( file, strname, target )
char *file;
char *strname;
char *target;
{
	int		l;
	int		found_it;
	char	*p;
	char	*fn = sv_cmdname();
#ifdef	DEVELOPMENT
	if( !*fn || *fn == ' ' )
		warning("grp_%s called before command invocation.",strname);
#endif
	testing("Verify '%s' wrote '%s' to %s.",fn,target,strname);
	start
#ifdef	DEVELOPMENT
		if( !*file ) {
			warning("grp_%s called with uninitialized file name.",strname);
			return;
		}
#endif
		expecting( SUCCESS );
		fp = temp_fopen(strname,"r");
		expecting( VOID );
		l = strlen( target );
		found_it = 0;
		while( fgets( buf, 1024, fp ) && !found_it ) {
			p = buf;
			while( *p && !found_it ) {
				if( !strncmp( p, target, l ) )
					found_it = 1;
				p++;
			}
		}
		fclose( fp );
		if( !found_it ) 
			error("'%s' did not write '%s' to %s",fn,target,strname);
	finis
}
grp_stdout( target )
char *target;
{
	grp_stdfile( sv_stdoutfile(), "stdout", target );
}
grp_stderr( target )
char *target;
{
	grp_stdfile( sv_stderrfile(), "stderr", target );
}
