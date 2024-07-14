/*LINTLIBRARY*/
/*
 *	sv_terrlook.c 
 *
 *	terrlookup():	return char str representation of t_errno value
 */

# include <sys/tiuser.h>

#ifdef	UNDEF
#ident	"@(#)sv_terrloo.c	1.2"
#endif

static struct errz {
	int errr;
	char *errrname;
} errrz[] = {
	{	TBADADDR,	"TBADADDR - incorrect addr format"				},	
	{	TBADOPT,	"TBADOPT - incorrect option format"				},
	{	TACCES,		"TACCES - incorrect permissions"				},
	{	TBADF,		"TBADF - illegal transport fd"					},
	{	TNOADDR,	"TNOADDR - couldn't allocate addr"				},
	{	TOUTSTATE,	"TOUTSTATE - out of state"						},
	{	TBADSEQ,	"TBADSEQ - bad call sequnce number"				},
	{	TSYSERR,	"TSYSERR - system error"						},
	{	TLOOK,		"TLOOK - event requires attention"				},
	{	TBADDATA,	"TBADDATA - illegal amount of data"				},
	{	TBUFOVFLW,	"TBUFOVFLW - buffer not large enough"			},
	{	TFLOW,		"TFLOW - flow control"							},
	{	TNODATA,	"TNODATA - no data"								},
	{	TNODIS,		"TNODIS - discon_ind not found on q"			},
	{	TNOUDERR,	"TNOUDERR - unitdata error not found"			},
	{	TBADFLAG,	"TBADFLAG - bad flags"							},
	{	TNOREL,		"TNOREL - no ord rel found on q"				},
	{	TNOTSUPPORT,"TNOTSUPPORT - primitive not supported"			},
	{	TSTATECHNG,	"TSTATECHNG - state is in process of changing"	},
	{	0,			"0 - No Error"									},
};
/*
** terr_lookup
**	Look up the symbolic name for a transport error.
**
** Arguments
**	num	Error number (probably from errno).
*/
char *terr_lookup(num)
int	num;
{
	register struct errz *p = &errrz[0];
	static char foo[24];

#ifdef DEBUG
	printf("terr_lookup() ");
#endif

	while(1)
	{
		if(p->errr == num)
			return(p->errrname);
		if(!((p++)->errr)) {
			(void) sprintf(foo,"%d - Unknown Error",num);
			return(foo);
		}
	}
}
