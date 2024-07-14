#ident	"@(#)exec5.c	1.7"
#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_2

static	int	t_execl();
static	int	t_execv();
static	int	t_execle();
static	int	t_execve();
static	int	t_execlp();
static	int	t_execvp();

static	char	*arv[]={
	"exec6",
	(char *)0,
};

static	char	**env;

static	int (*test[])() = {
	t_execl,
	t_execv,
	t_execle,
	t_execve,
	t_execlp,
	t_execvp,
};

#define	NT (sizeof(test) / (sizeof(int (*)())))
# endif

main(argc,argv,envp)
int		argc;
char	*argv[];
char	*envp[];
{
	int	i;

# ifndef SYSVR_2

	sv_sigset();
	setup("exec");
	funct("exec");

	env = envp;
	abstract("For each of the exec functions, set SIG_HOLD");
	abstract("on a signal and exec a test to see if the SIG_HOLD");
	abstract("is preserved.");
	start
		for(i = 0; i < NT; ++i){
			(*test[i])();
		}
	finis
	done();
}

static	int	t_execl()
{
	preparation("Fork a child to do an execl().");
	start
		if(!fork()){
			preparation("Set SIGUSR1 to SIG_HOLD and exec the test.");
			start
				sigset(SIGUSR1,SIG_HOLD);
				execl("exec6","exec6",(char *)0);
				error("exec failed.");
			finis
			exit(0);
		}
		wait((int *)0);
	finis
}

static	int	t_execv()
{
	preparation("Fork a child to do an execv().");
	start
		if(!fork()){
			preparation("Set SIGUSR1 to SIG_HOLD and exec the test.");
			start
				sigset(SIGUSR1,SIG_HOLD);
				execv("exec6",arv);
				error("exec failed.");
			finis
			exit(0);
		}
		wait((int *)0);
	finis
}

static	int	t_execle()
{
	preparation("Fork a child to do an execle().");
	start
		if(!fork()){
			preparation("Set SIGUSR1 to SIG_HOLD and exec the test.");
			start
				sigset(SIGUSR1,SIG_HOLD);
				execle("exec6","exec6",(char *)0,env);
			finis
			exit(0);
		}
		wait((int *)0);
	finis
}

static	int	t_execve()
{
	preparation("Fork a child to do an execve().");
	start
		if(!fork()){
			preparation("Set SIGUSR1 to SIG_HOLD and exec the test.");
			start
				sigset(SIGUSR1,SIG_HOLD);
				execve("exec6",arv,env);
			finis
			exit(0);
		}
		wait((int *)0);
	finis
}

static	int	t_execlp()
{
	preparation("Fork a child to do an execlp().");
	start
		if(!fork()){
			preparation("Set SIGUSR1 to SIG_HOLD and exec the test.");
			start
				sigset(SIGUSR1,SIG_HOLD);
				execlp("exec6","exec6",(char *)0);
				error("exec failed.");
			finis
			exit(0);
		}
		wait((int *)0);
	finis
}

static	int	t_execvp()
{
	preparation("Fork a child to do an execvp().");
	start
		if(!fork()){
			preparation("Set SIGUSR1 to SIG_HOLD and exec the test.");
			start
				sigset(SIGUSR1,SIG_HOLD);
				execvp("exec6",arv);
				error("exec failed.");
			finis
			exit(0);
		}
		wait((int *)0);

	finis
# else
	unsupported("The functionality tested here is unsupported by SVR2");
# endif
}
