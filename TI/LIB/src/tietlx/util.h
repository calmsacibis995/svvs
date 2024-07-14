/*
** SCCSID("@(#)util.h	3.1 (SVID) 12/1/86");
*/

extern void		free_text();
extern void		free_var();

extern BOOLEAN		text_num();
extern int		pprintf();

extern void		var_inflate();
extern void		var_deflate();

extern int		set_var_arg();
extern void		cx_initscr();
