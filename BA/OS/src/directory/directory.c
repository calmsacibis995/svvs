#ident	"@(#)directory.c	1.15"


/*
 The ../../data/directory.dir should look like this:

total 8
dr--------   2 svvs    svvs          32 Jul 10 14:17 emptydir
dr-x------   2 svvs    svvs         112 Jul 11 19:00 fivefiles
d-------wx   2 svvs    svvs          48 Jul 10 10:43 noreadall
d----wxrwx   2 svvs    svvs          48 Jul 10 10:43 noreadgroup
d-wxrwxrwx   2 svvs    svvs          48 Jul 10 10:44 noreadowner
d------rw-   2 svvs    svvs          48 Jul 10 10:44 nosearchall
d---rw-rwx   2 svvs    svvs          48 Jul 10 10:44 nosearchgroup
drw-rwxrwx   2 svvs    svvs          48 Jul 10 10:45 nosearchowner

emptydir, fivefiles, and nosearchowner are the only ones that can be read.
noreadowner and fivefiles are the only ones that can be searched
 */

/*
 *  rewinddir is VOID, so errno is explicitly examined
 *  rather than using a z_funct to do the checking
 */

#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>

#ifndef	SYSVR_2  /*  directory was added in SVR3  */

#include	<sys/types.h>
#include	<dirent.h>
#include	<sv_macro.h>


#define	NULL	(char *) 0
#define	ALLPERMS	0777	/*  temp files have all permissions set  */
#define	NUM_FILES	7	/*  the number of files in 'fivefiles'
					counting . and ..  */
#define	TMPDIRNAME	"thisisatempdir"


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
char	bigbuf [PATH_MAX + 10];
int	i, j;
char	exp_name [NUM_FILES][PATH_MAX];

DIR	*dirp;
struct dirent	*dirent;
long	offset;

setup ("directory");

funct ("opendir");

testing ("opendir (a directory where a part of the path prefix is not a directory)");
testing ("should error with ENOTDIR");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "noreadowner/hidden/whocares");

	expecting (ENOTDIR);
	opendir (bigbuf);
finis


testing ("opendir (a file which is not a directory)");
testing ("should error with ENOTDIR");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "noreadowner/hidden");

	expecting (ENOTDIR);
	opendir (bigbuf);
finis


