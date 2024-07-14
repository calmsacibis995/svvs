# include <stdio.h>
# include <ctype.h>
# include <sys/types.h>
# include <svvs.h>
# include <sv_env.h>
# include <string.h>
# include <sccs.h>

SCCSID("@(#)caller.c	3.1 (SVVS) 12/1/86");

char	*TermList[] = {
	"SVVS1",
	"SVVS2",
	"SVVS3",
	"SVVS4",
	""
};

main()
{
	char	**term_name;
	char	env_var[100];

	setup("testtput");
	abstract("for SVVS1 through SVVS4 terminfo descriptions, get each");
	abstract("of the capabilities and do a tput of it using system().");
	abstract("test one cycle of capabilities with the terminal");
	abstract("type set by an environment variable and another cycle");
	abstract("with the terminal type set by an argument to tput");
	abstract("the output of tput is redirected to a file, this file");
	abstract("is read and compared to what tigetstr returned.");
	start
		for (term_name = TermList; **term_name != NULL; term_name++)
		{
			prep("Set environment variable to terminal type");
			start
				exit_on(WARNING | ERROR | SEVERR);
				expecting(SUCCESS);

				sprintf(env_var, "SVVS_TERM=%s", *term_name);
				putenv(env_var);
			finis

			subtest("testtput0");
		}
	finis

	done();
}
