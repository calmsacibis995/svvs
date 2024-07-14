#include	<sv_base.h> /*replacement macros */
#include	<search.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)tsearch.c	1.3"



# define	TREESIZE       120

/*
** tsearch
**	Test the tsearch function.
*/
char			Key[TREESIZE];
char			Key2[TREESIZE];
char			*Keyptr;
char			*Key2ptr;
struct
{
	char		value;
	int		count;
} Treetable[TREESIZE];
unsigned		Last;

extern		char	*tsearch();
# ifndef	SYSVR_1
extern		char	*tfind();
# endif	/* Not SYSVR_1 */
extern		char	*tdelete();
extern		void	twalk();
extern		int	compar();
extern		int	action();

main()
{
setup("tsearch");
funct("tsearch");
funct("tfind");
funct("tdelete");
funct("twalk");

testing("Using tsearch to create a binary tree of 120 distinct elements");
testing("should return a pointer pointing to the element inserted.");
start
	init_tree();
finis

# ifndef	SYSVR_1
testing("Calling tfind to find all the elements of the tree");
testing("should return pointers pointing to the element found.");
start
	tfind_all();
finis

# endif	/* Not SYSVR_1 */
testing("Call tsearch to find all the elements of the tree");
testing("should return pointers pointing to the element found.");
start
	tsrch_all();
finis

# ifndef	SYSVR_1

testing("Calling tfind to find an element smaller than all");
testing("the elements in the tree should return NULL.");
start
	tfind_less();
finis

testing("Calling tfind to find an item bigger than the biggest");
testing("item in the tree should return NULL.");
start
	tfind_greater();
finis

testing("Calling tfind to find an item in the range of the tree");
testing("but not in the tree should return NULL.");
start
	tfind_range();
finis

testing("Use tsearch to create a second binary tree of 120 distinct elements.");
testing("Using tfind find all the elements of the first tree");
testing("should return pointers pointing to the element inserted in the first");
testing("tree.");
start
	init2_tree();
finis

# endif	/* Not SYSVR_1 */
testing("Call tdelete to delete a node. Check that all the other");
testing("nodes in the tree are still accessible,");
testing("returning a pointer to the item being found.");
start
	tdel_node();
finis

testing("Calling tdelete with a NULL root should return NULL.");
start
	tdel_nullroot();
finis

testing("Calling tdelete with a non existent node should return NULL.");
start
	tdle_nonode();
finis

testing("using twalk to walk a tree should cause all nodes to be visited.");
start
	twalk_visitall();
finis

done();
/*NOTREACHED*/
}

/*
** init_tree
**	Call tsearch to create a tree with 120 elements
**
*/
init_tree()
{
	char		i;
	char		item;
	char 		*found_item;
	Keyptr = NULL;

	for ( i = 0; i < TREESIZE; i++ )
	{
		Key[i] = (2)*(i) + 2;
		expecting (VOID);
		found_item = tsearch(&Key[i], &Keyptr, compar);

		if ( found_item == NULL )
		{
			error ("Tsearch did not insert the item %d correctly",
				i);
			break;
		}
		item = **((char **)found_item);
		if ( item != Key[i] )
		{
			error ("Tsearch returned '%s', '%s' was expected",
				item, Key [i]);
			break;
		}
	}
}

# ifndef	SYSVR_1
/*
** tfind_all
**	Call tfind to find all the elements of the tree.
**
*/
tfind_all()
{
	char		item;
	char		i;
	char 		*found_item;

	for ( i = 0; i < TREESIZE; i++ )
	{
		expecting (VOID);
		found_item = tfind((char *)&Key[i], (char *)&Keyptr, compar);

		if ( found_item == NULL )
		{
			error("Tfind did not find the item %d correctly",
				i);
			break;
		}

		item = **((char **)found_item);
		if ( item != Key[i] )
		{
			error ("Tfind returned '%s', '%s' was expected",
				item, Key [i]);
			break;
		}
	}
}
# endif	/* Not SYSVR_1 */

/*
** tsrch_all
**	Call tsearch to find all the elements of the tree.
**
*/
tsrch_all()
{
	char		item;
	char		i;
	char 		*found_item;

	for ( i = 0; i < TREESIZE; i++ )
	{
		expecting (VOID);
		found_item = tsearch((char *)&Key[i], (char *)&Keyptr, compar);

		if ( found_item == NULL )
		{
			error("Tsearch did not find item %d correctly",
				i);
			break;
		}

		item = **((char **)found_item);
		if ( item != Key[i] )
		{
			error ("Tsearch returned '%s', '%s' was expected",
				item, Key [i]);
			break;
		}
	}
}

# ifndef	SYSVR_1
/*
** tfind_less
**	Call tfind to find an element smaller than all
**	the elements in the tree.
**
*/
tfind_less()
{
	char		item;
	char 		*found_item;
	item = 0;
	expecting (NULL);
	tfind((char *)&item, (char *)&Keyptr, compar);
}

