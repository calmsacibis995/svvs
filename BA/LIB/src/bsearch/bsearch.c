#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)bsearch.c	1.6"


# define        SIZE1           40
# define        SIZE2           25

/*
** bsearch
**	Test the bsearch function.
*/

struct		T65byt{
		char	filler1[SIZE1];
		long	filler2;
		char	filler3[SIZE2];
}; 

extern		char	*bsearch();

#define	EACHTAB	(sizeof (long) + sizeof (long *) + sizeof (struct T65byt) + sizeof (long))
/*  allow a long for padding  */

main()
{
char	*ptr;
int	tablesize;
unsigned	size;
int	i;
char	*tab1, *tab2, *tab3;
extern	char *malloc ();

setup("bsearch");
funct("bsearch");

prep ("allocate the largest tables possible");
start

	exit_on (ERROR | SEVERR);

	size = 30000;
	while ((ptr = malloc (size)) == (char *)(0))
		{
		if (size < 3000)
			error ("cannot allocate enough memory");
		size -= 1000;
		}
	tablesize = size / EACHTAB;

	free (ptr);
	if ((tab1 = malloc (tablesize * sizeof (long))) == (char *)(0))
		error ("cannot malloc tab1\n");
	if ((tab2 = malloc (tablesize * sizeof (long *))) == (char *)(0))
		error ("cannot malloc tab2\n");
	if ((tab3 = malloc (tablesize * sizeof (struct T65byt))) == (char *)(0))
		error ("cannot malloc tab3\n");
finis

doit (tab1, tab2, tab3,tablesize);

done();
/*NOTREACHED*/
}

