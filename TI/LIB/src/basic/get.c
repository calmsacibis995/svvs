# include <stdio.h>
# include <ctype.h>
# include <svvs.h>
# include <sv_env.h>
# include <malloc.h>
# include <string.h>
# include <sccs.h>
# include "basiccap.h"

SCCSID("@(#)get.c	3.1 (SVVS) 12/1/86");

char *Ztigetstr();
char *Ztgetstr();

/*
**	tnames maps all the terminfo capability names to the
**	corresponding termcap capability name
*/
char *tnames[MAXTNAMES][2] = {
	"acsc", "ac",
	"am", "am",
	"bel", "bl",
	"blink", "mb",
	"bold", "md",
	"bw", "bw",
	"cbt", "bt",
	"cdmch", "CC",
	"chts", "HC",
	"civis", "vi",
	"clear", "cl",
	"cnorm", "ve",
	"cols", "co",
	"cr", "cr",
	"csr", "cs",
	"cub", "LE",
	"cub1", "le",
	"cud", "DO",
	"cud1", "do",
	"cuf", "RI",
	"cuf1", "nd",
	"cup", "cm",
	"cuu", "UP",
	"cuu1", "up",
	"cvvis", "vs",
	"da", "da",
	"db", "db",
	"dch", "DC",
	"dch1", "dc",
	"dim", "mh",
	"dl", "DL",
	"dl1", "dl",
	"dsl", "ds",
	"ech", "ec",
	"ed", "cd",
	"el", "ce",
	"el1", "cb",
	"enacs", "eA",
	"eo", "eo",
	"eslok", "es",
	"ff", "ff",
	"flash", "vb",
	"fsl", "fs",
	"gn", "gn",
	"hc", "hc",
	"hd", "hd",
	"home", "ho",
	"hpa", "ch",
	"hs", "hs",
	"ht", "ta",
	"hts", "st",
	"hu", "hu",
	"hz", "hz",
	"ich", "IC",
	"ich1", "ic",
	"if", "if",
	"il", "AL",
	"il1", "al",
	"in", "in",
	"ind", "sf",
	"indn", "SF",
	"invis", "mk",
	"ip", "ip",
	"iprog", "iP",
	"is1", "i1",
	"is2", "is",
	"is3", "i3",
	"it", "it",
	"kBEG", "&9",
	"kCAN", "&0",
	"kCMD", "*1",
	"kCPY", "*2",
	"kCRT", "*3",
	"kDC", "*4",
	"kDL", "*5",
	"kEND", "*7",
	"kEOL", "*8",
	"kEXT", "*9",
	"kFND", "*0",
	"kHLP", "#1",
	"kHOM", "#2",
	"kIC", "#3",
	"kLFT", "#4",
	"kMOV", "%b",
	"kMSG", "%a",
	"kNXT", "%c",
	"kOPT", "%d",
	"kPRT", "%f",
	"kPRV", "%e",
	"kRDO", "%g",
	"kRES", "%j",
	"kRIT", "%i",
	"kRPL", "%h",
	"kSAV", "!1",
	"kSPD", "!2",
	"kUND", "!3",
	"ka1", "K1",
	"ka3", "K3",
	"kb2", "K2",
	"kbeg", "@1",
	"kbs", "kb",
	"kc1", "K4",
	"kc3", "K5",
	"kcan", "@2",
	"kcbt", "kB",
	"kclo", "@3",
	"kclr", "kC",
	"kcmd", "@4",
	"kcpy", "@5",
	"kcrt", "@6",
	"kctab", "kt",
	"kcub1", "kl",
	"kcud1", "kd",
	"kcuf1", "kr",
	"kcuu1", "ku",
	"kdch1", "kD",
	"kdl1", "kL",
	"ked", "kS",
	"kel", "kE",
	"kend", "@7",
	"kent", "@8",
	"kext", "@9",
	"kf0", "k0",
	"kf1", "k1",
	"kf10", "ka",
	"kf11", "F1",
	"kf12", "F2",
	"kf13", "F3",
	"kf14", "F4",
	"kf15", "F5",
	"kf16", "F6",
	"kf17", "F7",
	"kf18", "F8",
	"kf19", "F9",
	"kf2", "k2",
	"kf20", "FA",
	"kf21", "FB",
	"kf22", "FC",
	"kf23", "FD",
	"kf24", "FE",
	"kf25", "FF",
	"kf26", "FG",
	"kf27", "FH",
	"kf28", "FI",
	"kf29", "FJ",
	"kf3", "k3",
	"kf30", "FK",
	"kf31", "FL",
	"kf32", "FM",
	"kf33", "FN",
	"kf34", "FO",
	"kf35", "FP",
	"kf36", "FQ",
	"kf37", "FR",
	"kf38", "FS",
	"kf39", "FT",
	"kf4", "k4",
	"kf40", "FU",
	"kf41", "FV",
	"kf42", "FW",
	"kf43", "FX",
	"kf44", "FY",
	"kf45", "FZ",
	"kf46", "Fa",
	"kf47", "Fb",
	"kf48", "Fc",
	"kf49", "Fd",
	"kf5", "k5",
	"kf50", "Fe",
	"kf51", "Ff",
	"kf52", "Fg",
	"kf53", "Fh",
	"kf54", "Fi",
	"kf55", "Fj",
	"kf56", "Fk",
	"kf57", "Fl",
	"kf58", "Fm",
	"kf59", "Fn",
	"kf6", "k6",
	"kf60", "Fo",
	"kf61", "Fp",
	"kf62", "Fq",
	"kf63", "Fr",
	"kf7", "k7",
	"kf8", "k8",
	"kf9", "k9",
	"kfnd", "@0",
	"khlp", "%1",
	"khome", "kh",
	"khts", "kT",
	"kich1", "kI",
	"kil1", "kA",
	"kind", "kF",
	"kll", "kH",
	"km", "km",
	"kmov", "%4",
	"kmrk", "%2",
	"kmsg", "%3",
	"knp", "kN",
	"knxt", "%5",
	"kopn", "%6",
	"kopt", "%7",
	"kpp", "kP",
	"kprt", "%9",
	"kprv", "%8",
	"krdo", "%0",
	"kref", "&1",
	"kres", "&5",
	"krfr", "&2",
	"kri", "kR",
	"krmir", "kM",
	"krpl", "&3",
	"krst", "&4",
	"ksav", "&6",
	"kslt", "*6",
	"kspd",	"&7",
	"ktbc", "k;",
	"kund", "&8",
	"lf0", "l0",
	"lf1", "l1",
	"lf10", "la",
	"lf2", "l2",
	"lf3", "l3",
	"lf4", "l4",
	"lf5", "l5",
	"lf6", "l6",
	"lf7", "l7",
	"lf8", "l8",
	"lf9", "l9",
	"lh", "lh",
	"lines", "li",
	"ll", "ll",
	"lm", "lm",
	"lw", "lw",
	"mc0", "ps",
	"mc4", "pf",
	"mc5", "po",
	"mc5i", "5i",
	"mc5p", "pO",
	"mgc", "MC",
	"mir", "mi",
	"mrcup", "CM",
	"msgr", "ms",
	"nel", "nw",
	"nlab", "Nl",
	"npc", "NP",
	"nrrmc", "NR",
	"nxon", "nx",
	"os", "os",
	"pad", "pc",
	"pb", "pb",
	"pfkey", "pk",
	"pfloc", "pl",
	"pfx", "px",
	"pln", "pn",
	"prot", "mp",
	"rc", "rc",
	"rep", "rp",
	"rev", "mr",
	"rf", "rf",
	"rfi", "RF",
	"ri", "sr",
	"rin", "SR",
	"rmacs", "ae",
	"rmam", "RA",
	"rmcup", "te",
	"rmdc", "ed",
	"rmir", "ei",
	"rmkx", "ke",
	"rmln", "LF",
	"rmm", "mo",
	"rmp", "rP",
	"rmso", "se",
	"rmul", "ue",
	"rmxon", "RX",
	"rs1", "r1",
	"rs2", "r2",
	"rs3", "r3",
	"sc", "sc",
	"sgr", "sa",
	"sgr0", "me",
	"smacs", "as",
	"smam", "SA",
	"smcup", "ti",
	"smdc", "dm",
	"smgl", "ML",
	"smgr", "MR",
	"smir", "im",
	"smkx", "ks",
	"smln", "LO",
	"smm", "mm",
	"smso", "so",
	"smul", "us",
	"smxon", "SX",
	"tbc", "ct",
	"tsl", "ts",
	"uc", "uc",
	"ul", "ul",
	"vpa", "cv",
	"vt", "vt",
	"wind", "wi",
	"wsl", "ws",
	"xenl", "xn",
	"xhp", "xs",
	"xmx", "sg",
	"xoffc", "XF",
	"xon", "xo",
	"xonc", "XN",
	"xsb", "xb",
	"xt", "xt",
};

