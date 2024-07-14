#include	<sv_base.h>
#include	<fcntl.h>
#include	<string.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#define		DLINE		256
#define 	DCARD  	        80
#undef		isdigit 
#undef		isalpha
#undef		isalnum
#define		isdigit(z)	(z>='0'&&z<='9')
#define		isalpha(zz)	(zz>='a'&&zz<='z'||zz>='A'&&zz<='Z')
#define		isalnum(z)	(isdigit(z)||isalpha(z))

/* external variables */
char	Name[DCARD];
char    Passwd[DCARD];
char    Usr_id[DCARD];
char	Grp_id[DCARD];
char	Free[DCARD];
char	Work_dir[DCARD];
char	Shell[DCARD];
char	Read_buff[DLINE];
char	Rd_buff[DLINE];
int     Num_char;
int     Fdes;
char    *Front_ptr;
char    *End_ptr;
long    lseek();
char	*strchr();

#ident	"@(#)passwd.c	1.3"

/*
** datafiles
**	Test for the presence of and the format of the
**      /etc/passwd file.
*/
main()
{
    void    verify_frmt();

setup ("passwd");
funct ("passwd");


testing ("open /etc/passwd for reading");
start
	expecting (SUCCESS);
	error_on( E_FAILURE );
	exit_on ( ERROR );
	Fdes = open ("/etc/passwd", O_RDONLY);
	verify_frmt();
	(void)close (Fdes);
finis

done();
/*NOTREACHED*/

}



/*
**  verify_frmt()
**       This function controls the processing for a users info in 
**       the passwd file. The info will be separated by new-line for 
**       each user.
**       The function is a void and it calls get_fields and test_fields.
**       Front_ptr  - points to the start of a users info
**       End_ptr    - marks the end of the users info, or the start of 
**                    the next user.
**       Num_char   - the number of characters actually read in.
**       Read_buff  - Holds a line of data from the passwd file
*/
void
verify_frmt()
{
    int       tested = 0;


	testing ("The fields should exist with the correct format/content:");
	testing ("The name, user ID and initial working directory fields exist,");
	testing ("the name and shell fields have a length less than NAME_MAX");
	testing ("and the user and group IDs are numeric.");
	start
	Front_ptr = Read_buff;
	End_ptr = Rd_buff;

	expecting( VOID );	

	while ((Num_char = read(Fdes, Front_ptr, DLINE)) > 0) {

		tested = 1;

		while (Front_ptr < (Read_buff + Num_char)) {

			if (*Front_ptr != '\n') {
				*End_ptr++ = *Front_ptr++;
			} else {
				Front_ptr++;
				*End_ptr = '\0';
				End_ptr = Rd_buff;
				get_fields();
				(void)  test_fields();
			}
		}

		Front_ptr = Read_buff;
	}
	expecting( SUCCESS );

	testing ("/etc/passwd should be present and non-empty,");
	testing ("containing at least the information for one user.");
	start
	if (!tested) {
		error("The /etc/passwd file is empty.  %d characters were read in from the file.", Num_char);
	}
	finis

	finis
}


/*
**      This function will take a user's info (pointed to by Front_ptr)
**      and get the fields out. Each field will be separated by a colon.
**      The fields to be examined will be name, passwd, usr id, grp id,
**      free, initial working directory and initial shell. Any fields
**      following the shell will be ignored.
**      This function is a void.
**      catch      - will catch the return value from the sscanf function
**      test_str   - holds the info for one user.
**      next_field - marks the starting point of the next field
*/
get_fields()

{

	char	*test_str = Rd_buff;
	int	catch;


/* take out the fields */
    catch = sscanf (test_str, "%[^:]", Name);
    if (catch <=0)
	Name[0] = '\0';

	test_str += (strlen(Name) + 1);

    catch = sscanf (test_str, "%[^:]", Passwd);
    if (catch <=0)
	Passwd[0] = '\0';

	test_str += (strlen(Passwd) + 1);

    catch = sscanf (test_str, "%[^:]", Usr_id);
    if (catch <=0)
	Usr_id[0] = '\0';

	test_str += (strlen(Usr_id) + 1);

    catch = sscanf (test_str, "%[^:]", Grp_id);
    if (catch <=0)
	Grp_id[0] = '\0';

	test_str += (strlen(Grp_id) + 1);

    catch = sscanf (test_str, "%[^:]", Free);
    if (catch <=0)
	Free[0] = '\0';

	test_str += (strlen(Free) + 1);

    catch = sscanf (test_str, "%[^:]", Work_dir);
    if (catch <=0)
	Work_dir[0] = '\0';

	test_str += (strlen(Work_dir) + 1);

    catch = sscanf (test_str, "%[^:]", Shell);
    if (catch <=0)
	Shell[0] = '\0';


}



