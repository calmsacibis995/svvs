/*LINTLIBRARY*/
/*
 *	sv_teval - function return value evaluation function, 
 *		also checks t_errno values
 */

#include <errno.h>
#include <sys/tiuser.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_teval.c	1.3"
#endif

char	*err_lookup();
char	*terr_lookup();
char	*ename();
char	*strcpy();

int		t_errno;
int		sv_look_fd;		/* kludge to get tli fd here for a tlook */

#define	SUCC_FUNCT		(fnct->sv_succ)
#define	SUCC_DEFINED	(SUCC_FUNCT && SUCC_FUNCT != VALUE)
#define	SUCC( z )		((*(fnct->sv_succ))(z))
#define	SUCCEEDED		SUCC( ret_val )
#define	FAIL_FUNCT		(fnct->sv_fail)
#define	FAIL_DEFINED	(FAIL_FUNCT && FAIL_FUNCT != VALUE)
#define	FAIL( z )		((*(fnct->sv_fail))(z))
#define	FAILED			FAIL( ret_val )
#define	EXP_RET			((*fnct->sv_xval)())
#define	TEST( z1, z2 )	((*fnct->sv_test)(z1,z2))
#define GOT_EXPECTED	TEST( ret_val, EXP_RET )
#define EXP_ERRNO		(sv_gete())
#define EXP_TERRNO		(sv_gett())
#define GOT_ERRNO		(EXP_ERRNO == errno)
#define GOT_TERRNO		(EXP_TERRNO == t_errno)
#define	FUNCT			(fnct->sv_name)
#define EXPECTING		(sv_xenv->sv_expecting)
#define SSTR			(fnct->sv_sstr)
#define FSTR			(fnct->sv_fstr)
#define FMT( z )		((*fnct->sv_fmt)(z))
#define ERRNO_DEFINED	(fnct->sv_errnodef & 1)
#define TERRNO_DEFINED	(fnct->sv_errnodef & 2)
#define EXP_RVAL		(EXPECTING & SV_VALU)
#define EXP_SUCCESS		(EXPECTING == SV_SUCC)
#define EXP_FAILURE		(EXPECTING == SV_FAIL || EXPECTING & SV_ERRN)
#define EXP_VOID		(EXPECTING == SV_VOID)
#define	CLR_RVAL		(EXPECTING &= ~SV_VALU)
#define	CLR_ERRNO		(sv_sete(0), sv_sett(0), EXPECTING &= ~SV_ERRN)
#define	PASS			return

#define EFAIL_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - failed unexpectedly\nexpected: %s\nreturned: %s\nerrno: %s", FUNCT, SSTR, FMT(ret_val),err_lookup(errno))

#define TFAIL_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - failed unexpectedly\nexpected: %s\nreturned: %s\nt_errno: %s", FUNCT, SSTR, FMT(ret_val),terr_lookup(t_errno))

#define ETFAIL_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - failed unexpectedly\nexpected: %s\nreturned: %s\nerrno: %s\nt_errno: %s", FUNCT, SSTR, FMT(ret_val),err_lookup(errno),terr_lookup(t_errno))

#define LFAIL_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - failed unexpectedly\nexpected: %s\nreturned: %s\nt_errno: %s\nt_look: %s", FUNCT, SSTR, FMT(ret_val),terr_lookup(t_errno),ename(t_look( sv_look_fd )))

#define FAIL_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - failed unexpectedly\nexpected: %s\nreturned: %s", FUNCT, SSTR, FMT(ret_val))

#define ESUCC_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - succeeded unexpectedly\nexpected: %s\nreturned: %s\nexpected errno: %s", FUNCT, FSTR, FMT(ret_val), err_lookup(EXP_ERRNO))

#define TSUCC_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - succeeded unexpectedly\nexpected: %s\nreturned: %s\nexpected t_errno: %s", FUNCT, FSTR, FMT(ret_val), terr_lookup(EXP_TERRNO))

#define ETSUCC_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - succeeded unexpectedly\nexpected: %s\nreturned: %s\nexpected errno: %s\nexpected t_errno: %s", FUNCT, FSTR, FMT(ret_val), err_lookup(EXP_ERRNO), terr_lookup(EXP_TERRNO))

#define SUCC_ERR	sv_prerr( sv_xenv->sv_fsev,"%s - succeeded unexpectedly\nexpected: %s\nreturned: %s", FUNCT, FSTR, FMT(ret_val))

