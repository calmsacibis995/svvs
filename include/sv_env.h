/*
 *	EMACS_MODES: tabstop=4
 *
 *	sv_env.h - macro definitions for svvs test control.
 *
 */

#ifdef	UNDEF
ident	"@(#)sv_env.h	1.7"
#endif

#include <setjmp.h>
#include <errno.h>
#include <sv_const.h>

/*
 *	typedefs of pointer types, useful for casts
 */
typedef	char *pointer;
typedef	unsigned int sv_ugid;	/* typedef for comparison of [ug]id's */
typedef	int (*ifunctptr)();
typedef	pointer (*pfunctptr)();
/*
 *	copy struct sv_env's and jmpbuf's
 */
pointer	sv_memcpy();
#define	sv_ecpy(a,b) (sv_memcpy((pointer)a,(pointer)b,sizeof(struct sv_env)))
#define	sv_jcpy(a,b) (sv_memcpy((pointer)a,(pointer)b,sizeof(jmp_buf)))

/*
 *	system detected error conditions
 *
 *	use as arguments to warn_on(), error_on(), severr_on().
 *
 *	failure	- funct returned failure when success was expected
 *  success	- funct returned success when failure was expected
 *  success	- funct returned unexpected value when explicit val expected
 *	errno	- funct returned expected failure, but with incorrect errno
 *	except	- caught signal, also use for other asynch problems
 *	undef	- funct returned value outside range of success/failure
 */
#define E_FAILURE		1<<8
#define E_SUCCESS		2<<8
#define E_UNEXP			4<<8
#define E_ERRNO			8<<8
#define	E_EXCEPT		16<<8
#define E_UNDEF			32<<8
#define	sv_fsev			sv_sev[0]	/* severity for unexp failure error	*/
#define sv_ssev			sv_sev[1]	/* severity for unexp success error	*/
#define sv_esev			sv_sev[2]	/* severity for unexp errno error	*/
#define sv_xsev			sv_sev[3]	/* severity for unexp signal error	*/
#define sv_vsev			sv_sev[4]	/* severity for unexp ret val error	*/
#define	sv_usev			sv_sev[5]	/* severity for undefined ret val	*/
#ifdef SV_ENV
int sv_err_table[] = {
	E_FAILURE,	E_SUCCESS,	E_UNEXP,	E_ERRNO,	E_EXCEPT,	E_UNDEF
};
#endif
#define	SV_NERRS	6	/*	( sizeof( sv_err_table )/sizeof( int ) )	*/

/*
 *	error types
 *
 *	these error flags are passed to the error reporting function
 *		to indicate the type of error, and also are used for
 *		tracking actions vs. error types
 *
 *	message - information only, used for suppressed tests
 *	warning	- will not prevent verification of system under test
 *	error	- prevents verification due to problem
 *	severr	- same as error, but more serious problem detected
 */
#define	SV_MID			0
#define	SV_UID			1
#define	SV_WID			2
#define	SV_EID			3
#define	SV_SID			4

#define MESSAGE			(1<<SV_MID)
#define UNSUPPORTED		(1<<SV_UID)
#define WARNING			(1<<SV_WID)
#define	ERROR			(1<<SV_EID)
#define SEVERR			(1<<SV_SID)

#define	sv_mact			sv_act[SV_MID]	/* action for messages			*/
#define	sv_uact			sv_act[SV_UID]	/* action for messages			*/
#define	sv_wact			sv_act[SV_WID]	/* action for warnings			*/
#define	sv_eact			sv_act[SV_EID]	/* action for errors 			*/
#define sv_sact			sv_act[SV_SID]	/* action for severrs			*/

#define	sv_mjmp			sv_jmpb[SV_MID]	/* jmpbuf for break on message	*/
#define	sv_ujmp			sv_jmpb[SV_UID]	/* jmpbuf for break on unsupport*/
#define	sv_wjmp			sv_jmpb[SV_WID]	/* jmpbuf for break on warning	*/
#define	sv_ejmp			sv_jmpb[SV_EID]	/* jmpbuf for break on error	*/
#define	sv_sjmp			sv_jmpb[SV_SID]	/* jmpbuf for break on severr	*/
#define	sv_jmp			sv_jmpb[SV_SID+1]/* jmpbuf for current block	*/

