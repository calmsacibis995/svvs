#ident "@(#)tmux.h	1.1"
/* 
 * Header file for streams test multiplexor
 */
struct tmx {
	unsigned tmx_state;
	queue_t *tmx_rdq;
};

struct tmxl {
	int muxid;	/* id of link */
	queue_t *muxq;	/* linked write queue on lower side of mux */
	queue_t *ctlq;	/* controlling write queue on upper side of mux */
};

/*
 * Driver state values.
 */
#define TMXOPEN 01

