# include "main.h"
# include "option.h"

SCCSID("@(#)pipe.c	3.1 (SVID) 12/1/86");

extern int	mknod();
extern int	unlink();
extern int	close();

static BOOLEAN	PipeMade = FALSE;

void make_named_pipe()
{
	char	tmp_str[BASIC_STR_LEN];
	int	mode;
	int	dev;

	(void) sprintf(tmp_str, "%s%d", Option.pipe_name, getpid());
	Option.pipe_name = malloc((unsigned) (strlen(tmp_str) + 1));
	(void) strcpy(Option.pipe_name, tmp_str);

	mode = 0010666;
	dev = 0;

	if (mknod(Option.pipe_name, mode, dev))
	{
		screech("make_named_pipe", 1, "\n");
	}

	PipeMade = TRUE;
}

/*
** Don't call "epilogue(); exit()" on failure.  You can only get here 
** through "epilogue()", so someone else will handle graceful exiting
*/

void rm_named_pipe()
{
	if ((PipeMade) && (Option.pipe_name))
	{
		if (unlink(Option.pipe_name))
		{
			screech("rm_named_pipe", 1, "unlink failed\n");
		}
		else
		{
			PipeMade = FALSE;
		}
	}
}
