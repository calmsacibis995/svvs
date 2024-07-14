/*LINTLIBRARY*/
/*
 *	EMACS_MODES: tabstop=4
 *
 *	errlook.c 
 *
 *	errlookup():	return char str representation of errno value
 */

#ifdef	UNDEF
#ident	"@(#)sv_errlook.c	1.3"
#endif

#include <errno.h>

static struct errz {
	int errr;
	char *errrname;
} errz[] = {
	{ E2BIG,	"E2BIG - Arg list too long"			},
	{ EACCES,	"EACCES - Permission denied"			},
	{ EAGAIN,	"EAGAIN - Resource temporarily unavailable"	},
	{ EBADF,	"EBADF - Bad file number"			},
	{ EBUSY,	"EBUSY - Device or resource busy"		},
	{ ECHILD,	"ECHILD - No child processes"			},
#ifdef EDEADLK
	{ EDEADLK,	"EDEADLK - Deadlock condition"			},
#endif
	{ EDOM,	 	"EDOM - Math arg out of domain of func"		},
	{ EEXIST,	"EEXIST - File exists"				},
	{ EFAULT,	"EFAULT - Bad address"				},
	{ EFBIG,	"EFBIG - File too large"			},
#ifdef EIDRM
	{ EIDRM,	"EIDRM - Identifier removed"			},
#endif
	{ EINTR,	"EINTR - Interrupted system call"		},
	{ EINVAL,	"EINVAL - Invalid argument"			},
	{ EIO,		"EIO - I/O error"				},
	{ EISDIR,	"EISDIR - Is a directory"			},
#ifdef ELNRNG
	{ ELNRNG,	"ELNRNG - Link number out of range"		},
#endif
	{ EMFILE, 	"EMFILE - Too many open files"			},
	{ EMLINK, 	"EMLINK - Too many links"			},
	{ ENFILE, 	"ENFILE - File table overflow"			},
	{ ENODEV, 	"ENODEV - No such device"			},
	{ ENOENT, 	"ENOENT - No such file or directory"		},
	{ ENOEXEC,	"ENOEXEC - Exec format error"			},
#ifdef ENOLCK
	{ ENOLCK,	"ENOLCK - No locks available"			},
#endif
	{ ENOMEM,	"ENOMEM - Not enough space"			},
#ifdef ENOMSG
	{ ENOMSG,	"ENOMSG - No message of desired type"		},
#endif
	{ ENOSPC,	"ENOSPC - No space left on device"		},
	{ ENOTBLK,	"ENOTBLK - Block device required"		},
	{ ENOTDIR,	"ENOTDIR - Not a directory"			},
	{ ENOTTY,	"ENOTTY - Not a character device"		},
	{ ENXIO,	"ENXIO - No such device or address"		},
	{ EPERM,	"EPERM - Not owner"				},
	{ EPIPE,	"EPIPE - Broken pipe"				},
	{ ERANGE,	"ERANGE - Result too large"			},
	{ EROFS,	"EROFS - Read only file system"			},
	{ ESPIPE,	"ESPIPE - Illegal seek"				},
	{ ESRCH,	"ESRCH - No such process"			},
	{ ETXTBSY,	"ETXTBSY - Text file busy"			},
	{ EXDEV,	"EXDEV - Cross-device link"			},
#ifdef EPROTO
	{ EPROTO,	"EPROTO - Protocol Error"			},
#endif
#ifdef EBADMSG
	{ EBADMSG,	"EBADMSG - Trying to read unreadable message"			},
#endif
#ifdef ENOSR
	{ ENOSR,	"ENOSR - Out of stream resources"			},
#endif
#ifdef ENOSTR
	{ ENOSTR,	"ENOSTR - Device not a stream"			},
#endif
#ifdef ETIME
	{ ETIME,	"ETIME - Timer expired"			},
#endif
#ifdef ECOMM
	{ ECOMM,	"ECOMM - Communications error on send"			},
#endif
#ifdef EMULTIHOP
	{ EMULTIHOP,	"EMULTIHOP - Multihop not allowed"			},
#endif
#ifdef ENOLINK
	{ ENOLINK,	"ENOLINK - The link has been severed"			},
#endif
#ifdef EREMOTE
	{ EREMOTE,	"EREMOTE - The object is remote"			},
#endif
{	 0,		"0 - No Error"					},
};
/*
** err_lookup
**	Look up the symbolic name for an error.
**
** Arguments
**	num	Error number (probably from errno).
*/
char *err_lookup(num)
int	num;
{
	register struct errz *p = &errz[0];
	static char foo[24];

#ifdef DEBUG
	(void) printf("err_lookup() ");
#endif

	while(1)
	{
		if(p->errr == num)
			return(p->errrname);
		if(!((p++)->errr)) {
			(void) sprintf(foo,"%d - Unknown Error",num);
			return(foo);
		}
	}
}
