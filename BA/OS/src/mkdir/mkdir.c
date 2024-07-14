#ident	"@(#)mkdir.c	1.10"


#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>

#ifndef	SYSVR_2  /*  mkdir was added in SVR3  */

#include	<sys/types.h>
#include	<dirent.h>
#include	<sv_macro.h>
#include	<sys/stat.h>

#ifndef	NULL
#define	NULL	(char *) 0;
#endif

#define	TESTDIRNAME	"thisisatestdir"
#define	ANOTHERFILENAME	"yetanotherone"
#define	FILENAME2	"thisisfile2"
#define	BADPATH		"foo/bar"

#define	ALLPERMS	0777

#undef	strlen	/*  disable strlen error checking  */
#undef	strcpy	/*  disable strcpy error checking  */


/*
 *  This is the null terminated list of expected file names
 */
static	char *exp_names [9];

/*
 *  This is a checkoff list of the names found
 */
static	int	act_cnt [9];

char	*err_lookup ();


main()
{
char	bigbuf [PATH_MAX + 20];
int	i;
int	limit;
char	bindir [PATH_MAX];

DIR	*dirp;
struct dirent	*dirent;
long	offset;

struct stat	statbuf;


setup ("mkdir");
funct ("mkdir");


prep ("Assure that the temp file doesn't exist");
start
	expecting (SUCCESS);	/*  chdir to the temp directory  */
	getcwd (bindir, PATH_MAX);
	chdir (sv_temp_path ());

	expecting (VOID);
	rmdir (TESTDIRNAME);  /*  make sure TESTDIRNAME is not there  */

	expecting (ENOENT);	/*  make sure the file doesn't exist  */
	stat (TESTDIRNAME, &statbuf);
finis


testing ("mkdir should create a directory that contains . and .. only");
start
	expecting (SUCCESS);
	mkdir (TESTDIRNAME, ALLPERMS);

	stat (TESTDIRNAME, &statbuf);
	if (!(statbuf.st_mode & S_IFDIR))
		error ("mkdir made a file that is not a directory");

	dirp = opendir (TESTDIRNAME);
	exp_names [0] = ".";
	exp_names [1] = "..";
	exp_names [2] = NULL;
	act_cnt [0] = act_cnt [1] = 0;

	expecting (VOID);
	errno = 0;  /*  readdir will set errno for errors but not for EOF  */
	while ((dirent = readdir (dirp)) > 0)
		{
		checkoff (dirent->d_name);
		errno = 0;
		}
	if (errno)
		error ("readdir returned with errno = %s", err_lookup (errno));
	if (act_cnt [0] == 0)
		error ("No entry for '.' was found");
	
	if (act_cnt [1] == 0)
		error ("No entry for '..' was found");

	expecting (SUCCESS);
	closedir (dirp);
finis



testing ("another mkdir with the same name should fail with errno EEXIST");
start
	expecting (EEXIST);
	mkdir (TESTDIRNAME, ALLPERMS);

	expecting (EEXIST);
	mkdir (".", ALLPERMS);

	expecting (EEXIST);
	mkdir ("..", ALLPERMS);

finis


testing ("change into the newly made directory and make an ordinary file");
start
	expecting (SUCCESS);
	chdir (TESTDIRNAME);
	dirp = opendir (".");

	close (creat (ANOTHERFILENAME, 0666));

	exp_names [0] = ".";
	exp_names [1] = "..";
	exp_names [2] = ANOTHERFILENAME;
	exp_names [3] = NULL;
	act_cnt [0] = act_cnt [1] = act_cnt [2] = 0;

	expecting (VOID);
	errno = 0;  /*  readdir will set errno for errors but not for EOF  */
	while ((dirent = readdir (dirp)) > 0)
		{
		checkoff (dirent->d_name);
		errno = 0;
		}
	if (errno)
		error ("readdir returned with errno = %s", err_lookup (errno));
	if (act_cnt [0] == 0)
		error ("No entry for '.' was found");
	
	if (act_cnt [1] == 0)
		error ("No entry for '..' was found");

	if (act_cnt [2] == 0)
		error ("No entry for '%s' was found", ANOTHERFILENAME);

	expecting (SUCCESS);
	closedir (dirp);
finis


testing ("mkdir should fail if the parent directory is unwriteable");
start
	expecting (SUCCESS);
	chmod (".", 0500);

	expecting (EACCES);
	mkdir (FILENAME2, ALLPERMS);
finis



testing ("mkdir should fail if the parent directory is unsearchable");
start
	expecting (SUCCESS);
	chdir ("..");  /*  chdir out of TESTDIRNAME before making it unsearchable */
	chmod (TESTDIRNAME, 0600);

	expecting (SUCCESS);
	sprintf (bigbuf, "%s/%s",TESTDIRNAME, FILENAME2);
	expecting (EACCES);
	mkdir (bigbuf, ALLPERMS);

	expecting (SUCCESS);
	chmod (TESTDIRNAME, 0700);
	chdir (TESTDIRNAME);
finis


testing ("mkdir should fail if part of the pathname isn't a directory");
start
	expecting (SUCCESS);
	sprintf(bigbuf, "%s/junk", ANOTHERFILENAME);

	expecting (ENOTDIR);
	mkdir (bigbuf, ALLPERMS);
finis


testing ("mkdir should fail if part of the pathname does not exist");
start
	expecting (ENOENT);
	mkdir (BADPATH, ALLPERMS);
finis

testing("Exceeding LINK_MAX links to a file");
testing("should return -1 with an errno of EMLINK.");
start
	if (LINK_MAX < 5001)
		limit = LINK_MAX;
	else
		{
		limit = 5000;  /*  a practical limit to test */
		message ("LINK_MAX test will test only to 5000, not to %d",
			LINK_MAX);
		}

	/*  remember, all directories start with 2 links  */
	for(i = 2; i < limit ; i++)
		{
		expecting ( SUCCESS );
		sprintf(bigbuf, "%d", i);

		expecting ( SUCCESS );
		mkdir (bigbuf, ALLPERMS);
		}

	if (LINK_MAX < 5001)  /*  the truncated test will not get EMLINK  */
		{
		expecting ( SUCCESS );
		sprintf(bigbuf, "./%d", i);

		expecting ( EMLINK );
		mkdir (bigbuf, ALLPERMS);
		}

/*
**	remove links from temp directory, rm command may not be able
**	to handle LINK_MAX arguments.
**/
	for(i = 2; i < LINK_MAX; i++)
		{
		expecting ( SUCCESS );
		sprintf(bigbuf, "./%d", i);
		rmdir (bigbuf);
		}
finis


prep ("clean up the temp file");
start
	expecting (SUCCESS);
	unlink (ANOTHERFILENAME);
	chdir ("..");  /*  can not delete your working directory!  */
	rmdir (TESTDIRNAME);
finis


chdir (bindir);
subtest ("mkdir1");


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
unsupported("mkdir is not a supported feature in SVR2");
}

#endif
