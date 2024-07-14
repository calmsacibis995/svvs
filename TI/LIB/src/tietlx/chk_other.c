# include "main.h"
# include "finish.h"
# include <signal.h>
# include <setjmp.h>

SCCSID("@(#)chk_other.c	3.3 (SVID) 12/30/86");

FINISH_TYPE	Finish = FINISH_BY_OTHER;
static jmp_buf	Environ;

extern unsigned	alarm();

void	chk_pitcher();
void	chk_catcher();
void	test_over();
BOOLEAN	test_start();

void chk_partner(rounds)
int	*rounds;
{
	if (++(*rounds) == EXAM_ROUNDS)
	{
		*rounds = 0;

		switch (SideType)
		{
		  case SIDE_PITCHER:
			chk_catcher();
			break;

		  case SIDE_CATCHER:
			chk_pitcher();
			break;

		  default:
			screech("chk_partner", 0, 
				"Bad SideType = %d\n", SideType);
		}
	}
}

void chk_catcher()
{
	if (getppid() != PID_Other)
	{
		Finish = FINISH_BY_SELF;
		screech(NULL, 0, "Catcher process killed\n");
	}
}

void chk_pitcher()
{
	BOOLEAN	result;

	if (signal(SIGINT, SIG_IGN) == SIG_IGN)
	{
		result = test_start();
	}
	else
	{
		result = test_start();
		(void) signal(SIGINT, handler);
	}

	if (result)
	{
		Finish = FINISH_BY_SELF;
		screech(NULL, 0, "Pitcher process killed\n");
	}
}

BOOLEAN test_start()
{
	if (!setjmp(Environ))
	{
		(void) signal(SIGALRM, test_over);
		(void) alarm(CHK_TEST_LEN);
		(void) wait((int *) 0);
		return(TRUE);
	}
	else
	{
		(void) signal(SIGALRM, SIG_IGN);
		return(FALSE);
	}
}

void test_over()
{
	(void) signal(SIGALRM, SIG_IGN);
	longjmp(Environ, 1);
}