doit (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	int	i;

	/*
	** Initialize the tables first
	*/

	for (i = 1; i <= tablesize; i++)
	{
		Table1[i-1] = 2*(i);
		Table2[i-1] = &Table1[i-1];
		Table3[i-1].filler2 = Table1[i-1];
	}

	testing("Test cases of bsearch for an item lexically less than");
	testing("the first element in the table.");
	start
		less_than_first(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for an item lexically greater");
	testing("the last element in the table.");
	start
		bigger_than_last(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for an item lexically within");
	testing("the bounds of the first and last elements of the table.");
	start
		in_the_range(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for an item lexically within");
	testing("the bounds of the first quarter of the table.");
	start
		first_quarter(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for an item lexically within");
	testing("the bounds of the last three quarters of the table.");
	start
		last_3quarter(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for the first element in the table.");
	start
		first_elem(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for the last element in the table.");
	start
		last_elem(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch for the center element in the table.");
	start
		center_elem(Table1, Table2, Table3, tablesize);
	finis
	testing("Test cases of bsearch covering all elements in the table.");
	start
		all_elements(Table1, Table2, Table3, tablesize);
	finis
}

/*
** less_than_first
**     Test bsearch in all three tables indicated above with a search
**     for an item lexically less than the first element in the table.
*/
less_than_first (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long		data1;
	long		*data2;
	struct	T65byt	data3;
	char		*ret_val;
	extern	int	Table1_comp();
	extern	int	Table2_comp();
	extern	int	Table3_comp();

testing("bsearch called with a number less than the ");
testing("first element in the table of longs & nel = TABLESIZE.");
testing("should return NULL.");
start
	data1 = 0;
	expecting (NULL);

	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	    (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with a number less than the ");
testing("first element in the table of pointers with a nel = TABLESIZE.");
testing("should return NULL.");
start
	data1 = Table1[0] - 1;
	data2 = &data1;
	expecting (NULL);

	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize,
	    (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with a number less than the ");
testing("first element in the table of structs >65 bytes with a");
testing("nel = TABLESIZE.  should return NULL.");
start
	data3.filler2 = 0;
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing(" bsearch called with a number less than the first element in");
testing ("the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return NULL.");
start
	data3.filler2 = 0;
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number less than the first element in");
testing("the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return NULL.");
start
	data3.filler2 = 0;
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


}

/*
** bigger_than_last
**     Test bsearch in all three tables indicated above with a search
**     for an item lexically greater than the last element in the table.
*/
bigger_than_last (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long		 	data1;
	long			*data2;
	struct	T65byt    	data3;
	char			*ret_val;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with a number greater than the ");
testing("last element in the table of longs with a");
testing("nel = TABLESIZE.  should return NULL.");
start
	data1 = 2*(tablesize+1);
	expecting (NULL);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	    (unsigned)sizeof(long), Table1_comp);
finis

testing("bsearch called with a number greater than the ");
testing("last element in the table of pointers with a");
testing("nel = TABLESIZE. should return NULL.");
start
	data1 = Table1[tablesize-1] + 1;
	data2 = &data1;
	expecting (NULL);
	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize,
	    (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with a number greater than the ");
testing("last element in the table of structs >65 bytes with a");
testing("nel = TABLESIZE. should return NULL.");
start
	data3.filler2 = 2*(tablesize+1);
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number greater than the last element ");
testing("in the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return NULL.");
start
	data3.filler2 = 2*(tablesize-7);
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number greater than the last element ");
testing("in the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return NULL.");
start
	data3.filler2 = 2*(tablesize - 15);
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis

}

/*
** in_the_range
**     Test bsearch in all three tables indicated above with a search
**     for an item in the range of the tables but not in the table.
*/
in_the_range (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	long			*data2;
	struct	T65byt		data3;
	char			*ret_val;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with a number in the range of the ");
testing("table, but with the item not in the table.");
testing("with a table of longs with a nel = TABLESIZE.");
testing("should return NULL.");
start
	data1 = 5;
	expecting (NULL);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	   (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with a number in the range of the ");
testing("table, but with the item not in the table.");
testing("with a table of pointers with a nel = TABLESIZE.");
testing("should return NULL.");
start
	data2 = &data1;
	*data2 = 7;
	expecting (NULL);
	bsearch((char *)&data2, (char *)Table2,
	    (unsigned)tablesize, (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with a number in the range of the ");
testing("table, but with the item not in the table.");
testing("with a table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return NULL.");
start
	data3.filler2 = 5;
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the range of the ");
testing("table, but with the item not in the table.");
testing("with a table of structs >65 bytes with a");
testing("nel = (TABLESIZE-8). should return NULL.");
start
	data3.filler2 = 5;
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the range of the ");
testing("table, but with the item not in the table.");
testing("with a table of structs >65 bytes with a");
testing("nel = (unsigned)(TABLESIZE-16). should return NULL.");
start
	data3.filler2 = 5;
	expecting (NULL);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis

}

/*
** first_quarter
**     Test bsearch in all three tables indicated above with a search
**     for an item in the first 1/4 of the table. 
*/
first_quarter (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	long			*data2;
	struct	T65byt		data3;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with a number in the first 1/4");
testing("of the table of longs with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data1 = 2*((tablesize/4) - 4);
	expecting (SUCCESS);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	    (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with a number in the first 1/4");
testing("of the table of pointers with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data2 = &Table1[tablesize/4];
	*data2 -= 4; 
	expecting (SUCCESS);
	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize,
	    (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with a number in the first 1/4");
testing("of the table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data3.filler2 = 2*((tablesize/4) - 4);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the first 1/4");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return not NULL.");
start
	data3.filler2 = 2*((tablesize/4) - 4);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the first 1/4");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return not NULL.");
start
	data3.filler2 = 2*((tablesize/4) - 4);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


}

/*
** last_3quarter
**     Test bsearch in all three tables indicated above with a search
**     for an item in the last  3/4 of the table. 
*/
last_3quarter (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	long			*data2;
	struct	T65byt		data3;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with a number in the last  3/4");
testing("of the table of longs with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data1 = 2*((tablesize/4) + 4);
	expecting (SUCCESS);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	    (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with a number in the last 3/4");
testing("of the table of pointers with a nel = TABLESIZE.");
testing("should return  not NULL.");
start
	data2 = &Table1[tablesize/4];
	*data2 += 4; 
	expecting (SUCCESS);
	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize, (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with a number in the last 3/4");
testing("of the table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return  not NULL.");
start
	data3.filler2 = 2*((tablesize/4) + 4);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the last 3/4");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return  not NULL.");
start
	data3.filler2 = 2*((tablesize/4) + 4);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the last 3/4");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return  not NULL.");
start
	data3.filler2 = 2*((tablesize/4) + 4);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis

}

/*
** last_elem
**     Test bsearch in all three tables indicated above with a search
**     for the last element in the table. 
*/
last_elem (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	long			*data2;
	struct	T65byt		data3;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with the last element");
testing("of the table of longs with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data1 = 2*(tablesize);
	expecting (SUCCESS);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	    (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with a number in the last element");
testing("of the table of pointers with a nel = TABLESIZE.");
testing("should return not  NULL.");
start
	data2 = &(Table1[tablesize-1]);
	expecting (SUCCESS);
	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize,
	    (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with a number in the last element");
testing("of the table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return not  NULL.");
start
	data3.filler2 = 2*(tablesize);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with a number in the last element");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return not NULL.");
start
	data3.filler2 = 2*(tablesize - 8);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);

finis

testing("bsearch called with a number in the last element");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return not NULL.");
start
	data3.filler2 = 2*(tablesize - 16);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16), (unsigned)sizeof(struct T65byt), Table3_comp);
finis


}

/*
** first_elem
**     Test bsearch in all three tables indicated above with a search
**     for the first element in the table. 
*/
first_elem (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	long			*data2;
	struct	T65byt		data3;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with the first element");
testing("of the table of longs with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data1 = 2;
	expecting (SUCCESS);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize, (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with the first element");
testing("of the table of pointers with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data2 = &(Table1[0]);
	expecting (SUCCESS);
	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize,
	    (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with the first element");
testing("of the table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data3.filler2 = 2;
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with first element");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return not NULL.");
start
	data3.filler2 = 2;
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with the first element");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return not NULL.");
start
	data3.filler2 = 2;
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis

}

/*
** center_elem
**     Test bsearch in all three tables indicated above with a search
**     for the center element in the table. 
*/
center_elem (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	long			*data2;
	struct	T65byt		data3;
	extern	int		Table1_comp();
	extern	int		Table2_comp();
	extern	int		Table3_comp();

testing("bsearch called with the center element");
testing("of the table of longs with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data1 = 2*(tablesize/2);
	expecting (SUCCESS);
	bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
	    (unsigned)sizeof(long), Table1_comp);
finis


testing("bsearch called with the center element");
testing("of the table of pointers with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data2 = &(Table1[tablesize/2 - 1]);
	expecting (SUCCESS);
	bsearch((char *)&data2, (char *)Table2, (unsigned)tablesize,
	    (unsigned)sizeof(long *), Table2_comp);
finis


testing("bsearch called with the center element");
testing("of the table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	data3.filler2 = 2*(tablesize/2);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with center element");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-8).");
testing("should return not NULL.");
start
	data3.filler2 = 2*(tablesize/2);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-8),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


testing("bsearch called with the center element");
testing("of the table of structs >65 bytes with a nel = (TABLESIZE-16).");
testing("should return not NULL.");
start
	data3.filler2 = 2*(tablesize/2);
	expecting (SUCCESS);
	bsearch((char *)&data3, (char *)Table3, (unsigned)(tablesize-16),
	    (unsigned)sizeof(struct T65byt), Table3_comp);
finis


}

/*
** all_elements
**     Test bsearch in all three tables indicated above with a search
**     for the center element in the table. 
*/
all_elements (Table1, Table2, Table3, tablesize)
long	Table1[];
long	*Table2[];
struct	T65byt	Table3[];
int	tablesize;
{
	long			data1;
	struct	T65byt		data3;
	int			i;
	extern	int		Table1_comp();
	extern	int		Table3_comp();

testing("bsearch called to find all the elements");
testing("of the table of longs with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	expecting (SUCCESS);
	for (i=1; i<=tablesize; i++)
	{
		data1 = 2*(i);
		bsearch((char *)&data1, (char *)Table1, (unsigned)tablesize,
		    (unsigned)sizeof(long), Table1_comp);
	}
finis


testing("bsearch called with all the elements");
testing("of the table of structs >65 bytes with a nel = TABLESIZE.");
testing("should return not NULL.");
start
	expecting (SUCCESS);
	for (i = 1; i <= tablesize; i++)
	{
		data3.filler2 = 2*i;
		bsearch((char *)&data3, (char *)Table3, (unsigned)tablesize,
		    (unsigned)sizeof(struct T65byt), Table3_comp);
	}
finis


}

/*
** Table1_comp()
**	This function is the 'compar' function for table 1.
*/
Table1_comp(ptr1, ptr2)
long		*ptr1;
long		*ptr2;
{
	if ( *ptr1 > *ptr2 )
		return(1);
	if ( *ptr1 == *ptr2 )
		return(0);

	return(-1);
}
/*
** Table2_comp()
**	This function is the 'compar' function for table 2.
*/
Table2_comp(ptr1, ptr2)
long		**ptr1;
long		**ptr2;
{
	long	*a,*b;

	a = *ptr1;
	b = *ptr2;
	if ( *a > *b )
		return(1);
	if ( *a == *b )
		return(0);

	return(-1);
}
/*
** Table3_comp()
**	This function is the 'compar' function for table 3.
*/
Table3_comp(ptr1, ptr2)
struct	T65byt	*ptr1;
struct	T65byt	*ptr2;
{
	if ( ptr1->filler2 > ptr2->filler2 )
		return(1);
	if ( ptr1->filler2 == ptr2->filler2 )
		return(0);

	return(-1);
}