/*
**	get() calls the routines that get the capability
**	info from the terminfo database
*/
get(tptr)
struct cap *tptr;
{
	char escstr[CAPSIZ];
	char *capstr;
	int numcap;
	int num;

	expecting(SUCCESS);
	/*
	** if cap_string is NULL, there is no associated string for
	** the capability and it must be a boolean capability
	*/
	if (tptr->cap_string[0] == '\0')
	{
		expecting(VOID);
		if ((num = Ztigetflag(tptr->cap_name)) == -1)
			error("tigetflag(%s) returned error", tptr->cap_name);
		else if (!num)
			error("tigetflag(%s) failed.  expected 1 and tigetflag returned 0.", tptr->cap_name);
	}

	/* if the cap_string is numeric, call tigetnum... */
	else if (isnum(tptr->cap_string))
	{
		num = atoi(tptr->cap_string);
		expecting(VOID);
		if ((numcap = Ztigetnum(tptr->cap_name)) == -2)
			error("tigetnum(%s) returned error", tptr->cap_name);
		else if (numcap != num)
			error("tigetnum(%s) failed, expected '%s', got '%s'", tptr->cap_name, num, numcap);
	}

	/* isn't boolean or numeric, so it must be a string capability */
	else
	{
		expecting(VOID);
		if ((capstr = Ztigetstr(tptr->cap_name)) == (char *) -1)
			error("tigetstr(%s) returned error", tptr->cap_name);
		else if (strcmp(tptr->cap_string, capstr) != 0)
		{
			/*
			** if the strings were not equal, it is possible
			** that the string from the terminfo description
			** (tptr->cap_string) needs to have its escape
			** sequences interpreted - esccmp does this
			*/
			if (esccmp(capstr, tptr->cap_string, escstr) != 0)
				error("tigetstr(%s) failed, expected '%s', got '%s'", tptr->cap_name, escstr, capstr);
		}
	}
}

