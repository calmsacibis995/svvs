#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	MINNEL		504
# define	AVGNEL		512
# define	MAXNEL		520


#ident	"@(#)qsort.c	1.3"



/*
** Test the qsort function.
*/

	long		QS_Tb1[MAXNEL];
	long		*QS_Tb2[MAXNEL];
	struct	QS_st
	{
		long	qs_elem1;
		int	qs_elem2[32];
	};
	struct	QS_st	*QS_Tb3[MAXNEL];

	extern		qs_comp();
	extern		qs2_comp();
	extern		qs3_comp();
	extern		qs4_comp();

main()
{
	int		i;
	extern	char	*malloc();

setup("qsort");
funct("qsort");

prep ("initialize the tables");
start
	/*
	** First initialize the 3 tables which will be used 
	** in the following tests.  These 3 are tables of 520
	** elements of :  long, pointers to long, and pointers
	** to 65 byte structures.
	*/

	for (i=0; i< MAXNEL; i++)
	{
		QS_Tb1[i] = 2*i;
		QS_Tb2[i] = &QS_Tb1[i];
		if ( (QS_Tb3[i] = (struct QS_st *)malloc(sizeof (struct QS_st))) == (struct QS_st *) 0 )
			error ("malloc of QS_Tb3 structure failed");
		QS_Tb3[i]->qs_elem1 = QS_Tb1[i];
	}
finis

testing("test qsort with an unsorted table");
start
	sort_table();
finis

testing ("Test qsort with 2 different functions");
start
	resort_table();
finis

testing ("Test qsort with a sorted table");
start
	sort_sorted();
finis

testing ("Reverse sort a table and check to see if it is sorted.");
start
	sort_reverse();
finis

done();
/*NOTREACHED*/
}


/*
** sort_table
**	Test qsort with an unsorted table -- check to see if the 
**	table is properly sorted.  There are three different tables
**	and 3 different nel values (512, 520, and 504).  This function
**	tests all 9 cases.
**
** Level:
**	This test is in the normal level (0).
*/
sort_table()
{

	int		i;

testing("Given a table of long and nel=504, qsort should sort it");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort((char *)&QS_Tb1[0], MINNEL, sizeof(long), qs_comp);
	for (i = 0; i < (MINNEL-1); i++)
	{
		if ( QS_Tb1[i] > QS_Tb1[i+1] )
		{
			error ("qsort did not sort the table of long with nel=504 correctly");
		}

	}
finis


testing("Given a table of long with nel=512, qsort should sort it,");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort((char *)&QS_Tb1[0], AVGNEL, sizeof(long), qs_comp);
	for (i = 0; i < (AVGNEL-1); i++)
	{
		if ( QS_Tb1[i] > QS_Tb1[i+1] )
		{
			error ("qsort did not sort the table of long with \
nel=512 correctly");
		}
	}
finis


testing("Given a table of long with nel=520, qsort should sort it,");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort((char *)&QS_Tb1[0], MAXNEL, sizeof(long), qs_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb1[i] > QS_Tb1[i+1] )
		{
			error ("qsort did not sort the table of long with \
nel=520 correctly");
		}
	}
finis


testing("Given a table of ptrs to long with nel=504, qsort should sort it,");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort((char *)&QS_Tb2[0], MINNEL, sizeof(long *), qs2_comp);
	for (i = 0; i < (MINNEL-1); i++)
	{
		if ( QS_Tb2[i] > QS_Tb2[i+1] )
		{
			error ("qsort did not sort the table of pointer to \
long with nel=504 correctly");
		}
	}
finis


testing("Given a table of ptrs to long with nel=512, qsort should sort it,");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort((char *)&QS_Tb2[0], AVGNEL, sizeof(long *), qs2_comp);
	for (i = 0; i < (AVGNEL-1); i++)
	{
		if ( QS_Tb2[i] > QS_Tb2[i+1] )
		{
			error ("qsort did not sort the table of pointer \
to long with nel=512 correctly");
		}
	}
finis


testing("Given a table of ptrs to long with nel=520, qsort should sort it,");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort((char *)&QS_Tb2[0], MAXNEL, sizeof(long *), qs2_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb2[i] > QS_Tb2[i+1] )
		{
			error ("qsort did not sort the table of pointer to \n\
long with nel=520 correctly");
		}
	}
finis



testing("Given a table of ptr to struct with nel=504, qsort should sort it,");
testing("yielded a sorted table.");
start
	expecting (VOID);
	qsort(&QS_Tb3[0], MINNEL, sizeof(struct QS_st *), qs3_comp);
	for (i = 0; i < (MINNEL-1); i++)
	{
		if ( QS_Tb3[i]->qs_elem1 > QS_Tb3[i+1]->qs_elem1 )
		{
			error ("qsort did not sort the table of pointer to \n\
struct with nel=504 correctly");
		}
	}
finis



testing("Given a table of ptr to struct with nel=512, qsort should sort it,");
testing("yielded a sorted table.");
start
	expecting (VOID);
	qsort(&QS_Tb3[0], AVGNEL, sizeof(struct QS_st *), qs3_comp);
	for (i = 0; i < (AVGNEL-1); i++)
	{
		if ( QS_Tb3[i]->qs_elem1 > QS_Tb3[i+1]->qs_elem1 )
		{
			error ("qsort did not sort the table of pointer to \n\
struct with nel=512 correctly");
		}
	}
