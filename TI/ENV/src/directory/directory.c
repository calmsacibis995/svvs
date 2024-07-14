# include	<sv_base.h> /*replacement macros */
# include	<stdio.h>
# include	<svvs.h>
# include	<sv_env.h>
# include	<sv_macro.h>
# include	<sccs.h>

SCCSID("@(#)directory.c	3.1 (SVVS) 12/1/86");

# define	DEFAULT_DIR	"/usr/lib/terminfo"
# define	MAX_PATH_NAME	1024
# define	READ_ACCESS	04

extern char	*getenv();

/*
** directory
**
**	Check to see if the terminfo database contains "tic"-ed descriptions
**	for SVVS1 throught SVVS4. Check first in the directory pointed to
**	by TERMINFO and then check in /usr/lib/terminfo if TERMINFO is not
**	defined.
*/
main()
{
	char	*terminfo_dir;
	int	TermID;
	char	path_name[MAX_PATH_NAME];

	setup("directory");

	prep("Check to see if TERMINFO is defined");
	start
		expecting(VOID);
		terminfo_dir = getenv("TERMINFO");
		if ((terminfo_dir == NULL) || (strlen(terminfo_dir) == 0))
		{
			terminfo_dir = DEFAULT_DIR;
		}
	finis

	for (TermID = 1; TermID <= 4; TermID++)
	{
		sprintf(path_name, "%s/S/SVVS%d", terminfo_dir, TermID);
		testing ("Check for read access on %s", path_name);
		start
			continue_on (ERROR | SEVERR);
			expecting (SUCCESS);

			access(path_name, READ_ACCESS);
		finis
	}

	done();
/*NOTREACHED*/
}
