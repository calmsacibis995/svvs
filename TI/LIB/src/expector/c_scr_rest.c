# include "main.h"
# include <stdio.h>
# include <fcntl.h>

SCCSID("@(#)c_scr_rest.c	3.2 (SVID) 12/23/86");

c_scrrestore(filename)
char *filename;
{
	char *newfile;
	extern TERMINAL *saved_scr;

	if ( (newfile = malloc((unsigned) strlen(filename) + 2)) == (char *) NULL )
	{
		screech("c_scrrestore",0,"ERROR: malloc of newfile failed\n");
	}

	(void) strcpy(newfile,filename);
	(void) strcat(newfile,"_");

	restorescr(newfile,DISPLAY,TRUE);
	return(OK);
}

restorescr(filename,display,real_call)
char		*filename;
TERMINAL	*display;
BOOLEAN		real_call;
{
	FILE		*fopen(), *fp;
 	int		i,j, fd, pos, numread;
	int		dummy1, dummy2;
	extern long	lseek();
/*
** This routine fills the screen (display) with the contents of the
** file "filename", which was filled by c_scrdump.
*/

	if (real_call)
	{
		(void) c_wclear(display);
	}

	if ( (fp = fopen(filename,"r")) == (FILE *) NULL )
	{
		screech("c_scrrestore",0,"ERROR in c_scrrestore: fopening %s failed\n",filename);
	}

/*
** Here, we simply fill "display" with the characters of the file "filename"
*/

	for (j = 0; j < display->term_height;   j++)
	{
		for (i = 0; i < display->term_width; i++)
		{
			if ((pos = getc(fp)) == EOF)
			{
				screech("c_scrrestore",0,"c_scrrestore: ERROR--premature EOF encountered while reading chars\n");
			}
			display->term_screen[j][i] = pos;
			if (real_call)
			{
				saved_scr->term_screen[j][i] = display->term_screen[j][i];
			}
		}

		if ((pos = getc(fp)) == EOF)
		{
			screech("c_scrrestore", 0,
				"ERROR--premature EOF encountered while reading chars\n");
		}

		if (pos != '\n')
		{
			screech("c_scrrestore", 0,
				"Expected '\\n'; got '\\%-03d'\n", pos);
		}
	}


/*
** We then fill the term_bits field of "display" with the rest of the
** dump file.
*/

	for (j = 0; j < display->term_height;   j++)
	{
		numread = fscanf(fp,"LINE %d\n",&dummy1);
		if (dummy1 != j)
		{
			screech("c_scrrest", 1,
				"Lines mismatch. dummy1 = %d; j = %d; i = %d\n", dummy1, j, i);
		}

		for (i = 0; i < display->term_width; i++)
		{
			numread = fscanf(fp," 0X%x",&(display->term_bits[j][i]));
			if ((numread < 0) || (numread == EOF))
			{
				screech("c_scrrest",1,"reading from file. numread = %d; j = %d; i = %d\n", numread, j, i);
			}
# ifdef TRACE
			if (Dbg)
			{
				(void) fprintf(stderr,"c_scrrestore: display->term_bits[%d][%d] = '%ld'\n",j,i,display->term_bits[j][i]);
			}
# endif	/* TRACE */

			if (real_call)
			{
				saved_scr->term_bits[j][i] = display->term_bits[j][i];
			}

			if ((i % 10) == 9)
			{
				fscanf(fp, "\n");
			}
		}
	}

	if (real_call)
	{
		(void) c_wnoutrefresh(display);
	}

	if (fclose(fp) == EOF)
	{
		screech("c_scr_restore",1,"c_scrrestore: attempted to close file\n");
	}
	
	return(OK);
}
