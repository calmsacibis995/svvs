# include <sv_base.h>
# include <svvs.h>
# include <sv_env.h>
# include <string.h>
# include <curses.h>

/*
**	tput.c
**
**	contains tput() and put() which test tputs and putp.
*/

# define STRSIZE	256	/* size of buffers used to hold strings */

# define CHARS		256	/* integers to check for ascii chars */

# define ONELINE	1	/* used in call to tputs when the effect */
				/* is for one line only			 */

# define LOTHIRD	40	/* lower third of ascii char set in a loop  */
# define MIDTHIRD	80	/* middle third of ascii char set in a loop */
# define HITHIRD	120	/* high third of ascii char set in a loop   */
# define NUMPADS	10	/* padding in tputs calls varies from 1 to  */
				/* 10 in loops */

# include <ctype.h>
# include <sccs.h>

SCCSID("@(#)tput.c	3.1 (SVVS) 12/1/86");

extern char	outbuf[];
extern char	*outptr;
extern int	outc();
extern int	RealTTY;

/*
**	tput()
**
**	tput() calls tputs() with various combinations of ascii
**	characters.  the output of tputs goes to the character
**	array outbuf.  the output is then compared to the expected
**	string of characters.
*/
tput()
{
	char		*pad_str;
	char		buf[STRSIZE];
	int		errret;
	int		i;
	int		j;
	int		k;
	extern char	*Ztigetstr();

	prep("setupterm for SVVS4 - SVVS4 defines the pad character to be '\\001'.");
	start
		exit_on(WARNING | ERROR | SEVERR);
		expecting(VOID);

		/* SVVS4 defines pad char as \001 */

		if (Zsetupterm("SVVS4", RealTTY, &errret) == ERR)
		{
			if (errret == 0)
				error("SVVS4 could not be found in terminfo database.");
			else if (errret == -1)
				error("terminfo database could not be found.");
			else 
				error("setupterm returned an error.");
		}
	finis

	prep("get pad string. pad string must be defined to run this test");
	start
		exit_on(WARNING | ERROR | SEVERR);
		expecting(VOID);

		pad_str = Ztigetstr("pad");
		if (pad_str == (char *) -1)
		{
			error("tigetstr(\"pad\") returned error");
		}
		else if (pad_str == NULL)
		{
			error("pad capability is not defined for this terminal.");
		}
		else if (*pad_str != '\001')
		{
			error("pad string = \\%d; expected \001", *pad_str);
		}
	finis

	testing("tputs() of NULL ('\\200').");
	start
	break_on(WARNING | ERROR);
	/* \200 - null for terminals */
	nullbuf();
	(void) sprintf(buf, "%c", '\200');
	expecting(VOID);
	tputs(buf, ONELINE, outc);
	if (strcmp(buf, outbuf) != 0)
		error("NULL failed, expected '%s', got '%s'", buf, outbuf);
	finis

	testing("tputs() of each single ascii character.");
	start
	break_on(WARNING | ERROR);
	/* ascii characters */
	for (i = 0; i < CHARS; i++)
	{
		if (isascii(i))
		{
			nullbuf();
			(void) sprintf(buf, "%c", i);
			expecting(VOID);
			tputs(buf, ONELINE, outc);
			if (strcmp(buf, outbuf) != 0)
				error("single char failed expected '%s', got '%s'", buf, outbuf);
		}
	}
	finis

	testing("tputs() of strings that consist of three ascii characters.");
	start
	break_on(WARNING | ERROR);
	/* ascii ascii ascii strings */
	for (i = 1; i < LOTHIRD; i++)
	{
		for (j = LOTHIRD; j < MIDTHIRD; j++)
		{
			for (k = MIDTHIRD; k < HITHIRD; k++)
			{
				if (isascii(i) && isascii(j) && isascii(k))
				{
					nullbuf();
					(void) sprintf(buf, "%c%c%c", i, j, k);
					expecting(VOID);
					tputs(buf, ONELINE, outc);
					if (strcmp(buf, outbuf) != 0)
						error("string failed, expected '%s', got '%s'", buf, outbuf);
				}
			}
		}
	}
	finis

	testing("tputs() of strings that consist of six ascii characters.");
	start
	break_on(WARNING | ERROR);
	/* ascii ascii ascii ascii ascii ascii strings */
	for (i = 1; i < LOTHIRD; i++)
	{
		for (j = LOTHIRD; j < MIDTHIRD; j++)
		{
			for (k = MIDTHIRD; k < HITHIRD; k++)
			{
				if (isascii(i) && isascii(j) && isascii(k))
				{
					nullbuf();
					(void) sprintf(buf, "%c%c%c%c%c%c",
						i, j, k, j, k, i);
					expecting(VOID);
					tputs(buf, ONELINE, outc);
					if (strcmp(buf, outbuf) != 0)
						error("tputs() failed, expected '%s', got '%s'", buf, outbuf);
				}
			}
		}
	}
	finis

# ifdef DO_PAD_TEST
	testing("tputs() of ascii strings with padding specified");
	testing("in the middle of the string.");
	start
	break_on(WARNING | ERROR);
	/* ascii pad ascii strings */
	for (i = 2; i < LOTHIRD; i++)
	{
		for (j = 1; j < NUMPADS; j++)
		{
			for (k = MIDTHIRD; k < HITHIRD; k++)
			{
				if (isascii(i) && isascii(k))
				{
					nullbuf();
					(void) sprintf(buf, "%c$<%d>%c",
						i, j, k);
					expecting(VOID);
					tputs(buf, ONELINE, outc);

					if (!compare(buf, pad_str))
						error("padding in the middle failed, expected '%s', got '%s'", buf, outbuf);
				}
			}
		}
	}
	finis

	testing("tputs() of ascii strings with padding at the end.");
	start
	break_on(WARNING | ERROR);
	/* ascii ascii pad strings */
	for (i = 2; i < LOTHIRD; i++)
	{
		for (j = MIDTHIRD; j < HITHIRD; j++)
		{
			for (k = 1; k < NUMPADS; k++)
			{
				if (isascii(i) && isascii(j))
				{
					nullbuf();
					(void) sprintf(buf, "%c%c$<%d>",
						i, j, k);
					expecting(VOID);
					tputs(buf, ONELINE, outc);
					if (!compare(buf, pad_str))
						error("padding at the end failed, expected '%s', got '%s'", buf, outbuf);
				}
			}
		}
	}
	finis

	testing("tputs() of ascii strings with padding at the beginning.");
	start
	break_on(WARNING | ERROR);
	/* pad ascii ascii strings */
	for (i = 1; i < NUMPADS; i++)
	{
		for (j = LOTHIRD; j < MIDTHIRD; j++)
		{
			for (k = MIDTHIRD; k < HITHIRD; k++)
			{
				if (isascii(j) && isascii(k))
				{
					nullbuf();
					(void) sprintf(buf, "$<%d.%d>%c%c",
						i, i, j, k);
					expecting(VOID);
					tputs(buf, ONELINE, outc);
					if (!compare(buf, pad_str))
						error("padding at the beginning failed, expected '%s', got '%s'", buf, outbuf);
				}
			}
		}
	}
	finis
# endif /* DO_PAD_TEST */

	cleanup("setupterm to restore original state.");
	start
	/* restore to whatever TERM is */
	if (Zsetupterm(NULL, RealTTY, &errret) == ERR)
	{
		error("setupterm returned an error.");
	}
	finis
}

