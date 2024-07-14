#include	<sv_base.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)chmod2.c	1.4"

#define	PROT	S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

/*
 * chmod2.c - a test of chmod(BA_OS) for enforcement record/file locking
 */
#ifndef	SYSVR_2
main()
{
	char	*path;
	struct	stat	buf;
	char	 *malloc();

	setup("chmod");
	funct("chmod");

	exit_on ( ERROR );

	prep("Allocate a path buffer.");
	start
		path = malloc( PATH_MAX );
	finis

	abstract("Set enforcement mode record locking on a file.");
	start
		prep("Create a temp file and initialize a path.");
		start
			temp_creat("chmod_2", PROT);
			strcpy(path, temp_path());
			strcat(path, "chmod_2");
		finis
		testing("Call chmod.");
		start
			chmod(path, S_ENFMT);
		finis
		testing("Call stat to verify the enforcement flag was set.");
		start
			stat(path,&buf);
			if((buf.st_mode & (S_IXGRP | S_ENFMT)) != S_ENFMT)
				error("The enforcement mode flag was not set by chmod.");
		finis
	finis
	done();
}
#else
main(argc,argv)
int		*argc;
char	*argv;
{
	unsupported("Record/file locking enforcement mode is not supported in System V Release 2");
	done();
}
#endif
