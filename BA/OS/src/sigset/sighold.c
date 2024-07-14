/*
**EMACS_MODES: tabstop=4
*/
#ident	"@(#)sighold.c	1.3"
#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2

extern	long	time();

#define	GOOD	1
#define	BAD		0
#define	KILL	3	/*Send signal 3 seconds after fork()*/
#define	RLSE	5	/*Release hold 5 seconds after fork()*/

void	catchhold();	/*Signal catcher for sigrelse() test*/

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

long	st,kl,rl;
int		signalled;

main(argc,argv)
int	argc;
char	*argv[];
{
	int	i;
	int	pid;

	sv_sigset();
	setup("sighold");
	funct("sighold");
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
			rl = st + RLSE;
			testing("Fork a child to receive a %s",tab[i].signam);
			start
				if(!(pid = fork())){
					if(tab[i].exp){
						testing("Set up to catch %s.  Then hold %s and",
							tab[i].signam,tab[i].signam); 
						testing("wait for the release time.");
						start
							sigset(tab[i].signo,catchhold);
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

#else
main(argc,argv)
int		argc;
char	*argv[];
{
	setup("sigset");
	funct("sighold");
	unsupported("sighold(BA_OS) is not supported in System V Release 2");
	done();
}
#endif
