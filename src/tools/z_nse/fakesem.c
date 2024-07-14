#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)fakesem.c	1.4"
#endif

extern int t_errno;

static int pfildes[2];
static int cfildes[2];
static int my_pid = -1;		/* pid of seminit() caller	*/
static int nsems = -1;		/* number of procs - 1		*/
static int p2p = -1;		/* point to point ?			*/
static int synced = 0;		/* child proc called t_sync	*/
static int active = 0;		/* connect role				*/

static struct endpoint {
	char	*provider;
	char	*addr;
	int		alen;
	char	*conn;
	int		clen;
	int		fd;
} tp[] = {
	{	NULL,	NULL,	0,	NULL,	0,	-1,	},
	{	NULL,	NULL,	0,	NULL,	0,	-1,	},
};

#define NPROVIDERS	(sizeof( tp )/sizeof(struct endpoint))

static	struct	t_info		info[ NPROVIDERS ];			
static	struct	t_bind		*req[ NPROVIDERS ];
static	struct	t_bind		*ret[ NPROVIDERS ];
static	struct	t_call		*sndcall[ NPROVIDERS ];
static	struct	t_call		*rcvcall[ NPROVIDERS ];
static	struct	t_call		*scall[ NPROVIDERS ];
static	struct	t_call		*rcall[ NPROVIDERS ];
static	struct	t_discon	*discon[ NPROVIDERS ];

void chk_unbnd();
void chk_idle();
void chk_incon();
void chk_outcon();
void chk_dataxfer();
void chk_state();
void set_ndelay();
void clr_ndelay();

