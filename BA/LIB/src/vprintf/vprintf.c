#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)vprintf.c	1.2"



/*
** vprintf
**	Test the vprintf family of functions (vprintf, vsprintf, vfprintf).
*/

main()
{
	int		pid;

setup("vprintf");
funct ("vprintf");
start
	subtest ("vprintf1");
finis

/*
** VFPRINTF
*/
funct ("vfprintf");
start
	subtest ("vprintf2");
finis

/*
** VSPRINTF
*/
funct ("vsprintf");
start
	subtest ("vprintf3");
finis

done();
/*NOTREACHED*/
	
}
