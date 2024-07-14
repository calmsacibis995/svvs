#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<ustat.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)ustat.c	1.2"


/*
** ustat
**	Test the ustat function.
**	The values for ustat are machine dependent and can
**	be found in limits.h
*/

main()
{
	struct	ustat	buf;
	struct	stat	dev;
	int stat();
	int ustat();

	setup("ustat");

	funct("ustat");

	exit_on ( ERROR );

	abstract("A legal invocation of ustat.");
	abstract("Expect a  return value of 0.");
	start

		prep("Call stat to obtain info for ustat.");
		start
			expecting ( SUCCESS );
			stat(SVVS_ROOT, &dev);
		finis

		testing("Issue the call to ustat.");
		start
			expecting ( SUCCESS );
# ifndef SYSVR_2
			ustat(dev.st_dev, &buf);
# endif
			ustat((int) dev.st_dev, &buf);
		finis


		testing("Validate free blocks field.");
		start
			if ( buf.f_tfree < (daddr_t) LOW_FREE)
			{
				warning("Value for free space from ustat is questionable.\nExpecting a value greater than %d.\nThe actual value was %d.\n", LOW_FREE, (int) buf.f_fname);	/* WARNING */
			}
		finis


		testing("Validate free inodes field.");
		start
			if ( buf.f_tfree < (ino_t) LOW_INODES)
			{
				warning("Value for free inodes from ustat is questionable.\nExpecting a  value than %d.\nThe actual value was %d.\n", LOW_INODES, (int) buf.f_tinode);	/* WARNING */
			}
		finis

		testing("Validate f_fname field.");
		start
			expecting ( VOID );
			if ( strncmp(buf.f_fname, FSYSNAME, 6) != 0)
			{
				error("Value for f_fname returned by ustat does not match FSYSNAME.\nThe expected string was %s.\nThe actual string was %s.\n", FSYSNAME, buf.f_fname);	/* ERROR */
			}
		finis

		testing("Validate f_fpack field.");
		start
			expecting ( VOID );
			if ( strncmp(buf.f_fpack, PACKNAME, 6) != 0)
			{
				error("Value for f_fpack returned by ustat does not match PACKNAME.\nThe expected string was %s.\nThe actual string was %s.\n", PACKNAME, buf.f_fpack);	/* ERROR */
			}
		finis

	finis

	done();
	/*NOTREACHED*/
}
