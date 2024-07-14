/*LINTLIBRARY*/
#include <errno.h>
/*
 * some magic so that errno values can used as arguments to expecting.
 * the evaluation function needs a flag to tell it that there is an
 * actual value set against which it must check, and whether the value
 * is an expected return value, or an expected errno.  Before the errno
 * values passed to the expecting() function, sv_expecting is set to
 * SV_ERRN, to indicate this is an expected errno.
 */

#ifdef	UNDEF
#ident	"@(#)sv_errno.c	1.5"
#endif

#ifdef	E2BIG
int	ZE2BIG(){
	return(E2BIG);
}
#endif

#ifdef	EACCES	
int	ZEACCES(){
	return(EACCES);
}
#endif

#ifdef	EAGAIN	
int	ZEAGAIN(){
	return(EAGAIN);
}
#endif

#ifdef	EBADF	
int	ZEBADF(){
	return(EBADF);
}
#endif

#ifdef	EBUSY	
int	ZEBUSY(){
	return(EBUSY);
}
#endif

#ifdef	ECHILD	
int	ZECHILD(){
	return(ECHILD);
}
#endif

#ifdef	EDEADLK	
int	ZEDEADLK(){
	return(EDEADLK);
}
#endif

#ifdef	EDOM	
int	ZEDOM(){
	return(EDOM);
}
#endif

#ifdef	EEXIST	
int	ZEEXIST(){
	return(EEXIST);
}
#endif

#ifdef	EFAULT	
int	ZEFAULT(){
	return(EFAULT);
}
#endif

#ifdef	EFBIG	
int	ZEFBIG(){
	return(EFBIG);
}
#endif

#ifdef	EIDRM
int	ZEIDRM(){
	return(EIDRM);
}
#endif

#ifdef	EINTR	
int	ZEINTR(){
	return(EINTR);
}
#endif

#ifdef	EINVAL	
int	ZEINVAL(){
	return(EINVAL);
}
#endif

#ifdef	EIO	
int	ZEIO(){
	return(EIO);
}
#endif

#ifdef	EISDIR	
int	ZEISDIR(){
	return(EISDIR);
}
#endif

#ifdef ELNRNG
int	ZELNRNG(){
	return(ELNRNG);
}
#endif

#ifdef	EMFILE	
int	ZEMFILE(){
	return(EMFILE);
}
#endif

#ifdef	EMLINK	
int	ZEMLINK(){
	return(EMLINK);
}
#endif

#ifdef	ENFILE	
int	ZENFILE(){
	return(ENFILE);
}
#endif

#ifdef	ENODEV	
int	ZENODEV(){
	return(ENODEV);
}
#endif

#ifdef	ENOENT	
int	ZENOENT(){
	return(ENOENT);
}
#endif

#ifdef	ENOEXEC	
int	ZENOEXEC(){
	return(ENOEXEC);
}
#endif

#ifdef ENOLCK
int	ZENOLCK(){
	return(ENOLCK);
}
#endif

#ifdef	ENOMEM	
int	ZENOMEM(){
	return(ENOMEM);
}
#endif

#ifdef ENOMSG
int	ZENOMSG(){
	return(ENOMSG);
}
#endif

#ifdef	ENOSPC	
int	ZENOSPC(){
	return(ENOSPC);
}
#endif

#ifdef	ENOTBLK	
int	ZENOTBLK(){
	return(ENOTBLK);
}
#endif

#ifdef	ENOTDIR	
int	ZENOTDIR(){
	return(ENOTDIR);
}
#endif

#ifdef	ENOTTY	
int	ZENOTTY(){
	return(ENOTTY);
}
#endif

#ifdef	ENXIO	
int	ZENXIO(){
	return(ENXIO);
}
#endif

#ifdef	EPERM	
int	ZEPERM(){
	return(EPERM);
}
#endif

#ifdef	EPIPE	
int	ZEPIPE(){
	return(EPIPE);
}
#endif

#ifdef	ERANGE	
int	ZERANGE(){
	return(ERANGE);
}
#endif

#ifdef	EROFS	
int	ZEROFS(){
	return(EROFS);
}
#endif

#ifdef	ESPIPE	
int	ZESPIPE(){
	return(ESPIPE);
}
#endif

#ifdef	ESRCH	
int	ZESRCH(){
	return(ESRCH);
}
#endif

#ifdef	ETXTBSY	
int	ZETXTBSY(){
	return(ETXTBSY);
}
#endif

#ifdef	EXDEV	
int	ZEXDEV(){
	return(EXDEV);
}
#endif

#ifdef	EPROTO
int	ZEPROTO(){
	return(EPROTO);
}
#endif

#ifdef	EBADMSG
int	ZEBADMSG(){
	return(EBADMSG);
}
#endif

#ifdef	ENOSR
int	ZENOSR(){
	return(ENOSR);
}
#endif

#ifdef	ENOSTR
int	ZENOSTR(){
	return(ENOSTR);
}
#endif

#ifdef	ETIME
int	ZETIME(){
	return(ETIME);
}
#endif

#ifdef	EREMOTE
int	ZEREMOTE(){
	return(EREMOTE);
}
#endif
