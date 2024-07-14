/*
** SCCSID("@(#)main.h	3.1 (SVID) 12/1/86");
*/

# include <bless.h>
# include <common.h>
# include <extern.h>
# include <parse.h>
# include <sccs.h>

extern void exit();
extern char *getenv();
extern char *strncpy();
extern char *strcpy();
extern char *strcat();
extern char *tigetstr();
extern void qsort();
extern void perror();
extern long lseek();
extern void free();

extern char  *TypeAhead;
extern int	 Dbg;
