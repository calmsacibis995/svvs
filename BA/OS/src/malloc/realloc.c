#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#ident	"@(#)realloc.c	1.4"

# define	PATTRN_SKIP	10240

/*
** realloc
**	Test the realloc function.
**
*/

main()
{

	unsigned	mem_limit;
	unsigned	find_limit();
	unsigned	i;
	void	free();
	char	*malloc();
	char	*realloc();
	char	*ptr;
	char	*ptr2;


	setup("malloc");
	setup("realloc");

	abstract("Allocating the largest block of memory (implementation dependent) possible.");
	abstract("Initial return values of non-NULL and an eventual NULL (not enough memory) value when memory can no longer be allocated.");
	start

		prep("Determine largest malloc.");
		start
			mem_limit = find_limit();
		finis

		testing("Alloc that big block.");
		start
			expecting ( VOID );
			ptr = malloc(mem_limit);
		finis

		testing("Writing data to the allocated memory.");
		testing("This may take a long time.");
		testing("Expecting not to encounter any address errors.");
		start

			ptr2 = ptr;
			for (i = 1; i <= mem_limit; i += PATTRN_SKIP, ptr += PATTRN_SKIP)
			{
				*ptr = 'a';
			}
			ptr = ptr2;

		finis

		testing("Realloc and cut the allocated memory to 1/10 of current value.");
		testing("Expecting a non-NULL return value.");
		start

			mem_limit /= 10;

			expecting ( NON_NULL );
			ptr = realloc(ptr, mem_limit);

		finis

		testing("Read the pattern back from the allocated memory.");
		testing("This may take a long time.");
		testing("Expecting not to encounter any address errors.");
		start

			ptr2 = ptr;
			for (i = 1; i <= mem_limit; i += PATTRN_SKIP, ptr += PATTRN_SKIP)
			{
				if ( *ptr != 'a' )
				{
					severr("Allocated memory location does not match the expected pattern.\nThe expected pattern was %c.\nThe  actual pattern  was %c.\n", 'a', *ptr);	/* HALT */
				}
			}

		finis
		cleanup("Freeing the allocated memory.");
		start
			ptr = ptr2;
			free(ptr);
		finis


	finis
	done();
}

unsigned	find_limit()
{
	char	*ptr;
	unsigned hbit,i,j;

	j = 0;
	expecting ( VOID );
	for(hbit=((unsigned)(USI_MAX)>>1)+1;hbit;hbit=(ptr)?(i>>1):(i>>2)){
		for(i=1;(ptr=malloc(i|j))&&(free(ptr),1)&&(i<hbit);i<<=1);
		j |= (ptr) ? i : i >> 1;
	}
	return(j);
}
