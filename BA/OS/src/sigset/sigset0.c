#ident	"@(#)sigset0.c	1.4"

#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
#ifndef	SYSVR_2
int				sig_dfl();
int				sig_ign();
int				sig_hold();
int				sig_catch();
extern	long	time();
void			catch();

#define	GOOD	1
#define	BAD		0
#define	TIMEOUT	30	/*Tests time out in 30 seconds*/
#define	KILL	5	/*Send signal 5 seconds after fork() to break pause*/
#define	REL		5	/*Release 5 seconds after signal is sent*/

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

struct	action	{
	char	*name;
	int		(*func)();
};

struct	action	acts[] = {
	{	"SIG_DFL",		sig_dfl		},
	{	"SIG_IGN",		sig_ign		},
	{	"SIG_HOLD",		sig_hold	},
	{	"catch signal",	sig_catch	},
};

#define	NACTS	(sizeof(acts)/sizeof(struct	action))

static	int	sigd = 0;
static	int	cursig = 0;

main(argc,argv)
int	argc;
char	*argv[];
{
	int	i,j;

	sv_sigset();
	setup("sigset");
	funct("sigset");
#ifndef ERIC
	abstract("The following tests the sigset(BA_OS) function");
	abstract("by touring through all of the possible signals");
	abstract("and all of the possible actions for a signal.");
	start
		for(i = 0; i < NACTS; ++i){
			testing("All signals for the action: %s",acts[i].name);
			start
				for(j = 0; j < NSGS; ++j){
					(*acts[i].func)(&tab[j]);
				}
			finis
		}
	finis
#endif
	abstract("The following tests the return values from sigset(BA_OS)");
	abstract("which should be equal to the previous function.  The error");
	abstract("return has been adequately tested in the previous cases");
	abstract("but the valid returns have so far been ignored.");
	start
		for(i = 0; i < NSGS; ++i){
			if(tab[i].exp){
				rettest(&tab[i]);
			}
		}
	finis
	done();
}

int		sig_dfl(sig)
struct	acttab	*sig;
{
	long	die,kl,st;
	int		pid;

	testing("%s using SIG_DFL as the action for the signal.",sig->signam);
	start
		st = time((long *)0);
		die = st + TIMEOUT;
		kl = st + KILL;
		pid = fork();
		if(!pid){
			if(sig->exp){
				testing("sigset(%s,SIG_DFL) and wait",sig->signam);
				testing("to be signalled.  If timeout comes first");
				testing("issue an error and exit.");
				start
					sigset(sig->signo,SIG_DFL);
					while(time((long *)0) < die);
					error("Signal never received by child.");
				finis
				exit(-1);
			}
			else{
				testing("sigset of %s, an invalid",sig->signam);
				testing("signal.");
				start
					expecting(EINVAL);
					sigset(sig->signo,SIG_DFL);
				finis
				exit(0);
			}
		}
		else{
			if(sig->exp){
				while(time((long *)0) < kl);
				testing("Kill child with %s",sig->signam);
				start
					kill(pid,sig->signo);
				finis
			}
			else{
				testing("Do not kill child, since sigset should have");
				testing("failed.");
				start
					/*Do nothing*/
				finis
			}
			wait((int *)0);
		}
	finis
}

int				sig_ign(sig)
struct	acttab	*sig;
{
	long	rel,kl,st;
	int		pid;

	testing("%s using SIG_IGN as the action for the signal.",sig->signam);
	start
		st = time((long *)0);
		kl = st + KILL;
		rel = kl + REL;
		pid = fork();
		if(!pid){
			if(sig->exp){
				testing("sigset(%s,SIG_IGN) and wait",sig->signam);
				testing("for agreed upon release time.  Signals");
				testing("should be ignored until that time.");
				start
					sigset(sig->signo,SIG_IGN);
				finis
					while(time((long *)0) < rel);	
				exit(0);
			}
			else{
				testing("sigset of %s, an invalid",sig->signam);
				testing("signal.");
				start
					expecting(EINVAL);
					sigset(sig->signo,SIG_IGN);
				finis
				exit(0);
			}
		}
		else{
			if(sig->exp){
				while(time((long *)0) < kl);
				testing("Kill child with %s and wait",sig->signam);
				testing("for it to exit.  Child should not exit");
				testing("until after the agreed upon release time.");
				start
					kill(pid,sig->signo);
 					wait((int *)0);
					if(time((long *)0) < rel){
						error("SIG_IGN failed to ignore the signal");
					}
				finis
			}
			else{
				testing("Do not kill child, since sigset should have");
				testing("failed.");
				start
					/*Do nothing*/
				finis
				wait((long *)0);
			}
		}
	finis
}

int		sig_hold(sig)
struct	acttab	*sig;
{
	long	kl,rel,die;
	int		pid;

