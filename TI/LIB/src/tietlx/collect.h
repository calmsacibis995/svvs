/*
** SCCSID("@(#)collect.h	3.1 (SVID) 12/1/86");
*/

extern TERMINAL	*BlessTerm;
extern char	*CollectBuff;
extern unsigned	CollectSize;
extern unsigned	CollectMax;

extern void		init_BlessTerm();
extern FLAG_TYPE	collect();
extern void		judge();
extern void		send_EOT();
