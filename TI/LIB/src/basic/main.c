# include <stdio.h>
# include <ctype.h>
# include <sys/types.h>
# include <svvs.h>
# include <sv_env.h>
# include <string.h>
# include <sccs.h>
# include "basiccap.h"

SCCSID("@(#)main.c	3.1 (SVVS) 12/1/86");

static char *skip();

int GetFlag;


/*
**	main()
**
**	read and parse each terminfo description twice.
**	once to verify that tigetflag, tigetnum and tigetstr
**	function correctly and once for tgetflag, tgetnum
**	and tgetstr.
*/
main()
{
	int		i;
	int		j;
	FILE		*fp;
	char		*term_name;
	extern FILE	*data_fopen();
	extern char	*getenv();

	setup("basiccap");

	preparation("Get terminal name from environment variable and");
	preparation("initialize terminal data structures");
	start
		exit_on(WARNING| ERROR | SEVERR);
		expecting(SUCCESS);

		term_name = getenv("SVVS_TERM");
		Zsetupterm(term_name, 2, NULL);
	finis

	preparation("Open alternate terminfo database file");
	preparation("for terminfo test comparison.");
	start
		exit_on(WARNING | ERROR | SEVERR);
		expecting(SUCCESS);

		if ((fp = data_fopen(term_name, "r")) == NULL)
		{
			error("cannot open %s for input.", term_name);
		}
	finis

	testing("terminfo capabilities for %s.", term_name);
	start
		break_on(WARNING | ERROR);

		GetFlag = TERMINFO;
		read_file(fp);
	finis

	testing("error conditions of terminfo routines.");
	start
		break_on(WARNING | ERROR);

		check_errs();
	finis

	preparation("Open alternate terminfo database file");
	preparation("for termcap test comparison.");
	start
		exit_on(WARNING| ERROR | SEVERR);
		expecting(SUCCESS);

		if ((fp = data_fopen(term_name, "r")) == NULL)
		{
			error("cannot open %s for input.", term_name);
		}
	finis

	testing("termcap capabilities for %s.", term_name);
	start
		break_on(WARNING | ERROR);

		GetFlag = TERMCAP;
		read_file(fp);
	finis

	cleanup("Resetting terminal to pre-test condition");
	start
		exit_on(WARNING| ERROR | SEVERR);
		expecting(SUCCESS);

		Zreset_shell_mode();
	finis

	return(0);
}

/*
**	read_file()
**
**	read in the terminfo description file and then
**	call parsebuf() to parse and verify it.
*/
read_file(term_file)
FILE *term_file;
{
	register char *cp;
	char	*termbuf;
	char	bufptr[CAPSIZ];
	char	inputbuf[CAPSIZ];

	if (term_file == NULL) {
		error("Null file name");
	}
	inputbuf[0] = 0;
	termbuf = bufptr;
	(void) strcpy(bufptr, inputbuf);
	for (;;) {
		if (fgets(inputbuf, sizeof inputbuf, term_file) == NULL) {
			(void) fclose(term_file);
			parsebuf(termbuf);
			break;
		}
		/* comment or blank line */
		if (inputbuf[0] == COMMENT || inputbuf[0] == '\n')
			continue;
		cp = &inputbuf[strlen(inputbuf)-3];
		/* Allow and ignore old style backslashes */
		if (*cp == SEPARATE && cp[1] == '\\')
			cp[1] = 0;
		cp[2] = 0;	/* get rid of newline */
		/* lines with leading white space are continuation */
		if (!isspace(inputbuf[0]) && *bufptr)
			break;
		if (strlen(bufptr) + strlen(inputbuf) >= CAPSIZ)
			error("terminfo entry too long:\n%s", bufptr);
		else {
			cp = inputbuf;
			while (isspace(*cp))
				cp++;
			(void) strcat(bufptr, cp);
		}
	}
}

