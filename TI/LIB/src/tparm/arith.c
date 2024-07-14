/*
**	arith.c
**
**	contains the routines for arithmetic operations of tparm().
*/

# include <sv_env.h>
# include <svvs.h>
# include <ctype.h>
# include <string.h>
# include <sccs.h>

SCCSID("@(#)arith.c	3.1 (SVVS) 12/1/86");

# define STRSIZE	256	/* size of buffers for strings */

# define MAXINT		99	/* tparm integers range from 0 to 99 */

# define HALFINTS	49

char *Ztparm();

/*
** singleop()
**
**	single arithemetic operations
*/
singleop()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* push two parameters, add them and pop the result */
	testing("push two integer parameters, add them and pop the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%+%d");
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(expect, "%d", i + j);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("addition of '%d' and '%d' incorrect, expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	/* push two parameters, subtract them and pop the result */
	testing("push two integer parameters, subtract them and pop the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%-%d");
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(expect, "%d", i - j);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("subtraction of '%d' and '%d' incorrect, expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	/* push two parameters, multiply them and pop the result */
	testing("push two integer parameters, multiply them and pop the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%*%d");
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(expect, "%d", i * j);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("multiplication of '%d' and '%d' incorrect, expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	/* push two parameters, divide them and pop the result */
	testing("push two integer parameters, divide them and pop the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%/%d");
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(expect, "%d", i / j);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("division of '%d' and '%d' incorrect, expected '%s', got '%s'", i, j, expect, result);
	}
	finis

	/* push two parameters, mod them and pop the result */
	testing("push two integer parameters, take the modulus and pop the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%m%d");
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(expect, "%d", i % j);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("modulus of '%d' and '%d' incorrect, expected '%s', got '%s'", i, j, expect, result);
	}
	finis
}

/*
** add()
**
**	add two integers and follow it with another arithmetic operation
*/
add()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push two integer parameters and add them.  push a");
	testing("third integer and subtract.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%+%p3%-%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i + j) - MAXINT);
		result = Ztparm(buf, i, j, MAXINT);
		if (strcmp(expect, result) != 0)
			error("(%d + %d) - %d error, got '%s'", i, j, MAXINT, expect, result);
	}
	finis

	testing("push two integer parameters and add them.  push a third");
	testing("integer and multiply.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%+%p3%*%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i + j) * (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d + %d) * %d error, expected '%s', got '%s'", i, j, MAXINT -i, expect, result);
	}
	finis

	testing("push two integer parameters and add them.  push a third");
	testing("integer and divide.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%+%p3%/%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i + j ) / (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d + %d) / %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integer parameters and add them.  push a third");
	testing("integer and take the modulus.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%+%p3%m%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i + j) % (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d + %d) %% %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis
}

/*
** sub()
**
**	subtract two integers and follow it with another
**	arithmetic operation
*/
sub()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push two integers and subtract them.  then push another");
	testing("integer and add.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%-%p3%+%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i - j) + MAXINT);
		result = Ztparm(buf, i, j, MAXINT);
		if (strcmp(expect, result) != 0)
			error("(%d - %d) + %d error, expected '%s', got '%s'", i, j, MAXINT, expect, result);
	}
	finis

	testing("push two integers and subtract them.  then push another");
	testing("integer and multiply.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%-%p3%*%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i - j) * (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d - %d) * %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and subtract them.  then push another");
	testing("integer and divide.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%-%p3%/%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i - j) / (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d - %d) / %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and subtract them.  then push another");
	testing("integer and take the modulus.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%-%p3%m%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i - j) % (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d - %d) %% %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis
}

/*
** mul()
**
**	multiply two integers and follow it with another
**	arithmetic operation
*/
mul()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push two integers and multiply.  then push another");
	testing("and add.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%*%p3%+%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i * j) + MAXINT);
		result = Ztparm(buf, i, j, MAXINT);
		if (strcmp(expect, result) != 0)
			error("(%d * %d) + %d error, expected '%s', got '%s'", i, j, MAXINT, expect, result);
	}
	finis

	testing("push two integers and multiply.  then push another");
	testing("and subtract.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%*%p3%-%d");
	for (i = 0, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i * j) - (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d * %d) - %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and multiply.  then push another");
	testing("and divide.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%*%p3%/%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i * j) / (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d * %d) / %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and multiply.  then push another");
	testing("and take the modulus.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%*%p3%m%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i * j) % (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d * %d) %% %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis
}

/*
** div()
**
**	divide two integers and follow it with another
**	arithmetic operation
*/
div()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push two integers and divide them.  then push");
	testing("another and add.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%/%p3%+%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i / j) + MAXINT);
		result = Ztparm(buf, i, j, MAXINT);
		if (strcmp(expect, result) != 0)
			error("(%d / %d) + %d error, expected '%s', got '%s'", i, j, MAXINT, expect, result);
	}
	finis

	testing("push two integers and divide them.  then push");
	testing("another and subtract.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%/%p3%-%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i / j) - (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d / %d) - %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and divide them.  then push");
	testing("another and multiply.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%/%p3%*%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i / j) * (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d / %d) * %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and divide them.  then push");
	testing("another and take the modulus.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%/%p3%m%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i / j) % (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d / %d) %% %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis
}

/*
** mod()
**
**	take the modulus of two integers and follow it with another
**	arithmetic operation
*/
mod()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push two integers and take the modulus.  then push");
	testing("another and add.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%m%p3%+%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i % j) + MAXINT);
		result = Ztparm(buf, i, j, MAXINT);
		if (strcmp(expect, result) != 0)
			error("(%d %% %d) + %d error, expected '%s', got '%s'", i, j, MAXINT, expect, result);
	}
	finis

	testing("push two integers and take the modulus.  then push");
	testing("another and subtract.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%m%p3%-%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i % j) - (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d %% %d) - %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and take the modulus.  then push");
	testing("another and multiply.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%m%p3%*%d");
	for (i = 1, j = HALFINTS; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i % j) * (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d %% %d) * %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis

	testing("push two integers and take the modulus.  then push");
	testing("another and divide.  pop and print the result.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	(void) strcpy(buf, "%p1%p2%m%p3%/%d");
	for (i = 1, j = MAXINT; i <= HALFINTS; i++, j--)
	{
		(void) sprintf(expect, "%d", (i % j) / (MAXINT - i));
		result = Ztparm(buf, i, j, MAXINT - i);
		if (strcmp(expect, result) != 0)
			error("(%d %% %d) / %d error, expected '%s', got '%s'", i, j, MAXINT - i, expect, result);
	}
	finis
}
