/*
**	logical.c
**
**	contains routines for bit operations, logical
**	operations and if statements
*/

# include <sv_env.h>
# include <svvs.h>
# include <ctype.h>
# include <string.h>
# include <sccs.h>

SCCSID("@(#)logical.c	3.1 (SVVS) 12/1/86");

# define MAXINT		99	/* integers for tparm range from 0 to 99 */

# define STRSIZE	256	/* size of buffer used to hold strings */

char *Ztparm();

/*
** bit_ops()
**
**	bitwise operations - &, | and ^
*/
bit_ops()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push two integer parameters, perform an and (&)");
	testing("operation on them and print the result in decimal format.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%&%d");
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j --)
	{
		(void) sprintf(expect, "%d", i & j);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("bitwise and failed.  '%d' & '%d' - expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	testing("push two integer parameters, perform an or (|)");
	testing("operation on them and print the result in decimal format.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%|%d");
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j --)
	{
		(void) sprintf(expect, "%d", i | j);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("bitwise or failed.  '%d' | '%d' - expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	testing("push two integer parameters, perform an exclusive or (^)");
	testing("operation on them and print the result in decimal format.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%^%d");
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j --)
	{
		(void) sprintf(expect, "%d", i ^ j);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("bitwise exclusive or failed.  '%d' ^ '%d' - expected '%s', got '%s'", i, j, expect, result);
	}
	finis
}

/*
** bit_logical()
**
**	single logical operations
*/
bit_logical()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* logical	*/
	testing("equal operator on equal integer parameters and unequal ones.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%=%d");
	for (i = 1, j = 1; i <= MAXINT; i++, j++)
	{
		(void) sprintf(expect, "%d", (i == j));
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("equality failed.  '%d' should be equal to '%d'", i, j);
		(void) sprintf(expect, "%d", (i == (j - 1)));
		expecting(SUCCESS);
		result = Ztparm(buf, i, (j - 1));
		if (strcmp(expect, result) != 0)
			error("inequality failed.  '%d' is not equal to '%d'", i, (j - 1));
	}
	finis

	testing("greater than operator with equal operands");
	testing("and one operand greater than the other.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%>%d");
	for (i = 1, j = 1; i <= MAXINT; i++, j++)
	{
		(void) sprintf(expect, "%d", (i > j));
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("not greater than failed.  '%d' should not be greater than '%d'", i, j);
		(void) sprintf(expect, "%d", (i > (j - 1)));
		expecting(SUCCESS);
		result = Ztparm(buf, i, (j - 1));
		if (strcmp(expect, result) != 0)
			error("greater than failed.  '%d' is greater than '%d'", i, (j - 1));
	}
	finis

	testing("less than operator with equal operands");
	testing("and with one operand less than the other.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%<%d");
	for (i = 1, j = 1; i <= MAXINT; i++, j++)
	{
		(void) sprintf(expect, "%d", (i < j));
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("not less than failed.  '%d' should not be less than '%d'", i, j);
		(void) sprintf(expect, "%d", ((i - 1) < j));
		expecting(SUCCESS);
		result = Ztparm(buf, (i - 1), j);
		if (strcmp(expect, result) != 0)
			error("less than failed.  '%d' is less than '%d'", (i - 1), j);
	}
	finis
}

