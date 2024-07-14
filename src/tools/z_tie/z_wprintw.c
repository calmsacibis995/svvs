/*LINTLIBRARY*/
/*
 * z_wprintw.c
 */

#ifdef	UNDEF
#ident	"@(#)z_wprintw.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef wprintw
int wprintw();
#endif

static int success( val )
int *val;
{
	return( (*val) == OK );
}

static int failure( val )
int *val;
{
	return( (*val) == ERR );
}

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

static struct ftable Twprintw = {
	"wprintw",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zwprintw( win, fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z )
WINDOW *win;
char *fmt;
long a;
long b;
long c;
long d;
long e;
long f;
long g;
long h;
long i;
long j;
long k;
long l;
long m;
long n;
long o;
long p;
long q;
long r;
long s;
long t;
long u;
long v;
long w;
long x;
long y;
long z;
{
	int ret_val;

	errno = 0;
	ret_val = wprintw( win, fmt, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z );
	evaluate( &Twprintw, (pointer)&ret_val );
	return( ret_val );
}

