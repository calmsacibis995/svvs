# include <curses.h>
# include <string.h>
# include <ctype.h>
# include <sccs.h>
# include <svvs.h>
# include <sv_env.h>

/*
**	tput.c
**
**	contains test_tput() which performs testing of the tput
**	command by constructing parameters, calling tput_str() to
**	do a system() of the capname, parms, etc. and comparing the
**	output of tput to what tigetstr returns for the string.
*/

# define MAXTNAMES	311

/*
**	tnames is a table of all the possible string capabilities.
*/
char *tnames[MAXTNAMES] = {
	"acsc",
	"bel",
	"blink",
	"bold",
	"cbt",
	"cdmch",
	"civis",
	"clear",
	"cnorm",
	"cr",
	"csr",
	"cub",
	"cub1",
	"cud",
	"cud1",
	"cuf", 
	"cuf1",
	"cup",
	"cuu",
	"cuu1",
	"cvvis",
	"dch",
	"dch1",
	"dim",
	"dl",
	"dl1",
	"dsl",
	"ech",
	"ed",
	"el",
	"el1",
	"enacs",
	"ff",
	"flash",
	"fsl",
	"hd",
	"home",
	"hpa",
	"ht",
	"hts",
	"hu",
	"ich",
	"ich1",
	"if",
	"il",
	"il1",
	"ind",
	"indn",
	"invis",
	"ip",
	"iprog",
	"is1",
	"is2",
	"is3",
	"kBEG",
	"kCAN",
	"kCMD",
	"kCPY",
	"kCRT",
	"kDC",
	"kDL",
	"kEND",
	"kEOL",
	"kEXT",
	"kFND",
	"kHLP",
	"kHOM",
	"kIC",
	"kLFT",
	"kMOV",
	"kMSG",
	"kNXT",
	"kOPT",
	"kPRT",
	"kPRV",
	"kRDO",
	"kRES",
	"kRIT",
	"kRPL",
	"kSAV",
	"kSPD",
	"kUND",
	"ka1",
	"ka3",
	"kb2",
	"kbeg",
	"kbs",
	"kc1",
	"kc3",
	"kcan",
	"kcbt",
	"kclo",
	"kclr",
	"kcmd",
	"kcpy",
	"kcrt",
	"kctab",
	"kcub1",
	"kcud1",
	"kcuf1",
	"kcuu1",
	"kdch1",
	"kdl1",
	"ked",
	"kel",
	"kend",
	"kent",
	"kext",
	"kf0",
	"kf1",
	"kf10",
	"kf11",
	"kf12",
	"kf13",
	"kf14",
	"kf15",
	"kf16",
	"kf17",
	"kf18",
	"kf19",
	"kf2",
	"kf20",
	"kf21",
	"kf22",
	"kf23",
	"kf24",
	"kf25",
	"kf26",
	"kf27",
	"kf28",
	"kf29",
	"kf3",
	"kf30",
	"kf31",
	"kf32",
	"kf33",
	"kf34",
	"kf35",
	"kf36",
	"kf37",
	"kf38",
	"kf39",
	"kf4",
	"kf40",
	"kf41",
	"kf42",
	"kf43",
	"kf44",
	"kf45",
	"kf46",
	"kf47",
	"kf48",
	"kf49",
	"kf5",
	"kf50",
	"kf51",
	"kf52",
	"kf53",
	"kf54",
	"kf55",
	"kf56",
	"kf57",
	"kf58",
	"kf59",
	"kf6",
	"kf60",
	"kf61",
	"kf62",
	"kf63",
	"kf7",
	"kf8",
	"kf9",
	"kfnd",
	"khlp",
	"khome",
	"khts",
	"kich1",
	"kil1",
	"kind",
	"kll",
	"kmov",
	"kmrk",
	"kmsg",
	"knp",
	"knxt",
	"kopn",
	"kopt",
	"kpp",
	"kprt",
	"kprv",
	"krdo",
	"kref",
	"kres",
	"krfr",
	"kri",
	"krmir",
	"krpl",
	"krst",
	"ksav",
	"kslt",
	"kspd",
	"ktbc",
	"kund",
	"lf0",
	"lf1",
	"lf10",
	"lf2",
	"lf3",
	"lf4",
	"lf5",
	"lf6",
	"lf7",
	"lf8",
	"lf9",
	"ll",
	"mc0",
	"mc4",
	"mc5",
	"mc5p",
	"mgc",
	"mrcup",
	"nel",
	"pad",
	"pfkey",
	"pfloc",
	"pfx",
	"pln",
	"prot",
	"rc",
	"rep",
	"rev",
	"rf",
	"rfi",
	"ri",
	"rin",
	"rmacs",
	"rmam",
	"rmcup",
	"rmdc",
	"rmir",
	"rmkx",
	"rmln",
	"rmm",
	"rmp",
	"rmso",
	"rmul",
	"rmxon",
	"rs1",
	"rs2",
	"rs3",
	"sc",
	"sgr",
	"sgr0",
	"smacs",
	"smam",
	"smcup",
	"smdc",
	"smgl",
	"smgr",
	"smir",
	"smkx",
	"smln",
	"smm",
	"smso",
	"smul",
	"smxon",
	"tbc",
	"tsl",
	"uc",
	"vpa",
	"wind",
	"xmx",
	"xoffc",
	"xonc",
	"",
};

