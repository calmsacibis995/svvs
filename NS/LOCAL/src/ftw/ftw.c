#include 	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<ftw.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	BADFNCALL	1
# define	OKAY		0
# define	BADFNPARM	2


#ident	"@(#)ftw.c	1.4"



/*
** ftw
**	Test the ftw function.
*/
char		PathPointer[STRLEN];
extern	char	*data_path();

main()
{
	int		ret_val;
	extern	int	test_func();
setup("ftw");


funct ("ftw");
testing("ftw(a nonexistent pathname) should return -1.");
start
	expecting (VOID);
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"nodir");
	if ( (ret_val = ftw(PathPointer, test_func, OPEN_MAX)) == BADFNCALL )
		error("The fn call should not have taken place");
	if ( ret_val != -1 )
		error("Incorrect return value from ftw = %d",ret_val);
finis


testing("ftw(a legal path, depth = -1) should return 0.");
start
	expecting (VOID);
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"dira");
	if ( (ret_val = ftw(PathPointer, test_func, -1)) == BADFNPARM )
		error("The fn call was made with bad parms of (%s, ?, -1)",
			PathPointer);

	if ( ret_val != 0 )
		error("ftw did not work with a depth = -1, returning %d",
			ret_val);
finis


testing("ftw(a legal path, depth = OPEN_MAX)");
testing("(# of available file descriptors)");
testing("should return 0.");
start
	expecting (VOID);
	/*
	** Test for dira first
	*/
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"dira");
	if ( (ret_val = ftw(PathPointer, test_func, OPEN_MAX)) == BADFNPARM )
		error("The fn call was made with bad parms of (%s,?, %d)",
			PathPointer, OPEN_MAX);

	if ( ret_val != 0 )
		error("ftw did not work with a depth = OPEN_MAX and a \n\
directory with all permissions");

finis


testing("ftw(a legal path, depth = OPEN_MAX) should return 0.");
start
	expecting (VOID);
	/*
	** Now test for the directory dirb
	*/
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"dirb");
	/*  this will fail if dirb doesn't exist,
	 *  or exists and is readable to this uid or gid
	 */
	if ( (ret_val = ftw(PathPointer, test_func, OPEN_MAX)) == BADFNPARM )
		error("The fn call was made with bad parms of (%s,?, %d)",
			PathPointer, OPEN_MAX);
	if ( ret_val != 0 )
		error("ftw did not work with a depth = OPEN_MAX and\n\
a directory with no read permission");
finis


testing("ftw(a legal path, depth = OPEN_MAX) should return 0.");
start
	expecting (VOID);
	/*
	** Now test for the directory dirc
	*/
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"dirc");
	if ( (ret_val = ftw(PathPointer, test_func, OPEN_MAX)) == BADFNPARM )
		error("The fn call was made with bad parms of (%s,?, %d)",
			PathPointer, OPEN_MAX);
	if ( ret_val != 0 )
		error("ftw did not work with a depth = OPEN_MAX and\n\
a directory with no execute permission");
finis


testing("ftw(a legal path = \"dird\", depth = OPEN_MAX)");
testing("should return immediately , returning 0.");
start
	expecting (VOID);
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"dird");
	if ( (ret_val = ftw(PathPointer, test_func, OPEN_MAX)) != 0 )
		error("The ftw call did not work with fn returning immediately\n\
for the directory: dird");
finis


testing("ftw(a legal path = \"dire\")");
testing("with all available file descriptors closed.");
testing("should return -1, and an errno of EBADF.");
start
	expecting (VOID);
	strcpy(PathPointer,data_path());
	strcat (PathPointer,"dire");
	expecting (-1);
	expecting (EBADF);
	ftw(PathPointer, test_func, OPEN_MAX);
finis


done();
/*NOTREACHED*/

}
/***************************************************
** test_func(path_name, ftw_integer)
** 
** This function will perform the fn function for 
** FTW.
**
****************************************************/

/*
 *  This is necessary to disable the error checking routines
 *  since this subroutine is nested within a function test
 */
#undef strcpy
#undef strcat
#undef strcmp
#undef strlen

test_func(path_name, stat_pointer, ftw_integer)
char		*path_name;
struct	stat	*stat_pointer;
int		ftw_integer;
{
	int	i;
	char	path[STRLEN];
	char	errs[6];

	strcpy(errs,&path_name[strlen(path_name) - 5]);
	if ( strcmp("/errs",errs) == 0 && (ftw_integer == FTW_F || ftw_integer == FTW_NS) )
	{
		ftw_integer = FTW_D;
		path_name[strlen(path_name) - 5] = '\0';
	}

	strcpy(path,data_path());
	strcat (path,"dird");
	if ( strcmp(path_name, path) == 0)
		return(OKAY);

	strcpy(path,data_path());
	strcat (path,"nodir");
	if ( strcmp(path_name, path) == 0)
		return(BADFNCALL);
	
	expecting (VOID);
	strcpy(path,data_path());
	strcat (path,"dire");
	if ( strcmp(path_name, path) == 0)
	{
		for (i = 4; i < OPEN_MAX; i++)
			close(i);
		return(-1);
	}

	strcpy(path,data_path());
	strcat (path,"dira");
	if ( strcmp(path_name, path) == 0 && ftw_integer == FTW_D)
		return(OKAY);
	else
	{
		strcpy(path,data_path());
		strcat (path,"dirb");
		if ( strcmp(path_name, path) == 0 && ftw_integer == FTW_DNR)
			return(OKAY);
		else
		{
			strcpy(path,data_path());
			strcat (path,"dirc");
			if ( strcmp(path_name, path) == 0 && (ftw_integer == FTW_NS || ftw_integer == FTW_D) )
				return(OKAY);
			else
				return(BADFNPARM);
		}
	}
}
