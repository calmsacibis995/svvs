/*
** SCCSID("@(#)misc.h	3.3 (SVID) 12/30/86");
*/

/*
** Various "# define" statements
*/

# define	Trace(flg,level)	( (TIETL_trace[flg] > level) )

/*
** Various local routines
*/

extern	void		prologue();

extern	void		prelim();
extern	void		split_up();

extern	STMT_TYPE	execute();
extern	TBL_INDEX	do_eval();
extern			operate();
extern	VAR_ENTRY	*convert();

extern	void		epilogue();
extern	void		screech();
extern	void		handler();
extern	void		chk_partner();
extern	void		grab_sig();
extern	void		reload_var();
extern	void		umpire();
