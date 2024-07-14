#include	<sv_base.h> /*replacement macros */
#include	<search.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)hsearch.c	1.5"



# define	TABLESIZE       130

/*
** hsearch
**	Test the hsearch function.
*/

ENTRY		Items[TABLESIZE+1];
char		Str_Item[TABLESIZE+1][5];

extern		ENTRY	*hsearch();
extern		void	hdestroy();

main()
{
setup("hsearch");
funct ("hsearch");
funct ("hcreate");
funct ("hdestroy");
testing ("test with TABLESIZE");
start
	/*
	** Test with TABLESIZE.
	*/
	testing("Use hcreate to create a table of TABLESIZE elements");
	testing("then adding TABLESIZE+1 elements should return NULL from hsearch.");
	start
		init1_table();
	finis

	testing("hsearch called to find all the elements of the table.");
	testing("should return not NULL.");
	start
		all_elements(TABLESIZE);
	finis

	testing("hsearch(a number less than the first element in the table)");
	testing("should return NULL.");
	start
		less_than_first();
	finis

	testing("hsearch(a number bigger than the last element in the table)");
	testing("should return NULL.");
	start
		bigger_than_last();
	finis

	testing("hsearch(a number in the range of the table");
	testing("but not in the table) should return NULL.");
	start
		in_the_range();
	finis

	testing("hsearch (the first element of the table)");
	testing("should return not NULL.");
	start
		first_elem();
	finis

	testing("hsearch(the last element of the table) should return not NULL.");
	start
		last1_elem();
	finis

	testing("hsearch(a duplicate entry of the table) should return");
	testing ("a value equal to the date portion of what was passed.");
	start
		duplicate_elem();
	finis
finis

	/*
	** Test with nel = TABLESIZE - 2
	*/
testing ("test with TABLESIZE -2");
start
	testing("Use hcreate to create a table of TABLESIZE-2 = 128  elements.");
	testing("then adding TABLESIZE+1 elements should return NULL from hsearch.");
	start
		init2_table();
	finis

	testing("hsearch called to find all the elements of the table.");
	testing("should return not NULL.");
	start
		all_elements(TABLESIZE-2);
	finis

	testing("hsearch(a number less than the first element in the table)");
	testing("should return NULL.");
	start
		less_than_first();
	finis

	testing("hsearch(a number bigger than the last element in the table)");
	testing("should return NULL.");
	start
		bigger_than_last();
	finis

	testing("hsearch(a number in the range of the table");
	testing("but not in the table) should return NULL.");
	start
		in_the_range();
	finis

	testing("hsearch (the first element of the table)");
	testing("should return not NULL.");
	start
		first_elem();
	finis

	testing("hsearch(the last element of the table of size 128)");
	testing("should return not NULL.");
	start
		last2_elem();
	finis

	testing("hsearch(a duplicate entry of the table) should return");
	testing ("a value equal to the date portion of what was passed.");
	start
		duplicate_elem();
	finis
finis

	/*
	** Test with nel = TABLESIZE - 4
	*/
testing ("test with TABLESIZE -4");
start
	testing("Use hcreate to create a table of TABLESIZE-4 = 126  elements.");
	testing("then adding one more than the total size elements");
	testing("should return NULL from hsearch.");
	start
		init3_table();
	finis

	testing("hsearch called to find all the elements of the table.");
	testing("should return not NULL.");
	start
		all_elements(TABLESIZE-4);
	finis

	testing("hsearch(a number less than the first element in the table)");
	testing("should return NULL.");
	start
		less_than_first();
	finis

	testing("hsearch(a number bigger than the last element in the table)");
	testing("should return NULL.");
	start
		bigger_than_last();
	finis

	testing("hsearch(a number in the range of the table");
	testing("but not in the table) should return NULL.");
	start
		in_the_range();
	finis

	testing("hsearch (the first element of the table)");
	testing("should return not NULL.");
	start
		first_elem();
	finis

	testing("hsearch(the last element of the table of size 128)");
	testing("should return not NULL.");
	start
		last3_elem();
	finis

	testing("hsearch(a duplicate entry of the table) should return");
	testing ("a value equal to the date portion of what was passed.");
	start
		duplicate_elem();
	finis
finis

done();
/*NOTREACHED*/
}

/*
** init1_table
**	Call hcreate to create a table with 130 elements
**	try to add 131 elements -- see that a return value
**	of NULL is received.
**
*/
init1_table()
{
	int		i;

	expecting (VOID);
	if ( hcreate(TABLESIZE) == 0 )
		error("Hcreate cannot allocate sufficient space for the table");

	for ( i = 0; i < TABLESIZE; i++ )
	{
		sprintf(Str_Item[i], "%d", (2*i+2));
		Items[i].key = (char *)Str_Item[i];
		Items[i].data = (char *)Str_Item[i];
		if ( hsearch(Items[i], ENTER) == NULL )
			error("Hsearch did not enter the Items correctly");
	}
}

