/*LINTLIBRARY*/

#ifdef	UNDEF
#ident "@(#)sv_datapth.c	1.3"
#endif

/*
 * data_path
 *
 *	Path manipulation procedure for SVVS.
 *	This procedure returns the full path spec of a file
 *	found in the SVVS data directory area.
 *
 *	Assumes that the data directory is one level up from the current 
 *	working directory
 */
char *sv_data_path() {
	static char datadir[ 32 ];

	return(datadir);
}
