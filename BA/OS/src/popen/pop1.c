#include <sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)pop1.c	1.1"


void exit();

main(argc,argv)
int argc;
char *argv[];
{
	register int i;
	argv++;
	for( i = 1; i < argc ; i++ )
		(void) printf("%s\n",*argv++);
	exit(0);
}
