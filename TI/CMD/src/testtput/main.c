# include <curses.h>
# include <sccs.h>
# include <svvs.h>
# include <sv_env.h>

/*
**	main.c
**
**	main.c contains main() which calls setup and abstract
**	and does the necessary setupterms before calling test_tput().
**
*/

SCCSID("@(#)main.c	3.2 (SVVS) 12/5/86");

# define STRSIZE	256

# define ENV		1	/* to determine whether to do the system of  */
# define PARM		0	/* tput with an environment var or parameter */

main()
{
	int	errret;
	char	*term_name;
	char	*getenv();

	setup("testtput");

	prep("get terminal type from environment variable");
	start
		exit_on(WARNING | ERROR | SEVERR);
		expecting(SUCCESS);

		term_name = getenv("SVVS_TERM");
	finis

	prep("setupterm of %s", term_name);
	start
		exit_on(WARNING | ERROR | SEVERR);
		expecting(SUCCESS);

		Zsetupterm(term_name, 1, &errret);
	finis

	testing("tput command with environment variable set to %s", term_name);
	start
		exit_on(SEVERR);
		break_on(WARNING | ERROR);

		test_tput(ENV, term_name);
	finis

	testing("tput command with argument set to %s", term_name);
	start
		exit_on(SEVERR);
		break_on(WARNING | ERROR);

		test_tput(PARM, term_name);
	finis

	cleanup("setupterm of NULL to restore, and reset of shell mode");
	start
		/* restore sanity to terminal */
		Zreset_shell_mode();
	finis

	done();
/*NOTREACHED*/
}
