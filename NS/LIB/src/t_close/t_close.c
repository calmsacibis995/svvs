#ident "@(#)t_close.c	1.1"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

main()
{
	int	tfd;


	setup("t_close");
	funct("t_close");

	abstract("Close a valid service provider.");
	abstract("Check that the state of the closed provider is T_UNINIT.");
	start    
		exit_on( ERROR );

		prep("Call t_open(%s, %s, %s)", TPVCFILE0, "O_RDONLY", NULL);
		start    
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis   

		testing("That t_close successfully returns. ");
		start    
			t_close( tfd );
		finis   

		prep("Call t_getstate() to get the state of the closed service ");
		prep("provider. Expecting failure and t_errno set to TBADF. ");
		start    
			break_on( ERROR );
			expecting( TBADF );
			t_getstate(tfd);
		finis   
	finis   



	abstract("Test that t_close returns TBADF if a file was opened ");
	abstract("that was not a service provider. Do this by opening ");
	abstract("the file /dev/console and using t_close to close it. ");
	start     
		prep("Call open(BA_OS) to open /dev/console. ");
		start    
			exit_on( ERROR );
			tfd = open("/dev/console", O_WRONLY);
		finis    

		testing("Use t_close to close /dev/console. Expecting ");
		testing("failure and t_errno = TBADF.");
		start    
			expecting ( TBADF );
			t_close( tfd );
		finis    

		cleanup("Use close(BA_OS) to close file /dev/console");
		start   
			close(tfd);
		finis   
	finis   

	done();
}

