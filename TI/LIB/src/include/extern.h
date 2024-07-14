
/*
** @(#)extern.h	3.1 (SVID) 12/1/86
*/
extern	COMM	Commands[];		/* A sorted list of all known cursor commands. */
extern	int	Numcommands;		/* The total number of commands */
extern	int	Commlength;		/* The length of the largest command */
extern	int	Sizecommands;		/* Size of the Commands array */

extern	struct	_termdata	Termdata[];
extern	COMM	*find_command();
extern	char	*malloc();
extern	char	*calloc();
extern	char	*get_data();
extern	TERMINAL	*init_terminfo();

/*
** The functions for COMM.
*/
extern	int	move_func();
extern	int	direct();
extern	int	home_func();
extern	int	cl_func();
extern	int	up_func();
extern	int	down_func();
extern	int	left_func();
extern	int	right_func();
extern	int	bell_func();
extern	int	cb_func();
extern	int	ce_func();
extern	int	cd_func();
extern	int	cr_func();
extern	int	ch_func();
extern	int	nw_func();
extern	int	insert_char();
extern	int	bt_func();
extern	int	cs_func();
extern	int	rP_func();
extern	int	ct_func();
extern	int	MC_func();
extern	int	vi_func();
extern	int	CM_func();
extern	int	ve_func();
extern	int	ll_func();
extern	int	vs_func();
extern	int	dc_func();
extern	int	dl_func();
extern	int	ds_func();
extern	int	hd_func();
extern	int	eA_func();
extern	int	as_func();
extern	int	SA_func();
extern	int	mb_func();
extern	int	md_func();
extern	int	ti_func();
extern	int	dm_func();
extern	int	mh_func();
extern	int	im_func();
extern	int	mp_func();
extern	int	mr_func();
extern	int	mk_func();
extern	int	so_func();
extern	int	us_func();
extern	int	SX_func();
extern	int	ec_func();
extern	int	ae_func();
extern	int	RA_func();
extern	int	me_func();
extern	int	te_func();
extern	int	ed_func();
extern	int	ei_func();
extern	int	se_func();
extern	int	ue_func();
extern	int	RX_func();
extern	int	vb_func();
extern	int	ff_func();
extern	int	fs_func();
extern	int	i1_func();
extern	int	is_func();
extern	int	i3_func();
extern	int	al_func();
extern	int	ks_func();
extern	int	ke_func();
extern	int	LF_func();
extern	int	LO_func();
extern	int	DC_func();
extern	int	DL_func();
extern	int	DO_func();
extern	int	IC_func();
extern	int	AL_func();
extern	int	LE_func();
extern	int	RI_func();
extern	int	UP_func();
extern	int	r1_func();
extern	int	r2_func();
extern	int	r3_func();
extern	int	rc_func();
extern	int	cv_func();
extern	int	sc_func();
extern	int	sa_func();
extern	int	ML_func();
extern	int	MR_func();
extern	int	st_func();
extern	int	ta_func();
extern	int	ts_func();
extern	int	uc_func();
extern	int	hu_func();

extern	char	Bool_bw;			/* automatic left margin */
extern	char	Bool_am;			/* automatic right margin */
extern	char	Bool_xs;			/* standout not erased by overwrite */
extern	char	Bool_xt;			/* destructive tabs */
extern	char	Bool_eo;			/* erase overstrike with a blank */
extern	char	Bool_hs;			/* has a status line */
extern	char	Bool_mi;			/* move while in insert mode */
extern	char	Bool_ms;			/* move while in standout mode */

extern	struct	_bools	Booleans[];
extern	short	Bless_trace[];