/*
** init2_table
**	Call hcreate to create a table with 128 elements
**	try to add 129 elements -- see that a return value
**	of NULL is received.
**
*/
init2_table()
{
	int		i;

	expecting (VOID);
	hdestroy();
	if ( hcreate(TABLESIZE-2) == 0 )
		error("Hcreate cannot allocate sufficient space for the table of size 128");

	for ( i = 0; i < TABLESIZE-2; i++ )
	{
		sprintf(Str_Item[i], "%d", (2*i+2));
		Items[i].key = (char *)Str_Item[i];
		Items[i].data = (char *)Str_Item[i];
		if ( (hsearch(Items[i], ENTER)) == NULL )
			error("Hsearch did not enter the Items correctly");
	}
}

/*
** init3_table
**	Call hcreate to create a table with 126 elements
**	try to add 127 elements -- see that a return value
**	of NULL is received.
**
*/
init3_table()
{
	int		i;

	hdestroy();
	if ( hcreate(TABLESIZE-4) == 0 )
		error("Hcreate cannot allocate sufficient space for the table of size 128");

	for ( i = 0; i < TABLESIZE-4; i++ )
	{
		sprintf(Str_Item[i], "%d", (2*i+2));
		Items[i].key = (char *)Str_Item[i];
		Items[i].data = (char *)Str_Item[i];
		if ( (hsearch(Items[i], ENTER)) == NULL )
			error("Hsearch did not enter the Items correctly");
	}
}

/*
** all_elements
**     Test hsearch in the table indicated above with a search
**     for all the element in the table. 
**
** Level:
**	This test is in the robustness level (2).
*/
all_elements(size)
int		size;
{
	int		i;

	for (i = 0; i < size; i++)
	{
		if ( (hsearch(Items[i], FIND)) == NULL )
			error("Incorrect return value from hsearch when looking for all the elements");

	}

}

/*
** less_than_first
**     Test hsearch in the table indicated above with a search
**     for an Item lexically less than the first element in the table.
**
** Level:
**	This test is in the normal level (0).
*/
less_than_first()
{
	ENTRY		data1;
	char		*data2;

	data2 = "0";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (VOID);
	if ( (hsearch(data1, FIND)) != NULL )
		error("Incorrect return value from hsearch for\n\
an item smaller than the first item in the table");
}

/*
** bigger_than_last
**     Test hsearch in the table indicated above with a search
**     for an Item lexically greater than the last element in the table.
**
** Level:
**	This test is in the normal level (0).
*/
bigger_than_last()
{
	ENTRY		data1;
	char		*data2;

	data2 = "264";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (VOID);
	if ( (hsearch(data1, FIND)) != NULL )
		error("Incorrect return value from hsearch for an item\n\
bigger than the last item in the table.");
}

/*
** in_the_range
**     Test hsearch in the table indicated above with a search
**     for an Item in the range of the table but not in the table.
**
** Level:
**	This test is in the normal level (0).
*/
in_the_range()
{
	ENTRY		data1;
	char		*data2;

	data2 = "5";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (NULL);
	hsearch(data1, FIND);

}

/*
** last_elem
**     Test hsearch in the table indicated above with a search
**     for the last element in the table. 
**
** Level:
**	This test is in the boundary level (1).
*/
last1_elem()
{
	ENTRY		data1;
	char		*data2;

	data2 = "260";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (SUCCESS);
	hsearch(data1, FIND);
}

last2_elem()
{
	ENTRY		data1;
	char		*data2;

	data2 = "256";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (SUCCESS);
	hsearch(data1, FIND);
}

last3_elem()
{
	ENTRY		data1;
	char		*data2;

	data2 = "252";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (SUCCESS);
	hsearch(data1, FIND);
}

/*
** first_elem
**     Test hsearch in the table indicated above with a search
**     for the first element in the table. 
**
** Level:
**	This test is in the boundary level (1).
*/
first_elem()
{
	ENTRY		data1;
	char		*data2;

	data2 = "2";
	data1.key = (char *)data2;
	data1.data = (char *)data2;
	expecting (SUCCESS);
	hsearch(data1, FIND);

}
/*
** duplicate_elem()
**	Try to insert a duplicate entry -- check that the new element
**	is now in the table.  A duplicate entry has the same key.
**
*/
duplicate_elem()
{
	ENTRY		*found_item;
	ENTRY		data1;
	char		*data2;
	char		*data3;

	data2 = "2";
	data3 = "5";
	data1.key = (char *)data2;
	data1.data = (char *)data3;
	found_item = hsearch(data1, FIND);
	if ( *(found_item->data) == *(data1.data) )
		error("Incorrect return value of '%c' from hsearch\n\
when inserting a duplicate entry in the table.", *(found_item->data));
}
