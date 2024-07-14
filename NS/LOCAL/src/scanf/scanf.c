#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)scanf.c	1.2"



/*
** scanf
**	Test the scanf family of functions (scanf, sscanf, fscanf).
*/

main()
{

setup("scanf");
funct ("scanf");
start
	subtest ("scanf1");
finis

/*
** FSCANF
*/
funct ("fscanf");
start
	subtest ("scanf2");
finis

/*
** SSCANF
*/
funct ("sscanf");
start
	subtest ("scanf3");
finis

done();
/*NOTREACHED*/
	
}
