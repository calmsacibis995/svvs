/*LINTLIBRARY*/
#include <stdio.h>
#include <signal.h>
#include <svvs.h>
#include <sv_env.h>

#ifdef UNDEF
#ident	"@(#)sv_setup.c	1.9"
#endif

/*
 *	sv_setup.c
 *
 *	Called once for every executable which:
 *
 *		a.	uses machine dep constants
 *		b.	cares about signal catching
 *		c.	wants to use tmp/data directories
 *
 *	Prints out the name of the man section.
 *	Generates a warning for subsequent calls.
 *	Sets up a whole lot of signal handlers.
 *
 *	Note:
 *		Any changes to setup may also be needed in OS/exec
 *		which has fake setup so as not to destroy signal settings,
 *		close open file descriptors, etc, while testing exec.
 *
 */

#ifdef	DEVELOPMENT
static int beenhere = 0;
#endif

struct siggs {
	int sigg;
	char *siggname;
	int sflg;
} siggs[] = {
	{	SIGHUP,		"SIGHUP",	0	},
	{	SIGINT,		"SIGINT",	0	},
	{	SIGQUIT,	"SIGQUIT",	0	},
	{	SIGILL,		"SIGILL",	0	},
	{	SIGTRAP,	"SIGTRAP",	0	},
	{	SIGFPE,		"SIGFPE",	0	},
	{	SIGSYS,		"SIGSYS",	0	},
	{	SIGPIPE,	"SIGPIPE",	0	},
	{	SIGALRM,	"SIGALRM",	0	},
	{	SIGTERM,	"SIGTERM",	0	},
	{	SIGUSR1,	"SIGUSR1",	0	},
	{	SIGUSR2,	"SIGUSR2",	0	},
# ifdef	SIGABRT
	{	SIGABRT,	"SIGABRT",	0	},
# endif
# ifdef	SIGIOT
	{	SIGIOT,		"SIGIOT",	0	},
# endif
# ifdef	SIGEMT
	{	SIGEMT,		"SIGEMT",	0	},
# endif
# ifdef	SIGBUS
	{	SIGBUS,		"SIGBUS",	0	},
# endif
# ifdef	SIGSEGV
	{	SIGSEGV,	"SIGSEGV",	0	},
# endif
# ifdef	SIGPWR
	{	SIGPWR,		"SIGPWR",	0	},
# endif
};

# define NSIGGS	(sizeof(siggs)/sizeof(struct siggs))

char	*strcpy(),*strcat();
static	char	*sv_nclose = NULL;
static	char	*sv_nsig = NULL;
static	siggy	(*(*signl)())() = (siggy (*(*)())())signal;
					/*ptr to function returning ptr*/
					/*to function returning void*/
sv_sigset()
{
#ifndef SYSVR_2		/*sigset not defined in release 2*/
	extern	void	(*sigset())();

	signl = sigset;		/*Change signal function to sigset*/
#endif
}

sv_no_close( fd )
int fd;
{
	char *calloc();

	if( !sv_nclose && !(sv_nclose = calloc( OPEN_MAX, 1 )))
		severr("Can't malloc space for retained fd list");
	*(sv_nclose+fd) = '\1';
}
sv_no_sig( sig )
int sig;
{
	char *calloc();
	int i;

	for(i=0;i<NSIGGS;i++)
		if( sig == siggs[i].sigg )
			siggs[i].sflg=1;
}
sv_setup(str)
char	*str;
{
	register int i;
	register char *datadir, *tempdir;
	void	sig_handler();
	char *sv_temp_path();
	char *sv_data_path();

# ifdef	DEVELOPMENT
	if ( beenhere++ )
		warning("sv_setup() called more than once");
# endif
	datadir = sv_data_path();
	tempdir = sv_temp_path();
	(void) strcpy(datadir,"../data/");
	(void) strcpy(tempdir,"../tmp/"); 
	(void) strcat(datadir, str);
	(void) strcat(tempdir, str);
	(void) strcat(datadir, ".dir/");
 	(void) strcat(tempdir, ".dir/");

/*
 * Set up installation dependent values for use in util.c
 * functions.
 */
	sv_config();
	sv_nproc();
	sv_catchsig(sig_handler);
/*
 * close everything except stdin, stdout, stderr
 */
	for( i = 3; i < OPEN_MAX; i++) {
		if( (i != sv_get_outfd()) && (!sv_nclose || !(*(sv_nclose+i))) )
			(void) close(i);
	}
	if( sv_nclose ) free( sv_nclose );
}
sv_catchsig( handler )
void	(*handler)();
{
	register struct siggs *p = &siggs[0];
	register int i;
/*
** Set the signal catching for all SVID signals.  signl is a pointer
** to the function to be used for this purpose, it could be either
** signal() or sigset() depending on whether sv_sigset() was called
*/

	for(i=0;i<NSIGGS;i++,p++) {
		if( !siggs[i].sflg ) {
			if ((*signl)(p->sigg, handler) == (siggy (*)())SIG_ERR) {
				error("Failed to set signal handler for %s",p->siggname);
			}
		}
	}
	if( sv_nsig ) free( sv_nsig );
}

/*
 *	sig_lookup
 *		Look up the symbolic name for a signal.
 */
char *sig_lookup(signum)
int	signum;
{
	register int i;
	register struct siggs *s = &siggs[0];
	static char buf[32];

	for(i=0;i<NSIGGS;i++,s++)
		if( s->sigg == signum )
			return(s->siggname);
	(void) sprintf(buf,"%d (unknown signal)",signum);
	return(buf);
}
/*
** sig_handler
**	Handle unexpected signals.
**
** Arguments
**	sig	signal number sent by the system.
**
** Side Effects
**	Prints an error message.
**	Attempts to send a SIGKILL to any children that might be around.
**	Calls done.
*/

void	sig_handler(sig)
int	sig;
{
	 (siggy) (*signl)(sig,sig_handler);
	 sv_prerr(sv_xenv->sv_xsev,
		"unexpected signal occurred: %s",sig_lookup(sig));
}
