/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)tempcreat.c	1.3"
#endif

/*
** temp_creat
**
**	Path manipulation procedure for SVVS.
**	This procedure create a file in the SVVS temp directory
**
**	Assumes that the temp directory is one level up from the current one
**
** Arguments
**	in_path		string indicating path offset from the base directory
**	mode	passed on the the creat call
**
*/

temp_creat(in_path, mode)

char	*in_path;
int	mode;

{
	char	*strcat();
	char	*strcpy();
	char	*sv_temp_path();
	char	temp_buf[PATH_MAX];

	(void) strcpy(temp_buf, sv_temp_path());
	(void) strcat(temp_buf, in_path);
	return(creat(temp_buf, sv_mode(mode)));
}
