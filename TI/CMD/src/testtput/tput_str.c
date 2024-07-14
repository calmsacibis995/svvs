# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sccs.h>
# include <svvs.h>
# include <sv_env.h>

# define STRSIZE	256

SCCSID("@(#)tput_str.c	3.1 (SVVS) 12/1/86");

/*
** tput_str
**	Given a string capability, run the command
**	"tput string > tmpfile". Read whatever data
**	is in "tmpfile", and return that data.
**
** Arguments
**	cap	The string with the requested capability.
**
** Returns
**	NULL	on any error, or if no output.
**	(char *)	of the output from tput
*/
char *
tput_str(term, cap, parms)
char *term;
char *cap;
char *parms;
{
	int	count;
	FILE	*fp;
	char	filename[FILESIZ + 1];
	char	system_str[FILESIZ + 1];
	struct	stat	st;
	static	char	buf[STRSIZE];

	start
	(void) sprintf(filename, "%stput", temp_path());

	if (*term == '-')
		(void) sprintf(system_str, "tput %s %s %s > %s", term, cap,
			parms, filename);
	else
		(void) sprintf(system_str, "%s tput %s %s > %s", term, cap,
			parms, filename);

	(void) system(system_str);

	if ( stat(filename,&st) == -1 )
	{
		sv_error("stat(%s) failed",filename);
		return ( (char *) NULL );
	}

	if ( st.st_size >= sizeof (buf) )
	{
		error("tput(%s) returned a string %d bytes long. Only %d allowed.",st.st_size,sizeof (buf) - 1);
		return ( (char *) NULL);
	}

	/*
	** If the file is empty, nothing was printed. Just return NULL
	*/
	if ( st.st_size == 0 )
		return ( (char *) NULL );

	if ( (fp = fopen(filename,"r")) == (FILE *) NULL )
	{
		sv_error("fopen(%s,\"r\") failed",filename);
		return ((char *) NULL);
	}

	if ( (count = fread(buf,1,st.st_size,fp)) != st.st_size )
	{
		sv_error("fread(buf,1,%d,fp) returned %d",count);
		return ( (char *) NULL);
	}

	(void) fclose(fp);
	buf[st.st_size] = '\0';
	return ( buf );

	finis
}