/*
**	compare()
**
**	checks to see that padding was added to outbuf, removes
**	it and compares what's left to buf.
*/
compare(buf, pad)
char *buf;
char *pad;
{
	int found = 0;
	char *optr = outbuf;
	char save[STRSIZE];
	char newbuf[STRSIZE];
	char *sptr = save;
	char *nptr = newbuf;
	char *bptr = buf;
	char *tptr;
	char *Ztigetstr();
	char *strchr();

	/* remove pad things ( $<>'s ) from buf */
	if (strchr(bptr, '$') != NULL)
	{
		while (*bptr != '\0' && (tptr = strchr(bptr, '$')) != NULL)
		{
			if (*(tptr + 1) == '<' && strchr(tptr, '>') != NULL)
			{
				found = 1;
				while (*bptr != '$')
					*nptr++ = *bptr++;
				while (*bptr != '>')
					bptr++;
				bptr++;
				while (*bptr != '\0' && *bptr != '$')
					*nptr++ = *bptr++;
			}
			else
			{
				if (*bptr == '$' && (*(bptr + 1) != '<'))
					*nptr++ = *bptr++;
				while (*bptr != '\0' && *bptr != '$')
					*nptr++ = *bptr++;
			}
		}
		*nptr = '\0';
	}
	else 
		(void) strcpy(newbuf, buf);

	if (pad == NULL)
	{
		return (strcmp(outbuf, newbuf) == 0);
	}

	/* remove all the pad chars from outbuf */

	if (strchr(optr, *pad) != NULL)
	{
		while (strchr(optr, *pad) != NULL)
		{
			while (*optr != *pad)
				*sptr++ = *optr++;

			/* skip null pad chars */
			while (*optr == *pad && optr < outbuf + STRSIZE)
				optr++;

			if (optr < outbuf + STRSIZE)
			{
				while (*optr != '\0'  && *optr != *pad)
					*sptr++ = *optr++;
			}
			else
				break;
		}
		*sptr = '\0';
	}
	else
	{

# ifdef NO_PAD_TEST
		if (found && (*pad != '\0'))
		{
			error("result did not contain padding.  expected '%s', got '%s'", buf, outbuf);
		}
# endif /* NO_PAD_TEST */

		(void) strcpy(save, outbuf);
	}

	return (strcmp(save, newbuf) == 0);
}
