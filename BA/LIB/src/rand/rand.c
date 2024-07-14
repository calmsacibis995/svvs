#include	<sv_base.h> /*replacement macros */
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	MAXCALL		300
# define	NEWSEED		300

#ident	"@(#)rand.c	1.2"


/*
** rand
**	Test the rand function.
*/
main()
{
	int	ret_val;
	int	our_ret_val;
	int	i;
	
	extern	void	srand();
	extern	int	rand();
	void		our_srand();
	int		our_rand();

setup("rand");
funct("rand");
funct("srand");


testing("rand() called 300 times should return");
testing("a random value same as what our_rand returns.");
start
	for (i = 0; i <MAXCALL; i++)
	{
		if ( (our_ret_val = our_rand()) != (ret_val = rand()) )
		{
			error ("rand returned %d, %d was expected",
				ret_val, our_ret_val);
		}
	}
finis


testing("After changing the seed, rand() called 300 times should return");
testing("a random value same as what our_rand returns.");
start
	/*
	** reset the seed using srand and our_srand
	*/
	srand(NEWSEED);
	our_srand(NEWSEED);

	for (i = 0; i <MAXCALL; i++)
	{
		if ( (our_ret_val = our_rand()) != (ret_val = rand()) )
		{
			error ("rand returned %d, %d was expected",
				ret_val, our_ret_val);
		}
	}
finis


testing("after setting the seed back to 1,");
testing("rand() called 300 times should return");
testing("a random value same as what our_rand returns.");
start
	/*
	** Set the seed back to 1 using srand and our_srand
	*/
	srand(1);
	our_srand(1);

	for (i = 0; i <MAXCALL; i++)
	{
		if ( (our_ret_val = our_rand()) != (ret_val = rand()) )
		{
			error ("rand returned %d, %d was expected",
				ret_val, our_ret_val);
		}
	}
finis


done();
/*NOTREACHED*/

}

/*
** our_rand
**	This function is a duplicate of rand in the 5.2 definition
**
*/
static	unsigned long 	next = 1;
int our_rand()
{
	next = next * 1103515245 + 12345;
	return((unsigned int)(next/65536) % 32768);
}

/*
** our_srand()
**	This function resets the random number generator seed.
**	It is a duplicate of the srand() function in 5.2 definition.
**
*/
void our_srand(seed)
unsigned	int	seed;
{
	next = seed;
}