#define XVAL_ERR	( (void) strcpy(exp_str,FMT(EXP_RET)), \
					sv_prerr(sv_xenv->sv_vsev,"%s - returned unexpected value\nexpected: %s\nreturned: %s",FUNCT,exp_str,FMT(ret_val))	)

#define SF_IND_ERR	sv_prerr(sv_xenv->sv_usev,"%s - returned indeterminate value outside range of success/failure\nreturned: %s\nsuccess: %s\nfailure: %s",FUNCT, FMT(ret_val), SSTR, FSTR)

#define F_IND_ERR	sv_prerr(sv_xenv->sv_usev,"%s - returned indeterminate value outside range of failure\nreturned: %s\nsuccess: %s\nfailure: %s",FUNCT, FMT(ret_val), SSTR, FSTR)

#define S_IND_ERR	sv_prerr(sv_xenv->sv_usev,"%s - returned indeterminate value outside range of success\nreturned: %s\nsuccess: %s\nfailure: %s",FUNCT, FMT(ret_val), SSTR, FSTR)

#define ERRNO_ERR	sv_prerr(sv_xenv->sv_esev,"%s - set errno incorrectly\nexpected errno: %s\nactual errno: %s",FUNCT, err_lookup(EXP_ERRNO), err_lookup(errno) )

#define TERRNO_ERR	sv_prerr(sv_xenv->sv_esev,"%s - set t_errno incorrectly\nexpected t_errno: %s\nactual t_errno: %s",FUNCT, terr_lookup(EXP_TERRNO), terr_lookup(t_errno) )

#ifdef	DEVELOPMENT

#define	BAD_DATA	warning("%s - error in evaluation data",FUNCT)

#define BAD_XVAL	warning("%s - expected value outside range of success/failure\nexpected: %s\nsuccess: %s\nfailure: %s",FUNCT,FMT(EXP_RET),SSTR,FSTR)

#define BAD_XCHK	warning("%s - indeterminate expected value, funct has no range of success/failure\nexpected: %s\nsuccess: %s",FUNCT,FMT(EXP_RET),SSTR)

#define BAD_SUCC	warning("%s - success expected for indeterminate funct\nsuccess: %s\nfailure: %s",FUNCT,SSTR,FSTR)

#define NEED_SVAL	warning("%s - success expected for funct which needs expected value\nsuccess: %s\nfailure: %s",FUNCT,SSTR,FSTR)

#define NEED_FVAL	warning("%s - failure expected for funct which needs expected value\nsuccess: %s\nfailure: %s",FUNCT,SSTR,FSTR)

#define NEED_VAL	warning("succ/fail funct sv_value() was called");

#define BAD_FAIL	warning("%s - failure expected, funct has no range of failure\nsuccess: %s\nfailure: %s",FUNCT,SSTR,FSTR);

#define BAD_ERRNO	warning("%s - expected success with errno",FUNCT)

#define BAD_TERRNO	warning("%s - expected success with t_errno",FUNCT)

#define	BAD_ECHK	warning("%s - errno expected, funct has no defined errno values\nexpected errno:%s",FUNCT,err_lookup(EXP_ERRNO))

#define	BAD_TECHK	warning("%s - t_errno expected, funct has no defined t_errno values\nexpected t_errno:%s",FUNCT,terr_lookup(EXP_TERRNO))

#define NEED_ECHK	warning("%s - errno defined but not explicitly checked",FUNCT)

#define NEED_TECHK	warning("%s - t_errno defined but not explicitly checked",FUNCT)

#define EXPCT_ERR	warning("%s - undefined value of 'expecting'",FUNCT);

#endif

