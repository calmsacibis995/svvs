/*LINTLIBRARY*/

#include	<sv_base.h>
#include	<sv_env.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)cmp_file.c	1.2"
#endif

cmp_file( buf1, buf2 )
char *buf1;
char *buf2;
{
	int i;

	for( i=0; ( i < FILESIZ ) && ( buf1[i] == buf2[i] ) ; i++ )
	{
		;
	}
	return i;
}
