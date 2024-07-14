#include <sv_conf.h>

#ifdef	UNDEF
#ident	"@(#)find_limit.c	1.2"
#endif

unsigned	find_limit()
{
	char	*ptr;
	unsigned hbit,i,j;

	char	*malloc();
	void	free();

	j = 0;
	for(hbit=((unsigned)(USI_MAX)>>1)+1;hbit;hbit=(ptr)?(i>>1):(i>>2)){
		for(i=1;(ptr=malloc(i|j))&&(free(ptr),1)&&(i<hbit);i<<=1);
		j |= (ptr) ? i : i >> 1;
	}
	return(j);
}
