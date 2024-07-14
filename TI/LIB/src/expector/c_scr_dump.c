# include "main.h"
# include <stdio.h>
# include <fcntl.h>

SCCSID("@(#)c_scr_dump.c	3.2 (SVID) 12/23/86");

extern int Dbg; 
int c_scrdump(filename)
char *filename;
{
	char *newfile;

	newfile = malloc((unsigned) strlen(filename) + 2);
	die(newfile,"c_scr_dump: malloc of newfile (string) failed\n");

/*
** We append an underscore to the filename so that expector
** will have a separate copy of the file apart from bless's.
*/
	(void) strcpy(newfile,filename);
	(void) strcat(newfile,"_"); 

	dumpscr(newfile,DISPLAY);
	return(OK);
}

int dumpscr(filename,display) 
char *filename;
TERMINAL *display;
{

/*
** This routine copies the contents of "display" (which represents the screen)
** to the file "filename", taking care to save the attributes of the screen
** as well as the characters.
*/

	int i,j, fd;
	FILE *fopen(), *fp;
	extern long lseek();

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"filename is '%s'\n",filename);
	}
# endif	/* TRACE */

	if ( (fp = fopen(filename,"w+")) == (FILE *) NULL )
	{
		screech("c_scrdump",0,"ERROR in c_scr_dump: fopening %s\n",filename);
	}
	
# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"filename's address is %x\n",fp);
	}
# endif /* TRACE */

	/*
	** Here, we simply copy the entire contents of "display" to the file
	** "filename"
	*/

	for (j = 0; j < display->term_height; j++)
	{
		for (i = 0; i < display->term_width; i++)
		{
			if ( putc(display->term_screen[j][i],fp) == EOF )
			{
				screech("c_scrdump",0,"ERROR in c_scrdump: putc of char value failed\n");
			}
		}

		putc('\n',fp);

# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"display->term_bits[%d][%d] = '%ld'\n",j,0,display->term_bits[j][0]);
		}
# endif	/* TRACE */
	}

	for (j = 0; j < display->term_height; j++)
	{
		fprintf(fp,"LINE %d\n",j);

		for (i = 0; i < display->term_width; i++)
		{
			if ( fprintf(fp," 0X%x",display->term_bits[j][i]) == -1)
			{
				screech("c_scrdump",1,"ERROR in c_scrdump: fprintf of char attributes failed\n");
			}

			if ((i % 10) == 9)
			{	
				fprintf(fp, "\n");
			}
		}

		if ((i % 10) != 0)
		{
			fprintf(fp, "\n");
		}
	}

	fflush(fp);

	if ( fclose(fp) == EOF )
	{
		screech("c_scrdump",1,"ERROR in c_scrdump: close failed\n");
	}

	return(OK);
}

