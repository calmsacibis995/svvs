#include	<sv_base.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#undef 		isalpha
#define		isalpha(zz)	(zz>='a'&&zz<='z'||zz>='A'&&zz<='Z')


int    Fdes;

#ident	"@(#)envvar.c	1.2"

char	*ptr;
char	*getenv ();

/*
** env
**	Test for the presence of HOME, PATH, TZ and TERM
**      then test the format of TZ
*/
main()
{
	int	test_format();


setup ("envvar");
funct ("envvar");
abstract ("the getenv will fail if the variable is undefined");

/*
 *  The directory "../tmp/environment.dir" is no longer needed
 */

start
testing ("That HOME has been defined (to a non null value).");
start
	expecting (SUCCESS);
	continue_on (ERROR);

	if (*getenv ("HOME") == '\0')
		error("HOME has a null value");
finis



testing ("That TERM has been defined (to a non null value).");
start
	expecting (SUCCESS);
	continue_on (ERROR);

	if (*getenv ("TERM") == '\0')
		error("TERM has a null value");
finis



testing ("That PATH has been defined (to a non null value).");
start
	expecting (SUCCESS);
	continue_on (ERROR);

	if (*getenv ("PATH") == '\0')
		error("PATH has a null value");
finis



testing ("That TZ has been defined (to a non null value).");
start
	expecting (SUCCESS);
	break_on (ERROR);

	if (*(ptr = getenv ("TZ")) == '\0')
		error("TZ has a null value");

	(void) test_format ();
finis
finis


done();
/*NOTREACHED*/
}



test_format()
{
    int        catch;
    int        mean_time;

testing ("the value of the timezone");


testing ("That the first 3 characters are alphabetic.");
start
    if (!isalpha(*ptr) || !isalpha(*(ptr+1)) || !isalpha(*(ptr+2)))
    {
	error ("Invalid local time zone, three letters = '%.3s'", ptr);
	return(0);
    }
finis

testing ("That the mean time is a positive or negative integer.");
start
    catch = 0;
    if (*(ptr+3) == '-')
	{
	if (*(ptr+4) == '\0')
	    error ("no mean time specified");
	else
	    catch = sscanf(ptr + 4, "%d", &mean_time);
	}
    else
	{
	if (*(ptr+3) == '\0')
	    error ("no mean time specified");
	else
	    catch = sscanf(ptr + 3, "%d", &mean_time);
	}
    if (catch != 1)
    {
	error ("Invalid mean time of a signed integer = '%s'", ptr + 3);
    }
finis

return (0);
}
