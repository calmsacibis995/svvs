/*
**	unary.c
**
**	contains routines for unary operations
*/

# include <sv_env.h>
# include <svvs.h>
# include <ctype.h>
# include <string.h>
# include <sccs.h>

SCCSID("@(#)unary.c	3.1 (SVVS) 12/1/86");

# define STRSIZE	256	/* size of buffers used to hold strings */

# define MAXINT		99	/* integers for tparm range from 0 to 99 */

char *Ztparm();

/*
** oneop()
**
**	single unary operations
*/
oneop()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* negation	*/
	testing("push an integer parameter and negate it.");
	testing("then pop the result and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(buf, "%%{%d}%%!%%d", i);
		(void) sprintf(expect, "%d", !i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("negation error, expected '%s', result '%s'", expect, result);
	}
	finis

	/* complement	*/
	testing("push an integer parameter and complement it.");
	testing("then pop the result and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(buf, "%%{%d}%%~%%d", i);
		(void) sprintf(expect, "%d", ~i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("complement error, expected '%s', result '%s'", expect, result);
	}
	finis

	/* add one to the first two parameters	*/
	testing("add one to the first two parameters, push them onto ");
	testing("the stack, then pop and print both in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%i%p1%p2%d%d");
		(void) sprintf(expect, "%d%d", j + 1, i + 1);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("error adding 1 to %d and %d, expected '%s', result '%s'", i, j, expect, result);
	}
	finis
}

/*
** neg()
**
**	negation followed by arithmetic operations
*/
neg()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* negation followed by + ... */
	testing("push an integer parameter and negate it.  then push a");
	testing("second integer and add.  pop and print the result.");
	testing("next, push two integers, add them, negate the result");
	testing("of the addition, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%!%%{%d}%%+%%d", i, j);
		(void) sprintf(expect, "%d", !i + j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!%d + %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%+%%!%%d", i, j);
		(void) sprintf(expect, "%d", !(i + j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!(%d + %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* negation followed by - ... */
	testing("push an integer parameter and negate it.  then push a");
	testing("second integer and subtract.  pop and print the result.");
	testing("next, push two integers, subtract them, negate the result");
	testing("of the subtraction, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%!%%{%d}%%-%%d", i, j);
		(void) sprintf(expect, "%d", !i - j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!%d - %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%-%%!%%d", i, j);
		(void) sprintf(expect, "%d", !(i - j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!(%d - %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* negation followed by * ... */
	testing("push an integer parameter and negate it.  then push a");
	testing("second integer and multiply.  pop and print the result.");
	testing("next, push two integers, multiply them, negate the result");
	testing("of the multiplication, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%!%%{%d}%%*%%d", i, j);
		(void) sprintf(expect, "%d", !i * j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!%d * %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%*%%!%%d", i, j);
		(void) sprintf(expect, "%d", !(i * j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!(%d * %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* negation followed by / ... */
	testing("push an integer parameter and negate it.  then push a");
	testing("second integer and divide.  pop and print the result.");
	testing("next, push two integers, divide them, negate the result");
	testing("of the division, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%!%%{%d}%%/%%d", i, j);
		(void) sprintf(expect, "%d", !i / j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!%d / %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%/%%!%%d", i, j);
		(void) sprintf(expect, "%d", !(i / j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!(%d / %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* negation followed by % ... */
	testing("push an integer parameter and negate it.  then push a");
	testing("second integer and take the modulus.  pop and print the");
	testing("result.  next, push two integers, divide them, negate the");
	testing("result of the modulus, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%!%%{%d}%%m%%d", i, j);
		(void) sprintf(expect, "%d", !i % j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!%d %% %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%m%%!%%d", i, j);
		(void) sprintf(expect, "%d", !(i % j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("!(%d %% %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis
}

/*
** com()
**
**	complement followed by arithmetic operations
*/
com()
{
	int i;
	int j;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* complement followed by + ... */
	start
	testing("push an integer parameter and complement it.  then push a");
	testing("second integer and add.  pop and print the result.");
	testing("next, push two integers, add them, complement the result");
	testing("of the addition, pop and print it in decimal format.");
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%~%%{%d}%%+%%d", i, j);
		(void) sprintf(expect, "%d", ~i + j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~%d + %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%+%%~%%d", i, j);
		(void) sprintf(expect, "%d", ~(i + j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~(%d + %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* complement followed by - ... */
	testing("push an integer parameter and complement it.  then push a");
	testing("second integer and subtract.  pop and print the result.");
	testing("next, push two integers, subtract them, complement the");
	testing("result of the subtraction, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%~%%{%d}%%-%%d", i, j);
		(void) sprintf(expect, "%d", ~i - j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~%d - %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%-%%~%%d", i, j);
		(void) sprintf(expect, "%d", ~(i - j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~(%d - %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* complement followed by * ... */
	testing("push an integer parameter and complement it.  then push a");
	testing("second integer and multiply.  pop and print the result.");
	testing("next, push two integers, multiply them, complement the");
	testing("result of the multiplication, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%~%%{%d}%%*%%d", i, j);
		(void) sprintf(expect, "%d", ~i * j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~%d * %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%*%%~%%d", i, j);
		(void) sprintf(expect, "%d", ~(i * j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~(%d * %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* complement followed by / ... */
	testing("push an integer parameter and complement it.  then push a");
	testing("second integer and divide.  pop and print the result.");
	testing("next, push two integers, divide them, complement the");
	testing("result of the division, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%~%%{%d}%%/%%d", i, j);
		(void) sprintf(expect, "%d", ~i / j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~%d / %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%/%%~%%d", i, j);
		(void) sprintf(expect, "%d", ~(i / j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~(%d / %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis

	/* complement followed by % ... */
	testing("push an integer parameter and complement it.  then push a");
	testing("second integer and take the modulus.  pop and print the");
	testing("result.  next, push two integers, take the modulus, complement");
	testing("the result of the modulus, pop and print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) sprintf(buf, "%%{%d}%%~%%{%d}%%m%%d", i, j);
		(void) sprintf(expect, "%d", ~i % j);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~%d %% %d error, expected '%s', result '%s'", i, j, expect, result);
		(void) sprintf(buf, "%%{%d}%%{%d}%%m%%~%%d", i, j);
		(void) sprintf(expect, "%d", ~(i % j));
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("~(%d %% %d) error, expected '%s', result '%s'", i, j, expect, result);
	}
	finis
}