/*
**	parsebuf()
**
**	for each pair of capnames and capstrings, call get() or
**	getcap() to verify that the same information is in the
**	terminfo file.
*/
parsebuf(buffer)
char	*buffer;
{
	char *bufptr = buffer;
	char termname[20];
	char *termptr = termname;
	struct cap current_cap;

	while (*bufptr && *bufptr != SEPARATE && *bufptr != NAMESEPARATOR &&
	       termptr < &termname[20])
		*termptr++ = *bufptr++;
	if (termptr < &termname[20])
		*termptr = '\0';
	else
		termname[19] = '\0';

	for (buffer = skip(buffer); *buffer; buffer = skip(buffer))
	{
		if (*buffer != CAPCOMMENT)
		{
			lookup(buffer, &current_cap);
			if (GetFlag == TERMINFO)
				get(&current_cap);
			else
				getcap(&current_cap);
		}
	}
}

/*
** Skip to the next SEPARATE delimited field. Allow \SEPARATE escape.
*/
static char *
skip(bp)
register char *bp;
{

	while (*bp && *bp != SEPARATE)
		if ((*bp++ == '\\') && *bp)
			bp++;
	if (*bp == 0)
		return (bp);
	bp++;
	while (isspace(*bp) || *bp == SEPARATE)
		bp++;
	return (bp);
}

/*
**	lookup()
**
**	separate the capability name and its string and return
**	them in cur_cap.
*/
lookup(buf, cur_cap)
register char 	*buf;
struct	cap	*cur_cap;
{
	char	name[NAMESIZE];
	char	*nptr = name;

	/* a name is followed by a SEPARATE, @, # or =. */
	while (*buf && *buf != SEPARATE && *buf != CANCEL && *buf != STRNG 
		&& *buf != NUMBER && !isspace(*buf) && nptr < &name[NAMESIZE-1])
		*nptr++ = *buf++;
	*nptr = '\0';

	switch(*buf) {
		case STRNG:		/* string capability */
			getstring(buf, cur_cap->cap_string);
			break;
		case NUMBER:		/* numeric capability */
			getnumber(buf, cur_cap->cap_string);
			break;
		case SEPARATE:		/* boolean capability */
			cur_cap->cap_string[0] = '\0';
			break;
		case CANCEL:		/* may be boolean, number or string */
			break;
		case ' ':
		case '\t':
			break;
		default:
			break;
	}

	(void) strcpy(cur_cap->cap_name, name);
}

getstring(buffer, str)
register char	*buffer;
register char	*str;
{
	char	*s_sptr;

	buffer++;	/* skip the equals sign */
	s_sptr = buffer;

	/* a string is followed by a SEPARATE */
	while (*buffer && *buffer != SEPARATE && !isspace(*buffer) &&
			buffer - s_sptr < CAPSIZ)
		buffer++;

	(void) strncpy(str, s_sptr, (buffer - s_sptr));
	*(str + (buffer - s_sptr)) = '\0';
}

getnumber(buffer, str)
register char	*buffer;
register char	*str;
{
	char	*n_sptr;

	buffer++; /* skip the pound sign */
	n_sptr = buffer;

	/* a number is followed by a SEPARATE */
	while (*buffer && *buffer != SEPARATE && !isspace(*buffer) 
		&& buffer - n_sptr < CAPSIZ)
		buffer++;

	(void) strncpy(str, n_sptr, (buffer - n_sptr));
	*(str + (buffer - n_sptr)) = '\0';
}

/*
**	check_errs()
**
**	calls tigetstr(), tigetflag() and tigetnum with names
**	of capabilities that don't exist or are not of the 
**	proper type for the routine.  verifies that the correct
**	error is returned.
*/
check_errs()
{
	extern char *Ztigetstr();

	expecting(VOID);
	if (Ztigetflag("foo") != -1)
		error("tigetflag(\"foo\") successful - should have returned -1");
	expecting(VOID);
	if (Ztigetflag("cup") != -1)
		error("tigetflag(\"cup\") successful - should have returned -1");

	expecting(VOID);
	if (Ztigetnum("bar") != -2)
		error("tigetnum(\"bar\") successful - should have returned -2");
	expecting(VOID);
	if (Ztigetnum("cup") != -2)
		error("tigetnum(\"cup\") successful - should have returned -2");

	expecting(VOID);
	if (Ztigetstr("foobar") != (char *) -1)
		error("tigetstr(\"cup\") successful - should have returned (char *) -1");
	expecting(VOID);
	if (Ztigetstr("co") != (char *) -1)
		error("tigetstr(\"cup\") successful - should have returned (char *) -1");
}
