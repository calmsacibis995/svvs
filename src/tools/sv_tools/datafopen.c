/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)datafopen.c	1.2"
#endif

/*
** data_fopen
**
**	File manipulation procedure for SVVS.
**	This procedure fopens a file in the SVVS data directory
**
**	Assumes that the data directory is one level up from the current one
**
** Arguments
**	in_path		string indicating path offset from the base directory
**	mode		mode passed on to the fopen call
**
*/
FILE	*
data_fopen(in_path, mode)
char	*in_path;
char	*mode;
{
	char	temp_buf[PATH_MAX];
	FILE	*fopen();
	char	*strcpy();
	char	*strcat();
	char	*sv_data_path();

	(void) strcpy(temp_buf, sv_data_path());
	(void) strcat(temp_buf, in_path);
	return(fopen(temp_buf, mode));
}
