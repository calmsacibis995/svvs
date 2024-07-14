# include "main.h"

SCCSID("@(#)yyerror.c	3.1 (SVID) 12/1/86");

yyerror(err_msg)
char	*err_msg;
{
	(void) fprintf(stderr, "%d: %s\n", LineNumber, err_msg);
}
