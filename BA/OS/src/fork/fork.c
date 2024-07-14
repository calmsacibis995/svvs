#ident	"@(#)fork.c	1.2"
#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

char	*tests[]={
	"fork0",
#ifndef	SYSVR_2
	"fork1",
	"fork2",
	"fork3",
#endif
};

#define	NTST	(sizeof(tests) / sizeof(char *))

main(argc,argv)
int		argc;
char	*argv[];
{
	int	i;

	setup("fork");
	funct("fork");

	for(i = 0; i < NTST; ++i){
		subtest(tests[i]);
	}
}
