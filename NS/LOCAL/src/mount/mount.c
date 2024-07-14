#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mount.c	1.5"

#ifndef SV_RFS
char *subs[] = {
	"mount0",
	"mount1",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("mount");
	funct("mount");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
#else

# define	READ_ONLY	1

main()
{
	int i;
	setup("mount");
	funct("mount");
	exit_on ( ERROR );

	testing("Attempt to mount '%s' on a remote resource.", SVVS_FS);
	testing("Verify mount fails with errno set to EREMOTE.");
	start
		char *temp_path();
		char *err_lookup();

		expecting ( FAILURE );
		if( mount(SVVS_FS, temp_path(), READ_ONLY) == -1 )
			if( errno != EREMOTE ) {
				if( errno == EMULTIHOP )
					warning( "%s\nmount set errno to EMULTIHOP",
					  ADVISORY );
				else
					error("%s\n%s\n%s%s",
					  "mount - set errno incorrectly",
					  "expected errno: EREMOTE",
					  "actual errno: ", 
					  err_lookup(errno) );
			}		
	finis
	done();
}
#endif
