# include "main.h"
# include <stdio.h>
# include <fcntl.h>

SCCSID("@(#)c_scr_init.c	3.1 (SVID) 12/1/86");

c_scrinit(filename)
char *filename;
{
	FILE *fopen(), *fp;
 	int i,j, fd, pos, numread;
	char *newfile;
	extern TERMINAL *saved_scr;
	extern long lseek();
/*
** This routine fills the screen (DISPLAY) with the contents of the
** file "filename" (which was filled by c_scrdump) and then initializes
** the screen's data structure to give the impression that no change
** in screen has occurred.
*/

	(void) c_wclear(DISPLAY);

	if ( (newfile = malloc((unsigned) strlen(filename) + 2)) == (char *) NULL )
	{
		screech("c_scrinit",0,"ERROR: malloc of newfile failed\n");
	}

	(void) strcpy(newfile,filename);
	(void) strcat(newfile,"_");

	if ( (fp = fopen(newfile,"r")) == (FILE *) NULL )
	{
		screech("c_scrinit",0,"ERROR in c_scrinit: fopening %s failed\n",newfile);
	}

/*
** Here, we simply fill DISPLAY with the characters of the file "filename"
*/

	for (j = 0; j < DISPLAY->term_height;   j++)
	{
		for (i = 0; i < DISPLAY->term_width; i++)
		{
			if ((pos = getc(fp)) == EOF)
			{
				screech("c_scrinit",0,"c_scrinit: ERROR--premature EOF encountered while reading chars\n");
			}

			DISPLAY->term_screen[j][i] = pos;
			DISPLAY->term_lastrefresh[j][i] = pos;

			saved_scr->term_screen[j][i] = DISPLAY->term_screen[j][i];
			saved_scr->term_lastrefresh[j][i] = DISPLAY->term_lastrefresh[j][i];
		}
	}

	if (fclose(fp) == EOF)
	{
		screech("c_scr_init",0,"ERROR in c_scrinit:fclose failed\n");
	}

/*
** We then fill the term_bits field of DISPLAY with the rest of the
** file "filename". We close and re-open the file because here we are
** reading and not getc'ing.
*/

	if ((fd = open(newfile,O_RDONLY,0)) == -1)
	{
		screech("c_scr_init",1,"ERROR in c_scrinit: opening %s failed\n", newfile);
	}

	if ( lseek(fd,(long) (DISPLAY->term_width) * (DISPLAY->term_height),0) == -1L)
	{
		screech("c_scr_init",1,"ERROR in c_scrinit: seeking middle of file\n");
	}

	for (j = 0; j < DISPLAY->term_height;   j++)
	{
		for (i = 0; i < DISPLAY->term_width; i++)
		{
			numread = read(fd,(char *) (&(DISPLAY->term_bits[j][i])),sizeof(long));
			if (numread == -1)
			{
				screech("c_scrinit",1,"c_scrinit: reading from file\n");
			}
# ifdef TRACE
			if (Dbg)
			{
				(void) fprintf(stderr,"c_scrinit: DISPLAY->term_bits[%d][%d] = '%ld'\n",j,i,DISPLAY->term_bits[j][i]);
			}
# endif	/* TRACE */

			DISPLAY->term_lastbits[j][i] = DISPLAY->term_bits[j][i];
			saved_scr->term_bits[j][i] = DISPLAY->term_bits[j][i];				 
			saved_scr->term_lastbits[j][i] = saved_scr->term_bits[j][i];
		}
	}

	(void) c_wnoutrefresh(DISPLAY);

	if (close(fd) == -1)
	{
		screech("c_scr_init",1,"c_scrinit: attempted to close file\n");
	}
	
	return(OK);
}

