# include <svvs.h>
# include <curses.h>
# include <sv_env.h>
# include <tiuser.h>
# include <sccs.h>

SCCSID("@(#)pvtest.c	3.2 (SVVS) 12/30/86");

/*
**	pvtest.c
**
**	contains test_vidattr() and test_putp() which check the
**	output of vidattr and putp curses routines.
*/

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

# define STRSIZE	256

/*
**	test_vidattr()
**
**	test_vidattr gets each attribute string that is needed and
**	looks for this string in the result.
*/
void test_vidattr(result, len, attrs)
char *result;
int len;
int attrs;
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
	extern char *Ztigetstr();

	*(result + len) = '\0';

	/* copy result to t to take it apart and look for attributes */
	(void) strcpy(t, result);


	/*
	** for each attribute, while the string is not null and
	** we have found all the attributes...
	*/
	while (ok && i < 6 && *t)
	{
		/* if the particular attribute was specified */
		if (attrs & atts[i])
		{
			str = Ztigetstr(names[i]);

			if (str == (char *) -1)
			{
				msg_drop("tigetstr error on '%s'", names[i]);
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
					padchars(t);
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
		(void) strcpy(t, result);

		dm = bl = ul = rv = so = bo = 0;

		str = Ztigetstr("sgr");
		if (str == (char *) -1)
		{
			msg_drop("tigetstr(sgr) returned error");
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
			padchars(t);

			if (strcmp(sgr, t) == 0)
				ok = 1;
		}
	}

	if (!ok || (*t != '\0'))
		msg_drop("vidattr(%lx) failed.  got '%s'", attrs, tmp);
}

/*
**	test_putp()
**
**	if padding was specified in the expect string, this is
**	removed (%<>'s).  if the result has padding chars, these
**	are removed.  then the strings are compared.
*/
void test_putp(result, len, expect)
char *result;
int len;
char *expect;
{
	char *rptr = result;
	char *save;
	char *newbuf; char *sptr;
	char *nptr;
	char *eptr = expect;
	char *tptr;
	char *pad;
	char *Ztigetstr();
	char *strchr();
	int found = 0;
	extern char *malloc();

	*(result + len) = '\0';

	if ((save = malloc((unsigned) (strlen(result) + 1))) == NULL)
		screech("test_putp", 0, "malloc failed");
	sptr = save;

	if ((newbuf = malloc((unsigned) (strlen(expect) + 1))) == NULL)
		screech("test_putp", 0, "malloc failed");
	nptr = newbuf;

	if ((pad = Ztigetstr("pad")) == (char *) -1)
		msg_drop("tigetstr(\"pad\") error.  cannot continue compare.");

	/* remove pad things ( $<>'s ) from expect */
	if ((tptr = strchr(eptr, '$')) != NULL)
	{
		while (*eptr != '\0' && (tptr = strchr(eptr, '$')) != NULL)
		{
			if (*(tptr + 1) == '<' && strchr(tptr, '>') != NULL)
			{
				found = 1;
				while (*eptr != '$')
					*nptr++ = *eptr++;
				while (*eptr != '>')
					eptr++;
				eptr++;
				while (*eptr != '\0' && *eptr != '$')
					*nptr++ = *eptr++;
			}
			else
			{
				if (*eptr == '$' && (*(eptr + 1) != '<'))
					*nptr++ = *eptr++;
				while (*eptr != '\0' && *eptr != '$')
					*nptr++ = *eptr++;
			}
		}
		*nptr = '\0';
	}
	else 
		(void) strcpy(newbuf, expect);

	/* 
	** remove all the pad chars from result if and only if a pad
	** character exists.
	*/

	if (pad && (strlen(pad) > 0))
	{
		if (strchr(rptr, *pad) != NULL)
		{
			while (strchr(rptr, *pad) != NULL)
			{
				while (*rptr != *pad)
					*sptr++ = *rptr++;

				/* skip null pad chars */
				while (*rptr == *pad && rptr < result + len)
					rptr++;

				if (rptr < result + len)
				{
					while (*rptr != '\0'  && *rptr != *pad)
						*sptr++ = *rptr++;
				}
				else
					break;
			}
			*sptr = '\0';
		}
		else
		{

# ifdef DO_PAD_TEST
			if (found)
			{
				msg_drop("Error, result did not contain padding.  Expected '%s', got '%s'\n", newbuf, result);
				return;
			}
# endif /* DO_PAD_TEST */

			(void) strcpy(save, result);
		}
	}
	else
	{
		(void) strcpy(save, result);
	}

	if (strcmp(save, newbuf) != 0)
		msg_drop("putp() failed.  expected '%s', got '%s'\n", newbuf, save);
	
	free(newbuf);
	free(save);
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

	while(*s)
	{
		if (*s == *find)
		{
			if (strncmp(find, s, strlen(find)) == 0)
				break;
		}
		s++;
	}

	if (*s)
		return(s);
	else
		return((char *) -1);
}

/*
**	remove() takes the first string, rem, out of string.
**	p tells the position in string of rem.
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
**	padding() removes any padding indicators from the string
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
padchars(str)
char *str;
{
	char *sptr = str;
	char temp[STRSIZE];
	char *t = temp;
	char *pad;

	if ((pad = Ztigetstr("pad")) == (char *) -1)
		msg_drop("tigetstr(\"pad\") error.  cannot complete compare.");

	if (pad && (strchr(sptr, *pad) != NULL))
	{
		while (strchr(sptr, *pad) != NULL)
		{
			while (*sptr != *pad)
				*t++ = *sptr++;

			/* skip null pad chars */
			while (*sptr == *pad && sptr < str + strlen(str))
				sptr++;

			if (sptr < str + strlen(str))
			{
				while (*sptr != '\0'  && *sptr != *pad)
					*t++ = *sptr++;
			}
			else
				break;
		}
		*t = '\0';

		(void) strcpy(str, temp);
	}

}
