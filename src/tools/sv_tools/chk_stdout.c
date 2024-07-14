/*
 *	chk_stdout.c
 *
 *	used by command tests to verify output to stdout/stderr
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)chk_stdout.c	1.5"

#include <sv_base.h>
#include <stdio.h>	
#include <sys/types.h>
#include <sys/stat.h>
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

char *sv_stdoutfile();
char *sv_stderrfile();
char *sv_cmdname();

struct stat st_buf;
static char buf[1024];
FILE *fp;
FILE *temp_fopen();

static chk_stdfile( flg, file, strname )
int flg;
char *file;
char *strname;
{
	char	*fn = sv_cmdname();
#ifdef	DEVELOPMENT
	if( !*fn || *fn == ' ' )
		warning("chk_%s called before command invocation.",strname);
#endif
	if( flg & 1 ) {
		testing("Verify '%s' wrote to %s.",fn,strname);
		start
#ifdef	DEVELOPMENT
			if( !*file ) {
				warning("chk_%s called with uninitialized file name.",
				  strname);
				return;
			}
#endif
			stat( file, &st_buf );
			if( !st_buf.st_size ) {
				if( flg & 2 )
					warning("%s\n'%s' did not write to %s.",ADVISORY,fn,strname);
				else
					error("'%s' did not write to %s.",fn,strname);
			}
		finis
	}
	else {
		testing("Verify '%s' did not write to %s.",fn,strname);
		start
#ifdef	DEVELOPMENT
			if( !*file ) {
				warning("chk_%s called with uninitialized file name.",
				  strname);
				return;
			}
#endif
			stat( file, &st_buf );
			if( st_buf.st_size ) {
				if( flg & 2 )
					warning("%s\n'%s' wrote to %s: %s",ADVISORY,fn,strname,buf);
				else {
					fp = temp_fopen(strname,"r");
					fgets( buf, 1024, fp);
					fclose( fp );
					error("'%s' wrote to %s: %s",fn,strname,buf);
				}
			}
		finis
	}
}
chk_stdout( flg )
int flg;
{
	chk_stdfile( flg, sv_stdoutfile(), "stdout" );
}
chk_stderr( flg )
int flg;
{
	chk_stdfile( flg, sv_stderrfile(), "stderr" );
}
