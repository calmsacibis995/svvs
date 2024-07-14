#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<sys/utsname.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)uname.c	1.3"

/*
** uname
**	Test the uname function.
**	The values for uname are machine dependant and can
**	be found in a configuration file.
**	See installation documentation for details.
*/

main()
{
	struct	utsname name;
	int	uname();

	setup("uname");

	funct("uname");

	exit_on ( ERROR );

	abstract("A legal invocation of uname.");
	abstract("Expect a  return value of >0.");
	start

		testing("Issue the call to uname.");
		start
			expecting ( SUCCESS );
			uname(&name);

		finis

		testing("Validate sysname field.");
		start
			expecting ( VOID );
			if ( strcmp(name.sysname, SYSNAME) != 0)
			{
				error("Value for sysname returned by uname does not match SYSNAME.\nThe expected string was %s.\nThe actual string was %s.\n", SYSNAME, name.sysname);	/* ERROR */
			}
		finis

		testing("Validate nodename field.");
		start

			expecting ( VOID );
			if ( strcmp(name.nodename, NODENAME) != 0)
			{
				error("Value for nodename returned by uname does not match NODENAME.\nThe expected string was %s.\nThe actual string was %s.\n", NODENAME, name.nodename);	/* ERROR */
			}
		finis

		testing("Validate release field.");
		start

			expecting ( VOID );
			if ( strcmp(name.release, RELEASE) != 0)
			{
				error("Value for release returned by uname does not match RELEASE.\nThe expected string was %s.\nThe actual string was %s.\n", RELEASE, name.release);	/* ERROR */
			}
		finis

		testing("Validate version field.");
		start

			expecting ( VOID );
			if ( strcmp(name.version, VERSION) != 0)
			{
				error("Value for version returned by uname does not match VERSION.\nThe expected string was %s.\nThe actual string was %s.\n", VERSION, name.version);	/* ERROR */
			}
		finis

		testing("Validate machine field.");
		start

			expecting ( VOID );
			if ( strcmp(name.machine, MACHINE) != 0)
			{
				error("Value for machine returned by uname does not match MACHINE.\nThe expected string was %s.\nThe actual string was %s.\n", MACHINE, name.machine);	/* ERROR */
			}

		finis

	finis

	done();
	/*NOTREACHED*/
}
