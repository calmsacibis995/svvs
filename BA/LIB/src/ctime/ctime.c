#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<time.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


# define	KNOWNTIME	0


#ident	"@(#)ctime.c	1.3"



/*
** ctime
**	Test the ctime function.
*/
main()
{

	char		*ret_time;
	ttime		time_var;
	char		*actual_time;
	char		*save_tzname[2];
	struct	tm	*ret_tm;
	struct	tm	known_tm;
	struct	tm	*pknown_tm;
	
	extern	long	timezone;
	extern	int	daylight;
	extern	char	*tzname[2];
	extern	char	*getenv();

setup("ctime");
prep ("Set the time zone to a known value");
start
	/*
	** set the time zone to a known value
	*/
	putenv("TZ=GMT");
finis

funct ("ctime");
start
    
    testing("ctime(KNOWNTIME) should return");
    testing("a return value that corresponds to KNOWNTIME.");
    start
    	expecting (SUCCESS);
    
    	time_var = KNOWNTIME;
    	ret_time = ctime(&time_var);
    	actual_time = "Thu Jan  1 00:00:00 1970\n";
    	expecting (VOID);
    
    	if ( ( strcmp(ret_time, actual_time) ) != 0 )
    		error ("Incorrect return value from ctime given the time zero = %s",
    		ret_time);
    finis
    
    
    testing("ctime(KNOWNTIME+1) should return");
    testing("a return value of 1 more than the previous call.");
    start
    	time_var = KNOWNTIME + 1;
    	ret_time = ctime(&time_var);
    	actual_time = "Thu Jan  1 00:00:01 1970\n";
    
    	expecting (VOID);
    
    	if ( ( strcmp(ret_time, actual_time) ) != 0 )
    		error("Incorrect return value from ctime for zero+1 time = %s",
    			ret_time);
    finis
finis


funct ("localtime");
start
    
    testing("localtime(KNOWNTIME) should return");
    testing("a return value of 0 for tm_sec/min/hour.");
    start
    	expecting (SUCCESS);
    
    	time_var = KNOWNTIME;
    	ret_tm = localtime(&time_var);
    
    	if ( ret_tm->tm_sec != 0 )
    		error ("Incorrect return seconds from localtime of %d",
    			ret_tm->tm_sec);
    	if ( ret_tm->tm_min != 0 )
    		error("Incorrect return minutes from localtime of %d",
    			ret_tm->tm_min);
    	if ( ret_tm->tm_hour != 0)
    		error("Incorrect return hours from localtime of %d",
    			ret_tm->tm_hour);
    finis
    
    
    testing("localtime(KNOWNTIME+1) should return");
    testing("a return value of 1 for tm_sec.");
    start
    
    	expecting (SUCCESS);
    
    	time_var = KNOWNTIME + 1;
    	ret_tm = localtime(&time_var);
    
    	if ( ret_tm->tm_sec != 1 )
    		error("Incorrect return seconds from localtime of %d",
    			ret_tm->tm_sec);
    	if ( ret_tm->tm_min != 0 )
    		error("Incorrect return minutes from localtime of %d",
    			ret_tm->tm_min);
    	if ( ret_tm->tm_hour != 0)
    		error("Incorrect return hours from localtime of %d",
    			ret_tm->tm_hour);
    finis
finis
   

funct ("gmtime");
start
    
    testing("gmtime(KNOWNTIME) should return");
    testing("a return value of 0.");
    start
    	expecting (SUCCESS);
    
    	time_var = KNOWNTIME;
    	ret_tm = gmtime(&time_var);
    
    	if ( ret_tm->tm_sec != 0 )
    		error("Incorrect return seconds from gmtime of %d",
    			ret_tm->tm_sec);
    	if ( ret_tm->tm_min != 0 )
    		error("Incorrect return minutes from gmtime of %d",
    			ret_tm->tm_min);
    	if ( ret_tm->tm_hour != 0)
    		error("Incorrect return hours from gmtime of %d",
    			ret_tm->tm_hour);
    finis
    
    
    testing("gmtime(KNOWNTIME+1) should return");
    testing("a return value of 1 for tm_sec.");
    start
    	expecting (SUCCESS);
    
    	time_var = KNOWNTIME + 1;
    	ret_tm = gmtime(&time_var);
    
    	if ( ret_tm->tm_sec != 1 )
    		error("Incorrect return seconds from gmtime of %d",
    			ret_tm->tm_sec);
    	if ( ret_tm->tm_min != 0 )
    		error("Incorrect return minutes from gmtime of %d",
    			ret_tm->tm_min);
    	if ( ret_tm->tm_hour != 0)
    		error("Incorrect return hours from gmtime of %d",
    			ret_tm->tm_hour);
    finis
finis


funct ("asctime");
start

    testing("asctime(known_tm) should return");
    testing("a return value of 0.");
    start
    	pknown_tm = &known_tm;
    
    	pknown_tm->tm_sec = 0;
    	pknown_tm->tm_min = 0;
    	pknown_tm->tm_hour = 0;
    	pknown_tm->tm_mday = 1;
    	pknown_tm->tm_mon = 0;
    	pknown_tm->tm_year = 70;
    	pknown_tm->tm_wday = 4;
    	pknown_tm->tm_yday = 0;
    	pknown_tm->tm_isdst = 0;
    
    	expecting (SUCCESS);
    
    	ret_time = asctime(pknown_tm);
    	actual_time = "Thu Jan  1 00:00:00 1970\n";
    	expecting (VOID);
    
    	if ( ( strcmp(ret_time, actual_time) ) != 0 )
    		error ("Incorrect return value from asctime\nExpecting: %s\n\
    Actual: %s", actual_time, ret_time);
    finis
finis


testing("the external long variable timezone should exist and");
testing("the timezone must exist (no compile errors).");
start
finis


testing("the external int  variable daylight exists.");
testing("no compile time errors.");
start
finis


testing("If the external variable 'tzname' exists");
testing("then there will be no compile time errors.");
start
	save_tzname[1] = tzname[1];
	save_tzname[2] = tzname[2];
finis


funct ("tzset");
start

    testing("tzname to be set from TZ.");
    start
    	expecting (SUCCESS);
    
    	tzset();
    	ret_time = getenv("TZ");
    	expecting (VOID);
    
    	if ( ( strcmp(ret_time, tzname[0]) ) != 0 )
    		error ("Incorrect return value from timezone\n\
    'getenv' returned '%s' when '%s' was expected",
    			ret_time, tzname [0]);
    finis
    
    
    testing("Changing the variable TZ");
    testing("should cause tzname to be set from TZ.");
    start
    	expecting (SUCCESS);
    
    	putenv("TZ=PST8PDT");
    	tzset();
    	ret_time = getenv("TZ");
    	expecting (VOID);
    
    	if ( ( strcmp("PST", tzname[0]) ) != 0 )
    		error ("tzset() did not set tzname properly, tzname [0] = '%s'",
    			tzname [0]);
    	if ( ( strcmp("PDT", tzname[1]) ) != 0 )
    		error ("tzset() did not set tzname properly, tzname [1] = '%s'",
    			tzname [1]);
    finis
finis


done();
/*NOTREACHED*/

}
