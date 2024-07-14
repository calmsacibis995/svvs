# include "main.h"
# include "yy.h"
# include "option.h"
# include <exec_head.h>

SCCSID("@(#)main.c	3.1 (SVID) 12/1/86");

main(argc, argv)
int	argc;
char	*argv[];
{
	int	exit_code;

	prologue(argc, argv);

	exit_code = yyparse();

	if ((!exit_code) && (Option.save_prgm))
	{
		save_prgm();
	}

	return(exit_code);
}
