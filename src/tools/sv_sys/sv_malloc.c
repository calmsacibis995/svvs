/*LINTLIBRARY*/
/*
 * sv_malloc.c - malloc hook, needed so mallopt test can defeat malloc 
 *			calls in config and sv_printf
 */
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_malloc.c	1.1"
#endif

char *sv_malloc( size )
unsigned size;
{
	char *malloc();

	return( malloc( size ));
}
