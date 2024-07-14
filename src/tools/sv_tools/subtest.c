/*LINTLIBRARY*/
#include <sv_base.h>
#include <sv_env.h>
#include <sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)subtest.c	1.4"
#endif

subtest(file)
char *file;
{
	int cpid;
	int wpid;
	int sts;

	preparation("Fork a subtest");
	start
		severr_on( E_FAILURE );	
		if( !(cpid = fork()) ) {
			exit_on( SEVERR );
			execlp( file, file, (char *)0 );
			severr("execlp of subtest failed.");
		}
		wpid = wait(&sts);
		if ( wpid != cpid ) {
			severr( "wait(): returned unexpected value: %d",wpid );
		}
		else if ( sts ) {
			if( sts & 0177 )
				error("subtest terminated due to signal, exit sts: %d",sts ); 
			else switch( (unsigned)sts >> 8 ) {
			case MESSAGE:
				break;
			case WARNING:
				warning("subtest terminated abnormally");
				break;
			case ERROR:
				error("subtest terminated abnormally");
				break;
			case SEVERR:
				severr("subtest terminated abnormally");
				break;
			default:
				error("subtest terminated with unknown status: %d",sts);
			}

		}
	finis
}	
