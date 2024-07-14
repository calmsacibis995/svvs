/*LINTLIBRARY*/
#ifdef	UNDEF
#ident "@(#)sv_temppth.c	1.3"
#endif

/*
 * temp_path
 *
 *	Path manipulation procedure for SVVS.
 *	This procedure returns the full path spec of a file
 *	found in the SVVS temp directory area.
 *
 *	Assumes that the temp directory is one level up from the current 
 *	working directory
 */
char *sv_temp_path() {
	static char tempdir[ 32 ];

	return(tempdir);
}