# define STRSIZE	256	/* size of buffers used to hold strings */

# define PARM1		1	/* PARM1 and PARM2 are used when the        */
# define PARM2		0	/* string from tigetstr needs to be tparmed */

# define ENV		1	/* to determine whether to do the system of  */
				/* tput with an environment var or parameter */

SCCSID("@(#)tput.c	3.1 (SVVS) 12/1/86");

/*
**	test_tput() calls tigetstr() on each capability name.  if the
**	capability exists for the type, build_parms() is called to
**	put together whatever parameters may be needed and to do
**	a tparm of the expected string as needed too. then tput_str()
**	is called to do a system() of tput and the parameters and things.
**	the string returned from tput_str is compared against the
**	string from tigetstr (or the tparm-ed version of it).
**
*/
test_tput(flag, term_name)
int	flag;
char	*term_name;
{
	char **names = tnames;
	char *string;
	char *expect;
	char *result;
	char *build_parms();
	extern char *tput_str();
	extern char *Ztigetstr();
	char *type;
	char buf[STRSIZE];
	char call_type[STRSIZE];
	char xpk_longname[STRSIZE];

	while (**names)
	{
		expecting(VOID);
		if (((string = Ztigetstr(*names)) != (char *) -1) && 
		    string &&
		    *string)
		{
			testing("'tput %s'", *names);
			start
			break_on(WARNING | ERROR);
			buf[0] = '\0';
			expect = build_parms(*names, buf, string);

			if (flag == ENV)
				sprintf(call_type, "TERM=%s", term_name);
			else
				sprintf(call_type, "-T%s", term_name);

			if ((result = tput_str(call_type, *names, buf)) == NULL)
				error("'tput %s' returned an error", *names);

			if (strcmp(expect, result) != 0)
				error("'tput %s' output incorrect.  expected '%s', got '%s'", *names, expect, result);

			finis
		}

		names++;
	}

	/*
	** handle longname, init, and reset separately. These are not
	** legal capability names, but the tput command maps them into
	** legal ones.
	*/

	testing("'tput longname'");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "longname");
	sprintf(xpk_longname, "SVVS Test Terminal %c", term_name[strlen(term_name) - 1]);

	if ((result = tput_str(call_type, *names, buf)) == NULL)
		error("'tput longname' returned an error");

	trim(result);

	if (strcmp(xpk_longname, result) != 0)
		error("'tput longname' output incorrect.  expected '%s', got '%s'", expect, result);

	finis

	testing("'tput init'");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "init");

	expecting(VOID);
	expect = Ztigetstr("is1");

	if (expect == (char *) -1)
	{
		error("tigetstr(\"init\") returned -1");
	}
	else if (expect != NULL)
	{
		if ((result = tput_str(call_type, *names, buf)) == NULL)
			error("'tput init' returned an error");

		trim(result);

		if (strcmp(expect, result) != 0)
			error("'tput init' output incorrect.  expected '%s', got '%s'", expect, result);
	}
	finis

	testing("'tput reset'");
	start
	break_on(WARNING | ERROR);
	(void) strcpy(buf, "reset");

	expecting(VOID);
	expect = Ztigetstr("is1");

	if (expect == (char *) -1)
	{
		error("tigetstr(\"is1\") returned -1");
	}
	else if (expect != NULL)
	{
		if ((result = tput_str(call_type, *names, buf)) == NULL)
			error("'tput reset' returned an error");

		trim(result);

		if (strcmp(expect, result) != 0)
			error("'tput reset' output incorrect.  expected '%s', got '%s'", expect, result);
	}
	finis
}