/*
** tfind_greater
**	Call tfind to find an item greater then the biggest item
**	in the tree.
**
*/
tfind_greater()
{
	char		item;
	char 		*found_item;
	item = 245;
	expecting (NULL);
	tfind((char *)&item, (char *)&Keyptr, compar);
}

/*
** tfind_range
**	Call tfind to find an item in the range of the tree but not 
**	in the tree.
**
*/
tfind_range()
{
	char		item;
	char 		*found_item;
	item = 3;

	expecting (NULL);
	tfind((char *)&item, (char *)&Keyptr, compar);
}

/*
** init2_tree
**	Call tsearch to create a second tree with 120 elements.
**	Check that the first tree is still the same as before.
**
*/
init2_tree()
{
	char		item;
	char		i;
	char 		*found_item;
	Key2ptr = NULL;

	for ( i = 0; i < TREESIZE; i++ )
	{
		Key2[i] = 2*i + 1;
		expecting (VOID);
		found_item = tsearch((char *)&Key2[i], (char *)&Key2ptr, compar);

		if ( found_item == NULL )
		{
			error("Tsearch did not insert item %d correctly",
				i);
			break;
		}

		item = **((char **)found_item);
		if ( item != Key2[i] )
		{
		
			error ("Tsearch returned '%s', '%s' was expected",
				item, Key [i]);
			break;
		}
	}

	/*
	** Now find all the elements of the first tree.
	** check that all the elements are still correct.
	*/
	for ( i = 0; i < TREESIZE; i++ )
	{
		expecting (VOID);
		found_item = tfind((char *)&Key[i], (char *)&Keyptr, compar);

		if ( found_item == NULL )
		{
			error("Tfind did not find item %d correctly",
				i);
			break;
		}

		item = **((char **)found_item);
		if ( item != Key[i] )
		{
			error ("Tfind returned '%s', '%s' was expected",
				item, Key [i]);
			break;
		}
	}
}
# endif	/* Not SYSVR_1 */

/*
** tdel_node
**	Call tdelete to delete a node. Check that all the other
**	nodes in the tree are still accessible.
**
*/
tdel_node()
{
	int		i;
	char		*found_item;
	char		item;
	char		*delete_item;

	expecting (VOID);
	delete_item = tdelete((char *)&Key[0], (char *)&Keyptr, compar);

	if ( delete_item == NULL )
		error("Tdelete did not find '%s' for deletion.",
			(char *)&Key[0]);

# ifndef	SYSVR_1
	/*
	** Now find all the other nodes.
	*/
	for ( i = 1; i < TREESIZE; i++ )
	{
		expecting (VOID);
		found_item = tfind((char *)&Key[i], (char *)&Keyptr, compar);

		if ( found_item == NULL )
		{
			error("Tfind did not find item %d correctly",
				i);
			break;
		}

		item = **((char **)found_item);
		if ( item != Key[i] )
		{
			error ("Tfind returned '%s', '%s' was expected",
				item, Key [i]);
			break;
		}
	}
# endif	/* Not SYSVR_1 */
}

/*
** tdel_nullroot
**	Call tdelete with a NULL root. Check that it returns
**	a NULL in this case.
**
*/
tdel_nullroot()
{
	char		*delete_item;
	Key2ptr = NULL;

	expecting (NULL);
	tdelete((char *)&Key[0], (char *)&Key2ptr, compar);
}

/*
** tdle_nonode
**	Call tdelete with a node not in the tree. See that
**	a NULL is returned in this case.
**
*/
tdle_nonode()
{
	char		item;
	char		*delete_item;
	item = 153;

	expecting (NULL);
	tdelete((char *)&item, (char *)&Keyptr, compar);
}

/*
** twalk_visitall
**	Call twalk -- check that all the items are visited
**
*/
twalk_visitall()
{
	char		i;

	Last = 0;

	expecting (VOID);
	twalk((char *)Keyptr, action);

	if ( Last < (TREESIZE -2) )
		error("Twalk did not call the action routine enough times");

	for ( i = 0; i < (TREESIZE - 1) ; i++ )
	{
		if ( Treetable[i].count < 1 )
		{
			error("Twalk did not visit item %i", i);
			break;
		}
	}
}

/*
** action
**	This is the action function for the twalk call above.
**
*/
action(node, order, level)
char		*node;
VISIT		order;
int		level;
{
	int		j;
	int		found;

	found = 0;
	for ( j = 0; j < Last; j++ )
	{
		if ( **((char **)node) == Treetable[j].value )
		{
			Treetable[j].count++;
			found = 1;
			break;
		}
	}
	if ( found == 0 )
	{
		Treetable[Last].value = **((char **)node);
		Treetable[Last].count = 1;
		Last++;
	}
}

/*
** compar
**	This function is the 'compar' function for the tree.
**
*/
compar(ptr1, ptr2)
char 		*ptr1;
char 		*ptr2;
{
	if ( *ptr1 > *ptr2 )
		return(1);
	if ( *ptr1 == *ptr2 )
		return(0);
	return(-1);
}