#ifdef SV_ENV
int sv_sev_table[] = {
	MESSAGE,  UNSUPPORTED,	WARNING,	ERROR,	SEVERR
};
char *sv_snam_table[] = {
	"MESSAGE",  "UNSUPPORTED",	"WARNING",	"ERROR",	"SEVERR"
};
#endif
#define	SV_NSEVS		5	/*	( sizeof( sv_sev_table )/sizeof( int ) )*/
/*
 *	error actions
 *
 *	these flags are used in conjunction with the error flags to determine
 *		what action to take when an error is encountered.
 *
 *	abort	- terminate entire test
 *	exit	- terminate current process
 *	break	- terminate current test block
 *	continue- no termination
 */
#define	ABORT			1
#define	EXIT			2
#define	BREAK			4
#define	CONTINUE		8
/*
 *	internal set of possible outcomes which are expected,
 *	these macros are not intended for use by the test writer.
 *
 *	possible outcomes which can be expected are:
 *
 *		SV_VALU:	success with a particular value
 *		SV_SUCC:	success
 *		SV_FAIL:	failure
 *		SV_ERRN:	failure with a particular errno
 *		SV_VOID:	don't check return value
 *		SV_LOCK:	set when in the expecting() macro
 *
 *	these are used when the expecting() macro is invoked, see the
 *	following section.
 */
#define SV_VALU			1
#define SV_SUCC			2
#define SV_FAIL			4
#define SV_ERRN			8
#define SV_VOID			16
#define SV_LOCK			32
/*
 *	expected returns from functions
 *
 *	These values are used as arguments to expecting() to indicate the
 *		expected result from a function call.
 *
 *		failure	- expecting function to fail
 *		success - expecting function to succeed
 *		void	- disable automatic function checking
 *
 *	In addition, the test writer may specify either an errno value, or
 *	an expected value, in which case the evaluation function will check
 *	the return/errno against the values passed to expecting.  In order 
 *	to implement this, a second variable, sv_expecting, is set with the
 *	macros SV_SUCC, SV_FAIL, SV_VOID, SV_VALU, and SV_ERRN.
 */
#define	SUCCESS			sv_sets()
#define FAILURE			sv_setf()
#define VOID			sv_setv()
/*
 *	some useful macros for setting, testing, and clearing sv_expecting.
 */
#define sv_svalu( xnv )	(xnv->sv_expecting |= SV_VALU)
#define sv_serrn( xnv )	(xnv->sv_expecting |= SV_ERRN)
#define sv_ssucc( xnv )	(xnv->sv_expecting = SV_SUCC)
#define sv_sfail( xnv )	(xnv->sv_expecting = SV_FAIL)
#define sv_svoid( xnv )	(xnv->sv_expecting = SV_VOID)
#define sv_slock( xnv )	(xnv->sv_expecting |= SV_LOCK)
#define sv_ltest( xnv )	(xnv->sv_expecting & SV_LOCK)
#define sv_ulock( xnv )	(xnv->sv_expecting &= ~SV_LOCK)

/*
 *	the expecting macro, passes the expected return status to sv_exp(),
 *	which saves it for later use by the evaluation function.
 */
#define	expecting(foo) 	(sv_slock(sv_xenv),sv_exp(foo))

/*
 *	possible success/failure existences for a function
 *
 *		NULL:	undefined/untestable, requires expecting( VOID )
 *		VALUE:	defined, but requires expecting( "expression" )
 *		GONE:	function will not return for this state
 *		RET:	if function returns, it is in this state
 */
int	sv_gone();
int sv_value();
int sv_return();
#define GONE			(sv_gone)
#define	VALUE			(sv_value)
#define	RET				(sv_return)