static void do_open( id ) 
int id;
{
	prep("Call t_open to open the transport provider: '%s'.",
	 tp[id].provider );
	start
		tp[id].fd = t_open( tp[id].provider, O_RDWR, &info[id]);
	finis
	chk_unbnd( id );
}
static void do_alloc( id ) 
int id;
{
	char	*t_alloc();
	prep("Call t_alloc to allocate some structures.");
	start
		ret[id] = (struct t_bind *) t_alloc( tp[id].fd, T_BIND, T_ALL );
		req[id] = (struct t_bind *) t_alloc( tp[id].fd, T_BIND, T_ALL );
		sndcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL,
		  T_ALL );
		rcvcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL,
		  T_ALL );
		rcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL,
		  T_ALL );
		scall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL, 
		  T_ALL );
		discon[id] = (struct t_discon *) t_alloc( tp[id].fd, T_DIS, 
		  T_ALL );
	finis
}
static void do_bind( id ) 
int id;
{
	prep("Copy the address to be bound into the addr buffer, and");
	prep("call t_bind.");
	start
		req[id]->addr.len = tp[id].alen;
		memcpy( req[id]->addr.buf, tp[id].addr, tp[id].alen );
		req[id]->qlen = active?0:1;
		t_bind( tp[id].fd, req[id], ret[id]);
	finis
	prep("Check that the requested address was the one bound.");
	start
		expecting( VOID );
		if( tp[id].alen != ret[id]->addr.len ||
		  memcmp( ret[id]->addr.buf, tp[id].addr, tp[id].alen ) )
			severr("Address bound was not address requested.");
	finis
	chk_idle( id );
}
static void do_connect( a ) 
int	a;
{

	int	b = 0;
	int	ret,i;

	if( !a ) b = 1;

	expecting( SUCCESS );
	prep("Establish communications.");
	start
		if( active ) {
			prep("Call t_connect for the active user.");
			start
				sndcall[a]->addr.len = tp[b].clen;
				memcpy( sndcall[a]->addr.buf, tp[b].conn, tp[b].clen );

				for(i = 0; i < 15; i++ ) {
					expecting( VOID );
					sleep( 5 );
					expecting( SUCCESS );
					warn_on( E_FAILURE );
					ret = t_connect( tp[a].fd, sndcall[a], rcvcall[a] );
					if( ret && t_errno == TLOOK ) {
						expecting( VOID );
						if( t_look( tp[a].fd ) == T_DISCONNECT )
							t_rcvdis( tp[a].fd, discon[a] );
							warning("Disconnect received - reason: %d",
							  discon[a]->reason);
					}
					if( !ret ) 
						break;
printf("sndcall[%d]:\n",a);
printf("addr.len:	0x%x\n",sndcall[a]->addr.len);
printf("addr.buf:	0x%x 0x%x 0x%x 0x%x\n",sndcall[a]->addr.buf[0],sndcall[a]->addr.buf[1],sndcall[a]->addr.buf[2],sndcall[a]->addr.buf[3]);
printf("rcvcall[%d]:\n",a);
printf("addr.len:	0x%x\n",rcvcall[a]->addr.len);
printf("addr.buf:	0x%x 0x%x 0x%x 0x%x\n",rcvcall[a]->addr.buf[0],rcvcall[a]->addr.buf[1],rcvcall[a]->addr.buf[2],rcvcall[a]->addr.buf[3]);
				}
				if( ret )
					error("could not connect.");
			finis
			prep("Check that the address returned by t_connect");
			prep("corresponds to the passive user.");
			start
				expecting( VOID );
				if( tp[b].clen != rcvcall[a]->addr.len ||
				  memcmp( rcvcall[a]->addr.buf, tp[b].conn, tp[b].clen ) )
					severr("Wrong addr returned by t_connect");
			finis
		} else {
			prep("Call t_listen for passive user.");
			start
				t_listen( tp[b].fd, rcall[b] );
			finis
			prep("Check the addr returned by t_listen corresponds to");
			prep("the active user.");
			start
				expecting( VOID );
				if( tp[a].clen != rcall[b]->addr.len ||
				  memcmp( rcall[b]->addr.buf, tp[a].conn, tp[a].clen ) )
					severr("Wrong addr returned by t_listen.");
			finis
			chk_incon( b );
			prep("Call t_accept for passive user.");
			start
				t_accept( tp[b].fd, tp[b].fd, rcall[b] );
			finis
			chk_dataxfer( b );
		}
	finis
}
static void chk_unbnd( id ) 
int id;
{
	chk_state( tp[id].fd, T_UNBND );
}
static void chk_idle( id ) 
int id;
{
	chk_state( tp[id].fd ,T_IDLE );
}
static void chk_incon( id ) 
int id;
{
	chk_state( tp[id].fd, T_INCON );
}
static void chk_outcon( id ) 
int id;
{
	chk_state( tp[id].fd, T_OUTCON );
}
static void chk_dataxfer( id ) 
int id;
{
	chk_state( tp[id].fd, T_DATAXFER );
}
static void chk_state( fd, state ) 
int fd;
int state;
{
	char *st_name;
	char *sname();

	st_name = sname( state );
	prep("Call t_getstate, verify the current state is %s.",st_name);
	start
		continue_on( ERROR );
		expecting( state );
		t_getstate( fd );
	finis
}
static void set_ndelay( id ) 
int id;
{
	int	oflag;

	prep("Call fcntl to get the file status flags.  If the");
	prep("O_NDELAY file status flag is not set, call fcntl");
	prep("again to set it.");
	start
		int oflag;
		expecting( VOID );
		oflag = fcntl( tp[id].fd, F_GETFL, NULL );
		if( !(oflag & O_NDELAY ) ) {
			expecting( SUCCESS );
			fcntl( tp[id].fd, F_SETFL, oflag | O_NDELAY );
		}
	finis
}
static void clr_ndelay( id ) 
int id;
{
	int	oflag;

	prep("Call fcntl to get the file status flags.  If the");
	prep("O_NDELAY file status flag is set, call fcntl");
	prep("again to clear it.");
	start
		int oflag;

		expecting( VOID );
		oflag = fcntl( tp[id].fd, F_GETFL, NULL );
		if( (oflag & O_NDELAY ) ) {
			expecting( SUCCESS );
			fcntl( tp[id].fd, F_SETFL, oflag & ~O_NDELAY );
		}
	finis
}
static void init_conn() {
	abort_on( ERROR );
	tp[0].provider = SEMAFILE0;
	tp[0].addr = SEMAADDR0;
	tp[0].alen = SEMAALEN0;
	tp[0].conn = SEMACONN0;
	tp[0].clen = SEMACLEN0;

	tp[1].provider = SEMAFILE1;
	tp[1].addr = SEMAADDR1;
	tp[1].alen = SEMAALEN1;
	tp[1].conn = SEMACONN1;
	tp[1].clen = SEMACLEN1;
	expecting( VOID );

	if( tp[0].alen > tp[0].clen )
		active = 1;
	else if (tp[0].alen == tp[0].clen 
	  && memcmp(tp[0].addr,tp[0].conn,tp[0].alen) > 0 )
		active = 1;

	expecting( SUCCESS );
	do_open( 0 );
	do_alloc( 0 );
	do_bind( 0 );
	do_open( 1 );
	do_alloc( 1 );
	do_bind( 1 );
	do_connect( 0 );
	do_connect( 1 );
}
p_sema() {
	int expect;
	char	ch;
	int	flags;
	int	id;

#ifdef	DEVELOPMENT
	if( nsems < 0 )
		severr("p_sema called without calling seminit().");
#endif
	expect = sv_xenv->sv_expecting;
	expecting( VOID );
	if( getpid() == my_pid ) {
		id = 1;
		if( nsems ) {
			expecting( 1 );
			read( pfildes[0], &ch, 1 );
			id = 0;
		}
		if( p2p ) {
			expecting( 1 );
			t_rcv( tp[id].fd, &ch, 1, &flags );
		}
	}
	else {
#ifdef	DEVELOPMENT
		if( !nsems )
			severr("In child proc when seminit(1) was called.");
#endif
		expecting( 1 );
		read( cfildes[0], &ch, 1 );
		if( p2p ) {
			if( !synced ) {
				synced = 1;
				expecting( T_DATAXFER );
				t_sync( tp[1].fd );
			}
			expecting( 1 );
			t_rcv( tp[1].fd, &ch, 1, &flags );
		}
	}
	sv_xenv->sv_expecting = expect;
}
v_sema() {
	int expect;
	char	ch;
#ifdef	DEVELOPMENT
	if( nsems < 0 )
		severr("v_sema called without calling seminit().");
#endif

	expect = sv_xenv->sv_expecting;
	expecting( VOID );
	if( getpid() == my_pid ) {
		if (p2p) {
			expecting( 1 );
			t_snd( tp[0].fd, &ch, 1, 0 );
		}
		if( nsems ) {
			expecting( 1 );
			write( cfildes[1], &ch, 1 );
		}
	}
	else {
#ifdef	DEVELOPMENT
		if( !nsems )
			severr("In child proc when seminit(1) was called.");
#endif
		if (p2p) {
			if( !synced ) {
				synced = 1;
				expecting( T_DATAXFER );
				t_sync( tp[1].fd );
			}
			expecting( 1 );
			t_snd( tp[1].fd, &ch, 1, 0 );
		}
		expecting( 1 );
		write( pfildes[1], &ch, 1 );
	}
	sv_xenv->sv_expecting = expect;
}
seminit( pcnt ) 
int	pcnt;	/* count of procs  - 1 or 2 */
{
	nsems = pcnt - 1;
	my_pid = getpid();
	if( nsems < 0 || nsems > 1 )
		severr("bad sem count arg to seminit");
	expecting( VOID );
	p2p = SEMAALEN0 != SEMACLEN0 || memcmp(SEMAADDR0, SEMACONN0, SEMAALEN0);
	if( (!p2p) != ( SEMAALEN1 == SEMACLEN1 && !memcmp(SEMAADDR1, SEMACONN1, SEMAALEN1) ) ) 
		severr("%s%s",
			"error in semaphore addr/len specification in config file\n",
			"(semaddr0==semconn0) != (semaddr1==semconn1)");
	expecting( SUCCESS );
	if( p2p )
		init_conn();
	if( nsems ) {
		pipe( pfildes );
		pipe( cfildes );
	}
}