/*
** rm_pad_macro will remove the padding macro token "$<?>" from the string
*/
rm_pad_macro(str)
char *str;
{
	char temp[STRSIZE];
	char *t = temp;
	char *sptr = str;
	char *tptr;
	extern char *strchr();

	if ((tptr = strchr(sptr, '$')) != NULL)
	{
		while (*sptr != '\0' && (tptr = strchr(sptr, '$')) != NULL)
		{
			if (*(tptr + 1) == '<' && strchr(tptr, '>') != NULL)
			{
				while (*sptr != '$')
					*t++ = *sptr++;
				while (*sptr != '>')
					sptr++;
				sptr++;
				while (*sptr != '\0' && *sptr != '$')
					*t++ = *sptr++;
			}
			else
			{
				if (*sptr == '$' && (*(sptr + 1) != '<'))
					*t++ = *sptr++;
				while (*sptr != '\0' && *sptr != '$')
					*t++ = *sptr++;
			}
		}
		*t = '\0';
		(void) strcpy(str, temp);
	}
}

/*
**	build_parms() decides if the capability is one that requires
**	parameters and constructs a string of them as needed.
**	it also runs expected string thru tparm with the parameters
**	and returns a pointer to the result of tparm.
*/
char *build_parms(name, buf, expect)
char *name;
char *buf;
char *expect;
{
	char *result;
	extern char *Ztparm();

	rm_pad_macro(expect);

	if (strcmp(name, "cub") == 0 || strcmp(name, "cud") == 0 ||
	    strcmp(name, "cuf") == 0 || strcmp(name, "cuu") == 0 ||
	    strcmp(name, "dch") == 0 || strcmp(name, "dl") == 0 ||
	    strcmp(name, "ech") == 0 || strcmp(name, "hpa") == 0 ||
	    strcmp(name, "ich") == 0 || strcmp(name, "il") == 0 ||
	    strcmp(name, "smgl") == 0 || strcmp(name, "smgr") == 0 ||
	    strcmp(name, "vpa") == 0)
	{
		(void) sprintf(buf, " %d", PARM1);
		result = Ztparm(expect, PARM1);
	}
	else if (strcmp(name, "cup") == 0 || strcmp(name, "rep") == 0)
	{
		(void) sprintf(buf, " %d %d", PARM1, PARM2);
		result = Ztparm(expect, PARM1, PARM2);
	}
	else
		return (expect);

	return(result);
}

/*
**	trim() removes newlines and carriage returns from the end
**	of the character string parameter.
*/
trim(str)
char *str;
{
	char *end;
	extern char *strchr();

	end = strchr(str, '\0');
	end--;

	while (*end == '\n' || *end == '\r')
	{
		*end = '\0';
		end--;
	}
}
