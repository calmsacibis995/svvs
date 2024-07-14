#include	<sv_base.h>

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<signal.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill_me_0.c	1.5"

main() {
	ttime t,time();
	void exit();

	expecting (VOID);
	signal (SIGTERM, SIG_DFL);

	expecting ( VOID );
	t = time( (ttime *)0) + 60;

	expecting ( VOID );
	while( time( (ttime *)0 )  < t );
	exit(0);
}
