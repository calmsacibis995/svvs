/*
 * tidg structure
 */
#define MAXADDR 12
#define DEFAULTOPT 1234
#define TIDU_SIZE 1024

struct ti_tidg {
	long 	ti_flags;		/* internal flags		     */
	queue_t	*ti_rdq;		/* stream read queue ptr	     */
	char  	ti_state;		/* necessary state info of interface */
	unsigned long	 ti_addr;	/* bound address		     */
	queue_t *ti_backwq;		/* back q on WR side for flow cntl   */
};

/*
 * internal flags
 */
#define USED		01	/* data structure in use	*/
#define FATAL		02	/* fatal error on endpoint	*/

#define TI_DG_NUM(X) (((int)X - (int)ti_tidg) / sizeof(struct ti_tidg))

/*
 * unitdata error codes
 */
#define UDBADADDR	1
#define UDBADOPTS	2
#define UDUNREACHABLE	3
