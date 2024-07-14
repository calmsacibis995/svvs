# include <svvs.h>
# include <sv_env.h>
# include <sccs.h>
# include <string.h>

SCCSID("@(#)tparm.c	3.1 (SVVS) 12/1/86");

char *Ztparm();

# define MAXINT		99

/*
**	main()
**
**	call the functions that do the tests.
*/
main()
{
	setup("tparm");
	abstract("call tparm() for all of the stack operations");
	abstract("and verify that the returned string is the");
	abstract("correct string.");
	start
	exit_on(WARNING | ERROR | SEVERR);
		testing("push and pop stack operations");
		ppints();
		ppchars();
		ppstrs();
		ppparms();
		morepp();
		testing("printf style printing");
		print();
		testing("arithmetic operations");
		singleop();
		add();
		sub();
		mul();
		div();
		mod();
		testing("unary operations");
		oneop();
		neg();
		com();
		testing("bitwise logical operations - &, |, and ^");
		bit_ops();
		testing("logical operations - >, <, and =");
		bit_logical();
		testing("logical operations - AND and OR");
		logical();
		testing("if statements");
		ifs();
	finis
	done();

/*NOTREACHED*/
}

/*
** print()
**
**	call tparm() for each of %s, %x, %X, %o and %d and use
**	all of the applicable options for each.
*/
print()
{
	int i;
	int j;
	char *result;
	char buf[80];
	char str[80];
	char expect[80];

	testing("push an integer constant, then pop and print in hexadecimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(buf, "%%{%d}%%x", i);
		(void) sprintf(expect, "%x", i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push an integer parameter.  then pop and print it");
	testing("in hexadecimal format using precision and right justify.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) strcpy(buf, "%p1%:+4.2x");
		(void) sprintf(expect, "%+4.2x", i);
		expecting(SUCCESS);
		result = Ztparm(buf, i);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters.  then pop and print both");
	testing("in hexadecimal format using width and left justify.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%:-4X%:-4X");
		(void) sprintf(expect, "%-4X%-4X", j, i);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters.  then pop and print them");
	testing("in hexadecimal format that uses CAPITAL letters.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%#X%#x");
		(void) sprintf(expect, "%#X%#x", j, i);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push an integer constant, then pop and print it in octal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(buf, "%%{%d}%%o", i);
		(void) sprintf(expect, "%o", i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push an integer parameter, then pop and print it in ");
	testing("octal format using a width and right justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) strcpy(buf, "%p1%:+10o");
		(void) sprintf(expect, "%+10o", i);
		expecting(SUCCESS);
		result = Ztparm(buf, i);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters, then pop and print them in");
	testing("octal format using prescision and left justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%:-5.3o%:-5.3o");
		(void) sprintf(expect, "%-5.3o%-5.3o", j, i);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters.  then pop and print");
	testing("them in octal format");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%#o%#o");
		(void) sprintf(expect, "%#o%#o", j, i);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push an integer constant then pop and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) sprintf(buf, "%%{%d}%%d", i);
		(void) sprintf(expect, "%d", i);
		expecting(SUCCESS);
		result = Ztparm(buf);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push an integer parameter.  then pop and print it in");
	testing("decimal format using precision and right justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) strcpy(buf, "%p1%:+9d");
		(void) sprintf(expect, "%+9d", i);
		expecting(SUCCESS);
		result = Ztparm(buf, i);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters.  then pop and print them");
	testing("in decimal format using width and left justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%:-7d%:-7d");
		(void) sprintf(expect, "%-7d%-7d", j, i);
		expecting(SUCCESS);
		result = Ztparm(buf, i, j);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters.  then pop and print them");
	testing("in decimal format using prescision.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%4.2d%4.2d");
		(void) sprintf(expect, "%4.2d%4.2d", j / 4, i / 9);
		expecting(SUCCESS);
		result = Ztparm(buf, i / 9, j / 4);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push two integer parameters.  then pop and print them");
	testing("in decimal format using precision and left justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = MAXINT; i <= MAXINT; i++, j--)
	{
		(void) strcpy(buf, "%p1%p2%:-3.2d%:-3.2d");
		(void) sprintf(expect, "%-3.2d%-3.2d", j / 2, i / 5);
		expecting(SUCCESS);
		result = Ztparm(buf, i / 5, j / 2);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push a string then pop and print it in string");
	testing("format using a width and left justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) strcpy(buf, "%p1%:-20s");
		(void) sprintf(str, "nowisthetimefor%d", i);
		(void) sprintf(expect, "%-20s", str);
		expecting(SUCCESS);
		result = Ztparm(buf, str);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push a string and then pop and print it in string");
	testing("format using a width and right justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) strcpy(buf, "%p1%:+20s");
		(void) sprintf(str, "a%d", i);
		(void) sprintf(expect, "%+20s", str);
		expecting(SUCCESS);
		result = Ztparm(buf, str);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis

	testing("push a string and then pop and print it in string");
	testing("format with precision and right justifying.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i <= MAXINT; i++)
	{
		(void) strcpy(buf, "%p1%:+20.30s");
		(void) sprintf(str, "a bit larger string of characters to work witha%d", i);
		(void) sprintf(expect, "%+20.30s", str);
		expecting(SUCCESS);
		result = Ztparm(buf, str);
		if (strcmp(expect, result) != 0)
			error("return value incorrect, expected '%s', got '%s'", expect, result);
	}
	finis
}
