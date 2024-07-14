# include "main.h"
# include <varargs.h>
# include <signal.h>

SCCSID("@(#)screech.c	3.1 (SVID) 12/1/86");

/*
** "screech()" takes the following arguments:
**
**	char *		Usually used for routine name
**	BOOLEAN		Should "perror()" be called?
**	char *		Format string for "fprintf()"
**	arg1..		Arguments for "fprintf()"
*/

/*VARARGS*/

void screech(va_alist)
va_dcl
{
	va_list	args;
	char	*fmt;
	char	*name;
	BOOLEAN	do_perror;

	va_start(args);

	name = 		va_arg(args, char *);
	do_perror = 	va_arg(args, BOOLEAN);
	fmt = 		va_arg(args, char *);

	if ((name) || (fmt))
	{
		(void) fprintf(stderr, "tietli");

		if (name)
		{
			(void) fprintf(stderr, ", %s", name);
		}

		if (fmt)
		{
			(void) fprintf(stderr, ": ");
			(void) vfprintf(stderr, fmt, args);
			if (fmt[strlen(fmt) - 1] != '\n')
			{
				(void) fprintf(stderr, "\n");
			}
		}
		else
		{
			(void) fprintf(stderr, "\n");
		}
	}

	if (do_perror)
	{
		perror("tietli");
	}

	va_end(args);

	exit(1);
}