struct sv_env {
	struct sv_env *sv_oenv;		/* ptr to prev frame					*/
	int	sv_expecting;			/* expect: succ, fail, void, errno, val	*/
	int sv_cnt;					/* cnt of test cases at current frame	*/
	int	sv_fcnt;				/* cnt of children at current frame		*/
	int	sv_levflg;				/* need current level "1.2.3" printed?	*/
	int sv_sev[SV_NERRS];		/* severities for errors				*/
	int sv_act[SV_NSEVS];		/* actions for severities				*/
	jmp_buf	sv_jmpb[SV_NSEVS+1];/* jmpbufs for breaks					*/
};

#define	sv_begblk		{ 												\
						struct	sv_env	sv_eblk;						\
						sv_einit( &sv_eblk );							\
						if ( !setjmp( sv_xenv->sv_jmp ) ) {

#define	sv_endblk		}												\
						sv_xenv=sv_eblk.sv_oenv;						\
						}							 					\
						sv_xenv->sv_levflg=1;							\
						sv_xenv->sv_cnt++;

#define abstract		sv_abstract
#define preparation		sv_preparation
#define prep			sv_preparation
#define testing			sv_testing
#define cleanup			sv_cleanup
#define	setup(function)	sv_setup(function,sv_params)
#define funct			sv_funct
#define done			sv_done
#define start			sv_begblk
#define finis			sv_endblk

#define warning_on		warn_on
#define warn_on(cond)	sv_sevset( cond, WARNING )
#define error_on(cond)	sv_sevset( cond, ERROR )
#define severr_on(cond)	sv_sevset( cond, SEVERR )
#define continue_on		cont_on
#define cont_on(sev)	sv_actset( sev, CONTINUE )
#define break_on(sev)	sv_actset( sev, BREAK )
#define exit_on(sev)	sv_actset( sev, EXIT )
#define abort_on(sev)	sv_actset( sev, ABORT )
#define reset(params)	sv_reset( params )

#define message			sv_message
#define unsupported		sv_unsupported
#define warning			sv_warning
#define error			sv_error
#define severr			sv_severr

/*
 *	ftable is a structure describing the behavior of a function 
 *	for both success and failure.  ftable is used by the evaluation 
 *	function to determine whether an unexpected event has occurred.
 *
 *	There is at least one struct ftable for each function in the SVID.
 *	Some functions which have different return ranges for different 
 *	inputs may have more than one struct. These use a hard coded switch to
 *	determine which particular structure applies in a given case.
 *
 *	Test writers may define their own struct ftable's if they desire
 *	automatic return value checking for their own functions.  For examples
 *	see the supplied replacement functions.
 */
struct ftable {
	char	*sv_name;		/* function name							*/
	char	*sv_sstr;		/* string describing success				*/
	char	*sv_fstr;		/* string describing failure				*/
	char	*((*sv_fmt)());	/* formatting function						*/
	int		(*sv_test)();	/* funct which returns 1/0 for act == exp	*/
	int		(*sv_succ)();	/* funct which returns 1/0 for success		*/
	int		(*sv_fail)();	/* funct which returns 1/0 for failure		*/
	pointer	(*sv_xval)();	/* funct which returns ptr to exp ret val	*/
	int		sv_errnodef;	/* 1: errno defined on failure, 2: t_errno	*/
};

#define STRVAL	64			/* static area for each sv_fmt funct		*/

#ifndef SV_ENV
extern struct sv_env sv_deflt;/* initial environment in sv_env.c		*/
extern struct sv_env *sv_env;/* initial environment ptr, set in sv_env.c*/
extern struct sv_env *sv_xenv;/* extern used for passing current env	*/
#endif

/*
 *	convenient macros to pass to expecting
 */
#define	NON_NULL	((pointer)01)
#define	ZERO		(sv_setz(),0.0)
/*
 *	kludge for getting fd to tli z_functs
 */
int	sv_look_fd;

#undef SV_IPC

#if	defined( MSG ) || defined( SHM ) || defined( SEM )
#define SV_IPC
struct	ipc_id {
	int	id;
	int typ;
};

#define	IPCSIZE	sizeof( struct ipc_id )
#define	SV_IPCFILE	"ipc_ids"
#endif

# define SV_MSG	1
# define SV_SHM	2
# define SV_SEM	3
