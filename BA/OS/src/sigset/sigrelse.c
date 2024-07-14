/*
**EMACS_MODES: tabstop=4
*/

#ident	"@(#)sigrelse.c	1.3"

#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2

#include <setjmp.h>

extern	long	time();

#define	GOOD	1
#define	BAD		0
#define	KILL	3	/*Send first signal 3 seconds after fork()*/
#define	KILL2	5	/*Send second signal 5 seconds after first signal*/
#define	RLSE	5	/*Release hold 5 seconds after final signal*/

void	catchhold();	/*Signal catcher for sighold() test*/
void	catchjmp();		/*longjmp() signal catcher*/

struct	acttab{
	int		signo;	
	int		exp;
	char	*signam;
};

struct	acttab tab[] ={
	{SIGHUP,	GOOD,	"SIGHUP"},
	{SIGINT,	GOOD,	"SIGINT"},
	{SIGQUIT,	GOOD,	"SIGQUIT"},
	{SIGILL,	GOOD,	"SIGILL"},
	{SIGTRAP,	GOOD,	"SIGTRAP"},
	{SIGABRT,	GOOD,	"SIGABRT"},
	{SIGFPE,	GOOD,	"SIGFPE"},
	{SIGKILL,	BAD,	"SIGKILL"},
	{SIGSYS	,	GOOD,	"SIGSYS"},
	{SIGPIPE,	GOOD,	"SIGPIPE"},
	{SIGALRM,	GOOD,	"SIGALRM"},
	{SIGTERM,	GOOD,	"SIGTERM"},
	{SIGUSR1,	GOOD,	"SIGUSR1"},
	{SIGUSR2,	GOOD,	"SIGUSR2"},
};

#define	NSGS	(sizeof(tab)/sizeof(struct acttab))

long	st,kl,kl2,rl;
int		signalled;
jmp_buf	env;

main(argc,argv)
int	argc;
char	*argv[];
{
	int	i,stop;
	int	pid;
	long	die;

	sv_sigset();
	setup("sigset");
	funct("sigrelse");
	abstract("Fork off a child which will do a sighold() and");
	abstract("wait until a predetermined time before releasing.");
	abstract("The parent will send a signal at the child at a time");
	abstract("before the child releases.  The signal should be");
	abstract("deferred until the specified release time.");
	start
		for(i = 0; i < NSGS; ++i){
			signalled = 0;
			st = time((long *)0);
			kl = st + KILL;
			rl = kl + RLSE;
			testing("Fork a child to receive a %s",tab[i].signam);
			start
				if(!(pid = fork())){
					if(tab[i].exp){
						testing("Set up to catch %s.  Then hold %s and",
							tab[i].signam,tab[i].signam); 
						testing("wait for the release time.");
						start
							exit_on(ERROR);
							sigset(tab[i].signo,catchhold);
							exit_on(SEVERR);
							expecting(SUCCESS);
							sighold(tab[i].signo);
							while(time((long *)0) < rl);
							sigrelse(tab[i].signo);
							if(!signalled){
								error("Signal was lost during hold.");
							}
						finis
					}
					else{
						testing("sighold(%s), which is invalid",
							tab[i].signam);
						start
							expecting(EINVAL);
							sighold(tab[i].signo);
							expecting(EINVAL);
							sigrelse(tab[i].signo);
						finis
					}
					done();
				}
				if(tab[i].exp){
					while(time((long *)0) < kl);
					testing("Send %s at child.",tab[i].signam);
					start
						kill(pid,tab[i].signo);
					finis
				}
				else{
					testing("Send no signal, hold should have failed.");
					start
						/*Do nothing*/
					finis
				}
				wait((int *)0);
			finis
		}
	finis
	abstract("This part tests that we can release a hold set by the");
	abstract("signal mechanism when a non-local goto is executed");
	abstract("within a signal catching function.");
	abstract("For all signals listed in the SVID, set the handling");
	abstract("function to a function which will longjmp() back,");
	abstract("bypassing the normal return procedure.  This should");
	abstract("leave the signal's action as SIG_HOLD.  Then use");
	abstract("sigrelse() to release the SIG_HOLD state, after another");
	abstract("signal has been sent.  This should bring us back to the");
	abstract("signal handler after the release, but no sooner.");
	start
		for(i = 0; i < NSGS; ++i){
			if(tab[i].exp){
				signalled = 0;
				st = time((long *)0);
				kl = st + KILL;
				kl2 = kl + KILL2;
				testing("Fork a child to receive %s.",tab[i].signam);
				start
					if(!(pid = fork())){
						preparation("Set %s to call handler",
							tab[i].signam);
						start
							exit_on(ERROR);
							sigset(tab[i].signo,catchjmp);
						finis
						exit_on(ERROR);
						stop = setjmp(env);
						exit_on(SEVERR);
						die = time((long *)0) + 60;
						while(!stop && (time((long *)0) < die));
						if(!stop)
							exit(1);
						while(time((long *)0) < rl);
						testing("Release %s, which was held.",
							tab[i].signam);
						start
							sigrelse(tab[i].signo);
						finis
						if(!signalled){
							error("Signal lost after longjmp.");
						}
						exit(0);
					}
					while(time((long *)0) < kl);
					testing("Send first %s to child.",tab[i].signam);
					start
						kill(pid,tab[i].signo);
					finis
					while(time((long *)0) < kl2);
					testing("Send second %s to child.",tab[i].signam);
					start
						kill(pid,tab[i].signo);
					finis
					wait((int *)0);
				finis
			}
		}
	finis
	done();
/*NOTREACHED*/
}

void	catchhold(sig)
int		sig;
{
	if(time((long *)0) < rl){
		error("Signal acted on before sigrelse() was called.");
	}
	signalled = 1;
}

void	catchjmp(sig)
int		sig;
{
	static	int		beenhere = 0;	/*If we have, don't longjmp()*/
	int		i;

	if(!beenhere){
		st = time((long *)0) + 2;
		rl = kl2 + RLSE;
		if(st > rl){
			rl = st + RLSE;	/*If we have past release time, move it back*/
							/*If it worked, the signal should be pending*/
							/*No matter what*/
		}
		beenhere = 1;
		longjmp(env,1);
	}
	for(i = 0; (i < NSGS) && (tab[i].signo != sig); ++i);
	if((i < NSGS) && (time((long *)0) < rl)){
		error("%s acted on before sigrelse() was called.",tab[i].signam);
	}
	beenhere = 0;
	signalled = 1;
}

#else

main(argc,argv)
int		argc;
char	*argv[];
{
	setup("sigset");
	funct("sigrelse");
	unsupported("sigrelse(BA_OS) is not supported in System V release 2");
	done();
}

#endif
