#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mknod2.c	1.3"

/*
** mknod
**	Test the mknod function for enforcement record/file
**	locking
**
**	This test depends on stat(BA_OS)
*/
#ifndef	SYSVR_2
main()
{
	char	path[PATH_MAX];
	struct	stat	buf;
	int		dev;
	int		mknod();

	setup("mknod");
	funct("mknod");

	exit_on ( ERROR );

	testing("Create an ordinary file with the record/file locking");
	testing("flag turned on.  Expect a return value of zero.");
	start
		strcpy(path, temp_path());
		strcat(path, "mknod_2");
		expecting ( SUCCESS );
		mknod(path, (S_IFREG | S_ENFMT), dev);
	finis
	testing("Now stat the file to make sure that the enforcement");
	testing("flag was set by the mknod.");
	start
		expecting(SUCCESS);
		stat(path,&buf);
		if((buf.st_mode & (S_IXGRP | S_ENFMT)) != S_ENFMT){
			error("Enforcement mode flag not set by mknod().");
		}
	finis
	cleanup("Unlink the file, since the driver won't be able to.");
	start
		unlink(path);
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
