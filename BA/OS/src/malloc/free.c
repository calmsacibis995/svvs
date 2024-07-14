#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif


#ident	"@(#)free.c	1.3"


/*
** free
**	Test the free function.
**
**
*/

main()
{

	unsigned	mem_limit;
	unsigned	find_limit();
	void	free();
	char	*malloc();
	char	*ptr;


	setup("malloc");
	funct("free");

	abstract("Allocating the largest block of memory (implementation dependent) possible.");
	abstract("Will malloc and free the space several times.");
	abstract("Expect at least the same amount of memory to be available.");
	start
		prep("Determine largest malloc.");
		start
			mem_limit = find_limit();
		finis

		testing("Allocating memory.");
		start
			expecting ( VOID );
			ptr = malloc(mem_limit);

			if ( ptr == (char *)0 )
			{
				error("Malloc could not re-allocate previous space.");	/* ERROR */
			}

		finis

		testing("Freeing memory.");
		start
			free(ptr);
		finis


		testing("Allocating memory.");
		start
			expecting ( VOID );
			ptr = malloc(mem_limit);

			if ( ptr == (char *)0 )
			{
				error("Malloc could not re-allocate previous space.");	/* ERROR */
			}
		finis

		testing("Freeing memory.");
		start
			free(ptr);

		finis

	finis
	done();
}

unsigned	find_limit()
{
	char	*ptr;
	unsigned hbit, i, j;

	j = 0;
	for(hbit=((unsigned)(USI_MAX)>>1)+1;hbit;hbit=(ptr)?(i>>1):(i>>2)){
		expecting ( VOID );
		for(i=1;(ptr=malloc(i|j))&&(free(ptr), 1)&&(i<hbit);i<<=1);
		j |= (ptr) ? i : i >> 1;
	}
	return(j);
}
