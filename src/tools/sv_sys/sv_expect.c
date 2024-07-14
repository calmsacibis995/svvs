/*LINTLIBRARY*/
/*
 *	sv_expect.c - svvs expected return value catching
 */
#include <varargs.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_expect.c	1.7"
#endif

static	char			c_saved;
static	int				i_saved;
static	unsigned		u_saved;
static	long			l_saved;
static	double			d_saved;
static	pointer			p_saved;
static	unsigned short	us_saved;
static	int				err_saved;
static	int				terr_saved;	/* for nse's t_errno */


sv_exp( va_alist )
va_dcl
{
	va_list	ap, p;

	if( sv_ltest( sv_xenv ) ) {	/* if sv_lock is set, this is exp value	*/
								/* else exp_errno, SUCC, FAIL, or VOID	*/
		sv_ulock( sv_xenv );	/* unlock								*/
		sv_svalu( sv_xenv );	/* set valu flag						*/
		va_start( ap );
		c_saved = va_arg( ap, char );
		va_end( ap );
		va_start( ap );
/*Lint will complain of ptr. alignment problems*/
		i_saved = va_arg( ap, int );
		va_end( ap );
		va_start( ap );
		u_saved = va_arg( ap, unsigned );
		va_end( ap );
		va_start( ap );
		l_saved = va_arg( ap, long );
		va_end( ap );
		va_start( ap );
		d_saved = va_arg( ap, double );
/*
 *		Following assignment added to circumvent optimizer
 *		problem that caused d_saved to lose low order word, 
 *		due to incorrect incrementing of ap.  Lint will
 *		complain that 'p' is unused.
 */		
		p = ap;	
		va_end( ap );
		va_start( ap );
		p_saved = va_arg( ap, pointer );
		va_end( ap );
		va_start( ap );
		us_saved = va_arg( ap, unsigned short );
/*Lint will not complain of ptr. alignment problems after this point*/
		va_end( ap );
	}
}
sv_sets() {
	if( sv_ltest( sv_xenv ) )
		sv_ssucc( sv_xenv );
}
sv_setf() {
	if( sv_ltest( sv_xenv ) )
		sv_sfail( sv_xenv );
}
sv_setv() {
	if( sv_ltest( sv_xenv ) )
		sv_svoid( sv_xenv );
}
static sv_errset( exp_err, sav_loc )
int  exp_err, *sav_loc;
{
	if( sv_ltest( sv_xenv ) ) {	/* true if within expecting() macro	*/
		sv_ulock( sv_xenv );		/* tell sv_exp to ignore this value */
		sv_serrn( sv_xenv );		/* tell sv_eval to expect an errno	*/
		*sav_loc = exp_err;
	}
	if( !exp_err ) *sav_loc = 0;
	return( exp_err );
}	

sv_sete( exp_errno )
int exp_errno;
{
	return( sv_errset( exp_errno, &err_saved ) );
}
sv_sett( exp_terrno )
int exp_terrno;
{
	return( sv_errset( exp_terrno, &terr_saved ) );
}
char *sv_getc() {
	return( &c_saved ); 
}
int *sv_geti() {
	return( &i_saved ); 
}
unsigned *sv_getu() {
	return( &u_saved ); 
}
long *sv_getl() {
	return( &l_saved ); 
}
double *sv_getd() { 
	return( &d_saved ); 
}
pointer *sv_getp() {
	return( &p_saved ); 
}
unsigned short *sv_getus() {
	return( &us_saved ); 
}
int sv_gete() {
	return( err_saved );
}
int sv_gett() {
	return( terr_saved );
}
void sv_getv() {
#ifdef	DEVELOPMENT
warning("sv_xval for void function called");
#endif
}
