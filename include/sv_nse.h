/*
 *	sv_nse.h - macro definitions for NS_OS and NS_LIB function replacement
 */

#ifdef	UNDEF
ident	"@(#)sv_nse.h	1.4"
#endif

#define	t_accept	Zt_accept
#define	t_alloc		Zt_alloc
#define	t_bind		Zt_bind
#define	t_close		Zt_close
#define	t_connect	Zt_connect
#define	t_error		Zt_error
#define	t_free		Zt_free
#define	t_getinfo	Zt_getinfo
#define	t_getstate	Zt_getstate
#define	t_listen	Zt_listen
#define	t_look		Zt_look
#define	t_open		Zt_open
#define	t_optmgmt	Zt_optmgmt
#define	t_rcv		Zt_rcv
#define	t_rcvconnect	Zt_rcvconnect
#define	t_rcvdis	Zt_rcvdis
#define	t_rcvrel	Zt_rcvrel
#define	t_rcvudata	Zt_rcvudata
#define	t_rcvuderr	Zt_rcvuderr
#define	t_snd		Zt_snd
#define	t_snddis	Zt_snddis
#define	t_sndrel	Zt_sndrel
#define	t_sndudata	Zt_sndudata
#define	t_sync		Zt_sync
#define	t_unbind	Zt_unbind
#define poll		Zpoll
#define putmsg		Zputmsg
#define getmsg		Zgetmsg
