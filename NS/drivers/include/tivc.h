
/*
 * tivc structure
 */

#define MAX_CONN_IND 4
#define MAXADDR 12
#define DEFAULTOPT 1234
#define TSDU_SIZE 4096
#define ETSDU_SIZE 64
#define TIDU_SIZE 4096
#define CDATA_SIZE 16
#define DDATA_SIZE 16


struct tiseq {
	long seqno;			/* sequence number */
	long used;			/* used? --> TRUE or FALSE */
};

#define TRUE	1			/* tiseq entry used */
#define FALSE	0			/* tiseq entry used */

struct ti_tivc {
	long 	 ti_flags;		/* internel flags                     */
	queue_t *ti_rdq;		/* stream read queue ptr              */
	char  	 ti_state;		/* necessary state info of interface  */
	unsigned long	 ti_addr;	/* bound address		      */
	long     ti_qlen;		/* qlen			              */
	struct tiseq ti_seq[MAX_CONN_IND];  /* outstanding conn_ind seq nos.  */
	int 	 ti_seqcnt;		/* number of outstanding conn_ind     */
	queue_t *ti_remoteq;		/* remote q ptr when connection       */
	long     ti_conseq;     	/* connection seq number id	      */
	int	 ti_etsdu;		/* expedited tsdu counter	      */
	int      ti_tsdu;		/* regular tsdu counter 	      */
	queue_t *ti_outq;		/* outgoing pending remote q	      */
};

/* internel flags */
#define USED		01	/* data structure in use          */
#define FATAL		02

#define TI_VC_NUM(X) (((int)X - (int)ti_tivc) / sizeof(struct ti_tivc))

/*
 * disconnect reason codes
 */
#define PROVIDERINITIATED 	1
#define USERINITIATED 		2
#define UNREACHABLE 		3
#define REMOTEBADSTATE		4
