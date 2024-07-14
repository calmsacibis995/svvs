# include "main.h"
# include "yy.h"
# include "store.h"
# include "yyparse.h"

SCCSID("@(#)yylex.c	3.1 (SVID) 12/1/86");

# ifdef ECHO
# undef ECHO
# endif

# include "yylex.txt"

void scan_init()
{
}
