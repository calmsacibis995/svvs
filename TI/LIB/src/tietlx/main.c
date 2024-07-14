# include "main.h"
# include "option.h"
# include <exec_head.h>
# include "vs.h"

SCCSID("@(#)main.c	3.1 (SVID) 12/1/86");

main(argc, argv)
int	argc;
char	*argv[];
{
	prologue(argc, argv);

	if (!Option.block_dump)
	{
		read_prgm();
	}

	split_up();

	epilogue();

	vs_done();
}
