#include	<sv_base.h> /*replacement macros */
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)ssignal.c	1.3"



int	handler();		/* Signal handler */
int	Cur_sig = -1;		/* Current signal being sent */
int	From_handler;		/* Value the signal handler is returing */
int	Expected;		/* Are we expecting to enter the handler? */

# define	ILLSIG	-1234	/* an illegal signal number */

/*
** ssignal
**	Test the software signal handler.
*/
main()
{
setup("ssignal");
funct("ssignal");
funct("gsignal");
/*
**  The functions ssignal/gsignal are used, not signal
*/

abstract ("Test that the default settings are all correct.");
start
	t_default();
finis

abstract ("Test all normal signals.");
start
	t_signals();
finis

abstract ("Test that ssignal(sig,SIG_IGN) works correctly.");
start
	t_sigign();
finis

abstract ("Test that ssignal(sig,SIG_DFL) works correctly.");
start
	t_sigdfl();
finis

abstract ("Test an illegal signal.");
start
	t_illsignal();
finis


done();
/*NOTREACHED*/
}

/*
** handler
**	The signal handler. We check that: 
**		The correct signal is passed to us
**		We were expected to be called
**	Then we set our return value to a global,
**	then return it.
**
** Arguments
**	sig	The signal number we were called with.
*/
handler(sig)
int	sig;
{
	static	int	cur_val = 0;	/* Current value for the handler to return */
	if ( Expected  == 0 )
		error("signal handler unexpectedly called");

	if ( Expected == -1 )
		error("signal handler has recursively called itself!");
	Expected = -1;
	if ( sig != Cur_sig )
		error("signal handler was not sent the correct signal\n\
signal = %d, expecting %d",
			sig, Cur_sig);

	cur_val++;
	From_handler = cur_val;	
	return ( cur_val );
}

/*
** t_default
**	Test that the default settings are all correct.
**
** Level
**	This routine is in the normal level (0).
*/
t_default()
{
	int	ret_val;
	int	i;

testing("the default settings for the software signals.");
testing("A return of 0 from gsignal for each signal.");
start
	for(i=1;i<16;i++)
	{
		expecting (0);
		gsignal(i);
	}
finis
}

/*
** t_signals
**	Test all normal signals
**
** Level
**	This routine is in the normal level (0).
*/
t_signals()
{
	int	ret_val;
	int	i;
testing("Change action to an 'action function', then raise the signal,");
testing("then raise the signal again.");
testing("ssignal should return SIG_DFL, gsignal to return the value");
testing("the 'action function' returned, then the next call to gsignal");
testing("should return 0.");
start
	for( i = 1 ; i < 16; i++ )	
	{
		expecting (SIG_DFL);
		ssignal(i,handler);

		Expected = 1;
		Cur_sig = i;

		/*
		 * I cannot use "expecting From_handler" because
		 * From_handler is not set until gsignal returns!
		 */
		expecting (VOID);
		if ( (ret_val = gsignal(i)) != From_handler )
		{
			error("gsignal() returned %d, expecting %d",
				ret_val, From_handler);
			break;
		}

		Expected = 0;
		expecting (VOID);  /*  give a more detailed error message  */
		if ((ret_val = gsignal(i)) != 0 )
		{
			error("gsignal() returned %d after a previous call \
to gsignal",
				ret_val);
			break;
		}

		expecting (SIG_DFL);
		ssignal(i,SIG_DFL);
	}
finis
}

/*
** t_sigign
**	Test that ssignal(sig,SIG_IGN) works correctly.
**
** Level
**	This routine is in the boundary level (1).
*/
t_sigign()
{
	int	ret_val;
	int	i;

testing("SIG_IGN: Set all signals to SIG_IGN and raise each signal.");
testing("This should return SIG_DFL from the first call to ssignal(),");
testing("a return of 1 from gsignal().");
start
	for( i = 1; i < 16; i++ )
	{	
		expecting (SIG_DFL);
		ssignal(i,SIG_IGN);

		expecting (1);
		gsignal(i);

		expecting (SIG_IGN);
		ssignal(i,SIG_IGN);
	}
finis
}

/*
** t_sigdfl
**	Test that ssignal(sig,SIG_DFL) works correctly.
**
** Level
**	This routine is in the boundary level (1).
*/
t_sigdfl()
{
	int	ret_val;
	int	i;
testing("SIG_DFL: Set all signals to SIG_DFL, then raise each signal.");
testing("A return of SIG_IGN from the first call to ssignal(),");
testing("a return of 1 from gsignal().");
start
	for( i = 1; i < 16; i++)
	{
		expecting (SIG_IGN);
		ssignal(i,SIG_DFL);

		expecting (0);
		gsignal(i);

		expecting (SIG_DFL);
		ssignal(i,SIG_DFL);
	}
finis
}

/*
** t_illsignal
**	Test an illegal signal.
*/
t_illsignal()
{
	int	ret_val;

testing("Setting an illegal signal.");
testing("A return value of SIG_DFL.");
start
	expecting (SIG_DFL);
	ssignal(ILLSIG,SIG_IGN);
finis

testing("Raising an illegal signal.");
testing("A return of 0 from gsignal.");
start
	expecting (0);
	gsignal(ILLSIG);
finis
}
