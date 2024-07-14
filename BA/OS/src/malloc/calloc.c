#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#ident	"@(#)calloc.c	1.5"

# define	PATTRN_SKIP	10240

/*
** calloc
**	Test the calloc function.
**
*/

main()
{

	unsigned	mem_limit;
	unsigned	find_limit();
	unsigned	i;

	void	free();

	char	*calloc();
	char	*malloc();
	char	*ptr;
	char	*ptr2;


	setup("malloc");
	funct("calloc");

	abstract("Using calloc, get biggest possible block of mem...nelem=mem_limit/16, elsize=16.");
	abstract("This may take a long time.");
	abstract("Expect a return value of non-NULL.");

	start
		prep("Determine largest malloc.");
		start
		mem_limit = find_limit();
		finis

	testing("Call calloc to reserve that memory.");
	testing("But do it in mem_limit/16 chunks."); /* this should be  a prime*/
	start
		expecting ( NON_NULL );
		ptr = calloc(16, mem_limit/16);
	finis

	testing("Read various locations from the allocated memory.");
	testing("Memory should be set to zeros.");
	testing("This may take a long time.");
	testing("Hoping not to encounter any address severrs.");
	start

		ptr2 = ptr;
		for (i = 1; i <= mem_limit; i += PATTRN_SKIP, ptr += PATTRN_SKIP)
		{
			if ( *ptr != (char) 0 )
	{
severr("Allocated memory location does not match the expected pattern.\nThe expected value was 'a'.\nThe actual value was %c.\n", *ptr);	/* HALT */
	}
		}

	finis

	cleanup("Free the calloc'd memory.");
	start
		ptr = ptr2;
		free(ptr);
	finis

	finis

	abstract("Allocating 2 separate memory regions.");
	abstract("Expecting the virtual address of the two regions not to overlap.");
	start

	testing("Calloc the first region.");
	start
	expecting ( NON_NULL );
	ptr = calloc(mem_limit / 4, 1);
	finis

	testing("Calloc the second region.");
	start
	expecting ( NON_NULL );
	ptr2 = calloc(mem_limit / 4, 1);
	finis

	testing("Do some arithmetic on the virtual addresses.");
	start
	if ( ptr2 > ptr )
		if ( ptr + (mem_limit / 4) > ptr2 )
	{
error("First memory region overlaps the second region.");	/* ERROR */
	}

	if ( ptr > ptr2 )
		if ( ptr2 + (mem_limit / 4) > ptr )
	{
error("Second memory region overlaps the first region");	/* ERROR */
	}
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
