/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)dataopen.c	1.2"
#endif


/*
** data_open
**
**	Path manipulation procedure for SVVS.
**	This procedure opens a file in the SVVS data directory
**
**	Assumes that the data directory is one level up from the current one
**
** Arguments
**	in_path		string indicating path offset from the base directory
**	oflag, mode	passed on the the open call
**
*/

data_open(in_path, oflag, mode)
char	*in_path;
int	oflag, mode;
{
	char	temp_buf[PATH_MAX];
	char	*strcpy();
	char	*strcat();
	char	*sv_data_path();

	(void) strcpy(temp_buf, sv_data_path());
	(void) strcat(temp_buf, in_path);
	return(open(temp_buf, oflag, mode));
}
