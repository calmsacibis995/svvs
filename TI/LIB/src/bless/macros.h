/*
** @(#)macros.h	3.1 (SVVS) 12/1/86
**
**
** Some psuedo-functions to ease typing. These are for
** manipulating TERMINAL fields.
*/
# define	getline(trm)	trm->term_screen[trm->term_yloc]
# define	getstring(trm,y) trm->term_screen[y]
# define	getpos(trm)	&trm->term_screen[trm->term_yloc][trm->term_xloc]
# define	getchr(trm)	trm->term_screen[trm->term_yloc][trm->term_xloc]
# define	getbits(trm)	trm->term_bits[trm->term_yloc][trm->term_xloc]
# define	getheight(trm)	trm->term_height
# define	gety(trm)	trm->term_yloc
# define	getlmargin(trm) trm->term_lmargin
# define	getrmargin(trm) trm->term_rmargin
# define	setwidth(trm)	trm->term_width
# define	setx(trm)	trm->term_xloc
# define	sety(trm)	trm->term_yloc