testing ("opendir (a component of the path-prefix denies search perm");
testing ("to the owner) should return EACCES.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "nosearchowner/hidden");

	expecting (EACCES);
	opendir (bigbuf);
finis


testing ("opendir (a component of the path-prefix denies search perm");
testing ("to the group) should return EACCES.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "nosearchgroup/hidden");

	expecting (EACCES);
	opendir (bigbuf);
finis


testing ("opendir (a component of the path-prefix denies search perm");
testing ("to all) should return EACCES.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "nosearchall/hidden");

	expecting (EACCES);
	opendir (bigbuf);
finis


testing ("opendir (a directory with no read perm for the owner)");
testing ("should return EACCES.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "noreadowner");

	expecting (EACCES);
	opendir (bigbuf);
finis


testing ("opendir (a directory with no read perm for the group)");
testing ("should return EACCES.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "noreadgroup");

	expecting (EACCES);
	opendir (bigbuf);
finis


testing ("opendir (a directory with no read perm for all)");
testing ("should return EACCES.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "noreadall");

	expecting (EACCES);
	opendir (bigbuf);
finis

testing ("opening an empty directory should succeed.");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "emptydir");

	expecting (SUCCESS);
	dirp = opendir (bigbuf);
finis


funct("readdir");
testing ("reading . and .. should succeed.  The third readdir should return NULL.");
start
	exp_names [0] = ".";
	exp_names [1] = "..";
	exp_names [2] = NULL;
	act_cnt [0] = act_cnt [1] = 0;

	expecting (SUCCESS);
	dirent = readdir (dirp);

	checkoff (dirent->d_name);

	expecting (SUCCESS);
	dirent = readdir (dirp);

	checkoff (dirent->d_name);

	expecting ((struct direct *)0);
	dirent = readdir (dirp);

	if (act_cnt [0] == 0)
		error ("No entry for '.' was found");
	
	if (act_cnt [1] == 0)
		error ("No entry for '..' was found");

	expecting (SUCCESS);
	closedir (dirp);
finis


funct("closedir");
testing ("closing dir again should return EBADF.");
start
	expecting (EBADF);
	closedir (dirp);
finis

funct("rewinddir");
testing ("rewinddir on the closed file should return EBADF.");
start
	errno = 0;
	expecting (VOID);
	rewinddir (dirp);
	if (errno != EBADF)
		error ("errno was set to %s, EBADF was expected",
			err_lookup (errno) );
finis

funct("readdir");
testing ("readdir on the closed file should return EBADF.");
start
	expecting (EBADF);
	readdir (dirp);
finis


testing ("read through the 'fivefiles' directory,");
testing ("saving the record positions and names for subsequent tests");
start
	expecting (VOID);
	strcpy (bigbuf, sv_data_path());
	strcat (bigbuf, "fivefiles");

	expecting (SUCCESS);
	dirp = opendir (bigbuf);

	for (i=0 ; i<NUM_FILES; i++)
		{
		expecting (SUCCESS);
		dirent = readdir (dirp);

		expecting (VOID);
		strcpy (exp_name [i], dirent -> d_name);
		act_cnt [i] = 0;
		}

	expecting ((struct dirent *)0);	/*  there should be no more files */
	dirent = readdir (dirp);
finis

testing ("rewind the file and the same names should be read.");
start
	errno = 0;
	expecting (VOID);
	rewinddir (dirp);
	if (errno)
		error ("rewinddir caused errno %s", err_lookup (errno));

	for (i=0 ; i < NUM_FILES ; i++)	/*  read through it again  */
		{
		expecting (SUCCESS);
		dirent = readdir (dirp);

		expecting (VOID);
		if (strcmp (dirent -> d_name, exp_name [i]))
			error ("%s '%s'\nname %d should be '%s'",
				"readdir returned the name",
				dirent -> d_name, i, exp_name [i]);

		}

	expecting (SUCCESS);
	closedir (dirp);  /*  finished with the fivefiles directory  */
finis


testing ("make a temp directory and read it");
testing ("while creating then deleting files");
start
	/*  make the temp directory */
	expecting (VOID);
	strcpy (bigbuf, sv_temp_path());
	strcat (bigbuf, TMPDIRNAME);

	expecting (SUCCESS);
	mkdir (bigbuf, ALLPERMS);
	dirp = opendir (bigbuf);

	exp_names [0] = ".";
	exp_names [1] = "..";
	exp_names [2] = "one";
	exp_names [3] = "two";
	exp_names [4] = "three";
	exp_names [5] = "four";

	/* loop, each time creating a file and reading the entire directory  */
	for (i=3 ; i<7 ; i++)
		{
		/*  make a subdirectory  */
		expecting (VOID);
		strcpy (bigbuf, sv_temp_path());
		strcat (bigbuf, "/");
		strcat (bigbuf, TMPDIRNAME);
		strcat (bigbuf, "/");
		strcat (bigbuf, exp_names [i-1]);
		expecting (SUCCESS);
		mkdir (bigbuf, ALLPERMS);

		errno = 0;
		expecting (VOID);
		rewinddir (dirp);
		if (errno)
			error ("rewinddir caused errno %s", err_lookup (errno));

		act_cnt [0] = act_cnt [1] = act_cnt [2] = 0;
		act_cnt [3] = act_cnt [4] = act_cnt [5] = 0;

		/*  read all the names  */
		for (j=0 ; j<i ; j++)
			{
			dirent = readdir (dirp);
			checkoff (dirent -> d_name);
			}
		/*  this should return EOF */
		expecting ((struct dirent *) 0);
		dirent = readdir (dirp);

		/*  check the name counts  */
		for (j=0 ; j<i ; j++)
			{
			if (act_cnt [j] != 1)
				error ("file '%s' was found %d times",
					exp_names [j], act_cnt [j]);
			}
		}


	/* loop, each time deleting a file and reading the entire directory  */
	for (i=5 ; i>1 ; i--)
		{
		/*  delete a subdirectory  */
		expecting (VOID);
		strcpy (bigbuf, sv_temp_path());
		strcat (bigbuf, "/");
		strcat (bigbuf, TMPDIRNAME);
		strcat (bigbuf, "/");
		strcat (bigbuf, exp_names [i]);
		expecting (SUCCESS);
		rmdir (bigbuf);

		errno = 0;
		expecting (VOID);
		rewinddir (dirp);
		if (errno)
			error ("rewinddir caused errno %s", err_lookup (errno));

		act_cnt [0] = act_cnt [1] = act_cnt [2] = 0;
		act_cnt [3] = act_cnt [4] = act_cnt [5] = 0;

		/*  read all the names  */
		for (j=0 ; j<i ; j++)
			{
			dirent = readdir (dirp);
			checkoff (dirent -> d_name);
			}
		/*  this should reaturn EOF */
		expecting ((struct dirent *) 0);
		dirent = readdir (dirp);

		/*  check the name counts  */
		for (j=0 ; j<i ; j++)
			{
			if (act_cnt [j] != 1)
				error ("file '%s' was found %d times",
					exp_names [j], act_cnt [j]);
			}
		}
	/*  delete the temp directory */
	expecting (VOID);
	strcpy (bigbuf, sv_temp_path());
	strcat (bigbuf, TMPDIRNAME);

	expecting (SUCCESS);
	rmdir (bigbuf);
	closedir (dirp);
finis


testing ("opening more than OPEN_MAX directories should fail with EMFILE");
start
/*
 *  This is the last test case because I'm letting exit close all the files
 */
	for (i=3 ; i<OPEN_MAX ; i++)
		{
		expecting (SUCCESS);
		dirp = opendir (".");  /*  exit will clean up for me  */
		}
	expecting (EMFILE);
	opendir (".");
finis

done ();
/*NOTREACHED*/
}

/*
 *  CHECKOFF - checks if the file name is in the list
 *   and was found only once
 */ checkoff (name)
char	*name;
{
register int	i = 0;

for ( ; exp_names [i]; i++)	/*  search the list  */
	{
	expecting(VOID);
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
unsupported("directory is not a supported feature in SVR2");
}

#endif
