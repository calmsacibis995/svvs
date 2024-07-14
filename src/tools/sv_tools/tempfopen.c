/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)tempfopen.c	1.2"
#endif

/*
** temp_fopen
**
**	File manipulation procedure for SVVS.
**	This procedure fopens a file in the SVVS temp directory
**
**	Assumes that the temp directory is one level up from the current one
**
** Arguments
**	in_path		string indicating path offset from the base directory
**	mode		mode passed on to the fopen call
**
*/
FILE	*
temp_fopen(in_path, mode)
char	*in_path;
char	*mode;
{
	char	temp_buf[PATH_MAX];
	FILE	*fopen();
	char	*strcpy();
	char	*strcat();
	char	*sv_temp_path();

	(void) strcpy(temp_buf, sv_temp_path());
	(void) strcat(temp_buf, in_path);
	return(fopen(temp_buf, mode));
}
