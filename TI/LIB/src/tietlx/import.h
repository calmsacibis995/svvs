/*
** SCCSID("@(#)import.h	3.3 (SVID) 12/23/86");
*/

extern	TERMINAL	*init_terminfo();
extern	TERMINAL	*term_malloc();

extern	short		Bless_trace[];
extern	TRACE_MAP	Bless_map[];
extern	BOOLEAN		clr_trace();
extern	BOOLEAN		set_trace();
extern	void		test_vidattr();
extern	void		test_putp();
extern	void		set_stdout();
