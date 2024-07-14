/*LINTLIBRARY*/
#include <stdio.h>
#include <svvs.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)pshpop.c	1.2"
#endif

char *strcpy(),*strcat();

static	int	Dup_stdout = -1;	/* Duplicate of stdout fd */
/*
** push_stdout
**	This redirects stdout to a file, but saves a copy
**	of the file descriptor attached to stdout so we can
**	restore it later.
**
** Arguments
**	file	Filename to open stdout to
**	mode	What mode to open the file to
*/
push_stdout(file,mode)
char	*file;
char	*mode;
{
	char	pathname[STRLEN];
	char	*sv_temp_path();

	if ( (Dup_stdout = dup(fileno(stdout))) == -1 )
		severr("dup of fileno(stdout) failed");

	(void) strcpy(pathname,sv_temp_path());
	(void) strcat(pathname,file);
	if ( freopen(pathname,mode,stdout) == NULL )
	{
		(void) fclose(stdout);
		(void) fdopen(Dup_stdout,"w");
/*
 *		probably can't get message out, but try anyway
 *		in any event, will generate a severr which should
 *		have some effect
 */
		severr("push_stdout: could not freopen(stdout)");
	}
}

/*
** pop_stdout
**	Reset stdout back to what it used to point to.
**
** Assumes
**	You want stdout opened for writing...
*/
pop_stdout()
{
	
	if ( Dup_stdout == -1 )
		severr("pop_stdout: push_stdout never called");
	(void) fclose(stdout);
	if ( dup(Dup_stdout) != 1 )
		severr("pop_stdout: dup of stdout back to 1 failed");
	if ( fdopen(1,"w") == NULL )
		severr("pop_stdout: couldn't fdopen stdout");
	(void) close(Dup_stdout);
}
