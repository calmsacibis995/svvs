/*LINTLIBRARY*/
#include	<string.h>
#include	<sys/types.h>
#include	<svvs.h>

#ifdef	UNDEF
#ident	"@(#)find.c	1.3"
#endif

/*
** find
**	Try and find a substring.
**
** Returns
**	0 if it is found
**	1 if it isn't there.
*/
find(buf,pattern)
char	*buf, *pattern;
{
#ifndef	SYSVR_2
	size_t	len;
#else
	int	len;
#endif
	extern	strlen();
	extern	strncmp();

	len = strlen(pattern);
	while ( *buf )
		if (!strncmp(buf++,pattern,len))
			return ( 0 );
	return ( 1 );
}
