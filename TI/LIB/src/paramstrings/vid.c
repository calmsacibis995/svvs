# include <sv_base.h>
# include <svvs.h>
# include <sv_env.h>
# include <string.h>
# include <curses.h>

/*
**	vid.c
**
**	contains vputs() - tests vidputs().  output of vidputs is
**	sent to the global buffer outbuf by way of outc().
*/

# include <ctype.h>
# include <sccs.h>

SCCSID("@(#)vid.c	3.2 (SVVS) 12/30/86");

extern char	outbuf[];
extern char	*outptr;
extern int	outc();
extern int	RealTTY;

int atts[] = {
	A_STANDOUT,
	A_UNDERLINE,
	A_REVERSE,
	A_BLINK,
	A_DIM,
	A_BOLD
};

char *names[] = {
	"smso",
	"smul",
	"rev",
	"blink",
	"dim",
	"bold"
};

char *Ztigetstr();

# define STRSIZE	256	/* length of buffers to hold strings */

# define LASTTERM	4	/* last number of SVVSx term descriptions */
# define FIRSTTERM	1	/* first number of SVVSx term descriptions */

# define NUMATTS	6	/* number of screen attributes */

/*
**	vputs()
**
**	put each of the attributes and all possible combinations
**	thereof and compare the result in outbuf to expected output.
*/
vputs()
{
	int i;
	int j;
	int k;
	char buf[STRSIZE];
	int errret;
	char *expect;
	char *capname;
	char *pad_str;

	/* for each SVVSx terminal description */
	for (i = FIRSTTERM; i <= LASTTERM; i++)
	{
		prep("do setupterm for SVVS%d.", i);
		start
		(void) sprintf(buf, "%s%d", "SVVS", i);
		if (Zsetupterm(buf, RealTTY, &errret) == ERR)
		{
			if (errret == 0)
				error("%s could not be found in terminfo database.", buf);
			else if (errret == -1)
				error("terminfo database could not be found.");
			else 
				error("setupterm returned an error.");
		}
		finis

		prep("get padding string if one exists");
		start
			expecting(VOID);
			exit_on(WARNING | ERROR | SEVERR);

			if ((pad_str = Ztigetstr("pad")) == (char *) -1)
			{
				error("tigetstr(\"pad\") returned error");
			}
		finis

		testing("single attributes.");
		start
		break_on(WARNING | ERROR);
		/* for each attribute */
		for (j = 0; j < 6; j++)
		{
			nullbuf();
			clrbuf(buf);
			expecting(VOID);
			Zvidputs(atts[j], outc);
			if (!checkattrs(atts[j], pad_str))
				error("%s failed, got '%s'", names[j], outbuf);
			nullbuf();
			clrbuf(buf);
			expecting(VOID);
			Zvidputs(0, outc);
		}
		finis

		expecting(VOID);
		Zvidputs(0, outc);

		testing("turn off single attribute.");
		start
		break_on(WARNING | ERROR);
		/* turning off each attribute */
		for (j = 0; j < 6; j++)
		{
			nullbuf();
			clrbuf(buf);
			expecting(VOID);
			Zvidputs(atts[j], outc);
			nullbuf();
			clrbuf(buf);
			switch (atts[j])
			{
			  case A_UNDERLINE :	
				capname = "rmul";
				break;
			  case A_STANDOUT :	
				capname = "rmso";
				break;
			  case A_DIM :
			  case A_BOLD :
			  case A_BLINK :
			  case A_REVERSE :	
				capname = "sgr0";
				break;
			  default :		
				error("Switch error - Invalid attribute!");
				break;
			}
			expecting(VOID);
			expect = Ztigetstr(capname);
			if ((expect != (char *) -1) && (expect != NULL))
			{
				(void) strcpy(buf, expect);
				tputs(buf, 1, outc);
				if (!compare(buf, pad_str))
					error("Attribute off failed, expected '%s', got '%s'", buf, outbuf);
			}
		}
		finis

		Zvidputs(0, outc);
		expecting(VOID);

		testing("all combinations of two attributes.");
		start
		break_on(WARNING | ERROR);
		/* for each combination of two attributes */
		for (j = 0; j < 6; j++)
		{
			for (k = j + 1; k < 6; k++)
			{
				nullbuf();
				clrbuf(buf);
				expecting(VOID);
				Zvidputs(atts[j] | atts[k], outc);
				if (!checkattrs((atts[j] | atts[k]), pad_str))
				error("%s and %s failed, got '%s'", names[j], names[k], outbuf);
				nullbuf();
				clrbuf(buf);
				expecting(VOID);
				Zvidputs(0, outc);
			}
		}
		finis

		cleanup("vidputs(0) to clear screen of any attributes.");
		start
		expecting(VOID);
		Zvidputs(0, outc);
		finis

		testing("combination of all the attributes.");
		start
		break_on(WARNING | ERROR);

		/* all attributes at once */
		nullbuf();
		clrbuf(buf);
		expecting(VOID);
		Zvidputs(A_STANDOUT | A_REVERSE | A_UNDERLINE | A_DIM |
			A_BOLD | A_BLINK, outc);
		if (!checkattrs((A_STANDOUT | A_REVERSE | A_UNDERLINE |
			A_DIM | A_BOLD | A_BLINK), pad_str))
			error("All attributes failed, expected '%s', got '%s'", buf, outbuf);
		finis
	}

	cleanup("setupterm to restore original state.");
	start
	if (Zsetupterm(NULL, RealTTY, &errret) == ERR)
	{
		if (errret == 0)
			error("%s could not be found in terminfo database.", buf);
		else if (errret == -1)
			error("terminfo database could not be found.");
		else 
			error("setupterm returned an error.");
	}
	finis
}

