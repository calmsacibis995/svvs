#ident "@(#)sv_streams.h	1.2"

#include	<sys/types.h>
#define	INFLAG	1000
#define DRIVER	"/dev/lo0"
#define DRIVER2	"/dev/lo1"
#define DRIVER3	"/dev/lo2"
#define MUX	"/dev/tmux"
#define MODB	"lmodb"
#define MODT	"lmodt"
#define MODE	"lmode"
#define MODR	"lmodr"
#define	getmsgerr	"Incorrect value returned from getmsg."
#define	eventerr	"Incorrect value for revents."
#define	pollerr		"Incorrect value returned from poll."
#define	SIGALRM	14
#define SIGCLD  18
#if lint
#define	SIG_IGN	(void (*)())0
#else
#define	SIG_IGN	(void (*)())1
#endif

/*from lo.h */
/*
 * Loopback driver ioctl requests
 */
#define I_NOARG 20
#define I_INTARG 21
#define I_DDARG 22
#define I_ERRNAK 23
#define I_TIMOUT 24
#define I_ERROR 25
#define I_UDARG 26
#define I_SLOW 28
#define I_MODCMD 30
#define I_SETBIGB 40
#define I_CLRBIGB 41
#define I_SETHANG 42
#define I_SETERR 43
#define I_SETOFAIL 44
#define I_GRAB 50 
#define I_FREE 51
#define I_SETWOFF 52
#define I_CLRWOFF 53

#define I_GRABLINK	245
#define I_RELLINK	246
#define I_GRABSEVENT	247
#define I_RELSEVENT	248
#define	I_TIMEOUT	249
#define	I_STRTEST	250
#define I_SLPTEST	251
#define I_QPTR		252
#define	I_SETRANGE	253
#define I_UNSETRANGE	254


/*
 * Driver state values.
 */
#define LOOPEN 01
#define LOFAIL 02
#define LOWOFF 04
#define	LOSLPTEST	32

/*
 * Misc parameters
 */
#define IOCTLSZ		1024
#define LONGTIME	4000000
#define DELAY		1000000

/* from tmux.h */


/*
 * Driver state values.
 */
#define TMXOPEN 01

#define	SND		1
#define	RCV		2

#define	ONEMEG		1048576
#define BUFSIZE		256
#define BUFSZ2		8192
#define	SMSG		256	/* normal packet size */
#define	STRMSGSZ	4096	/* modified packet size */

#define MINSNDSIZE	63
#define MAXBUFSIZE	1024

#define TIMEOUT		30
#define SILENT		1
#define NOEXITONERR	2




int	gotsig;

