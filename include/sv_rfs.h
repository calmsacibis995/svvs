/*
 *	sv_rfs.h
 *
 *	local dir to advertise, and mount point for remote resource
 */

#ident	"@(#)sv_rfs.h	1.1"

/*
 *	Local point to advertise for RFS tests
 *	RF_LOCAL is appended to SVVSPATH
 */
#define	RF_LOCAL			"/NS/LOCAL"

/*
 *	Mount point for remote mount of RFS tests.
 *	RF_REMOTE_MOUNT is appended to SVVSPATH
 */
#define	RF_REMOTE_MOUNT		"/NS/REMOTE"
/*
 *	An invalid domain name
 */
#define	RF_BADDOMAIN	"!@#$%_domain_name"