/*
** ifs()
**
**	various kinds of if conditions
*/
ifs()
{
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* if statements */
	testing("push integer parameters 1 and 0.  if 1 is greater than 0");
	testing("push both again then pop and print them.  repeat, pushing");
	testing("0 then 1 this time.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%?%>%t%p1%p2%d%d%;");
	result = Ztparm(buf, 1, 0);
	(void) sprintf(expect, "%d%d", 0, 1);
	if (strcmp(result, expect) != 0)
		error("if then incorrect, expected '%s', got '%s'", expect, result);
	result = Ztparm(buf, 0, 1);
	(void) strcpy(expect, "");
	if (strcmp(result, expect) != 0)
		error("if then incorrect, expected '%s', got '%s'", expect, result);
	finis

	testing("push integer parameters 1 and 0.  if 1 is less than 0");
	testing("push the character 't' on the stack else push the ");
	testing("the character 'e'.  then print a character.  repeat, ");
	testing("pushing 0 then 1 this time.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%?%<%t%'t'%e%'e'%;%c");
	result = Ztparm(buf, 1, 0);
	(void) sprintf(expect, "%c", 'e');
	if (strcmp(result, expect) != 0)
		error("if then else incorrect, expected '%s', got '%s'", expect, result);
	result = Ztparm(buf, 0, 1);
	(void) sprintf(expect, "%c", 't');
	if (strcmp(result, expect) != 0)
		error("if then else incorrect, expected '%s', got '%s'", expect, result);
	finis

	testing("push integer parameters 0 and 1.  if 1 equals 0");
	testing("push the character 't' on the stack else push the");
	testing("character 'e'.  push two more integer parameters, 1 and 1,");
	testing("if they are equal push the character 't' and print it.");
	testing("then print the previous character.  repeat pushing 0,");
	testing("1, 0 and 1.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%?%=%t%'t'%e%'e'%p3%p4%?%=%t%'t'%c%;%c");
	result = Ztparm(buf, 0, 1, 1, 1);
	(void) strcpy(expect, "te");
	if (strcmp(result, expect) != 0)
		error("if then else if incorrect, expected '%s', got '%s'", expect, result);
	result = Ztparm(buf, 0, 1, 0, 1);
	(void) strcpy(expect, "e");
	if (strcmp(result, expect) != 0)
		error("if then else if incorrect, expected '%s', got '%s'", expect, result);
	finis

	/* if then with logical AND */
	testing("push integer parameters 99 and 1.  if 99 AND 1 is true,");
	testing("push the paramters again and print the AND of them.");
	start
	break_on(WARNING | ERROR);
	/* copy in three pieces 'cause sccs does things with % A */
	(void) strcpy(buf, "%p1%p2%?%");
	(void) strcat(buf, "A%t%p1%p2%");
	(void) strcat(buf, "A%d%;");
	result = Ztparm(buf, MAXINT, 1);
	(void) sprintf(expect, "%d", MAXINT & 1);
	if (strcmp(result, expect) != 0)
		error("if then with AND incorrect, expected '%s', got '%s'", expect, result);
	finis

	/* if then with logical OR */
	testing("push integer parameters 0 and 1.  if 0 OR 1 is true,");
	testing("push the paramters again and print the OR of them.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%?%O%t%p1%p2%O%d%;");
	result = Ztparm(buf, 0, 1);
	(void) sprintf(expect, "%d", 0 | 1);
	if (strcmp(result, expect) != 0)
		error("if then with OR incorrect, expected '%s', got '%s'", expect, result);
	finis
}

/*
** logical()
**
**	logical operations
*/
logical()
{
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];
	int i;
	int j;

	/* logical stuff */
	testing("AND two integer parameters and print the");
	testing("result in decimal format.");
	start
	break_on(WARNING | ERROR);
	/* copy in two pieces 'cause sccs does things with % A */
	(void) strcpy(buf, "%p1%p2%");
	(void) strcat(buf, "A%d");
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		result = Ztparm(buf, i, j);
		(void) sprintf(expect, "%d", i && j);
		if (strcmp(result, expect) != 0)
			error("logical AND error of '%d' and '%d'.  expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	testing("OR two integer parameters and print the");
	testing("result in decimal format.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%p2%O%d");
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		result = Ztparm(buf, i, j);
		(void) sprintf(expect, "%d", i || j);
		if (strcmp(result, expect) != 0)
			error("logical OR error of '%d' and '%d'.  expected '%s', got '%s'", i, j, expect, result);
	}
	finis
}
