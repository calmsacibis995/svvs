#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)printf.c	1.2"



/*
** printf
**	Test the printf family of functions (printf, sprintf, fprintf).
*/

main()
{

setup("printf");
funct ("printf");
start
	subtest ("printf1");
finis

/*
** FPRINTF
*/
funct ("fprintf");
start
	subtest ("printf2");
finis

/*
** SPRINTF
*/
funct ("sprintf");
start
	subtest ("printf3");
finis

done();
/*NOTREACHED*/
	
}
