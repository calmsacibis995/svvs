/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)tempopen.c	1.2"
#endif

/*
** temp_open
**
**	Path manipulation procedure for SVVS.
**	This procedure opens a file in the SVVS temp directory
**
**	Assumes that the temp directory is one level up from the current one
**
** Arguments
**	in_path		string indicating path offset from the base directory
**	oflag, mode	passed on the the open call
**
*/

temp_open(in_path, oflag, mode)
char	*in_path;
int	oflag, mode;
{
	char	temp_buf[PATH_MAX];
	char	*strcpy();
	char	*strcat();
	char	*sv_temp_path();

	(void) strcpy(temp_buf, sv_temp_path());
	(void) strcat(temp_buf, in_path);
	return(open(temp_buf, oflag, mode));
}