/*
**	checkattrs() decides which attributes were passed to vidputs,
**	gets the string capability for these and searches for them in
**	the output in outbuf.
*/
checkattrs(attrs, pad_str)
int attrs;
char *pad_str;
{
	int i = 0;
	int ok = 1;
	int dm, bl, ul, rv, so, bo;
	char *p;
	char *sgr;
	char *str;
	char tmp[STRSIZE];
	char *t = tmp;
	char *pos();
	extern char *Ztparm();

	/*
	** copy to a temp var because it gets taken apart while
	** looking for the various attributes
	*/
	*t = '\0';
	(void) strcpy(t, outbuf);

	/* for each attribute and while each is found in the output */
	while (ok && i < NUMATTS && *t)
	{

		/* if the attribute was specified */
		if (attrs & atts[i])
		{
			expecting(VOID);
			str = Ztigetstr(names[i]);

			if (str == (char *) -1)
			{
				error("tigetstr error on '%s'", names[i]);
			}
			else if (str == NULL)
			{
				i++;
			}
			else
			{
				if (*str)
				{
					padding(str);
					padchars(t, pad_str);
					if ((p = pos(str, t)) != (char *) -1)
					{
						remove(p, str, t);
						i++;
					}
					else
						ok = 0;
				}
				else
					i++;
			}
		}
		else
			i++;
	}

	/* if string isn't empty now, then it's wrong */
	if (ok && *t != '\0')
		ok = 0;

	/*
	** if there was an error, check to see if sgr was used
	** to set the attributes instead
	*/
	if (!ok)
	{
		(void) strcpy(t, outbuf);

		dm = bl = ul = rv = so = bo = 0;
		expecting(VOID);
		str = Ztigetstr("sgr");
		if (str == (char *) -1)
		{
			error("tigetstr(sgr) returned error");
		}
		else if (str != NULL)
		{
			if (attrs & A_DIM)
				dm = 1;
			if (attrs & A_BLINK)
				bl = 1;
			if (attrs & A_UNDERLINE)
				ul = 1;
			if (attrs & A_REVERSE)
				rv = 1;
			if (attrs & A_STANDOUT)
				so = 1;
			if (attrs & A_BOLD)
				bo = 1;
			sgr = Ztparm(str, so, ul, rv, bl, dm, bo, 0, 0, 0);

			padding(sgr);
			padchars(t, pad_str);

			if (strcmp(sgr, t) == 0)
				ok = 1;
		}
	}

	/* if string isn't empty now, then it's wrong */
	if (ok && *t != '\0')
		ok = 0;

	return(ok);
}

/*
**	pos() looks for the first string in the second and
**	returns a pointer to it or -1 if it's not there.
*/
char *pos(find, string)
char *find;
char *string;
{
	char *s = string;
	int found = 0;

	while(*s && !found)
	{
		if (*s == *find)
		{
			if (strncmp(find, s, strlen(find)) == 0)
				found = 1;
			else
				s++;
		}
		else 
			s++;
	}
	if (found)
		return(s);
	else
		return((char *) -1);
}

/*
**	remove() takes the string rem out of string.
**	p indicates where in string rem is.
*/
remove(p, rem, string)
char *p;
char *rem;
char *string;
{
	char *s = string;
	char tmp[STRSIZE];
	char *t = tmp;

	*t = '\0';

	if (p == s && strlen(s) == strlen(rem)
		&& strcmp(s, rem) == 0)
		*s = '\0';
	else
	{
		while (p != s)
			*t++ = *s++;

		s += strlen(rem);

		while (*s)
			*t++ = *s++;

		*t = '\0';

		(void) strcpy(string, tmp);
	}
}

/*
**	clrbuf() puts all nulls in buf
*/
clrbuf(buf)
char *buf;
{
	int i;

	for (i = 0; i < STRSIZE; i++)
		buf[i] = '\0';
}

/*
**	padding() removes any padding indications from the string
*/
padding(str)
char *str;
{
	char temp[STRSIZE];
	char *t = temp;
	char *sptr = str;
	char *tptr;
	extern char *strchr();

	if ((tptr = strchr(sptr, '$')) != NULL)
	{
		while (*sptr != '\0' && (tptr = strchr(sptr, '$')) != NULL)
		{
			if (*(tptr + 1) == '<' && strchr(tptr, '>') != NULL)
			{
				while (*sptr != '$')
					*t++ = *sptr++;
				while (*sptr != '>')
					sptr++;
				sptr++;
				while (*sptr != '\0' && *sptr != '$')
					*t++ = *sptr++;
			}
			else
			{
				if (*sptr == '$' && (*(sptr + 1) != '<'))
					*t++ = *sptr++;
				while (*sptr != '\0' && *sptr != '$')
					*t++ = *sptr++;
			}
		}
		*t = '\0';
		(void) strcpy(str, temp);
	}
}

/*
**	padchars() removes padding characters from the string
*/
padchars(str, pad_str)
char *str;
char *pad_str;
{
	char *sptr = str;
	char temp[STRSIZE];
	char *t = temp;

	if ((pad_str == NULL) || (strlen(pad_str) == 0))
	{
		return;
	}

	if (strchr(sptr, *pad_str) != NULL)
	{
		while (strchr(sptr, *pad_str) != NULL)
		{
			while (*sptr != *pad_str)
				*t++ = *sptr++;

			/* skip null pad chars */
			while (*sptr == *pad_str && sptr < str + strlen(str))
				sptr++;

			if (sptr < str + strlen(str))
			{
				while (*sptr != '\0'  && *sptr != *pad_str)
					*t++ = *sptr++;
			}
			else
				break;
		}
		*t = '\0';

		(void) strcpy(str, temp);
	}
}
