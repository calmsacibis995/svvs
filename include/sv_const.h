/*
 *	sv_const.h	- implementation dependent constants
 */

#ifdef	UNDEF
ident	"@(#)sv_const.h	1.4"
#endif

#define	MAXDOUBLE	1.79769313486231470e+308 /*	maximum value for a double: */
#define	DBL_MIN		4.94065645841246544e-324 /*	minimum positive value for double: */
#define	FLT_MAX		3.40282346638528860e+38 /*	maximum value for a float: */
#define	FLT_MIN		1.40129846432481707e-45 /*	minimum positive value for a float: */
#define	CHILD_MAX	50	/*	maximum number of processes per user: */
#define	PIPE_MAX	5120	/*	maximum number of bytes written to a pipe in a write: */
#define	OPEN_MAX	20	/*	maximum number of files a process can have open: */
#define	PATH_MAX	1024	/*	maximum number of characters in a path name: */

#define NZERO		20	/*  offset for nice				*/

