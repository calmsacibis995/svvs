#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#ident	"@(#)malloc0.c	1.4"

# define	PATTRN_SKIP	10240

/*
** malloc
**	Test the malloc function.
**
**
*/

unsigned	find_limit();
char	*malloc();
void	free();

main()
{

	unsigned	mem_limit;
	unsigned	i;
	void	free();
	char	*malloc();
	char	*pt;
	char	*ptr;
	char	*ptr2;


	setup("malloc");
	funct("malloc");

	abstract("Allocating the largest block of memory (implementation dependent) possible.");
	abstract("Initial return values of non-NULL and an eventual NULL (not enough memory) value when memory can no longer be allocated.");

	start

		prep("Determine malloc high-water mark.");
		start
		mem_limit = find_limit();
		finis

		testing("Call malloc to allocate memory.");
		start
		expecting ( NON_NULL );
		ptr = malloc(mem_limit);
		finis

		pt = ptr;
		if(mem_limit == USI_MAX){
			ptr2 = NULL;

			expecting ( VOID );
			while(ptr = malloc(mem_limit)){
				*((char **)ptr) = ptr2;
				ptr2 = ptr;
			}

			do{
				ptr = *((char **)ptr2);
				free(ptr2);
				ptr2 = ptr;
			}while(ptr2);
		}

	testing("Writing a pattern to the allocated memory.");
	testing("This may take a long time.");
	testing("Hoping not to encounter any address errors.");

	start
		ptr = pt;
		ptr2 = ptr;
		for (i = 1; i <= mem_limit; i += PATTRN_SKIP, ptr += PATTRN_SKIP)
		{
			*ptr = 'a';
		}
		ptr = ptr2;

	finis

	testing("Reading the pattern back from the allocated memory.");
	testing("This may take a long time.");
	testing("Expecting a pattern match and no address errors.");
	start

		ptr2 = ptr;
		for (i = 1; i <= mem_limit; i += PATTRN_SKIP, ptr += PATTRN_SKIP)
		{
			if ( *ptr != 'a' )
			{
				severr("Allocated memory location does not match the expected pattern.\nThe expected value was 'a'.\nThe actual value was %c.\n",*ptr);	/* HALT */
			}
		}


		cleanup("Freeing the allocated memory.");
		start
			ptr = ptr2;
			free(ptr);

		finis
	finis
	finis

	abstract("Allocating 2 separate memory regions.");
	abstract("The virtual address of the two regions not to overlap.");
	start

		testing("Allocating the first region.");
		start
			expecting ( NON_NULL );
			ptr = malloc(mem_limit / 4);

		finis

		testing("Allocating the second region.");
		start
			expecting ( NON_NULL );
			ptr2 = malloc(mem_limit / 4);
		finis

		if ( ptr > ptr2 )
			if ( ptr2 + (mem_limit / 4) > ptr )
			{
				error("First memory region overlaps the second region");
			}

		if ( ptr2 > ptr )
			if ( ptr + (mem_limit / 4) > ptr2 )
			{
				error("Second memory region overlaps the first region");
			}

	finis
	done();
	/*NOTREACHED*/
}
unsigned	find_limit()
{
	char	*ptr;
	unsigned hbit, i, j;

	j = 0;
	expecting ( VOID );
	for(hbit=((unsigned)(USI_MAX)>>1)+1;hbit;hbit=(ptr)?(i>>1):(i>>2)){
		for(i=1;(ptr=malloc(i|j))&&(free(ptr), 1)&&(i<hbit);i<<=1);
		j |= (ptr) ? i : i >> 1;
	}
	return(j);
}
