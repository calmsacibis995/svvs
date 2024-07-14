# include <sv_base.h>
# include <svvs.h>
# include <sv_env.h>
# include <string.h>

/*
**	tgo.c
**
**	contains tgo() which tests tgoto()
*/

# include <sccs.h>

SCCSID("@(#)tgo.c	3.1 (SVVS) 12/1/86");

# define STRSIZE	256

# define SMLOOP		30
# define LGLOOP		60
# define VERYSM		15

# define ADD		4
# define SUB		1
# define MUL		17
# define DIV		9
# define MOD		3

extern char outbuf[];
extern char *outptr;

/*
**	tgo()
**
**	tgo calls tgoto and verifies that the returned
**	string is the expected stuff.
**
*/
tgo()
{
	char *result;
	char buf[STRSIZE];
	char *pbuf = buf;
	char exp[STRSIZE];
	char *pexp = exp;
	int i;
	int j;
	extern char *Ztgoto();

	/* Push and pop the row and col */
	testing("push, pop and print in decimal format two integer parameters.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i < SMLOOP; i++)
	{
		for (j = 0; j < LGLOOP; j++)
		{
			(void) strcpy(pbuf, "%p1%p2%d%d");
			(void) sprintf(pexp, "%d%d", j, i);
			expecting(VOID);
			result = Ztgoto(buf, j, i);
			if (strcmp(result, exp) != 0)
				error("push pop integers error. parameters '%d' '%d'.  expected '%s' got '%s'", j, i, pexp, result); 
		}
	}
	finis

	/* push and pop the row and col, print with field width */
	testing("Push, pop and print with field width two integer parameters.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i < SMLOOP; i++)
	{
		for (j = 0; j < LGLOOP; j++)
		{
			(void) strcpy(pbuf, "%p1%p2%02d%02d");
			(void) sprintf(pexp, "%02d%02d", j, i);
			expecting(VOID);
			result = Ztgoto(buf, j, i);
			if (strcmp(result, exp) != 0)
				error("push pop integers with a  field width error.  parameters '%d' '%d'.  expected '%s' got '%s'", j, i, pexp, result); 
		}
	}
	finis

	/* Variables */
	testing("push two integer parameters.  pop them into variables,");
	testing("then push the variables, pop and print them in zero");
	testing("filled decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i < SMLOOP; i++)
	{
		for (j = 0; j < LGLOOP; j++)
		{
			(void) strcpy(pbuf, "%p1%Pa%p2%Pb%ga%gb%02d%02d");
			(void) sprintf(pexp, "%02d%02d", j, i);
			expecting(VOID);
			result = Ztgoto(buf, j, i);
			if (strcmp(result, exp) != 0)
				error("integer variables error.  parameters '%d' '%d'.   '%s' got '%s'", j, i, pexp, result); 
		}
	}
	finis

	/* Integer constants */
	testing("push, pop and print in decimal format two integer constants.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i < SMLOOP; i++)
	{
		for (j = 0; j < VERYSM; j++)
		{
			(void) sprintf(pbuf, "%%{%d}%%{%d}%%d%%d",
				j, i);
			(void) sprintf(pexp, "%d%d", i, j);
			expecting(VOID);
			result = Ztgoto(buf);
			if (strcmp(result, exp) != 0)
				error("push and pop of two constants error.  constants '%d' '%d'.  expected '%s' got '%s'", j, i, pexp, result); 
		}
	}
	finis

	/* add to row and col */
	testing("add constants to two integer parameters, then pop the");
	testing("results and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0; i < VERYSM; i++)
	{
		for (j = 0; j < SMLOOP; j++)
		{
			(void) sprintf(pbuf, "%%p1%%{%d}%%+%%d%%p2%%{%d}%%+%%d", ADD, ADD);
			(void) sprintf(pexp, "%d%d", j + ADD, i + ADD);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("integer parameter with integer constant addition error.  '%d' added to parameters '%d' '%d'.  expected '%s' got '%s'", ADD, i, j, pexp, result);
		}
	}
	finis

	/* sub from row and col */
	testing("subtract constants from two integer parameters, then pop");
	testing("the results and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1; i < SMLOOP; i++)
	{
		for (j = 1; j < LGLOOP; j++)
		{
			(void) sprintf(pbuf, "%%p1%%{%d}%%-%%d%%p2%%{%d}%%-%%d", SUB, SUB);
			(void) sprintf(pexp, "%d%d", j - SUB, i - SUB);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("integer parameter with integer constant subtraction error.  '%d' subtracted from parameters '%d' '%d'.  expected '%s' got '%s'", SUB, i, j, pexp, result);
		}
	}
	finis

	/* multiply row and col */
	testing("multiply two integer parameters by constants, then pop");
	testing("the results and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1; i < VERYSM; i++)
	{
		for (j = 1; j < SMLOOP; j++)
		{
			(void) sprintf(pbuf, "%%p1%%{%d}%%*%%d%%p2%%{%d}%%*%%d", MUL, MUL);
			(void) sprintf(pexp, "%d%d", j * MUL, i * MUL);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("integer parameter with integer constant multiplication error.  parameters '%d' '%d' multiplied by '%d'.  expected '%s' got '%s'", i, j, MUL, pexp, result);
		}
	}
	finis

	/* Divide row and col */
	testing("divide two integer parameters by constants, then pop");
	testing("the results and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1; i < VERYSM; i++)
	{
		for (j = 1; j < SMLOOP; j++)
		{
			(void) sprintf(pbuf, "%%p1%%{%d}%%/%%d%%p2%%{%d}%%/%%d", DIV, DIV);
			(void) sprintf(pexp, "%d%d", j / DIV, i / DIV);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("integer parameter with integer constant division error.  parameters '%d' '%d' divided by '%d'.  expected '%s' got '%s'", i, j, DIV, pexp, result);
		}
	}
	finis

	/* modulus of row and col */
	testing("modulus of integer parameters and constants, then pop");
	testing("the results and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1; i < SMLOOP; i++)
	{
		for (j = 1; j < VERYSM; j++)
		{
			(void) sprintf(pbuf, "%%p1%%{%d}%%m%%d%%p2%%{%d}%%m%%d", MOD, MOD);
			(void) sprintf(pexp, "%d%d", j % MOD, i % MOD);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("integer parameter with integer constant modulus error.  parameters '%d' '%d' modulus with '%d'.  expected '%s' got '%s'", i, j, MOD, pexp, result);
		}
	}
	finis

	/* Bitwise or row and col of row and col */
	testing("Bitwise OR two integer parameters together then");
	testing("pop and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 1; i < SMLOOP; i++)
	{
		for (j = 1; j < VERYSM; j++)
		{
			(void) strcpy(pbuf, "%p1%p2%|%d");
			(void) sprintf(pexp, "%d", j | i);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("bitwise OR error. OR-ed '%d' with '%d'. expected '%s' got '%s'", j, i, pexp, result);
		}
	}
	finis

	/* Bitwise AND of row and col */
	testing("Bitwise AND two integer parameters together then");
	testing("pop and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = SMLOOP; i > 0; i--)
	{
		for (j = 1; j < VERYSM; j++)
		{
			(void) strcpy(pbuf, "%p1%p2%&%d");
			(void) sprintf(pexp, "%d", j & i);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("bitwise AND error. AND-ed '%d' with '%d'. expected '%s' got '%s'", j, i, pexp, result);
		}
	}
	finis

	/* Bitwise Exclusive OR of row and col */
	testing("Bitwise exclusive OR two integer parameters together");
	testing("then pop and print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = SMLOOP; i > 0; i--)
	{
		for (j = SMLOOP; j > 0; j--)
		{
			(void) strcpy(pbuf, "%p1%p2%^%d");
			(void) sprintf(pexp, "%d", j ^ i);
			expecting(VOID);
			result = Ztgoto(buf, i, j);
			if (strcmp(result, exp) != 0)
				error("bitwise exclusive OR error. OR-ed '%d' with '%d'. expected '%s' got '%s'", j, i, pexp, result);
		}
	}
	finis

	/* if and equal */
	testing("push two integer parameters.  if the first is greater");
	testing("than the second push the parameters again then pop and");
	testing("print in decimal format.");
	start
	break_on(WARNING | ERROR);
	for (i = 0, j = 1; i < VERYSM; i++, j++)
	{
		(void) strcpy(buf, "%p1%p2%?%>%t%p1%p2%d%d%;");
		(void) sprintf(pexp, "%d%d", i, j);
		expecting(VOID);
		result = Ztgoto(buf, i, j);
		if (strcmp(result, exp) != 0)
			error("If greater than error.  '%d' should be greater than '%d'.  expected '%s' got '%s'", j, i, pexp, result);
	}
	finis

	/* if and less than */
	testing("push two integer parameters.  if the first is less");
	testing("than the second push the parameters again then pop and");
	testing("print in decimal format else push in inverse order then");
	testing("pop and print");
	start
	break_on(WARNING | ERROR);
	for (i = SMLOOP, j = VERYSM; j > 0; i--, j--)
	{
		(void) strcpy(buf, "%p1%p2%?%<%t%%p1%p2%e%p2%p1%;%d%d");
		(void) sprintf(pexp, "%d%d", i, j);
		expecting(VOID);
		result = Ztgoto(buf, j, i);
		if (strcmp(result, exp) != 0)
			error("If less than error.  '%d' is not less than '%d'.  expected '%s' got '%s'", j, i, pexp, result);
	}
	finis

	/* if and equality */
	testing("push two integer parameters.  if the first is equal to ");
	testing("the second push the parameters again then pop and");
	testing("print in decimal format else print zero.");
	start
	break_on(WARNING | ERROR);
	for (i = 1, j = 1; i < VERYSM; i++, j++)
	{
		(void) strcpy(buf, "%p1%p2%?%=%t%p1%p2%d%d%e%{0}%d%;");
		(void) sprintf(exp, "%d%d", j, i);
		expecting(VOID);
		result = Ztgoto(buf, j, i);
		if (strcmp(result, exp) != 0)
			error("If less than error.  '%d' is equal '%d'.  expected '%s' got '%s'", j, i, pexp, result);
	}
	finis
}
