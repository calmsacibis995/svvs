
#include	<sv_base.h>

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<signal.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill_me_1.c	1.4"

main() {
	ttime t,time();
	void exit();

	expecting ( VOID );

	if(setgid(BIN_GID)) exit(-1);
	if(setuid(BIN_UID)) exit(-1);

	t = time( (ttime *)0) + 60;
	while( time( (ttime *)0 )  < t );

	exit(0);
}
