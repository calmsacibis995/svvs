# include	<bless.h>
# include	<common.h>
# include	<tie.h>

SCCSID("@(#)extern.c	3.2 (SVID) 12/30/86");

char	Bool_bw = 0;			/* automatic left margin */
char	Bool_am = 0;			/* automatic right margin */
char	Bool_xs = 0;			/* standout not erased by overwrite */
char	Bool_xt = 0;			/* destructive tabs */
char	Bool_eo = 0;			/* erase overstrike with a blank */
char	Bool_hs = 0;			/* has a status line */
char	Bool_mi = 0;			/* move while in insert mode */
char	Bool_ms = 0;			/* move while in standout mode */

int nop_func();

struct	_bools	Booleans[] =
{
	"bw",	&Bool_bw,
	"am",	&Bool_am,
	"xs",	&Bool_xs,
	"xt",	&Bool_xt,
	"eo",	&Bool_eo,
	"hs",	&Bool_hs,
	"mi",	&Bool_mi,
	"ms",	&Bool_ms,
	0,	0
};




struct	_termdata	Termdata[] =
{
	"bl",	bell_func,
	"cr",	cr_func,
	"cl",	cl_func,
	"cb",	cb_func,
	"ce",	ce_func,
	"cd",	cd_func,
	"ch",	ch_func,
	"cm",	move_func,
	"do",	down_func,
	"ho",	home_func,
	"le",	left_func,
	"nd",	right_func,
	"up",	up_func,
	"ic",	insert_char,
	"nw",	nw_func,
	"bt",	bt_func,
	"cs",	cs_func,
	"rP",	rP_func,
	"ct",	ct_func,
	"MC",	MC_func,
	"vi",	vi_func,
	"CM",	CM_func,
	"ve",	ve_func,
	"ll",	ll_func,
	"vs",	vs_func,
	"dc",	dc_func,
	"dl",	dl_func,
	"ds",	ds_func,
	"hd",	hd_func,
	"eA",	eA_func,
	"as",	as_func,
	"SA",	SA_func,
	"mb",	mb_func,
	"md",	md_func,
	"ti",	ti_func,
	"dm",	dm_func,
	"mh",	mh_func,
	"im",	im_func,
	"mp",	mp_func,
	"mr",	mr_func,
	"mk",	mk_func,
	"so",	so_func,
	"us",	us_func,
	"SX",	SX_func,
	"ec",	ec_func,
	"ae",	ae_func,
	"RA",	RA_func,
	"me",	me_func,
	"te",	te_func,
	"ed",	ed_func,
	"ei",	ei_func,
	"se",	se_func,
	"ue",	ue_func,
	"RX",	RX_func,
	"vb",	vb_func,
	"ff",	ff_func,
	"fs",	fs_func,
	"i1",	i1_func,
	"is",	is_func,
	"i3",	i3_func,
	"al",	al_func,
	"ks",	ks_func,
	"ke",	ke_func,
	"LF",	LF_func,
	"LO",	LO_func,
	"DC",	DC_func,
	"DL",	DL_func,
	"DO",	DO_func,
	"IC",	IC_func,
	"AL",	AL_func,
	"LE",	LE_func,
	"RI",	RI_func,
	"UP",	UP_func,
	"r1",	r1_func,
	"r2",	r2_func,
	"r3",	r3_func,
	"rc",	rc_func,
	"cv",	cv_func,
	"sc",	sc_func,
	"sa",	sa_func,
	"ML",	ML_func,
	"MR",	MR_func,
	"st",	st_func,
	"ta",	ta_func,
	"ts",	ts_func,
	"uc",	uc_func,
	"hu",	hu_func,
	"@@",	nop_func,
	"kd",	nop_func,
	"ku",	nop_func,
	"kl",	nop_func,
	"kr",	nop_func,
	"kh",	nop_func,
	"kb",	nop_func,
	"k0",	nop_func,
	"k1",	nop_func,
	"k2",	nop_func,
	"k3",	nop_func,
	"k4",	nop_func,
	"k5",	nop_func,
	"k6",	nop_func,
	"k7",	nop_func,
	"k8",	nop_func,
	"k9",	nop_func,
	"ka",	nop_func,
	"F1",	nop_func,
	"F2",	nop_func,
	"F3",	nop_func,
	"F4",	nop_func,
	"F5",	nop_func,
	"F6",	nop_func,
	"F7",	nop_func,
	"F8",	nop_func,
	"F9",	nop_func,
	"FA",	nop_func,
	"FB",	nop_func,
	"FC",	nop_func,
	"FD",	nop_func,
	"FE",	nop_func,
	"FF",	nop_func,
	"FG",	nop_func,
	"FH",	nop_func,
	"FI",	nop_func,
	"FJ",	nop_func,
	"FK",	nop_func,
	"FL",	nop_func,
	"FM",	nop_func,
	"FN",	nop_func,
	"FO",	nop_func,
	"FP",	nop_func,
	"FQ",	nop_func,
	"FR",	nop_func,
	"FS",	nop_func,
	"FT",	nop_func,
	"FU",	nop_func,
	"FV",	nop_func,
	"FW",	nop_func,
	"FX",	nop_func,
	"FY",	nop_func,
	"FZ",	nop_func,
	"Fa",	nop_func,
	"Fb",	nop_func,
	"Fc",	nop_func,
	"Fd",	nop_func,
	"Fe",	nop_func,
	"Ff",	nop_func,
	"Fg",	nop_func,
	"Fh",	nop_func,
	"Fi",	nop_func,
	"Fj",	nop_func,
	"Fk",	nop_func,
	"Fl",	nop_func,
	"Fm",	nop_func,
	"Fn",	nop_func,
	"Fo",	nop_func,
	"Fp",	nop_func,
	"Fq",	nop_func,
	"Fr",	nop_func,
	"kt",	nop_func,
	"ku",	nop_func,
	"kL",	nop_func,
	"kA",	nop_func,
	"kD",	nop_func,
	"kI",	nop_func,
	"kC",	nop_func,
	"kS",	nop_func,
	"kE",	nop_func,
	"kF",	nop_func,
	"kR",	nop_func,
	"kN",	nop_func,
	"kP",	nop_func,
	"kT",	nop_func,    
	"kt",	nop_func,
	"k;",	nop_func,
	"@8",	nop_func,
	"@@",	nop_func,
	"@@",	nop_func,
	"%9",	nop_func,
	"kH",	nop_func,
	"K1",	nop_func,
	"K3",	nop_func,
	"K2",	nop_func,
	"K4",	nop_func,
	"K5",	nop_func,
	"kB",	nop_func,
	"@1",	nop_func,
	"@2",	nop_func,
	"@3",	nop_func,
	"@4",	nop_func,
	"@5",	nop_func,
	"@6",	nop_func,
	"@7",	nop_func,
	"@9",	nop_func,
	"@0",	nop_func,
	"%1",	nop_func,
	"%2",	nop_func,
	"%3",	nop_func,
	"%4",	nop_func,
	"%5",	nop_func,
	"%6",	nop_func,
	"%7",	nop_func,
	"%8",	nop_func,
	"%0",	nop_func,
	"&1",	nop_func,
	"&2",	nop_func,
	"&3",	nop_func,
	"&4",	nop_func,
	"&5",	nop_func,
	"&6",	nop_func,
	"&9",	nop_func,
	"&0",	nop_func,
	"*1",	nop_func,
	"*2",	nop_func,
	"*3",	nop_func,
	"*4",	nop_func,
	"*5",	nop_func,
	"*6",	nop_func,
	"*7",	nop_func,
	"*8",	nop_func,
	"*9",	nop_func,
	"*0",	nop_func,
	"#1",	nop_func,
	"#2",	nop_func,
	"#3",	nop_func,
	"#4",	nop_func,
	"%a",	nop_func,
	"%b",	nop_func,
	"%c",	nop_func,
	"%d",	nop_func,
	"%e",	nop_func,
	"%f",	nop_func,
	"%g",	nop_func,
	"%h",	nop_func,
	"%i",	nop_func,
	"%j",	nop_func,
	"!1",	nop_func,
	"!2",	nop_func,
	"!3",	nop_func,
	"&7",	nop_func,
	"&8",	nop_func,
	NULL,	0
};