finis


testing("Given a table of ptr to struct with nel=520, qsort should sort it,");
testing("yielding a sorted table.");
start
	expecting (VOID);
	qsort(&QS_Tb3[0], MAXNEL, sizeof(struct QS_st *), qs3_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb3[i]->qs_elem1 > QS_Tb3[i+1]->qs_elem1 )
		{
			error ("qsort did not sort the table of pointer to \n\
struct with nel=520 correctly");
		}
	}
finis

}


/*
** resort_table
**	Test qsort with 2 different functions -- check to see if the 
**	table is properly sorted. The table is one with 520 long 
**	elements.
**
** Level:
**	This test is in the normal level (0).
*/
resort_table()
{
	int		i;

testing("Given a table of long with nel=520, qsort should sort it");
testing("with two different compare functions,");
testing("yielding a sorted table.");
start
	/*
	** First sort the table with the function qs_comp
	*/
	expecting (VOID);
	qsort((char *)&QS_Tb1[0], MAXNEL, sizeof(long), qs_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb1[i] > QS_Tb1[i+1] )
		{
			error ("qsort did not sort the table of long with \n\
nel=520  and first compare function correctly");
		}
	}

	/*
	** Next sort the table with the function qs4_comp
	*/
	expecting (VOID);
	qsort((char *)&QS_Tb1[0], MAXNEL, sizeof(long), qs4_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb1[i] > QS_Tb1[i+1] )
		{
			error ("qsort did not sort the table of long with \n\
nel=520  and second compare function correctly");
		}
	}
finis


}




/*
** sort_sorted()
**	Test qsort with a sorted table. check to see if the table
**	is unchanged.  This function performs the test for 3 
**	different tables and for nel values of 520, 512, and 504. 
**
** Level:
**	This test is in the boundary level (1).
*/
sort_sorted()
{
	int		i;

testing("Given a SORTED table of pointer to struct with nel=520,");
testing("qsort should leave it sorted.");
start
	expecting (VOID);
	qsort(&QS_Tb3[0], MAXNEL, sizeof(struct QS_st *), qs3_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb3[i]->qs_elem1 > QS_Tb3[i+1]->qs_elem1 )
		{
			error ("qsort UNsorted the table of pointer to \n\
struct with nel=520");
			break;
		}
	}
finis

}



/*
** sort_reverse()
**	Reverse sort a table and check to see if it is sorted.
**
** Level:
**	This test is in the Robustness level (2).
*/
sort_reverse()
{
	int		i;
	int		j;

testing("Given a reverse sorted table of long with nel=520,");
testing("qsort should sort it.");
start
	/*
	** Reverse sort table of long
	*/
	j = 0;
	for (i=519; i>=0; i--)
	{
		QS_Tb1[j] = 2*i;
		j ++;
	}

	expecting (VOID);
	qsort((char *)&QS_Tb1[0], MAXNEL, sizeof(long), qs_comp);
	for (i = 0; i < (MAXNEL-1); i++)
	{
		if ( QS_Tb1[i] > QS_Tb1[i+1] )
		{
			error ("qsort did not sort the reverse sorted table \n\
of long with nel=520 correctly");
		}
	}
finis


}

/*
** qs_comp()
**	This function is called by qsort with 2 parameters.  This
**	function will:
**	return -1 if the first parm is smaller than the second
**	        0 if the first and the second are equal
**	       +1 if the first is larger than the second
**
**
*/
qs_comp(elem1, elem2)
long 		*elem1;
long		*elem2;
{
	if ( *elem1 == *elem2 )
		return(0);

	if ( *elem1 > *elem2 )
		return(1);

	return(-1);
}

/*
** qs2_comp()
**	This function is called by qsort with 2 parameters.  This
**	function will:
**	return -1 if the first parm is smaller than the second
**	        0 if the first and the second are equal
**	       +1 if the first is larger than the second
**
**
*/
qs2_comp(elem1, elem2)
long 		**elem1;
long		**elem2;
{
	if ( **elem1 == **elem2 )
		return(0);

	if ( **elem1 > **elem2 )
		return(1);

	return(-1);
}

/*
** qs3_comp()
**	This function is called by qsort with 2 parameters.  This
**	function will:
**	return -1 if the first parm is smaller than the second
**	        0 if the first and the second are equal
**	       +1 if the first is larger than the second
**
**
*/
qs3_comp(elem1, elem2)
struct	QS_st 	**elem1;
struct	QS_st	**elem2;
{
	if ( (*elem1)->qs_elem1 == (*elem2)->qs_elem1 )
		return(0);

	if ( (*elem1)->qs_elem1 > (*elem2)->qs_elem1 )
		return(1);

	return(-1);
}
/*
** qs4_comp()
**	This function is called by qsort with 2 parameters.  This
**	function will:
**	return -1 if the first parm is smaller than the second
**	        0 if the first and the second are equal
**	       +1 if the first is larger than the second
**
**
*/
qs4_comp(elem1, elem2)
long 		*elem1;
long		*elem2;
{
	if ( *elem1 == *elem2 )
		return(0);

	if ( *elem1 > *elem2 )
		return(1);

	return(-1);
}