/*
**	getcap() locates the appropriate termcap capability name in
**	the table tnames and calls the routines to get the data from
**	the terminfo database
*/
getcap(tptr)
struct cap *tptr;
{
	char escstr[CAPSIZ];
	char *capstr;
	char termcap[3];
	char *worksp;
	char *savewksp;
	char **area = &worksp;
	int numcap;
	int num;

	/*
	** nmchg() finds the name of the corresponding termcap
	** capability and returns it in termcap.
	*/
	if (!nmchg(tptr->cap_name, termcap))
		error("%s capability not found", tptr->cap_name);

	/*
	** expect VOID 'cos termcap routines don't return the values
	** that SVVS expects for SUCCESS and FAILURE.
	*/
	expecting(VOID);

	/*
	** if cap_string is NULL, there is no associated string for
	** the capability and it must be a boolean capability
	*/
	if (tptr->cap_string[0] == '\0')
	{
		if (!(num = Ztgetflag(termcap)))
			error("tgetflag(%s) failed", termcap);
	}

	/* if cap_string is numeric... */
	else if (isnum(tptr->cap_string))
	{
		num = atoi(tptr->cap_string);
		numcap = Ztgetnum(termcap);
		if (numcap != num)
			error("tgetnum(%s) failed, expected '%d', got '%d'",
				termcap, num, numcap);
	}

