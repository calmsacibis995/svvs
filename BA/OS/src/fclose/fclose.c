#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fclose.c	1.2"

main()
{
	setup("fclose");

	funct("fclose");
	subtest ("fclose0");	/* fclose file opened "r","a","w","r+","a+","w+"*/
	funct("fflush");
	subtest ("fflush0");	/* fflush file opened "r","a","w","r+","a+","w+"*/

	done();
}
