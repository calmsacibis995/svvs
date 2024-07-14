#include	<sv_base.h> /*replacement macros */
#ifndef	SYSVR_2
#include	<sys/types.h>
#endif
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	BUFLEN		100
# define	BUFFSIZE	BUFLEN*sizeof(short)
# define	ONE_TO_ONE	0177777
# define	PAIR		077777
# define	ALL_INS		02
# define	PR_OFF1	00
# define	PR_OFF2	01
# define	WAITTIME	5

#ident	"@(#)profil.c	1.7"



/*
** profil
**	Test the execution time profile function.
*/

main()
{
#ifdef PROFIL
	long		i;
	short		buf[BUFLEN];
	short		sv_buf1[BUFLEN];
	short		sv_buf2[BUFLEN];
	short		allzero[BUFLEN];
	short		cr_clk1;
	short		cr_clk2;
	short		cr_clk3;
	int		process_id;
	ttime		t;

	extern	void	profil();
	void	profile_subroutine();
	ttime	time();

setup("profil");
funct("profil");

/*DEBUG message("PR_OFFSET = %d %d",main,PR_OFFSET);  */
testing("profil with a scale of 2. Run the program for a while");
testing("with periodic sampling of the first entry in the buffer.");
testing("Turn off profiling at the end.");
testing("The sample values should increase consistently and no other");
testing("part of the buffer should be updated.");
start
	expecting (VOID);
	for (i = 0; i < BUFLEN; i++)
		allzero [i] = buf[i] = 0;

	profil((char *)buf, BUFFSIZE, main, ALL_INS);
	cr_clk1 = buf[0];

	t = time((ttime *)0) + 10;
	while(t > time((ttime *)0));
	cr_clk2 = buf[0];

	t = time((ttime *)0) + 10;
	while(t > time((ttime *)0));
	cr_clk3 = buf[0];

	/*
	** Turn profiling off now.
	*/
	profil((char *)buf, 0, main, PR_OFF1);
/*DEBUG message("clocks = %d %d %d",cr_clk3, cr_clk2, cr_clk1);  */

	if ( cr_clk3 <= cr_clk2 || cr_clk2 <= cr_clk1 )
		error ("Expected the core clock values in the first entry\n\
 of the buffer to consistently increase.");
	
	for ( i = 1; i < BUFLEN; i++ )
	{
		if ( buf[i] != 0 )
		{
			error ("%s\n%s\nbuf [%d] = %d",
		"Buffer entries other than the first entry were incremented",
		"in profile of all instructions (scale = 2).",
		i, buf[i]);
			break;
		}
	}
finis

testing("profil with a scale of 0177777. Run a program");
testing("and save the buffer.  Run the same program again and");
testing("save this buffer also. Turn off profiling at the end.");
testing("The same locations should be used and the values of all");
testing("be increasing.");
start
	expecting (VOID);
	/*
	** Initialize all the buffers.
	*/
	for (i = 0; i < BUFLEN; i++)
		sv_buf1[i] = sv_buf2[i] = buf[i] = 0;

	/*
	** Turn on profiling.
	*/
	profil((char *)buf, BUFFSIZE, profile_subroutine, ONE_TO_ONE);
	profile_subroutine(buf);

	/*
	** Save the contents of the first buffer.
	*/
	for ( i = 0; i < BUFLEN; i++ )
		sv_buf1[i] = buf[i];

	profile_subroutine(buf);

	/*
	** Turn profiling off.
	*/
	profil((char *)buf, 0, main, PR_OFF1);

	/*
	** Save the contents of the second buffer.
	*/
	for ( i = 0; i < BUFLEN; i++ )
		sv_buf2[i] = buf[i];

	if (!memcmp (buf, allzero, BUFFSIZE))
		warning ("no entries in buf were incremented");
	else
	{
		for ( i = 0; i < BUFLEN; i++ )
		{
			if ( sv_buf1[i] > sv_buf2[i] )
			{
				error ("%s\n%s\nsv_buf1 [%d] = %d, buf2 = %d",
			"Buffer entries should be increasing from buffer one",
			"to buffer two.(scale = 2).",
			i, sv_buf1 [i], sv_buf2[i]);
				break;
			}
	/*DEBUG if ( sv_buf1[i] < sv_buf2[i] )  */
	/*DEBUG message("field %d was increased from %d to %d",i,sv_buf1[i], sv_buf2[i]);  */
	/*DEBUG if ( sv_buf1[i] != 0  )  */
	/*DEBUG message("field %d is still %d, %d", i,sv_buf1[i], sv_buf2[i]);  */
		}
	}
finis


testing("profil with a scale of 077777. Run a program ");
testing("and save the buffer.  Run the same program again and");
testing("save this buffer also. Turn off profiling at the end.");
testing("The same locations should be used and the values to all ");
testing("be increasing.");
start
	expecting (VOID);
	/*
	** Initialize all the buffers.
	*/
	for (i = 0; i < BUFLEN; i++)
		sv_buf2[i] = sv_buf1[i] = buf[i] = 0;

	/*
	** Turn on profiling.
	*/
	profil((char *)buf, BUFFSIZE, profile_subroutine, PAIR);
	profile_subroutine(buf);

	/*
	** Save the contents of the first buffer.
	*/
	for ( i = 0; i < BUFLEN; i++ )
		sv_buf1[i] = buf[i];

	profile_subroutine(buf);

	/*
	** Turn profiling off.
	*/
	profil((char *)buf, 0, main, PR_OFF1);

	/*
	** Save the contents of the second buffer.
	*/
	for ( i = 0; i < BUFLEN; i++ )
		sv_buf2[i] = buf[i];

	if (!memcmp (buf, allzero, BUFFSIZE))
		warning ("no entries in buf were incremented");
	else
	{
		for ( i = 0; i < BUFLEN; i++ )
		{
			if ( sv_buf1[i] > sv_buf2[i] )
			{
				error ("%s\n%s\nsv_buf1 [%d] = %d, buf2 = %d",
				  "Buffer entries should be increasing from",
				  "buffer one to buffer two.(scale = 2).",
				  i, sv_buf1[i], sv_buf2 [i]);
				break;
			}
	/*DEBUG if ( sv_buf1[i] < sv_buf2[i] )  */
	/*DEBUG message("field %d was increased from %d to %d",i,sv_buf1[i], sv_buf2[i]);  */
	/*DEBUG if ( sv_buf1[i] != 0  )  */
	/*DEBUG message("field %d is still %d, %d", i,sv_buf1[i], sv_buf2[i]);  */
		}
	}
finis

#ifndef SYSVR_2

testing("Turn off profiling with a scale of 0 or 1.");
testing("Both 0 and 1 should turn profiling off.");
#else
testing("Turn off profiling with a scale of 0.");
#endif

start
	expecting (VOID);
	/*
	** Turn on profiling. Then call a subroutine
	** and after it returns turn off profiling
	** using scale=0.  Next call the subroutine
	** again.  After it returns, make sure that 
	** the contents of buf are exactly the same
	** as what was saved.
	*/

	/*
	** Initialize all the buffers.
	*/
	for (i = 0; i < BUFLEN; i++)
		buf[i] = sv_buf1[i] = 0;

	profil((char *)buf, BUFFSIZE, profile_subroutine, PAIR);
	profile_subroutine(buf);

	profil((char *)buf, BUFFSIZE, profile_subroutine, PR_OFF1);

	/*
	** Save the contents of the buffer.
	*/
	for ( i = 0; i < BUFLEN; i++ )
		sv_buf1[i] = buf[i];

	profile_subroutine(buf);

	if (!memcmp (buf, allzero, BUFFSIZE))
		warning ("no entries in buf were incremented");
	else
	{
		for ( i = 0; i < BUFLEN; i++ )
		{
			if ( sv_buf1[i] != buf[i] )
			{
				error ("%s\n%s\nsv_buf1 [%d] = %d, buf = %d",
			"Buffer entries should be exactly the same as",
			"they were right after profiling was turned off (scale = 0).",
			i, sv_buf1[i], buf[i]);
				break;
			}
	/*DEBUG if ( sv_buf1[i] != 0  )  */
	/*DEBUG message("field %d is still %d, %d", i,sv_buf1[i], buf[i]);  */
		}
	}

#ifndef	SYSVR_2		/*  SKIP THIS TEST FOR VERSION 2  */

	/*
	** Turn on profiling. Then call a subroutine
	** and after it returns turn off profiling
	** using scale=1.  Next call the subroutine
	** again.  After it returns, make sure that 
	** the contents of buf are exactly the same
	** as what was saved.
	*/

	/*
	** Initialize all the buffers.
	*/
	for (i = 0; i < BUFLEN; i++)
		buf[i] = sv_buf1[i] = 0;

	profil((char *)buf, BUFFSIZE, profile_subroutine, PAIR);
	profile_subroutine(buf);

	profil((char *)buf, BUFFSIZE, profile_subroutine, PR_OFF2);

	/*
	** Save the contents of the buffer.
	*/
	for ( i = 0; i < BUFLEN; i++ )
		sv_buf1[i] = buf[i];

	profile_subroutine(buf);

	if (!memcmp (buf, allzero, BUFFSIZE))
		warning ("no entries in buf were incremented");
	else
	{
		for ( i = 0; i < BUFLEN; i++ )
		{
			if ( sv_buf1[i] != buf[i] )
			{
				warning ("%s\n%s\n%s\nsv_buf1 [%d] = %d, buf = %d",
			ADVISORY,
			"Buffer entries should be exactly the same as",
			"they were right after profiling was turned off (scale = 1).",
			i, sv_buf1[i], buf[i]);
				break;
			}
	/*DEBUG if ( sv_buf1[i] != 0  )  */
	/*DEBUG message("field %d is still %d, %d", i,sv_buf1[i], buf[i]);  */
		}
	}

#endif

finis


testing("Use profil with a bufsiz of 0.");
testing("Profiling should not be operational.");
start
	expecting (VOID);
	/*
	** Initialize all the buffers.
	*/
	for (i = 0; i < BUFLEN; i++)
		buf[i] = 0;

	profil((char *)buf, 0, main, PAIR);
	profile_subroutine(buf);

	if (memcmp (buf, allzero, BUFFSIZE))
		error ("the buffer was incremented, profiling was not disabled");

finis


testing("Turn profiling on in the current process and create a child.");
testing("The child process should inherit profiling.");
start
	expecting (VOID);
	profil((char *)buf, BUFFSIZE, main, ALL_INS);
	process_id = fork();
	if ( process_id == -1 )
		error ("Could not fork from profil()");

	if ( process_id == 0 )
	{
		/* CHILD process */

		buf[0] = 0;
		profile_subroutine(buf);
		if ( !buf[0] )
		{
			error ("Child process did not retain profiling");
		}
		exit(0);
/*NOTREACHED*/
	}
	else
	{
		if ( wait_for(process_id, WAITTIME) )
			error ("The child process terminated abnormally.");
	}
	profil((char *)buf, 0, main, PR_OFF2);
finis

done();
/*NOTREACHED*/

}
/*
** profile_subroutine(buf)
**	This is a dummy subroutine that just performs a loop,
**	returning when any number in the profiling array increases,
**	or 10 seconds elapses.
*/
void
profile_subroutine(buf)
short *buf;
{
	ttime	t;
	int	i;
	ttime	time();
	short	my_buf[BUFLEN];
	t = time((ttime *)0) + 10;
	for(i=0;i<BUFLEN;i++)
		my_buf[i]=buf[i];
	while(t > time((ttime *)0))
		for(i=0;i<BUFLEN;i++)
			if(my_buf[i]!=buf[i])
/*DEBUG {message("wait over - buf [%d] incremented from %d to %d",i,my_buf[i],buf[i]);  */
				return;
/*DEBUG }  */
/*DEBUG warning ("subroutine profile_subroutine timed out\n\  
before any profile buffers were incremented");  */
/*DEBUG this is OK - should time out for TURNED OFF test  */

#else
	void exit();
	
unsupported("Execution time profiling is not supported.");
	exit(0);
/*NOTREACHED*/


#endif
}
