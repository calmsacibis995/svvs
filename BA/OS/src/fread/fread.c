#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fread.c	1.1"

char *subs[] = {
	"fread0",	/* fread existing files with "r","r+","a+"	*/
	"fwrite0",	/* fwrite new files with "a","w","r+","a+","w+"	*/
	"fwrite1",	/* fwrite existing files with "a","r+","a+"	*/
	"freadwrit0",	/* fread/fwrite new files with "a+","w+"	*/
	"freadwrit1",	/* fread/fwrite existing files with "r+","a+"	*/
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("fread");
	funct("fread");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
