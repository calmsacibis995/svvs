/*
**	pushpop.c
**
**	contains routines for push and pop operations of tparm().
*/

# include <sv_env.h>
# include <svvs.h>
# include <ctype.h>
# include <string.h>
# include <sccs.h>

SCCSID("@(#)pushpop.c	3.1 (SVVS) 12/1/86");

# define MAXINT		99 	/* integers vary from 0 to 99 */

# define STRSIZE	256	/* size of buffers used to hold strings */

# define MAXPARMS	9	/* max number of parameters to tparm is 9 */

char *Ztparm();

/*
** ppints()
**
**	push and pop operations of tparm() for integers
*/
ppints()
{
	int i;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* push & pop of integer constants 0-99.  construct the    */
	/* string for tparm() using sprintf - to vary the constant */
	testing("push an integer constant.  then pop and");
	testing("print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(buf, "%%{%d}%%d", i);
		(void) sprintf(expect, "%d", i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect integer, expected '%s', got '%s'", expect, result);
	}
	finis

	/* push and pop of integer parameters 0-99 */
	(void) strcpy(buf, "%p1%d");
	testing("push an integer parameter.  then pop and");
	testing("print it in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(expect, "%d", i);
		expecting(SUCCESS);
		result = Ztparm(buf, i);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect integer, expected '%s', got '%s'", expect, result);
	}
	finis

	/* push and pop integer parameters 0-99 - specify */
	/* a field width for printing			  */
	(void) strcpy(buf, "%p1%4d");
	testing("push an integer parameter,then print in decimal");
	testing("format with a field width.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(expect, "%4d", i);
		expecting(SUCCESS);
		result = Ztparm(buf, i);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect integer, expected '%s', got '%s'", expect, result);
	}
	finis

	/* push and pop integer parameters 0-99 - specify */
	/* a field width and zero filling for printing    */
	(void) strcpy(buf, "%p1%07d");
	testing("push an integer parameter, then print in decimal");
	testing("format with a zero padded field width.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(expect, "%07d", i);
		expecting(SUCCESS);
		result = Ztparm(buf, i);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect integer, expected '%s', got '%s'", expect, result);
	}
	finis

}

/*
** ppchars()
**
**	push and pop operations of tparm() for characters
*/
ppchars()
{
	int i;
	int c;
	char *result;
	char buf[STRSIZE];
	char expect[STRSIZE];

	/* push and pop character constants */
	testing("push a character constant, then pop and");
	testing("print it in character format.  do this for");
	testing("all possible character constants.");
	start
	break_on(WARNING | ERROR);
	for (i = 1; isascii(i); i++)
	{
		(void) sprintf(buf, "%%\'%c\'%%c", toascii(i));
		(void) sprintf(expect, "%c", i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect character, expected '%s', got '%s'", expect, result);
	}
	finis

	/* push and pop character parameters */
	testing("push a character parameter, then pop and");
	testing("print it in character format.");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "%p1%c");
	for (i = 1; isascii(i); i++)
	{
		c = toascii(i);
		(void) sprintf(expect, "%c", i);
		expecting(SUCCESS);
		result = Ztparm(buf, c);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect character, expected '%s', got '%s'", expect, result);
	}
	finis
}

/*
** ppstrs()
**
**	push and pop operations of tparm() on strings of characters.
**	the loop catenates the string to itself each time to test a
**	variety of string lengths.
*/
ppstrs()
{
	int max;
	char *result;
	char sbuf[STRSIZE];
	char lbuf[STRSIZE];
	char expect[STRSIZE];
	char inc[STRSIZE];

	max = sizeof(expect);
	(void) strcpy(inc, "foobar&gibberish");
	(void) strcpy(sbuf, "%p1%s");
	(void) strcpy(lbuf, "%p1%l%d");
	testing("push and pop a string parameter.  then push the same");
	testing("string parameter and pop and print the string length.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	do
	{
		result = Ztparm(sbuf, expect);
		if (strcmp(expect, result) != 0)
			error("tparm() returned incorrect string, expected '%s', got '%s'", expect, result);

		result = Ztparm(lbuf, expect);
		if (atoi(result) != strlen(expect))
			error("tparm() returned incorrect string length, expected '%s', got '%s'", expect, result);

		(void) strcat(expect, inc);

	} while (strlen(expect) + strlen(inc) < max);
	finis
}

/*
** ppparms()
**
**	push and pop operations of tparm() 1 parameter, 2,
**	3, ...
*/
ppparms()
{
	int i;
	char *result;
	char foo[STRSIZE];
	char buf[STRSIZE];
	char expect[STRSIZE];

	testing("push an integer parameter.  then pop and print it");
	testing("in decimal format.  then push, pop and print two integer");
	testing("parameters, followed by push, pop and print of three integer");
	testing("parameters...and so on up to nine integer parameters.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	*buf = '\0';
	*expect = '\0';
	for (i = 1; i <= MAXPARMS; i++)
	{
		(void) sprintf(foo, "%%p%d%%d", i);
		(void) strcat(buf, foo);
		(void) sprintf(foo, "%d", i);
		(void) strcat(expect, foo);
		switch (i)
		{
		  case 1:
			result = Ztparm(buf, i);
			break;
		  case 2:	
			result = Ztparm(buf, i - 1, i);
			break;
		  case 3:	
			result = Ztparm(buf, i - 2, i - 1, i);
			break;
		  case 4:	
			result = Ztparm(buf, i - 3, i - 2, i - 1, i);
			break;
		  case 5:
			result = Ztparm(buf, i - 4, i - 3, i - 2, i - 1, i);
			break;
		  case 6:	
			result = Ztparm(buf, i - 5, i - 4, i - 3, i - 2,
					i - 1, i);
			break;
		  case 7:	
			result = Ztparm(buf, i - 6, i - 5, i - 4, i - 3,
					i - 2, i - 1, i);
			break;
		  case 8:	
			result = Ztparm(buf, i - 7, i - 6, i - 5, i - 4,
					i - 3, i - 2, i - 1, i);
			break;
		  case 9:	
			result = Ztparm(buf, i - 8, i - 7, i - 6, i - 5,
					i - 4, i - 3, i - 2, i - 1, i);
			break;
		  default:	
			error("unexpected case ('%d') in switch statement", i);
		}
		if (strcmp(expect, result) != 0)
			error("incorrect parameter(s) returned, expected '%s', got '%s'", expect, result);
	}
	finis
}

/*
** morepp()
**
**	push and pop variables
*/
morepp()
{
	char c;
	char *result;
	char buf[STRSIZE];
	char fub[STRSIZE];
	char expect[STRSIZE];

	testing("push a character parameter and pop it into a variable.");
	testing("then push the variable back on the stack and pop and ");
	testing("print it in character format.");
	start
	break_on(WARNING | ERROR);
	exit_on(SEVERR);
	for (c = 'a'; c <= 'z'; c++)
	{
		(void) sprintf(buf, "%%p1%%");
		(void) sprintf(fub, "P%c%%g%c%%c", c, c);
		(void) strcat(buf, fub);
		(void) sprintf(expect, "%c", c);
		result = Ztparm(buf, c);
		if (strcmp(expect, result) != 0)
			error("variable push/pop incorrect, expected '%s', got '%s'", expect, result);
	}
	finis
}
