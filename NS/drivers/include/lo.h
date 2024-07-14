#ident "@(#)lo.h	1.1"

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

/*
added by awr
*/
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

struct lo {
	unsigned lo_state;
	queue_t	*lo_rdq;
};

/*
 * Driver state values.
 */
#define LOOPEN 01
#define LOFAIL 02
#define LOWOFF 04
/*
added by awr.
*/
#define	LOSLPTEST	32

/*
 * Misc parameters
 */
#define IOCTLSZ		1024
#define LONGTIME	4000000
#define DELAY		1000000

