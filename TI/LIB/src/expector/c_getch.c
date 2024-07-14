# include "main.h"
# include <malloc.h>
# include <curses.h>
# include <search.h>
# include <ctype.h>
# include <unctrl.h>

SCCSID("@(#)c_getch.c	3.1 (SVID) 12/1/86");

# define TABSIZE	152	

/*
** "fkeytab" contains ordered pairs of a termcap-namecode for a function
** key and the value for that key. It's used by c_gtch for returning 
** values
*/

static struct keytab {
	char name[2];
	int value;
	} fkeytab[] = {
		"@@",0401,"kd",0402,"ku",0403,"kl",0404,"kr",0405,"kh",0406,
		"kb",0407,"k0",0410,"k1",0411,"k2",0412,"k3",0413,"k4",0414,
		"k5",0415,"k6",0416,"k7",0417,"k8",0420,"k9",0421,"ka",0422,
		"F1",0423,"F2",0424,"F3",0425,"F4",0426,"F5",0427,"F6",0430,
		"F7",0431,"F8",0432,"F9",0433,"FA",0434,"FB",0435,"FC",0436,
		"FD",0437,"FE",0440,"FF",0441,"FG",0442,"FH",0443,"FI",0444,
		"FJ",0445,"FK",0446,"FL",0447,"FM",0450,"FN",0451,"FO",0452,
		"FP",0453,"FQ",0454,"FR",0455,"FS",0456,"FT",0457,"FU",0460,
		"FV",0461,"FW",0462,"FX",0463,"FY",0464,"FZ",0465,"Fa",0466,
		"Fb",0467,"Fc",0470,"Fd",0471,"Fe",0472,"Ff",0473,"Fg",0474,
		"Fh",0475,"Fi",0476,"Fj",0477,"Fk",0500,"Fl",0501,"Fm",0502,
		"Fn",0503,"Fo",0504,"Fp",0505,"Fq",0506,"Fr",0507,"kL",0510,
		"kA",0511,"kD",0512,"kI",0513,"ei",0514,"kC",0515,"kS",0516,
		"kE",0517,"kF",0520,"kR",0521,"kN",0522,"kP",0523,"kT",0524,    
		"kt",0525,"k;",0526,"@8",0527,"@@",0530,"@@",0531,"%9",0532,
		"kH",0533,"K1",0534,"K3",0535,"K2",0536,"K4",0537,"K5",0540,
		"kB",0541,"@1",0542,"@2",0543,"@3",0544,"@4",0545,"@5",0546,
		"@6",0547,"@7",0550,"@9",0551,"@0",0552,"%1",0553,"%2",0554,
		"%3",0555,"%4",0556,"%5",0557,"%6",0560,"%7",0561,"%8",0562,
		"%0",0563,"&1",0564,"&2",0565,"&3",0566,"&4",0567,"&5",0570,
		"&6",0571,"&9",0572,"&0",0573,"*1",0574,"*2",0575,"*3",0576,
		"*4",0577,"*5",0600,"*6",0601,"*7",0602,"*8",0603,"*9",0604,
		"*0",0605,"#1",0606,"#2",0607,"#3",0610,"#4",0611,"%a",0612,
		"%b",0613,"%c",0614,"%d",0615,"%e",0616,"%f",0617,"%g",0620,
		"%h",0621,"%i",0622,"%j",0623,"!1",0624,"!2",0625,"!3",0626,
		"&7",0627,"&8",0630
		};

typedef struct keytab KEYTAB;

int compar(str1,str2) 
char *str1, *str2;
{
/*
** This routine is used by the bsearch routine, provided by the system.
** It just compares the two strings lexicographically.
*/

	if ((str1 == NULL) && (str2 == NULL))
	{
		return(0);
	}
	else if (str1 == NULL)
	{
		return(-1);
	}
	else if (str2 == NULL)
	{
		return(1);
	}
	else
	{
		return(strcmp(str1,str2));
	}
}

	
c_gtch()	/* catcher's getch */
{
	chtype ch;

	ch = c_wchget(std);

	return(ch);
}

c_wchget(win)	/* catcher's wgetch */
TERMINAL *win;
{
	char ch, *tmp;
	KEYTAB *result;
	COMM *comm;
	extern char *bsearch();


	/*
	** TypeAhead contains all unread keyboard input. Since at each call to
	** "wchget", there may be new input (i.e., KB_Buffer is non-empty), we start
	** appending to TypeAhead the new input, if any.
	*/

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"TypeAhead = %x \n",TypeAhead);
		(void) fprintf(stderr,"win's address is %x \n",win);
	}
