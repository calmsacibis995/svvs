#include	<sv_base.h> /*replacement macros */
#include	<search.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)lsearch.c	1.3"


# define	TABLESIZE       130
# define	PADDING		100

/*
** lsearch
**	Test the lsearch function.
*/

int		Items[TABLESIZE+PADDING];

extern		char	*lsearch();

# ifndef	SYSVR_1
extern		char	*lfind();
# endif	/* Not SYSVR_1 */

main()
{
setup("lsearch");
funct("lsearch");
funct("lfind");
	/*
	** Test with TABLESIZE.
	*/
	init1_table();
        all_elements(TABLESIZE);
	less_than_first(TABLESIZE); /* this will add one to table size */
	bigger_than_last(TABLESIZE+1); /* this will add one to table size */
	in_the_range(TABLESIZE+2); /* this will add one to table size */
        first_elem(TABLESIZE+3); /* this will NOT add one to table size */
        last_elem(TABLESIZE+3); /* this will NOT add one to table size */

	/*
	** Test with nel = TABLESIZE - 2
	*/
	init2_table();
        all_elements(TABLESIZE-2);
	less_than_first(TABLESIZE-2);
	bigger_than_last(TABLESIZE-2+1);
	in_the_range(TABLESIZE-2+2);
        first_elem(TABLESIZE-2+3);
        last_elem(TABLESIZE-2+3);

	/*
	** Test with nel = TABLESIZE - 4
	*/
	init3_table();
        all_elements(TABLESIZE-4);
	less_than_first(TABLESIZE-4);
	bigger_than_last(TABLESIZE-4+1);
	in_the_range(TABLESIZE-4+2);
        first_elem(TABLESIZE-4+3);
        last_elem(TABLESIZE-4+3);

done();
/*NOTREACHED*/
}

/*
** init1_table
**	Call lsearch to create a table with 130 elements
**
*/
init1_table()
{
	int		i;
	char		*found_item;
	int		*iptr;
	unsigned	nelp;
	extern	int	compar();

testing("Using lsearch to create a table of TABLESIZE elements");
testing("should return a pointer to the last element added.");
start

	for ( i = 0; i < TABLESIZE; i++ )
	{
		Items[i] = 2*i + 2;
		nelp = i;
		found_item = lsearch((char *)&Items[i], (char *)Items,
			&nelp, sizeof(Items[i]), compar);
		iptr = (int *) found_item;
		if ( *iptr != Items[i] )
			error("Lsearch did not enter the item correctly");
	}
finis

}

/*
** init2_table
**	Call hsearch to create a table with 128 elements
**
*/
init2_table()
{
	int		i;
	char		*found_item;
	int		*iptr;
	unsigned	nelp;
	extern	int	compar();

testing("Using hsearch to create a table of TABLESIZE-2 = 128  elements");
testing("should return a pointer to the last element added.");
start

	for ( i = 0; i < TABLESIZE-2; i++ )
	{
		Items[i] = 2*i + 2;
		found_item = lsearch((char *)&Items[i], (char *)&Items[0],
			&nelp, sizeof(Items[i]), compar);
		iptr = (int *) found_item;
		if ( *iptr != Items[i] )
			error("Lsearch did not enter the item correctly");
	}
finis
}

/*
** init3_table
**	Call hsearch to create a table with 126 elements
**
*/
init3_table()
{
	int		i;
	char		*found_item;
	int		*iptr;
	unsigned	nelp;
	extern	int	compar();

testing("Using hsearch to create a table of TABLESIZE-4 = 126  elements");
testing("should return a pointer to the last element added.");
start
	for ( i = 0; i < TABLESIZE-4; i++ )
	{
		Items[i] = 2*i + 2;
		found_item = lsearch((char *)&Items[i], (char *)&Items[0],
			&nelp, sizeof(Items[i]), compar);
		iptr = (int *) found_item;
		if ( *iptr != Items[i] )
			error("Lsearch did not enter the item correctly");
	}
finis
}

/*
** all_elements
**     Test lsearch in the table indicated above with a search
**     for all the element in the table. 
**
** Level:
**	This test is in the robustness level (2).
*/
all_elements(size)
int		size;
{
	int		i;
	char		*found_item;
	int		*iptr;
	unsigned	nelp;
	extern	int	compar();


# ifndef	SYSVR_1
testing("Using lfind to find all the elements of the table");
testing("should return not NULL.");
start
	nelp = size;
	for (i = 0; i < size; i++)
	{
		expecting (VOID);
		found_item = lfind((char *)&Items[i], (char *)&Items[0],
			&nelp, sizeof(Items[i]), compar);
		iptr = (int *) found_item;
		if ( *iptr != Items[i] )
			error("Lfind did not find item %d correctly",
				i);
	}
finis

# endif	/* Not SYSVR_1 */

testing("Using lsearch to find all the elements of the table");
testing("should return of not NULL.");
start
	nelp = size;
	for (i = 0; i < size; i++)
	{
		found_item = lsearch((char *)&Items[i], (char *)&Items[0],
			&nelp, sizeof(Items[i]), compar);
		iptr = (int *) found_item;
		if ( *iptr != Items[i] )
			error("Lsearch did not find the item %d correctly",
				i);
	}
finis
}

