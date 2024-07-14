# include <svvs.h>
# include <sv_env.h>
# include <sccs.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>

SCCSID("@(#)all.c	3.2 (SVVS) 12/23/86");

/*
**	all.c
**
**	all.c gets compiled into all.o.  all.o can then be linked
**	and named after one of the directories under svvs/test.  this
**	program will execute all of the tests under the directory whose 
**	name the program bears.
*/

char *tietl = "tietlx";


/*
**	the following arrays are lists of tietl tests to be
**	found under the various directories.
*/

/* areaclear */
char *acnames[] = {
	"addch",
	"addstr",
	"clear",
	"clrtobot",
	"clrtoeol",
	"delch",
	"deleteln",
	"delwin",
	"erase",
	"mvprintw",
	"mvwin",
	"mvwprintw",
	"overlay",
	"printw",
	"scroll",
	"waddch",
	"waddstr",
	"wclear",
	"wclrtobot",
	"wclrtoeol",
	"wdelch",
	"wdeleteln",
	"werase",
	"wprintw",
	""
};

/* delline */
char *dlnames[] = {
	"clear",
	"clrtobot",
	"copywin",
	"deleteln",
	"delwin",
	"erase",
	"insertln",
	"overlay",
	"overwrite",
	"scroll",
	"scrrest",
	""
};

/* highlight */
char *hinames[] = {
	"addch",
	"addstr",
	"beep",
	"clear",
	"clrtobot",
	"clrtoeol",
	"copywin",
	"delch",
	"deleteln",
	"delwin",
	"erase",
	"flash",
	"inch",
	"insertln",
	"overlay",
	"overwrite",
	"printw",
	"scrdump",
	"slkclear",
	"slklabel",
	"slkrestore",
	"slkset",
	"slktouch",
	"standout",
	""
};

/* input */
char *inpnames[] = {
	"flushinp",
	"getch",
	"getstr",
	"inch",
	"scanw",
	"unctrl",
	"ungetch",
	""
};

/* insert */
char *insnames[] = {
	"delch",
	"delwin",
	"inch",
	""
};

/* keypad */
char *keynames[] = {
	"getch",
	"getstr",
	"keypad",
	""
};

/* motion */
char *motnames[] = {
	"addch",
	"addstr",
	"box",
	"clear",
	"clrtobot",
	"clrtoeol",
	"copywin",
	"delch",
	"delwin",
	"erase",
	"horizontal",
	"insch",
	"move",
	"mvcur",
	"mvwin",
	"overlay",
	"overwrite",
	"printw",
	"scr_restore",
	"scroll",
	"slk_clear",
	"slk_init1",
	"slk_init2",
	"slk_label",
	"slk_nout",
	"slk_refresh",
	"slk_restore",
	"slk_set",
	"slk_touch",
	""
};

/* pads */
char *padnames[] = {
	"misc",
	"newpad",
	"padinput",
	"padnoecho",
	"pechochar",
	"pnoutrefresh",
	"prefresh",
	"subpad",
	""
};

/* slk */
char *slknames[] = {
	"slkclear",
	"slklabel",
	"slkrestore",
	"slkset",
	"slktouch",
	""
};

/* strings */
char *strngnames[] = {
	"putp1",
	"putp2",
	"vidattr",
	""
};

/* tabs */
char *tabnames[] = {
	"addch",
	"addstr",
	"delch",
	"initscr",
	"insch",
	"newwin",
	"overlay",
	"overwrite",
	"printw",
	""
};

/* windows */
char *winnames[] = {
	"copywin",
	"delwin",
	"mvwin",
	"newwin",
	"subwin",
	"touchwin",
	""
};

/* errors */
char *errnames[] =
{
	"mvwin",
	""
};

char	*Terminals[] =
{
	"SVVS1",
	"SVVS2",
	"SVVS3",
	"SVVS4",
	""
};

char whichdir[20];
int except();

# define STRSIZE	256

