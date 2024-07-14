/*
** SCCSID("@(#)misc.h	3.1 (SVID) 12/1/86");
*/

/*
** Routines from libc
*/

extern ENTRY	*hsearch();
extern int	hcreate();

/*
** Various local routines
*/

extern	void		prologue();
extern	void		load_var();
extern	void		prgmsave();
extern	void		screech();
extern	TBL_INDEX	tbl_alloc();
extern	void		save_prgm();