# endif	/* TRACE */

	if (KB_Buffer)
	{
		if (TypeAhead)
		{
			TypeAhead = realloc(TypeAhead,(unsigned)(strlen(KB_Buffer) + strlen(TypeAhead) + 1));
			die(TypeAhead, "c_wgetch: realloc of TypeAhead failed\n");
		}
		else
		{
			TypeAhead = malloc((unsigned) (strlen(KB_Buffer) + 1));
			die(TypeAhead, "c_wgetch: malloc of TypeAhead failed\n");
			*TypeAhead = '\0';
		}

		if (strlen(TypeAhead) == 0)
		{
			(void) strcpy(TypeAhead,KB_Buffer);
		}
		else
		{
			(void) strcat(TypeAhead,KB_Buffer);
		}
		free(KB_Buffer);
		KB_Buffer = NULL;
	}
	else if (!TypeAhead)
	{
		return(EOF);
	}

# ifdef TRACE
	if (Dbg)
	{
		printf("TypeAhead = '%s'\n",TypeAhead);
	}
# endif	/* TRACE */

/*
** If the character being read is an escape character, we check the
** rest of the input to see if we have an escape sequence.
*/

	if ((win->keypad_on) && (!((isascii(*TypeAhead) && isprint(*TypeAhead)) || (*TypeAhead == '\n') || (*TypeAhead == '\t') || (*TypeAhead == '\0'))))
	{
# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"possible escape sequence found\n");
		}
# endif	/* TRACE */

	/*
	** "find_command" (from bless) returns, if possible, a structure providing
	** information about the command sequence that was passed as input, if it
	** was indeed a command sequence.
	*/

		comm = find_command(TypeAhead,strlen(TypeAhead));
	
		if (comm)
		{
# ifdef TRACE
			if (Dbg)
			{
				(void) fprintf(stderr,"command found: `%s`\n",comm->comm_name);
			}
# endif	/* TRACE */

			tmp = malloc((unsigned) (strlen(TypeAhead)));
			die(tmp,"c_wgetch: malloc of tmp failed\n");
			
			(void) strcpy(tmp,TypeAhead + strlen(comm->comm_string));
			(void) strcpy(TypeAhead,tmp);
			free(tmp);

/*
** We put fkeytab in alphabetical order so it can be searched
*/

			qsort((char *) fkeytab,(unsigned) TABSIZE,(unsigned) sizeof(KEYTAB),compar);

/*
** If the command sequence is present in the fkeytab list, we return
** its numeric value.
*/

			result = (KEYTAB *)bsearch(comm->comm_name,(char *)fkeytab,TABSIZE,sizeof(KEYTAB),compar);
			if (result)
			{
# ifdef TRACE
				if (Dbg)
				{
					(void) fprintf(stderr,"character value of escape sequence = '%s'\n",result->name);
					(void) fprintf(stderr,"numeric value of escape sequence = '%o'\n",result->value);
				}
# endif	/* TRACE */
				
				if (Echo_on)
				{
					if (!(win->is_a_pad))
					{
						(void) c_wrefresh(win);
					}
				}

				return(result->value);
			}
			else
			{
				screech("c_getch",0,"ERROR in getch: undefined command read in\n");
			}
		}
	}

	ch = *TypeAhead;

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"strlen(TypeAhead) = %d \n",strlen(TypeAhead));
	}
# endif	/* TRACE */

/*
** Here, we lop the character read in from the input, getting it ready to
** make available the next input character.
*/

	tmp = malloc((unsigned) (strlen(TypeAhead)));
	die(tmp,"c_wgetch: malloc of tmp failed\n");

	(void) strcpy(tmp,TypeAhead + 1);
	(void) strcpy(TypeAhead,tmp);

	free(tmp);

	if (Echo_on)
	{
# ifdef TRACE
		if (Dbg)
		{
			printf("echoing char '%c'\n",ch);
		}
# endif	/* TRACE */

		if (!(win->is_a_pad))
		{
			(void) c_wchadd(win,((chtype) ch));
			(void) c_wrefresh(win);
		}
	}

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"TypeAhead = %x \n",TypeAhead);
	}
# endif	/* TRACE */

	return(ch);
}

c_getchmv(yvar,xvar)	/* catcher's version of mvgetch */
VAR_ENTRY *yvar, *xvar;
{
		(void) c_move(yvar,xvar);

	return(c_gtch());

}

c_chgetmvw(winvar,yvar,xvar)	/* catcher's version of mvwgetch */
VAR_ENTRY *winvar, *yvar, *xvar;
{
	TERMINAL *win;
	int y,x;

		win = winvar->var_data.var_term;
		y = yvar->var_data.var_num;
		x = xvar->var_data.var_num;

		(void) c_wmove(win,y,x);

	return(c_wchget(win));
}

