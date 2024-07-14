# include <stdio.h>
# include <ctype.h>
# include <sys/types.h>
# include <svvs.h>
# include <sv_env.h>
# include <string.h>
# include <sccs.h>
# include "basiccap.h"

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

	setup("basiccap");

	abstract("for each of SVVS1 through SVVS4, the terminfo");
	abstract("description is read, and each capability is");
	abstract("retrieved from the terminfo database and the");
	abstract("returned string compared to that in the terminfo");
	abstract("description file.  this is then done again for the");
	abstract("termcap compatability routines.");
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

			subtest("basiccap0");
		}
	finis

	done();
}
