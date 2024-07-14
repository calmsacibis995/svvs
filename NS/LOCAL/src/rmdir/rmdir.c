#ident	"@(#)rmdir.c	1.6"

#ifndef	SYSVR_2  /*  rmdir was added in SVR3  */

#include	<sys/types.h>
#include	<dirent.h>

#include	<sv_base.h>

#include	<sys/stat.h>	/*This must come after sv_base.h*/

#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	NULL
#define	NULL	(char *) 0;
#endif

#define	TESTDIRNAME	"thisisatestdir"
#define	ANOTHERFILENAME	"yetanotherone"
#define	FILENAME2	"thisisfile2"
#define	BADPATH		"foo/bar"

#define	ALLPERMS	0777


/*
 *  This is the null terminated list of expected file names
 */
static	char *exp_names [9];

/*
 *  This is a checkoff list of the names found
 */
static	int	act_cnt [9];


main()
{
char	bigbuf [PATH_MAX + 9];
int	i;
int	limit;

DIR	*dirp;
struct dirent	*dirent;
long	offset;
char	bindir [PATH_MAX];

struct stat	statbuf;


setup ("rmdir");
funct ("rmdir");


prep ("set up the temp directories needed to test rmdir");
start
	expecting (SUCCESS);	/*  chdir to the temp directory  */
	getcwd (bindir, PATH_MAX);
	chdir (sv_temp_path ());

	expecting (VOID);
	rmdir (TESTDIRNAME);  /*  just to be sure  */

	expecting (ENOENT);	/*  make sure the file doesn't exist  */
	stat (TESTDIRNAME, &statbuf);

	expecting (SUCCESS);
	mkdir (TESTDIRNAME, ALLPERMS);

/*  change into the newly made directory and make an ordinary file  */
	expecting (SUCCESS);
	chdir (TESTDIRNAME);

	close (creat (ANOTHERFILENAME, 0666));

/*  make a directory  */
	mkdir (FILENAME2, ALLPERMS);
finis


testing ("rmdir should fail if the parent directory is unwriteable");
start
	expecting (SUCCESS);
	chmod (".", 0500);

	expecting (EACCES);
	rmdir (FILENAME2);
finis



testing ("rmdir should fail if the parent directory is unsearchable");
start
	expecting (SUCCESS);
	chdir ("..");  /*  chdir out of TESTDIRNAME before making it unsearchable */
	chmod (TESTDIRNAME, 0600);

	expecting (SUCCESS);
	sprintf (bigbuf, "%s/%s",TESTDIRNAME, FILENAME2);
	expecting (EACCES);
	rmdir (bigbuf);

	expecting (SUCCESS);
	chmod (TESTDIRNAME, 0700);
	chdir (TESTDIRNAME);
finis


testing ("rmdir should fail if part of the pathname isn't a directory");
start
	expecting (SUCCESS);
	sprintf(bigbuf, "%s/junk", ANOTHERFILENAME);

	expecting (ENOTDIR);
	rmdir (bigbuf);
finis


testing ("rmdir should fail if part of the pathname does not exist");
start
	expecting (ENOENT);
	rmdir (BADPATH);
finis


testing ("rmdir . should fail with EINVAL");
start
	expecting (EINVAL);
	rmdir (".");
finis


testing ("rmdir (a non empty directory) should fail with EEXIST");
start
	expecting (EEXIST);
	rmdir ("..");
finis


testing ("rmdir (the current directory) should fail with EINVAL");
start
	expecting (VOID);
	strcpy (bigbuf, "../");
	strcat (bigbuf, TESTDIRNAME);

	expecting (EINVAL);
	rmdir (bigbuf);
finis


cleanup ("clean up the temp file");
start
	expecting (SUCCESS);
	rmdir (FILENAME2);
	unlink (ANOTHERFILENAME);
	chdir ("..");  /*  can not delete your working directory!  */
	rmdir (TESTDIRNAME);
finis

chdir (bindir);
subtest ("rmdir1");

done ();
/*NOTREACHED*/
}

/*
 *  CHECKOFF - checks if the file name is in the list
 *   and was found only once
 */
checkoff (name)
char	*name;
{
register int	i = 0;

for ( ; exp_names [i]; i++)	/*  search the list  */
	{
	if (strcmp (exp_names [i], name) == 0)	/*  they match  */
		{
		if (act_cnt [i]++)	/*  Is this a duplicate?  */
			{
			error ("The name '%s' is a duplicate", name);
			return (0);  /*  failure  */
			}
		return (1); /*  It's okay and checked off the list  */
		}
	}
error ("The name '%s' is invalid", name);
return (0);
}


#else

main()
{
unsupported("rmdir is not a supported feature in SVR2");
}

#endif
