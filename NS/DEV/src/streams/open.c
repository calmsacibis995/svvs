#ident "@(#)open.c	1.3"

#include <stdio.h>      
#include <fcntl.h>
#include <sv_base.h>
#include <sv_nse.h>
#include <svvs.h>       
#include <sv_env.h>
#include <sv_macro.h>
#include <sys/stropts.h>
#include <sv_streams.h>
#include <string.h>
#include <values.h>

char	*flag_nms[] = {"O_RDONLY", "O_RDONLY|O_NDELAY", "O_WRONLY",
			"O_WRONLY|O_NDELAY", "O_RDWR", "O_RDWR|O_NDELAY" };

int	flags[] = { O_RDONLY, O_RDONLY|O_NDELAY, O_WRONLY, 
			O_WRONLY|O_NDELAY, O_RDWR, O_RDWR|O_NDELAY };

main() {
	register	int	i,	fd;
	setup("streams");

	abstract("Verify the streams based loop back driver \"%s\" can",DRIVER);
	abstract("be opened with oflag set to O_RDONLY, O_RDONLY|O_NDELAY,");
	abstract("O_RDWR, O_RDWR|O_NDELAY, O_WRONLY, and O_WRONLY|O_NDELAY.");
	start
		for(i=0; i < (sizeof(flags)/sizeof(int)); i++) {
			testing("Call open with an oflag of %s.",flag_nms[i]);
			start     
				continue_on( ERROR );
				fd = open(DRIVER,  flags[i]);
			finis

			cleanup("Close the opened file.");
			start     
				close( fd );
			finis      
		}
	finis  
	done();
	/* NOTREACHED */
}
