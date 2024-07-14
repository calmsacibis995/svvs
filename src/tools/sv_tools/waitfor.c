/*LINTLIBRARY*/

#include	<signal.h>

#include	<svvs.h>
#include	<sv_env.h>

#ifdef	UNDEF
#ident	"@(#)waitfor.c	1.6"
#endif

static int Childpid = -1;
/*
 * timer
 *	Signal handling routine for wait_for.
 *	If we come in here, we have timed out,
 *	call severr to inform user.
 */
static void timer()
{
	if( Childpid > 0 ) /* possible race condition, don't kill(9,-1)  */
		(void) kill(SIGKILL,Childpid);
	severr("Timeout waiting for a child to exit");
}
/*
** wait_for
**	Wait for a child process.
**
** Returns
**	child process status.
**
** Arguments
**	pid		Process id of the child we are waiting for.
**	timeout		Maximum time (in minutes) to wait.
*/
wait_for(pid,timeout)
int	pid;
int	timeout;
{
	siggy		(*func)();
	auto	int	child;
	int		ret_val;
	void		timer();
	unsigned	alarm();

	if ( timeout < 0 )
		severr("wait_for was handed a negative timeout");
/*
 *	grab the old alarm function
 */
	if((func = (siggy (*)())signal(SIGALRM,timer)) == (siggy (*)())SIG_ERR )
		severr("signal returned SIG_ERR in wait_for");

	(void) alarm( (unsigned) (timeout * WAITMUL));
	Childpid = pid;
	ret_val = wait(&child);
	Childpid = -1;
	/*
	** Turn the alarm back off
	*/
	(void) alarm( (unsigned) 0);
	if ( ret_val == -1 )
		severr("wait_for: wait returned -1");
	if ( ret_val != pid )
		severr("wait_for: unexpected child returned from wait");
	if ((siggy (*)()) signal(SIGALRM,func) == (siggy (*)())SIG_ERR )
		severr("wait_for signal returned SIG_ERR, trying to reset SIGALRM");
	return ( child );
}