/*
**	main()
**
**	the name of the program indicates which directory of
**	tietl files to run - this program executes them all.
*/
main(argc, argv)
int argc;
char **argv;
{
	char	**terms;
	char	**filename;
	char	**initfiles;
	char	buf[STRSIZE];
	char	*bptr = buf;
	char	svvs_envbuf[STRSIZE];
	char	term_envbuf[STRSIZE];
	extern	char	*putenv();
	extern	int	errno;


	init(argv, &filename);
	setup(whichdir);
	testing("executing all subtests for %s", whichdir);
	start

	expecting(VOID);
	break_on(ERROR);
	sprintf(svvs_envbuf,"SVVS_TEST=%s",whichdir);
	putenv(svvs_envbuf);

	/*
	** filename has been set to point to the appropriate
	** array of names.  while the name pointed to is not null...
	*/
	initfiles = filename;

	for (terms = Terminals; **terms; terms++ )
	{
		if (except(whichdir, *terms, NULL))
		{
			continue;
		}

		prep("setting TERM=%s into the environment",*terms);
		start
			(void) sprintf(term_envbuf,"TERM=%s",*terms);
			exit_on(ERROR);
			putenv(term_envbuf);
		finis

		for (filename = initfiles; **filename; filename++)
		{
			if (except(whichdir, *terms, *filename))
			{
				continue;
			}

			/* open the file for tietl to read */
			
			prep("resetting stdin to be test file");
			start
				(void) sprintf(buf, "%s%s", data_path(),*filename);

				exit_on(ERROR);
				freopen(buf,"r",stdin);

			finis

			subtest(tietl);
		}
	}
	finis

	done();

/*NOTREACHED*/
}

/*
**	init() takes argv and strips off all the leading path
**	stuff so all that is left is a directory name.  this is
**	then put into the global var whichdir.
**	the array of names to be used is determined and file will
**	be set to point to that array.
*/
init(argv, file)
char **argv;
char ***file;
{
	char *p;
	char *w = whichdir;

	/* strip off any leading path stuff to leave only the dir name */
	if ((p = strrchr(*argv, '/')) == NULL)
		(void) strcpy(whichdir, *argv);
	else 
	{
		p++;
		while (*p)
			*w++ = *p++;
		*w = '\0';
	}

	if (strcmp(whichdir, "areaclear") == 0)
		*file = acnames;
	else if (strcmp(whichdir, "delline") == 0)
		*file = dlnames;
	else if (strcmp(whichdir, "highlight") == 0)
		*file = hinames;
	else if (strcmp(whichdir, "input") == 0)
		*file = inpnames;
	else if (strcmp(whichdir, "insert") == 0)
		*file = insnames;
	else if (strcmp(whichdir, "keypad") == 0)
		*file = keynames;
	else if (strcmp(whichdir, "motion") == 0)
		*file = motnames;
	else if (strcmp(whichdir, "pads") == 0)
		*file = padnames;
	else if (strcmp(whichdir, "slk") == 0)
		*file = slknames;
	else if (strcmp(whichdir, "strings") == 0)
		*file = strngnames;
	else if (strcmp(whichdir, "tabs") == 0)
		*file = tabnames;
	else if (strcmp(whichdir, "windows") == 0)
		*file = winnames;
	else if (strcmp(whichdir,"errors") == 0 )
		*file = errnames;
	else
	{
		setup(whichdir);
		printf("%s is not a valid test\n", whichdir);
		done();
	}
}

int except(sect, trm, tst)
char	*sect;
char	*trm;
char	*tst;
{
	if (strcmp(trm, "SVVS4") == 0)
	{
		if (strcmp(sect, "motion") == 0)
		{
			if (strcmp(tst, "horizontal") == 0)
			{
				return(0);
			}
		}
		else if (strcmp(sect, "strings") == 0)
		{
			return(0);
		}
		return(1);
	}

	if (strcmp(trm, "SVVS2") == 0)
	{
		if (strcmp(sect, "strings") == 0)
		{
			return(1);
		}
	}

	return(0);
}
