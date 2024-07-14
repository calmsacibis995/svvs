/*
 *	sv_errno.h
 *
 *	some magic so that errno values can used as arguments to expecting.
 *	the evaluation function needs a flag to tell it that there is an
 *	actual value set against which it must check, and whether the value
 *	is an expected return value, or an expected errno.  Before the errno
 *	values passed to the expecting() function, sv_expecting is set to
 *	SV_ERRN, to indicate this is an expected errno.
 */

#ifdef	UNDEF
ident	"@(#)sv_errno.h	1.4"
#endif

#ifdef	E2BIG
#undef	E2BIG
int	ZE2BIG();
#define	E2BIG	sv_sete(ZE2BIG())
#endif

#ifdef	EACCES	
#undef	EACCES	
int	ZEACCES();
#define	EACCES	sv_sete(ZEACCES())
#endif

#ifdef	EAGAIN	
#undef	EAGAIN	
int	ZEAGAIN();
#define	EAGAIN	sv_sete(ZEAGAIN())
#endif

#ifdef	EBADF	
#undef	EBADF	
int	ZEBADF();
#define	EBADF	sv_sete(ZEBADF())
#endif

#ifdef	EBUSY	
#undef	EBUSY	
int	ZEBUSY();
#define	EBUSY	sv_sete(ZEBUSY())
#endif

#ifdef	ECHILD	
#undef	ECHILD	
int	ZECHILD();
#define	ECHILD	sv_sete(ZECHILD())
#endif

#ifdef	EDEADLK	
#undef	EDEADLK	
int	ZEDEADLK();
#define	EDEADLK	sv_sete(ZEDEADLK())
#endif

#ifdef	EDOM	
#undef	EDOM	
int	ZEDOM();
#define	EDOM	sv_sete(ZEDOM())
#endif

#ifdef	EEXIST	
#undef	EEXIST	
int	ZEEXIST();
#define	EEXIST	sv_sete(ZEEXIST())
#endif

#ifdef	EFAULT	
#undef	EFAULT	
int	ZEFAULT();
#define	EFAULT	sv_sete(ZEFAULT())
#endif

#ifdef	EFBIG	
#undef	EFBIG	
int	ZEFBIG();
#define	EFBIG	sv_sete(ZEFBIG())
#endif

#ifdef	EIDRM
#undef	EIDRM
int	ZEIDRM();
#define	EIDRM	sv_sete(ZEIDRM())
#endif

#ifdef	EINTR	
#undef	EINTR	
int	ZEINTR();
#define	EINTR	sv_sete(ZEINTR())
#endif

#ifdef	EINVAL	
#undef	EINVAL	
int	ZEINVAL();
#define	EINVAL	sv_sete(ZEINVAL())
#endif

#ifdef	EIO	
#undef	EIO	
int	ZEIO();
#define	EIO	sv_sete(ZEIO())
#endif

#ifdef	EISDIR	
#undef	EISDIR	
int	ZEISDIR();
#define	EISDIR	sv_sete(ZEISDIR())
#endif

#ifdef ELNRNG
#undef ELNRNG
int	ZELNRNG();
#define	ELNRNG	sv_sete(ZELNRNG())
#endif

#ifdef	EMFILE	
#undef	EMFILE	
int	ZEMFILE();
#define	EMFILE	sv_sete(ZEMFILE())
#endif

#ifdef	EMLINK	
#undef	EMLINK	
int	ZEMLINK();
#define	EMLINK	sv_sete(ZEMLINK())
#endif

#ifdef	ENFILE	
#undef	ENFILE	
int	ZENFILE();
#define	ENFILE	sv_sete(ZENFILE())
#endif

#ifdef	ENODEV	
#undef	ENODEV	
int	ZENODEV();
#define	ENODEV	sv_sete(ZENODEV())
#endif

#ifdef	ENOENT	
#undef	ENOENT	
int	ZENOENT();
#define	ENOENT	sv_sete(ZENOENT())
#endif

#ifdef	ENOEXEC	
#undef	ENOEXEC	
int	ZENOEXEC();
#define	ENOEXEC	sv_sete(ZENOEXEC())
#endif

#ifdef ENOLCK
#undef ENOLCK
int	ZENOLCK();
#define	ENOLCK	sv_sete(ZENOLCK())
#endif

#ifdef	ENOMEM	
#undef	ENOMEM	
int	ZENOMEM();
#define	ENOMEM	sv_sete(ZENOMEM())
#endif

#ifdef ENOMSG
#undef ENOMSG
int	ZENOMSG();
#define	ENOMSG	sv_sete(ZENOMSG())
#endif

#ifdef	ENOSPC	
#undef	ENOSPC	
int	ZENOSPC();
#define	ENOSPC	sv_sete(ZENOSPC())
#endif

#ifdef	ENOTBLK	
#undef	ENOTBLK	
int	ZENOTBLK();
#define	ENOTBLK	sv_sete(ZENOTBLK())
#endif

#ifdef	ENOTDIR	
#undef	ENOTDIR	
int	ZENOTDIR();
#define	ENOTDIR	sv_sete(ZENOTDIR())
#endif

#ifdef	ENOTTY	
#undef	ENOTTY	
int	ZENOTTY();
#define	ENOTTY	sv_sete(ZENOTTY())
#endif

#ifdef	ENXIO	
#undef	ENXIO	
int	ZENXIO();
#define	ENXIO	sv_sete(ZENXIO())
#endif