	testing("%s using SIG_HOLD as the action for the signal.",sig->signam);
	start
		kl = time((long *)0) + KILL;
		rel = kl + REL;
		die = rel + TIMEOUT;
		pid = fork();
		if(!pid){
			if(sig->exp){
				preparation("Set SIG_DFL for  %s",sig->signam);
				preparation("to ensure a known starting state.");
				start
					sigset(sig->signo,SIG_DFL);
				finis
				testing("Set SIG_HOLD on %s and wait",sig->signam);
				testing("for release time.");
				start
					sigset(sig->signo,SIG_HOLD);
				finis
				while(time((long *)0) < rel);
				testing("Release the signal, set it back to SIG_DFL.");
				start
					sigset(sig->signo,SIG_DFL);
				finis
				testing("Wait for released signal to take effect.");
				start
					while(time((long *)0) < die);
					error("Signal not released.");
				finis
			}
			else{
				testing("Use sigset to set %s to SIG_HOLD",sig->signam);
				testing("%s is an invalid signal.",sig->signam);
				start
					expecting(EINVAL);
					sigset(sig->signo,SIG_HOLD);
				finis
			}
			exit(0);
		}
		if(sig->exp){
			while(time((long *)0) < kl);
			testing("Send first signal to child, it should be held.");
			start
				kill(pid,sig->signo);
			finis
			testing("Wait for child, it should not exit until the");
			testing("release time.");
			start
				wait((int *)0);
				if(time((long *)0) < rel){
					error("Child exited too early, signals did not hold.");
				}
			finis
		}
		else{
			testing("Do not kill child, sigset should have failed");
			start
				/*Do nothing*/
			finis
			wait((int *)0);
		}
	finis
}

static	long	kl1,kl2,die,rel;
static	int		resigd = 0;

int				sig_catch(sig)
struct	acttab	*sig;
{
	int		pid;

	testing("%s using a user defined signal catcher.",sig->signam);
	start
		kl1 = time((long *)0) + KILL;
		kl2 = kl1 + KILL;
		rel = kl2 + REL;
		die = kl2 + TIMEOUT;
		pid = fork();
		if(!pid){
			if(sig->exp){
				testing("Call sigset to set action to a signal catching");
				testing("function for %s.",sig->signam);
				start
					sigset(sig->signo,catch);
					sigd = 0;
					resigd = 0;
					cursig = sig->signo;
					while((time((long *)0) < die) && !(sigd && resigd));
					if(!sigd){
						error("Signal not handled.");
					}
					if(!resigd){
						error("No signals released after the handler.");
					}
				finis
			}
			else{
				testing("call sigset to set action to a signal catching");
				testing("function for %s, an invalid signal",sig->signam);
				start
					expecting(EINVAL);
					sigset(sig->signo,catch);
				finis
			}
			exit(0);
		}
		if(sig->exp){
			while(time((long *)0) < kl1);
			testing("Send one signal to child to invoke the signal");
			testing("handler.");
			start
				kill(pid,sig->signo);
			finis
			while(time((long *)0) < (kl2));
			testing("Send another signal to child to test SIG_HOLD");
			testing("inside the signal handler.");
			start
					kill(pid,sig->signo);
			finis
			testing("Wait for child to exit, this should not occur");
			testing("before the release time.");
			start
				wait((int *)0);
				if(time((long *)0) < rel){
					error("Child aborted unexpectedly on signal.");
				}
			finis
		}
		else{
			testing("Do not kill child since sigset should have failed");
			start
				/*Do nothing*/
			finis
			wait((int *)0);
		}
	finis
}

rettest(sig)
struct	acttab	*sig;
{
	void	(*orig)();

	preparation("Fork a child to do this test, in order to prevent");
	preparation("corruption of the SVVS signal handling systrem.");
	start
		if(!fork()){
			preparation("Set action for %s to SIG_DFL to",sig->signam);
			preparation("begin the test in a known state.");
			start
				orig = sigset(sig->signo,SIG_DFL);
			finis
			testing("Change action to SIG_IGN. Value returned should be");
			testing("SIG_DFL.");
			start
				expecting(SIG_DFL);
				sigset(sig->signo,SIG_IGN);
			finis
			testing("Change action to SIG_HOLD. Value returned should be");
			testing("SIG_IGN.");
			start
				expecting(SIG_IGN);
				sigset(sig->signo,SIG_HOLD);
			finis
			testing("Change action to address, Value returned should be");
			testing("SIG_HOLD.");
			start
				expecting(SIG_HOLD);
				sigset(sig->signo,catch);
			finis
			testing("Return to the original action, the value returned");
			testing("should be the previous address.");
			start
				expecting(catch);
				sigset(sig->signo,orig);
			finis
			exit(0);
		}
		wait((int *)0);
	finis
}

void	catch(sig)
int	sig;
{
	testing("Signal handler should have been called with the");
	testing("signal number of the received signal.  Check whether");
	testing("it is the one we expect.");
	start
		if(sig != cursig){
			error("Signal handler called with wrong signal");
		}
	finis
	if(sigd){
		if(time((long *)0) < rel){
			error("Signal handler did not hold, re-entry encountered.");
		}
		else{
			resigd = 1;
		}
	}
	else{
		sigd = 1;
		testing("Signal handler is supposed to set SIG_HOLD until");
		testing("it returns.  Wait inside signal handler.  Parent will");
		testing("send another signal, which should not cause the signal");
		testing("handler to re-enter.");
		start
			while(time((long *)0) < rel);
		finis
	}
}
#else
main(argc,argv)
int		argc;
char	*argv[];
{
	setup("sigset");
	funct("sigset");
	unsupported("sigset(BA_OS) not supported on System V Release 2");
	done();
}
#endif
