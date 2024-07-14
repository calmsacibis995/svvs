#include	<sv_base.h> /*replacement macros */
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)filsys.c	1.6"

/*
** filsys: file system -- directory tree structure
**	Test the directory tree structure i.e., test that 
**      /etc, /tmp, /bin, /usr, /usr/bin, /usr/tmp are present
**      on the system.
**
**	There is no need to test for /dev's existence
**	since DEVCON, DEVNUL and DEVTTY should fail if /dev does not exist
**	and there is no need for duplication of the tests.
**
**	/etc/profile is tested for existence,
**	but /etc/passwd is tested by the 'passwd' test for validity.
*/
main()
{


setup ("filsys");
funct ("file_system");

testing ("chdir to /bin should return Zero (/bin is present).");
start
	expecting (SUCCESS);
	continue_on (ERROR | SEVERR);
	chdir("/bin");
finis


testing ("chdir to /etc should return Zero (/etc is present).");
start
	expecting (SUCCESS);
	continue_on (ERROR | SEVERR);
	chdir("/etc");
finis


testing ("chdir to /tmp should return Zero (/tmp is present).");
start
	expecting (SUCCESS);
	continue_on (ERROR | SEVERR);
	chdir("/tmp");
finis


testing ("chdir to /usr should return Zero (/usr is present).");
start
	expecting (SUCCESS);
	exit_on (ERROR | SEVERR);
	chdir("/usr");


	testing ("chdir to /usr/tmp should return Zero (/usr/tmp is present).");
	start
		expecting (SUCCESS);
		continue_on (ERROR | SEVERR);
		chdir("/usr/tmp");
	finis


	testing ("chdir to /usr/bin should return Zero (/usr/bin is present).");
	start
		expecting (SUCCESS);
		continue_on (ERROR | SEVERR);
		chdir("/usr/bin");
	finis
finis


testing ("open /etc/profile for reading");
start
	expecting (SUCCESS);
	error_on (E_FAILURE);
	open ("/etc/profile", O_RDONLY);
finis

done();
/*NOTREACHED*/
}