#ifdef	EPERM	
#undef	EPERM	
int	ZEPERM();
#define	EPERM	sv_sete(ZEPERM())
#endif

#ifdef	EPIPE	
#undef	EPIPE	
int	ZEPIPE();
#define	EPIPE	sv_sete(ZEPIPE())
#endif

#ifdef	ERANGE	
#undef	ERANGE	
int	ZERANGE();
#define	ERANGE	sv_sete(ZERANGE())
#endif

#ifdef	EROFS	
#undef	EROFS	
int	ZEROFS();
#define	EROFS	sv_sete(ZEROFS())
#endif

#ifdef	ESPIPE	
#undef	ESPIPE	
int	ZESPIPE();
#define	ESPIPE	sv_sete(ZESPIPE())
#endif

#ifdef	ESRCH	
#undef	ESRCH	
int	ZESRCH();
#define	ESRCH	sv_sete(ZESRCH())
#endif

#ifdef	ETXTBSY	
#undef	ETXTBSY	
int	ZETXTBSY();
#define	ETXTBSY	sv_sete(ZETXTBSY())
#endif

#ifdef	EXDEV	
#undef	EXDEV	
int	ZEXDEV();
#define	EXDEV	sv_sete(ZEXDEV())
#endif

#ifdef	EPROTO
#undef	EPROTO
int	ZEPROTO();
#define	EPROTO	sv_sete(ZEPROTO())
#endif

#ifdef	EBADMSG
#undef	EBADMSG
int	ZEBADMSG();
#define	EBADMSG	sv_sete(ZEBADMSG())
#endif

#ifdef	ENOSR
#undef	ENOSR
int	ZENOSR();
#define	ENOSR	sv_sete(ZENOSR())
#endif

#ifdef	ENOSTR
#undef	ENOSTR
int	ZENOSTR();
#define	ENOSTR	sv_sete(ZENOSTR())
#endif

#ifdef	ETIME
#undef	ETIME
int	ZETIME();
#define	ETIME	sv_sete(ZETIME())
#endif

#ifdef	EPROTO
#undef	EPROTO
int	ZEPROTO();
#define	EPROTO	sv_sete(ZEPROTO())
#endif

#ifdef	EREMOTE
#undef	EREMOTE
int	ZEREMOTE();
#define	EREMOTE	sv_sete(ZEREMOTE())
#endif

#ifdef	TSYSERR
#undef	TSYSERR
int	ZTSYSERR();
#define	TSYSERR	sv_sett(ZTSYSERR())
#endif

#ifdef	TBADF
#undef	TBADF
int	ZTBADF();
#define	TBADF	sv_sett(ZTBADF())
#endif

#ifdef	TOUTSTATE
#undef	TOUTSTATE
int	ZTOUTSTATE();
#define	TOUTSTATE	sv_sett(ZTOUTSTATE())
#endif

#ifdef	TACCES
#undef	TACCES
int	ZTACCES();
#define	TACCES	sv_sett(ZTACCES())
#endif

#ifdef	TBADOPT
#undef	TBADOPT
int	ZTBADOPT();
#define	TBADOPT	sv_sett(ZTBADOPT())
#endif

#ifdef	TBADDATA
#undef	TBADDATA
int	ZTBADDATA();
#define	TBADDATA	sv_sett(ZTBADDATA())
#endif

#ifdef	TBADSEQ
#undef	TBADSEQ
int	ZTBADSEQ();
#define	TBADSEQ	sv_sett(ZTBADSEQ())
#endif

#ifdef	TLOOK
#undef	TLOOK
int	ZTLOOK();
#define	TLOOK	sv_sett(ZTLOOK())
#endif

#ifdef	TNOTSUPPORT
#undef	TNOTSUPPORT
int	ZTNOTSUPPORT();
#define	TNOTSUPPORT	sv_sett(ZTNOTSUPPORT())
#endif

#ifdef	TBADADDR
#undef	TBADADDR
int	ZTBADADDR();
#define	TBADADDR	sv_sett(ZTBADADDR())
#endif

#ifdef	TNOADDR
#undef	TNOADDR
int	ZTNOADDR();
#define	TNOADDR	sv_sett(ZTNOADDR())
#endif

#ifdef	TBUFOVFLW
#undef	TBUFOVFLW
int	ZTBUFOVFLW();
#define	TBUFOVFLW	sv_sett(ZTBUFOVFLW())
#endif

#ifdef	TNODATA
#undef	TNODATA
int	ZTNODATA();
#define	TNODATA	sv_sett(ZTNODATA())
#endif

#ifdef	TSTATECHNG
#undef	TSTATECHNG
int	ZTSTATECHNG();
#define	TSTATECHNG	sv_sett(ZTSTATECHNG())
#endif

#ifdef	TBADFLAG
#undef	TBADFLAG
int	ZTBADFLAG();
#define	TBADFLAG	sv_sett(ZTBADFLAG())
#endif

#ifdef	TNODIS
#undef	TNODIS
int	ZTNODIS();
#define	TNODIS	sv_sett(ZTNODIS())
#endif

#ifdef	TNOREL
#undef	TNOREL
int	ZTNOREL();
#define	TNOREL	sv_sett(ZTNOREL())
#endif

#ifdef	TNOUDERR
#undef	TNOUDERR
int	ZTNOUDERR();
#define	TNOUDERR	sv_sett(ZTNOUDERR())
#endif

#ifdef	TFLOW
#undef	TFLOW
int	ZTFLOW();
#define	TFLOW	sv_sett(ZTFLOW())
#endif