char *
correct_path(path)
char	path[DCARD];
{
    long	catch;
    char        name[DCARD];
    char        *p;


    p = path;
    if (*path == '/')
	p++;
    for (catch = sscanf (p, "%[^/]", name); 
		catch > 0 && strlen(name) <= NAME_MAX; 
		catch = sscanf (p, "%[^/]", name))
	p = p + strlen(name) + 1;

	if(strlen(name) > NAME_MAX && catch == 1)
	    (void) strcpy(p, name);
	else
	    (void) strcpy(p, "\0");

	return(p);

}





/*
**      Test the presence and content/format of the fields 
**      The name, user ID and the initial working directory to exist.
**      The name to be less than NAME_MAX. The user ID and group ID 
**      (if present) to be numeric and the name of the shell program 
**      (if used) to be less than NAME_MAX.
**      The function is declared as an integer. Each time an error message
**      is written out, the function will return zero.
*/
test_fields()
{
    char        *ptr;
    char        format_buff[STRLEN];



/*
**  Test the existance of the login name
*/
    if (!Name)
    {
	error ("The login name is missing.");
	return(0);
    }

/*
**  Test the length of the login name
*/
    if (strlen(Name) > NAME_MAX)
    {
	error ("The length of login name '%s' is greater than NAME_MAX.",
	    Name);
	return(0);
    }

    if (ptr = strchr(Name, '/'))
    {
	error ("Login name '%s' contains a slash.", Name);
	return(0);
    }


/* 
**  Test the existance of the password
**  This test was removed 10/17/1986 per MR #us86-28802
**  Issue 2 of the SVID no longer specifies the format of the passwd field.
*/


/*
**  Test the fact that the user id exists and is a numeric value
*/

if (Usr_id)
{
    for(ptr=Usr_id; *ptr && isdigit(*ptr); ptr++)
    ;
    /* the user id was numeric if *ptr == '\0' */
    if (*ptr)
    {
	error ("The user id for login name %s has a non numeric value '%s'.",
		    Name, Usr_id);
	return(0);
    }
}
else
{
    error ("The user id for login name '%s' is missing", Name);
    return(0);
}


/*
**  If the group id exists, test the fact that it is numeric
*/
    if (Grp_id)
    {
	for(ptr=Grp_id; *ptr && isdigit(*ptr); ptr++)
	;
    /* the group id was numeric if *ptr == '\0' */
	if (*ptr)
	{
	    error ("The group id for login name %s has a non numeric value '%s'.",
		    Name, Grp_id);
	    return(0);
	}
    }


/*
**  Test the fact that the initial working directory exists and each
**  path name is less than or equal to NAME_MAX
*/
    if (!Work_dir)
    {
	error ("The initial working directory for login name '%s' is missing",
	   Name);
	return(0);
    }
    else
	if (*(ptr = correct_path(Work_dir)))
	{
	    error ("The working directory for login name '%s' has a path name longer than NAME_MAX.", Name);
	    return(0);
	}


/*
**  Test the fact that the working shell (if it exists) has a path name
**  less than or equal to NAME_MAX
*/
    if (Shell)
    {
	if (!(strchr(Shell, '/')) && strlen(Shell) > NAME_MAX)
	{
	    error ("The working shell for login name '%s' has a name longer than NAME_MAX.", Name);
	    return(0);
	}

	if ((strchr(Shell, '/')) && *(ptr = correct_path(Shell)))
	{
	    error ("The path name of the shell for login name '%s' has a file name/directory name longer than NAME_MAX.", Name);
	    return(0);
	}
    }

    return(0);
}
