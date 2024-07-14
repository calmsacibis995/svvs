/*LINTLIBRARY*/
/*
 *	sv_env.c - svvs test control
 */

#ifdef	UNDEF
#ident	"@(#)sv_env.c	1.14"
#endif

#define SV_ENV

#include <signal.h>
#ifndef	SYSVR_2
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include <sv_env.h>
#include <svvs.h>

#define	PSIZE 128

#ifndef SYSVR_2
# define 	WORLD	S_IWOTH | S_IROTH | S_IWGRP | S_IRGRP | S_IWRITE | S_IREAD
# else
# define	WORLD	0666
# endif

char *sv_temp_path();
char *strcpy();
void exit();
unsigned alarm();

static char sv_prefix[ PSIZE ] = "";

struct sv_env sv_deflt = {
	(struct sv_env *)0,			/* ptr to prev frame					*/
	SV_SUCC,					/* expecting success failure or void	*/
	1,							/* id of next test blk at current frame	*/
	1,							/* id of next child at current frame	*/
	1,							/* need current level "1.2.3" printed?	*/
	{	ERROR,					/* severity for unexp failure error		*/
		ERROR,					/* severity for unexp success error		*/
		ERROR,					/* severity for unexp errno error		*/
		SEVERR,					/* severity for unexp signal error		*/
		ERROR,					/* severity for unexp ret val error		*/
		ERROR,					/* severity for undef ret val error		*/
	},
	{	CONTINUE,				/* action for normal messages			*/
		CONTINUE,				/* action for unsupport messages		*/
		CONTINUE,				/* action for errors of sev warning		*/
		CONTINUE,				/* action for errors of sev error		*/
		ABORT,					/* action for errors of sev severr		*/
	},
};

struct sv_env *sv_env = &sv_deflt;
struct sv_env *sv_xenv = &sv_deflt;
static int do_setpgrp = 1;	/* turn off setpgrp and abort facility */
/*
 *	generate table index for severity
 */
sv_sid( severity )
int severity;
{
	int i;
	for( i=0; i<SV_NSEVS; i++ )
		if( sv_sev_table[i]==severity )
			return(i);
#ifdef	DEVELOPMENT
	warning("Unknown severity in sv_sid()\n");
#endif
	return(0);
}
/*
 *	return severity name
 */
char *sv_snm( severity )
int severity;
{
	return( sv_snam_table[sv_sid(severity)] );
}
/*
 *	severity declaration function
 */
sv_sevset( conditions, severity )
int conditions;
int severity;
{
	int i;
#ifdef DEBUG
	(void) printf("sv_sevset() ");
#endif
	for( i = 0; i < SV_NERRS; i++ )
		if( conditions & sv_err_table[i] )
			sv_xenv->sv_sev[i] = severity;
}
/*
 *	action declaration function
 */
sv_actset( severities, action )
int severities;
int action;
{
	int i;
#ifdef DEBUG
	(void) printf("sv_act_set() ");
#endif
#ifdef	DEVELOPMENT
	for( i = 0; i < SV_NERRS; i++ )
		if( severities & sv_err_table[i] )
			warning("sv_act_set() called with an error type.");
	if( !do_setpgrp && (severities & ABORT) )
		warning("no_setpgrp set, cannot call abort_on().");
#endif
	for( i = 0; i < SV_NSEVS; i++ )
		if( severities & sv_sev_table[i] )
			if(( sv_xenv->sv_act[i] = action ) == BREAK )
				(void)sv_jcpy( sv_xenv->sv_jmpb[i], sv_xenv->sv_jmp );
}
/*
 *	reset actions and/or severities, replace break points
 */
sv_reset( typs )
int typs;
{
	int i;
#ifdef DEBUG
	(void) printf("sv_reset() ");
#endif
	if( !sv_xenv->sv_oenv ) return;
	for( i = 0 ; i < SV_NSEVS; i++ )
		if( typs & sv_sev_table[i] ) {
			if( sv_xenv->sv_act[i] & BREAK)
				(void)sv_jcpy( sv_xenv->sv_jmpb[i],
				  sv_xenv->sv_oenv->sv_jmpb[i] );
			sv_xenv->sv_act[i] = sv_xenv->sv_oenv->sv_act[i];
		}
	for( i = 0 ; i < SV_NERRS; i++ )
		if( typs & sv_err_table[i] )
			sv_xenv->sv_sev[i] = sv_xenv->sv_oenv->sv_sev[i];
}
int	sv_cproc() {
	char	buf[ PSIZE ];
	char	*sv_getlev();

#ifdef DEBUG
	(void) printf("sv_cproc(): ");
#endif
	(void) sv_getlev( buf );
	(void) sprintf(buf+strlen(buf), "<p%d>.",sv_xenv->sv_fcnt);	
	(void) strcpy( sv_prefix, buf );
	sv_xenv->sv_fcnt = 1;
	sv_set_exit_val( 0 );
#ifdef DEBUG
	(void) printf("prefix: %s\n",sv_prefix);
#endif

}
int	sv_pproc() {
	sv_xenv->sv_fcnt++;
}
int sv_mproc() {
	int fd;
	int outfd;
	unsigned len;
	char name[ 64 ];
	char buf[ PSIZE ];

 	(void) sprintf(name,"%s/tmp/%s/pid.%d",SVVSPATH,SUBTMP,getpid());
	fd = open(name, O_CREAT | O_WRONLY, WORLD);
	if( fd < 0 )
		warning("can't create temp file: '%s'\n",name);
	else {
		outfd = sv_get_outfd();
		if( write(fd,&outfd,sizeof(int)) != sizeof(int))
			warning("can't write temp file: '%s'\n",name);
		else {
			(void) sv_getlev( buf );
			len = strlen( buf );
			if( write(fd,buf,len) != len)
				warning("can't write temp file: '%s'\n",name);
		}
		if( close(fd) )
			warning("can't close temp file: '%s'\n",name);
	}

}
#ifndef NO_KEYBD_SIGS
/*
 *	Since test is running with a different process group id, 
 *	it cannot be killed with signals from tty.
 *	Leave a signal catching proc around with tty grpid which
 *	can pass the signal on to the test.
 *	If desired, this can be removed for security reasons.
 */
