/*
** SCCSID("@(#)lib_pitch.h	3.1 (SVID) 12/1/86");
*/

extern BOOLEAN	InitTIE;
extern char	*DeferScrDump;

extern int	p_adch();
extern int	p_adstr();
extern int	p_offattr();
extern int	p_onattr();
extern int	p_setattr();
extern int	p_barber();
extern int	p_clear();
extern int	p_clrbot();
extern int	p_clreol();
extern int	p_dlch();
extern int	p_deleteln();
extern int	p_charecho();
extern int	p_endwin();
extern int	p_erase();
extern int	p_flushinp();
extern int	p_gtch();
extern int	p_gtstr();
extern int	p_getyx();
extern int	p_getbegyx();
extern int	p_getmaxyx();
extern WINDOW	*p_initscr();
extern chtype	p_inc();
extern int	p_insch();
extern int	p_insertln();
extern int	p_move();
extern int	p_addchmv();
extern int	p_addstrmv();
extern int	p_delchmv();
extern int	p_getchmv();
extern int	p_getstrmv();
extern chtype	p_inchmv();
extern int	p_mvinsch();
extern int	p_mvprintw();
extern int	p_mvscanw();
extern int	p_chaddmvw();
extern int	p_straddmvw();
extern int	p_chdelmvw();
extern int	p_chgetmvw();
extern int	p_strgetmvw();
extern chtype	p_chinmvw();
extern int	p_mvwinsch();
extern int	p_mvwprintw();
extern int	p_mvwscanw();
extern int	p_refresh();
extern int	p_scrdump();
extern int	p_setscrreg();
extern int	p_endstand();
extern int	p_standout();
extern int	p_system();
extern char	*p_temp_path();
extern char	*p_unctrl();
extern int	p_ungetch();
extern int	p_wgetstr();
extern int	p_wsetscrreg();
extern chtype	p_wchin();


extern int	p_printf();
extern int	p_printw();
extern int	p_wprintw();

extern int	p_scanf();
extern int	p_scanw();
extern int	p_wscanw();
