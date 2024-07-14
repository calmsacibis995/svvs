/*
**EMACS_MODES: tabstop=4
*/

#ident	"@(#)sigignore.c	1.3"

#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2
extern	long	time();
void			catch();

#define	GOOD	1
#define	BAD		0
#define	START	5		/*Start sending signals 5 seconds after fork()*/
#define	STOP	5		/*Stop sending signals 5 seconds later*/
#define	REL		2		/*Change signal action 2 seconds after stop*/
#define	CNTRL	3		/*Send control signal 3 seconds after REL*/
#define	TIMEOUT	60		/*Give control signal one minute before timeout*/

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

static	int	signalled = 0;

main(argc,argv)
int		argc;
char	*argv[];
{
	long	first,last,rel,cntrl,timeout;
	int		pid;
	int		i;

	sv_sigset();
	setup("sigset");
	funct("sigignore");
	abstract("This test verifies that the sigignore() function acually");
	abstract("causes signals to be ignored.  For each of the signals in");
	abstract("the SVID, we will fork a child which will call sigignore()");
	abstract("and wait until a predetermined time.  Meanwhile, the");
	abstract("parent process will be sending the signal under test at");
	abstract("the child. After a few seconds, the child will");
	abstract("change the signal action to a function which will check");
	abstract("that signals are actually arriving at the child process");
	start
		for(i = 0; i < NSGS; ++i){
			preparation("Fork a child to receive %s",tab[i].signam);
			start
				first = time((long *)0) + START;
				last = first + STOP;
				rel = last + REL;
				cntrl = rel + CNTRL;
				pid = fork();
				if(!pid){
					if(!tab[i].exp){
						testing("Call sigignore with invalid signal");
						testing("%s",tab[i].signam);
						start
							abort_on(ERROR);
							expecting(EINVAL);
							sigignore(tab[i].signo);
						finis
						exit(0);
					}
					testing("Call sigignore with %s and wait for",
						tab[i].signam);
					testing("release time.");
					start
						sigignore(tab[i].signo);
						while(time((long *)0) < rel);
					finis
					testing("Set signal action to test function and");
					testing("wait for control signal.  If the timeout is");
					testing("reached, we can safely assume the signal is");
					testing("not going to arrive.");
					start
						timeout = time((long *)0) + TIMEOUT;
						signalled = 0;
						sigset(tab[i].signo,catch);
						while(!signalled){
							if(time((long *)0) > timeout){
								error("Signal: %s never received at child",
									tab[i].signam);
								break;
							}
						}
					finis					
					exit(0);
				}/*End of child*/
				else{
					if(tab[i].exp){
						while(time((long *)0) < first);
						testing("Blast child with repeated %s until",
							tab[i].signam);
						testing("it is time to stop.");
						start
							while(time((long *)0) < last){
								kill(pid,tab[i].signo);
							}
						finis
						while(time((long *)0) < cntrl);
						testing("Send control %s to child to be",
							tab[i].signam);
						testing("sure that the signals are getting there");
						start
							kill(pid,tab[i].signo);
						finis
					}
					else{
						testing("Do not send %s to child, sigignore()",
							tab[i].signam);
						testing("should have failed.");
						start
							/*Do nothing*/
						finis
					}
				}
				wait((int *)0);
			finis
		}
	finis
	done();
}

void	catch(sig)
int		sig;
{
	signalled = 1;
}

#else

main(argc,argv)
int		argc;
char	*argv[];
{
	setup("sigset");
	funct("sigignore");
	unsupported("sigignore(BA_OS) is not supported in System V release 2");
	done();
}
#endif