static int child_pid = -1;
static int caught_sig = 0;
static int got_ext_sig = 0;
static void timed_out() {
	char *sig_lookup();
	severr( "test caught signal: %s from outside its process group - \ntest NOT terminated\npid: %d", sig_lookup( caught_sig ), child_pid );
}
static void keybd_sig( sig )
int sig;
{
	char *sig_lookup();
	
	got_ext_sig = 1;	/* if wait ret's due to this, wait again */
	caught_sig = sig;

	if( child_pid <= 0 )	/* should never happen */
		exit((int) SEVERR );
	(void) signal( sig, keybd_sig );
	if( child_pid > 0 )
		(void) kill( (-1)*child_pid, sig );
	(void) signal( SIGALRM, timed_out );
	(void) alarm( (unsigned) 5 );/* wait 5 sec for children to suicide */
}
static keybd_trap() {
	int wpid;
	int sts;
	int sv_errno;

	child_pid = fork();
	if( child_pid < 0 ) {
		exit_on( SEVERR );
		severr("test cannot fork");
	}
	if( child_pid ) {
		exit_on( WARNING | ERROR | SEVERR );
/*
 *		if catch sig, kill child, set 5 sec alarm, and wait
 *		if alarm times out, note that child is still alive
 *		and exit
 */
		sv_catchsig( keybd_sig );
		errno = 0;
		do {
			got_ext_sig = 0;
			errno = 0;
			wpid = wait(&sts);
			sv_errno = errno;
		} while( got_ext_sig );
		(void) alarm( (unsigned) 0);	/* may have come back from alarm */
		if ( wpid != child_pid ) {
			if( sv_errno == EINTR ) {
/*
 *				alarm went off, should have called 
 *				timed_out via alarm mechanism, so
 *				something is broken, but call it anyway
 */
				severr("wait failed\nsignal handling failed\ntest caught signal: %s from outside its process group -\ntest NOT terminated\npid: %d", sig_lookup( caught_sig ), child_pid );

			}
			severr( "wait returned unexpected value: %d",wpid );
		}
		if( sts & 0177 )
			error("test terminated due to signal, exit sts: %d",sts ); 
		exit( (int)((unsigned)sts >> 8));
	}
}
#endif

no_setpgrp() {
	do_setpgrp = 0;
	sv_deflt.sv_sact = EXIT;
#ifdef	DEVELOPMENT
	if( sv_xenv != &sv_deflt )
		warning("no_setpgrp() called from within start / finis block.");
#endif
}
int sv_nproc() {
	int fd, cnt;
	int outfd;
	char name[ 64 ];

	sv_prefix[0] = '\0';
 	(void) sprintf(name,"%s/tmp/%s/pid.%d",SVVSPATH,SUBTMP,getpid());
 	fd = open( name, O_RDONLY );
	if( fd < 0 )	{
/*
 *	first process of a test will not have a tmp/pid.???? file, 
 */
#ifndef NO_KEYBD_SIGS
		keybd_trap();
#endif
		if( do_setpgrp && setpgrp() != getpid() )
			error("cannot initialize: setpgrp failed");
#ifdef	SV_IPC
		sv_clr_ipc_file();
#endif
		return;
	}
	if( read(fd, &outfd, sizeof(int)) != sizeof(int))
			error("cannot initialize: read failed for temp file: '%s'\n",name);
	else {
		sv_set_outfd( outfd );
		if( !(cnt=read(fd,sv_prefix,(PSIZE-1))) )
			error("cannot initialize: read failed for temp file: '%s'\n",name);
		else
			sv_prefix[cnt] = '\0';
	}
	if( close(fd) )
		error("cannot initialize: close failed for temp file: '%s'\n",name);
}
char *sv_newlev( sv_xnv, buf )
struct sv_env *sv_xnv;
char *buf;
{

	if( sv_xnv->sv_oenv )	
		(void) sv_newlev( sv_xnv->sv_oenv, buf );
	else
		if( sv_prefix[0] )
			(void) sprintf(buf,"%s",sv_prefix);
	(void) sprintf(buf+strlen(buf), "%d." ,sv_xnv->sv_cnt);
	return( buf );
}
char *sv_getlev( buf )
char *buf;
{
	buf[ 0 ] = '\0';
	if( sv_xenv->sv_oenv )
		(void) sv_newlev( sv_xenv->sv_oenv, buf );
	return( buf );
}
sv_prlev() {
	char buf[256];
	buf[0] = '\0';
	sv_printf("%s ",sv_newlev( sv_xenv , buf ));
}
pointer sv_memcpy(p1,p2,i)
pointer	p1, p2;
unsigned	i;
{
	char *r = p1;
	if( p2 ) 
		while(i--) 
			*p1++ = *p2++;
	return( (pointer) r );
}
sv_einit( sv_env )
struct sv_env *sv_env;
{
	sv_printf("\n");
	sv_flush();
	sv_ecpy( sv_env, sv_xenv );
	sv_env->sv_oenv = sv_xenv;
	sv_env->sv_cnt=1;
	sv_env->sv_fcnt=1;
	sv_env->sv_levflg=1;
	sv_xenv=sv_env;
}