	/* isn't numeric or boolean, must be a string capability */
	else
	{
		if ((worksp = malloc((unsigned)2048)) == NULL)
			error("malloc failed");

		/*
		** tgetstr changes worksp - save it so
		** it can be freed up later
		*/
		savewksp = worksp;

		capstr = Ztgetstr(termcap, area);
		if (strcmp(tptr->cap_string, capstr) != 0)
		{
			/*
			** if the strings were not equal, it is possible
			** that the string from the terminfo description
			** (tptr->cap_string) needs to have its escape
			** sequences interpreted - esccmp does this
			*/
			if (esccmp(capstr, tptr->cap_string, escstr) != 0)
				error("tgetstr(%s) failed, expected '%s', got '%s'", termcap, escstr, capstr);
		}

		(void) free(savewksp);
	}
}

/*
**	isnum()
**
**	isnum returns 1 if all the chars are digits and a
**	0 if they are not.
*/
isnum(str)
char *str;
{
	int i;
	int c;

	for (i = 0; (*(str + i)) != '\0'; i++)
	{
		c = (int) (*(str + i));
		if (!isdigit(c))
			return(0);
	}
	return(1);
}

/*
**	esccmp()
**
**	esccmp() converts all the escape sequences in its second
**	parameter to actual escape character and then does a string
**	compare of that and the first parameter.  the converted 
**	string is placed at location esccap points to.
*/
esccmp(ticap, cap, esccap)
char *ticap;
char *cap;
char *esccap;
{
	char *capptr = cap;
	char *strptr = esccap;

	while (*capptr != '\0')
	{
		if (*capptr == '^')
		{
			if ((*(capptr + 1)) > '\100')
				*strptr = (*(capptr + 1)) - 64;
			else 
				*strptr = (*(capptr + 1)) + 64;
			capptr++;
		}
		else if (*capptr == '\\')
		{
			switch (*(capptr + 1))
			{
			  case '\\':	*strptr = '\\';
					capptr++;
					break;
			  case 'E':	*strptr = '\033';
					capptr++;
					break;
			  case 'b':	*strptr = '\010';
					capptr++;
					break;
			  case 'f':	*strptr = '\014';
					capptr++;
					break;
			  case 'n':	*strptr = '\012';
					capptr++;
					break;
			  case 'r':	*strptr = '\015';
					capptr++;
					break;
			  case 's':	*strptr = ' ';
					capptr++;
					break;
			  case 't':	*strptr = '\011';
					capptr++;
					break;
			  case '2':	if (*(capptr + 2) == '0' &&
						*(capptr + 3) == '0')
					{
						*strptr = '\200';
						capptr += 3;
					}
					else
						*strptr = *capptr;
					break;
			  default:	*strptr = *capptr;
					break;
			}
		}	
		else 
			*strptr = *capptr;
		strptr++;
		capptr++;
	}
	*strptr = '\0';

	return(strcmp(ticap, esccap));
}

/*
** nmchg()
**
**	nmchg() takes the terminfo name for the capability and
**	locates the corresponding termcap capability name.  this
**	termcap name is copied into the second parameter
*/
nmchg(old, new)
char *old;
char *new;
{
	int min = 0;
	int max = MAXTNAMES;
	int curr;
	int result;
	int done = 0;

	curr = max / 2;

	/* do a binary search of tnames looking for old */
	while (!done)
	{
		if ((result = strcmp(old, tnames[curr][0])) == 0)
		{
			done = 1;
			(void) strcpy(new, tnames[curr][1]);
		}
		else if (result < 0)
		{
			if (min == max)
				break;
			else if (min == max - 1)
				curr = min = max;
			else 
			{
				max = curr;
				curr = (max + min) / 2;
			}
		}
		else
		{
			if (min == max)
				break;
			else if (min == max - 1)
				curr = min = max;
			else
			{
				min = curr;
				curr = (max + min) / 2;
			}
		}
	}
	return(done);
}
