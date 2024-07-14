/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)init_buf.c	1.2"
#endif

void init_buf(buf)
char *buf;
{
	int 		i;
	long 		rnd;
	char 		c;

	rnd = 17l;
	for ( i = 0; i < BUFSIZ ; i++ )
	{
		do {
			c = ((rnd = rnd * 1103515245L + 12345)>>16) & 0x7f;
		} while ( c < 0x20 || c > 0x7e ) ;
		buf[i] = c;
	}
}
