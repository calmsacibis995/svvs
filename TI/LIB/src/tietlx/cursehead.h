/*
** SCCSID("@(#)cursehead.h	3.3 (SVID) 12/24/86");
*/

extern int	Zbeep();
extern int	Zbox();
extern int	Zcbreak();
extern int	Zclearok();
extern int	Zcopywin();
extern int	Zdelay_output();
extern int	Zdelwin();
extern int	Zdoupdate();
extern int	Zecho();
extern int	Zendwin();
extern int	Zflash();
extern int	Zflushinp();
extern int	Zidlok();
extern WINDOW	*Zinitscr();
extern int	Zkeypad();
extern int	Zleaveok();
extern int	Zmvcur();
extern int	Zmvwin();
extern WINDOW	*Znewpad();
extern WINDOW	*Znewwin();
extern int	Znoecho();
extern int	Zoverlay();
extern int	Zoverwrite();
extern int	Zpechochar();
extern int	Zpnoutrefresh();
extern int	Zprefresh();
extern int	printw();
extern int	Zputp();
extern int	scanw();
extern int	Zscr_dump();
extern int	Zscr_init();
extern int	Zscr_restore();
extern int	Zscroll();
extern int	Zscrollok();
extern int	Zslk_clear();
extern int	Zslk_init();
extern char	*Zslk_label();
extern int	Zslk_noutrefresh();
extern int	Zslk_refresh();
extern int	Zslk_restore();
extern int	Zslk_set();
extern int	Zslk_touch();
extern WINDOW	*Zsubpad();
extern WINDOW	*Zsubwin();
extern int	Ztgetnum();
extern int	Ztouchline();
extern int	Ztouchwin();
extern char	*Zunctrl();
extern int	Zungetch();
extern int	Zvidattr();
extern int	Zwaddch();
extern int	Zwaddstr();
extern int	Zwattroff();
extern int	Zwattron();
extern int	Zwattrset();
extern int	Zwclear();
extern int	Zwclrtobot();
extern int	Zwclrtoeol();
extern int	Zwdelch();
extern int	Zwdeleteln();
extern int	Zwechochar();
extern int	Zwerase();
extern int	Zwgetch();
extern int	Zwgetstr();
extern int	Zwinsch();
extern int	Zwinsertln();
extern int	Zwmove();
extern int	Zwnoutrefresh();
extern int	wprintw();
extern int	Zwrefresh();
extern int	wscanw();
extern int	Zwstandend();
extern int	Zwstandout();