evaluate( fnct, ret_val )
struct 		ftable	*fnct;			/* evaluation criteria				*/
pointer		ret_val;				/* pointer to actual return value	*/
{
	char exp_str[512];				/* save fmt str before buff re-used	*/

#ifdef DEBUG
	printf("evaluate() ");
#endif

#ifdef TRACE
	sv_printf("\tevaluating: %s\n",FUNCT);
#endif
#ifdef	DEVELOPMENT
		if( SUCC_FUNCT == RET && FAIL_FUNCT && FAIL_FUNCT != GONE )
			BAD_DATA;					/* bad sv_succ/sv_fail pair		*/
		else if( FAIL_FUNCT == RET && SUCC_FUNCT && SUCC_FUNCT != GONE )
			BAD_DATA;					/* bad sv_succ/sv_fail pair		*/
		else if( SUCC_FUNCT == GONE && FAIL_FUNCT == GONE )
			BAD_DATA;					/* bad sv_succ/sv_fail pair		*/
		else if( SUCC_DEFINED && SUCCEEDED && FAIL_DEFINED && FAILED )
			BAD_DATA;					/* bad sv_succ/sv_fail pair		*/
#endif
	if( EXP_RVAL ) {
#ifdef TRACE
		sv_printf("\texpecting: explicit return value %s\n",FMT( EXP_RET ));
#endif
#ifdef	DEVELOPMENT
		if( SUCC_DEFINED && FAIL_DEFINED ) {
			if( SUCC( EXP_RET ) && FAIL( EXP_RET ))
				BAD_DATA;				/* bad succ/fail funct pair		*/
			if( !SUCC( EXP_RET ) && !FAIL( EXP_RET ))
				BAD_XVAL;				/* bad exp ret val, outside s/f	*/
		}
		if(FAIL_DEFINED && FAIL( EXP_RET ) 
			&& TERRNO_DEFINED && !EXP_TERRNO)
				NEED_TECHK;
		if(FAIL_DEFINED && FAIL( EXP_RET ) 
			&& ERRNO_DEFINED && !EXP_ERRNO 
			&& (!TERRNO_DEFINED || EXP_TERRNO == TSYSERR ))
				NEED_ECHK;
		if( !SUCC_FUNCT && !FAIL_FUNCT )
			BAD_XCHK;
#endif			
		if( GOT_EXPECTED )
			if ( EXP_TERRNO )
				if( GOT_TERRNO ) {
					if ( EXP_ERRNO )
						if( GOT_ERRNO ) {
							CLR_RVAL;
							CLR_ERRNO;
							PASS;		/* matched exp val/terrno/errno	*/
						}
						else
							ERRNO_ERR;	/* errno mismatch */
					else {
						CLR_RVAL;
						CLR_ERRNO;
						PASS;			/* matched failure/terrno	*/
					}
				}
				else
					TERRNO_ERR;			/* terrno mismatch				*/
			else 						/* not expecting a terrno */
				if ( EXP_ERRNO ) 
					if( GOT_ERRNO ) {
						CLR_RVAL;
						CLR_ERRNO;
						PASS;			/* matched exp val/errno	*/
					}
					else
						ERRNO_ERR;		/* errno mismatch */
				else {					/* not expecting errno */
					CLR_RVAL;
					CLR_ERRNO;
					PASS;				/* matched expected ret val 	*/
				}
		else
			XVAL_ERR;					/* didn't match expected ret 	*/
/*
 *			exit point for non-fatal errors when expecting explict value
 */
		CLR_RVAL;
		CLR_ERRNO;
		return;
	}
	if( EXP_VOID ) {
#ifdef TRACE
		sv_printf("\texpecting: VOID\n");
#endif
		CLR_RVAL;
		CLR_ERRNO;
		PASS;							/* expect void, don't check		*/
	}
	if( EXP_SUCCESS ) {
#ifdef TRACE
		sv_printf("\texpecting: SUCCESS with %s\n",SSTR);
#endif
#ifdef	DEVELOPMENT
		if( SUCC_FUNCT == VALUE )
			NEED_SVAL;
		if( !SUCC_FUNCT )
			BAD_SUCC;
		if( EXP_ERRNO )
			BAD_ERRNO;
#endif
		if( SUCC_DEFINED && SUCCEEDED )	
			PASS;						/* expected and got success 	*/
		else 							/* return val outside success	*/
			if( FAIL_DEFINED )
				if( FAILED )	{		/* E_FAIL						*/
					if( ERRNO_DEFINED && !TERRNO_DEFINED )			
						EFAIL_ERR;		/*exp succ, rpt fail w/ errno	*/
					else if( TERRNO_DEFINED && t_errno == TLOOK )
						LFAIL_ERR;		/*exp succ, rpt fail w/ event	*/
					else if( TERRNO_DEFINED &&
					  (!ERRNO_DEFINED || t_errno != TSYSERR))
						TFAIL_ERR;		/*exp succ, rpt fail w/ t_errno	*/
					else if( ERRNO_DEFINED && TERRNO_DEFINED 
					  && t_errno == TSYSERR)
						ETFAIL_ERR;		/*exp succ, rpt fail w/ both	*/
					else
						FAIL_ERR;		/* exp succ, rpt fail w/ neither*/
				}
				else 					/* E_UNDEF						*/
					SF_IND_ERR;			/* outside range of succ & fail	*/
			else						/* E_UNDEF						*/
				S_IND_ERR;				/* outside success, fail undef	*/
/*
 *			exit point for non-fatal errors when expecting success
 */
		return;
	}
	if ( EXP_FAILURE || EXP_ERRNO || EXP_TERRNO ) {
#ifdef TRACE
 		if( EXP_ERRNO && EXP_TERRNO)
			sv_printf("\texpecting: FAILURE with errno: %d t_errno: %d\n",
				EXP_ERRNO, EXP_TERRNO);
 		else if( EXP_ERRNO )
			sv_printf("\texpecting: FAILURE with errno: %d\n",EXP_ERRNO);
 		else if( EXP_TERRNO )
			sv_printf("\texpecting: FAILURE with t_errno: %d\n",
				EXP_TERRNO);
		else
			sv_printf("\texpecting: FAILURE with %s\n",FSTR);
#endif
#ifdef	DEVELOPMENT
		if( !FAIL_FUNCT )
			BAD_FAIL;
		if( FAIL_FUNCT == VALUE )
			NEED_FVAL;
		if( TERRNO_DEFINED && !EXP_TERRNO )
			NEED_TECHK;
		if( TERRNO_DEFINED && (EXP_TERRNO == TSYSERR) 
		  && ERRNO_DEFINED && !EXP_ERRNO )
			NEED_ECHK;
		if( !ERRNO_DEFINED && EXP_ERRNO )
			BAD_ECHK;
		if( !TERRNO_DEFINED && EXP_TERRNO )
			BAD_TECHK;
#endif
		if( FAIL_FUNCT && FAILED )
			if ( EXP_TERRNO )
				if( GOT_TERRNO ) {
					if ( EXP_ERRNO )
						if( GOT_ERRNO ) {
							CLR_RVAL;
							CLR_ERRNO;
							PASS;		/* matched failure/terrno/errno	*/
						}
						else
							ERRNO_ERR;	/* errno mismatch */
					else {
						CLR_RVAL;
						CLR_ERRNO;
						PASS;		/* matched failure/terrno	*/
					}
				}
				else
					TERRNO_ERR;			/* terrno mismatch				*/
			else 						/* not expecting a terrno */
				if ( EXP_ERRNO ) 
					if( GOT_ERRNO ) {
						CLR_RVAL;
						CLR_ERRNO;
						PASS;			/* matched failure/errno	*/
					}
					else
						ERRNO_ERR;		/* errno mismatch */
				else {					/* not expecting errno */
 					CLR_RVAL;
					CLR_ERRNO;
					PASS;				/* got expected failure 	*/
				}
		else							/* ret outside of failure		*/
			if( SUCC_DEFINED )
				if( SUCCEEDED )			/* E_SUCCESS					*/
					if( TERRNO_DEFINED )
						if( EXP_ERRNO )
							ETSUCC_ERR;	/* exp fail/errno/terrno		*/
						else
							TSUCC_ERR;	/* exp fail/terrno, got success	*/
					else if( ERRNO_DEFINED )
						ESUCC_ERR;		/* exp fail/errno, got success	*/
					else
						SUCC_ERR;		/* exp failure, got success	*/
				else					/* E_UNDEF						*/
					SF_IND_ERR;			/* outside range of succ & fail	*/
			else						/* E_UNDEF						*/
				F_IND_ERR;				/* ret outside range of failure	*/
/*
 *			exit point for non-fatal errors when expecting failure/errno
 */
		CLR_ERRNO;
		return;
	}
#ifdef	DEVELOPMENT
	EXPCT_ERR;
#endif
}
/*
 *	Possible outcomes for expected success/failure for functions with
 *	strange properties, or which cannot fail.  
 *
 *	Ex. 
 *		exit:
 *			success:	gone		(success is a gone process)
 *			failure:	(-1)
 *			others:		undefined
 *
 *		sync:
 *			success:	return		(success is a continued process)
 *			failure:	void		(can't be checked)
 *	
 *		tolower:
 *			success:	value		(all are success, req. usr val to chk)
 *			failure:	void		(can't occur)
 *
 *	Note that sv_value() should never be called to check an expected 
 *	outcome, it is here as a convenient unique address for a function ptr.
 */
sv_gone() {
	return(0);	/* no checking possible, fail if process still running */
}
sv_return() {
	return(1);	/* no checking possible, pass if process still running */
}
sv_value() {
#ifdef	DEVELOPMENT
	NEED_VAL;
#endif
	return(1);	/* all vals represent success, but should not be called */
}
