/*LINTLIBRARY*/
/*
 * z_ptrace.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ptrace.c	1.2"
#endif

#include <sv_env.h>

#ifndef ptrace
int ptrace();
#endif

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%d", *val );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}

int *sv_geti();

static struct ftable Tptrace = {
	"ptrace",
	"not defined",
	"not defined",
	format,
	compare,
	(ifunctptr)0,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	1,
};

int  Zptrace(request, pid, addr, data)
int request;
int pid;
char *addr;
int data;
{
	int ret_val;

	errno = 0;
	ret_val = ptrace(request, pid, addr, data);
	evaluate( &Tptrace, (pointer)&ret_val );
	return( ret_val );
}