/*
** A sorted list of all known cursor commands.
*/
COMM	Commands[ (sizeof (Termdata)) / (sizeof (struct _termdata)) ];
int	Numcommands = 0;	/* The total number of commands */
int	Commlength = 0;		/* The length of the largest command */
int	Sizecommands = sizeof(Commands) / sizeof (COMM);

TERMINAL	Actual;

int	Soft_label_keys_on = 0;	/* Are the soft label keys on? */
TERMINAL *DISPLAY, *std;	/* standard display */
TERMINAL *status_line;		/* Status line */
int	c_LINES, c_COLS;	/* catcher's version of lines and columns */
int 	Status_line_initialized = 0;
				/* Has the status line been initialized? */
int	Echo_on = 1;		/* Is echo on? */
int	Keypad_on = 0;		/* Is keypad enabled? */
long	Default_attr; 		/* attribute used if attempt is made to assign an unavailable attribute */
int 	Labelwidth = 8;		/* width of label on status line (assigned relative to width of screen) */

short	Bless_trace[TRACENUM];	/* The trace flags */

nop_func()  
{
	return(NULL);
}

# ifdef	TRACE
/*
** Trace_map structure. This structure is used to map symbolic
** names into trace values.
*/
TRACE_MAP	Bless_map[] =
{
	"apply",		10,
	"bless"	,		20,
	"b_eval",		35,
	"compare",		2,
	"direct",		40,
	"doper",		10,
	"evaluate",		35,
	"find_command",		12,
	"get_data",		7,
	"getx",			45,
	"init_terminfo",	1,
	"init_data",		7,
	"print_term",		51,
	"print_line",		50,
	"push",			30,
	"term_init",		3,
	"uses_sl",		9,
	NULL,			0
};
# endif	/* TRACE */
