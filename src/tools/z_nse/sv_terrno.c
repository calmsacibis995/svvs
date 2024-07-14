/*LINTLIBRARY*/

#ifdef	UNDEF
#ident	"@(#)sv_terrno.c	1.2"
#endif

#include <sys/tiuser.h>
/*
 *	sv_terrno.c
 *
 *	some magic so that t_errno values can used as arguments to expecting.
 *	the evaluation function needs a flag to tell it that there is an
 *	actual value set against which it must check, and whether the value
 *	is an expected return value, or an expected errno.  Before the t_errno
 *	values passed to the expecting() function, sv_expecting is set to
 *	SV_ERRN, to indicate this is an expected errno.
 */

#ifdef	TSYSERR
int	ZTSYSERR(){
	return(TSYSERR);
}
#endif

#ifdef	TBADF
int	ZTBADF(){
	return(TBADF);
}
#endif

#ifdef	TOUTSTATE
int	ZTOUTSTATE(){
	return(TOUTSTATE);
}
#endif

#ifdef	TACCES
int	ZTACCES(){
	return(TACCES);
}
#endif

#ifdef	TBADOPT
int	ZTBADOPT(){
	return(TBADOPT);
}
#endif

#ifdef	TBADDATA
int	ZTBADDATA(){
	return(TBADDATA);
}
#endif

#ifdef	TBADSEQ
int	ZTBADSEQ(){
	return(TBADSEQ);
}
#endif

#ifdef	TLOOK
int	ZTLOOK(){
	return(TLOOK);
}
#endif

#ifdef	TNOTSUPPORT
int	ZTNOTSUPPORT(){
	return(TNOTSUPPORT);
}
#endif

#ifdef	TBADADDR
int	ZTBADADDR(){
	return(TBADADDR);
}
#endif

#ifdef	TNOADDR
int	ZTNOADDR(){
	return(TNOADDR);
}
#endif

#ifdef	TBUFOVFLW
int	ZTBUFOVFLW(){
	return(TBUFOVFLW);
}
#endif

#ifdef	TNODATA
int	ZTNODATA(){
	return(TNODATA);
}
#endif

#ifdef	TSTATECHNG
int	ZTSTATECHNG(){
	return(TSTATECHNG);
}
#endif

#ifdef	TBADFLAG
int	ZTBADFLAG(){
	return(TBADFLAG);
}
#endif

#ifdef	TNODIS
int	ZTNODIS(){
	return(TNODIS);
}
#endif

#ifdef	TNOREL
int	ZTNOREL(){
	return(TNOREL);
}
#endif

#ifdef	TNOUDERR
int	ZTNOUDERR(){
	return(TNOUDERR);
}
#endif

#ifdef	TFLOW
int	ZTFLOW(){
	return(TFLOW);
}
#endif