/*
** less_than_first
**     Test lsearch in the table indicated above with a search
**     for an Item lexically less than the first element in the table.
**
** Level:
**	This test is in the normal level (0).
*/
less_than_first(nelp)
unsigned	nelp;
{
	int		new_item;
	char		*found_item;
	int		*iptr;
	extern	int	compar();

testing("lsearch(a number less than the first element in the table)");
testing("should cause Lsearch to insert the item.");
start
	new_item = 1;
	found_item = lsearch((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	iptr = (int *) found_item;
	if ( *iptr != new_item )
		error("Lsearch did not insert the item correctly");
finis


# ifndef	SYSVR_1
testing("lfind(a number less than the first element in the table)");
testing("should return NULL.");
start
	new_item = 0;
	expecting (VOID);
	found_item = lfind((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	if ( found_item != NULL )
		{
		if ( found_item != (char *)-1 )
			error ("Lfind found a nonexistent item");
		else
			warning ("%s\nlfind returned -1", ADVISORY);
		}
finis
# endif	/* Not SYSVR_1 */
}

/*
** bigger_than_last
**     Test lsearch in the table indicated above with a search
**     for an Item lexically greater than the last element in the table.
**
** Level:
**	This test is in the normal level (0).
*/
bigger_than_last(nelp)
unsigned	nelp;
{
	int		new_item;
	char		*found_item;
	int		*iptr;
	extern	int	compar();

testing("lsearch(a number bigger than the last element in the table)");
testing("should cause Lsearch to insert the item.");
start
	new_item = 265;
	expecting (VOID);
	found_item = lsearch((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	iptr = (int *) found_item;
	if ( *iptr != new_item )
		error("Lsearch did not insert the item correctly");
finis


# ifndef	SYSVR_1
testing("lfind(a number greater than the last element in the table)");
testing("should return NULL.");
start
	new_item = 266;
	expecting (VOID);
	found_item = lfind((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	if ( found_item != NULL )
		if ( found_item != (char *)-1 )
			error ("Lfind found a nonexistent item");
		else
			warning ("%s\nlfind returned -1", ADVISORY);
finis
# endif	/* Not SYSVR_1 */
}

/*
** in_the_range
**     Test lsearch in the table indicated above with a search
**     for an Item in the range of the table but not in the table.
**
** Level:
**	This test is in the normal level (0).
*/
in_the_range(nelp)
unsigned	nelp;
{
	int		new_item;
	char		*found_item;
	int		*iptr;
	extern	int	compar();

testing("lsearch(a number in the range of the table) -- Not in the table");
testing("should cause Lsearch to insert the item.");
start
	new_item = 129;
	expecting (VOID);
	found_item = lsearch((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	iptr = (int *) found_item;
	if ( *iptr != new_item )
		error("Lsearch did not insert the item correctly");
finis

# ifndef	SYSVR_1
testing("lfind(a number in the range of the table)  -- not in the table");
testing("should return NULL.");
start
	new_item = 127;
	expecting (VOID);
	found_item = lfind((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	if ( found_item != NULL )
		if ( found_item != (char *)-1 )
			error ("Lfind found a nonexistent item");
		else
			warning ("%s\nlfind returned -1", ADVISORY);
finis
# endif	/* Not SYSVR_1 */
}

/*
** last_elem
**     Test lsearch in the table indicated above with a search
**     for the last element in the table. 
**
** Level:
**	This test is in the boundary level (1).
*/
last_elem(nelp)
unsigned	nelp;
{
	int		new_item;
	char		*found_item;
	int		*iptr;
	extern	int	compar();

testing("lsearch(the last element of the table)");
testing("should find the item.");
start
	new_item = 129;
	expecting (VOID);
	found_item = lsearch((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	iptr = (int *) found_item;
	if ( *iptr != new_item )
		error("Lsearch did not find the item");
finis

# ifndef	SYSVR_1
testing("lfind(the last element in the table)");
testing("should return the last item in the table.");
start
	new_item = 129;
	expecting (VOID);
	found_item = lfind((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	if ( found_item == NULL )
		error ("Lfind did not find the last element in the table.");
finis
# endif	/* Not SYSVR_1 */
}

/*
** first_elem
**     Test lsearch in the table indicated above with a search
**     for the first element in the table. 
**
** Level:
**	This test is in the boundary level (1).
*/
first_elem(nelp)
unsigned	nelp;
{
	int		new_item;
	char		*found_item;
	int		*iptr;
	extern	int	compar();

testing("lsearch(the first element of the table)");
testing("should find the item.");
start
	new_item = 2;
	found_item = lsearch((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	iptr = (int *) found_item;
	if ( *iptr != new_item )
		error("Lsearch did not find the item");
finis

# ifndef	SYSVR_1
testing("lfind(the first element in the table)");
testing("should return the first item in the table.");
start
	new_item = 2;
	expecting (VOID);
	found_item = lfind((char *)&new_item, (char *)&Items[0],
		&nelp, sizeof(Items[0]), compar);
	if ( found_item == NULL )
		error ("Lfind did not find the first element in the table.");
finis
# endif	/* Not SYSVR_1 */
}
/*
** compar()
**	compare the two elements for equality.
**
*/
compar(a, b)
char		*a;
char		*b;
{
	int	*i,*j;

	i = (int *) a;
	j = (int *) b;

	if ( *i == *j )
		return(0);
	else
		return(1);
}
