/*LINTLIBRARY*/
/*
 *	EMACS_MODES: tabstop=4
 */
#include <setjmp.h>
#include <svvs.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_act.c	1.7"
#endif

char	*strcat(),*strcpy();
void	exit();

static int exit_val = 0;

sv_act( severity, action, jmp )
int severity;
int action;
jmp_buf jmp;
{
	char	path[512];
#ifdef DEBUG
	(void ) printf("sv_act() ");
#endif
	if( exit_val < severity ) sv_set_exit_val( severity );
	switch( action ) {

		case ABORT:
			sv_printf("\tABORT\n");
			sv_flush();
			(void ) strcpy(path, SVVSPATH );
			(void ) strcat(path, "/bin/sv_abort");
			(void ) execlp(path, "sv_abort", SVVSPATH, (char *)0 );
			sv_printf("SEVERR:\n\texec of abort process failed\n\tEXIT\n");
			sv_done();
		case EXIT:
			sv_printf("\tEXIT\n");
			sv_done();
		case BREAK:
			sv_printf("\tBREAK\n");
			sv_flush();
			longjmp( jmp, severity );
		case CONTINUE:
			sv_flush();
			return( severity );
		default:
#ifdef	DEVELOPMENT
			sv_printf("WARNING:\n\tsv_act called with unknown action\n\tEXIT\n");
			sv_flush();
			exit(1);
#endif
			return( WARNING );
	}
}
sv_done() {
	char tname[64];

	sv_flush();
	sprintf(tname,"%s/tmp/%s/pid.%d",SVVSPATH,SUBTMP,getpid());
	unlink( tname );
#ifdef SV_IPC
	sprintf(tname,"%s/tmp/%s",SVVSPATH,SV_IPCFILE);
	unlink( tname );
#endif
	exit( exit_val );
}						
sv_set_exit_val( x )
int x;
{
	exit_val = x;			
}
