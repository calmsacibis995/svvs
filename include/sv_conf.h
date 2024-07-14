/*
 *	sv_conf.h	config variables header file
 */

#ifdef UNDEF
#ident	"@(#)sv_conf.h	1.13"
#endif

struct cnfg {
	char	*name;
	char	*value;
	int		typ;
};

#ifndef SV_CONFIG
extern struct cnfg sv_params[];
#endif

#define	SVVSPATH		((char *)(sv_params[0].value))
#define	ROOT_UID		*((int *)(sv_params[1].value))
#define	ROOT_GID		*((int *)(sv_params[2].value))
#define	BIN_UID			*((int *)(sv_params[3].value))
#define	BIN_GID			*((int *)(sv_params[4].value))
#define	UID				*((int *)(sv_params[5].value))
#define	GID				*((int *)(sv_params[6].value))
#define	PROC_LIMIT		*((int *)(sv_params[7].value))
#define	PURETEXT		*((int *)(sv_params[8].value))
#define	SYSNAME			((char *)(sv_params[9].value))
#define	NODENAME		((char *)(sv_params[10].value))
#define	RELEASE			((char *)(sv_params[11].value))
#define	VERSION			((char *)(sv_params[12].value))
#define	MACHINE			((char *)(sv_params[13].value))
#define	SVVS_ROOT		((char *)(sv_params[14].value))
#define	FSYSNAME		((char *)(sv_params[15].value))
#define	PACKNAME		((char *)(sv_params[16].value))
#define	SVVS_FS			((char *)(sv_params[17].value))
#define	MOUNT_POINT		((char *)(sv_params[18].value))
#define	OFF_LINE_DEV	*((int *)(sv_params[19].value))
#define	CDEV			*((int *)(sv_params[20].value))
#define	BDEV			*((int *)(sv_params[21].value))
#define	PR_OFFSET		*((int *)(sv_params[22].value))
#define	CHAR_BIT		*((int *)(sv_params[23].value))
#define	UID_MAX			*((int *)(sv_params[24].value))
#define	GID_MAX			*((int *)(sv_params[25].value))
#define	PID_MAX			*((int *)(sv_params[26].value))
#define	NAME_MAX		*((int *)(sv_params[27].value))
#define	SYS_NMLN		*((int *)(sv_params[28].value))
#define	LINK_MAX		*((int *)(sv_params[29].value))
#define	CMD_MAX			*((int *)(sv_params[30].value))
#define	CMD_MIN			*((int *)(sv_params[31].value))
#define	SIG_MAX			*((int *)(sv_params[32].value))
#define	SIG_MIN			*((int *)(sv_params[33].value))
#define	USI_MAX			*((unsigned int *)(sv_params[34].value))
#define	USHRT_MAX		*((unsigned short *)(sv_params[35].value))
#define	SHRT_MAX		*((short *)(sv_params[36].value))
#define	LONG_MAX		*((long *)(sv_params[37].value))
#define	INT_MAX			*((int *)(sv_params[38].value))
#define	INT_MIN			*((int *)(sv_params[39].value))
#define	LONG_MIN		*((long *)(sv_params[40].value))
#define	SHRT_MIN		*((short *)(sv_params[41].value))

#define AMAXLEN			*((int *)(sv_params[42].value))
#define TPDGTYPE		((char *)(sv_params[43].value))
#define	TPDGWAIT		*((int *)(sv_params[44].value))
#define	TPDGFILE0		((char *)(sv_params[45].value))
#define TPDGMINOR0		*((int *)(sv_params[46].value))
#define TPDGADDR0		((char *)(sv_params[47].value))
#define	TPDGALEN0		*((int *)(sv_params[48].value))
#define TPDGCONN0		((char *)(sv_params[49].value))
#define	TPDGCLEN0		*((int *)(sv_params[50].value))
#define	TPDGFILE1		((char *)(sv_params[51].value))
#define TPDGMINOR1		*((int *)(sv_params[52].value))
#define TPDGADDR1		((char *)(sv_params[53].value))
#define	TPDGALEN1		*((int *)(sv_params[54].value))
#define TPDGCONN1		((char *)(sv_params[55].value))
#define	TPDGCLEN1		*((int *)(sv_params[56].value))
#define TPVCTYPE		((char *)(sv_params[57].value))
#define	TPVCWAIT		*((int *)(sv_params[58].value))
#define	TPVCFILE0		((char *)(sv_params[59].value))
#define TPVCMINOR0		*((int *)(sv_params[60].value))
#define TPVCADDR0		((char *)(sv_params[61].value))
#define	TPVCALEN0		*((int *)(sv_params[62].value))
#define TPVCCONN0		((char *)(sv_params[63].value))
#define	TPVCCLEN0		*((int *)(sv_params[64].value))
#define	TPVCFILE1		((char *)(sv_params[65].value))
#define TPVCMINOR1		*((int *)(sv_params[66].value))
#define TPVCADDR1		((char *)(sv_params[67].value))
#define	TPVCALEN1		*((int *)(sv_params[68].value))
#define TPVCCONN1		((char *)(sv_params[69].value))
#define	TPVCCLEN1		*((int *)(sv_params[70].value))
#define	TPVCFILE2		((char *)(sv_params[71].value))
#define TPVCMINOR2		*((int *)(sv_params[72].value))
#define TPVCADDR2		((char *)(sv_params[73].value))
#define	TPVCALEN2		*((int *)(sv_params[74].value))
#define TPVCCONN2		((char *)(sv_params[75].value))
#define	TPVCCLEN2		*((int *)(sv_params[76].value))
#define	TPVCFILE3		((char *)(sv_params[77].value))
#define TPVCMINOR3		*((int *)(sv_params[78].value))
#define TPVCADDR3		((char *)(sv_params[79].value))
#define	TPVCALEN3		*((int *)(sv_params[80].value))
#define TPVCCONN3		((char *)(sv_params[81].value))
#define	TPVCCLEN3		*((int *)(sv_params[82].value))

#define TPDGINFO_ADDR	*((int *)(sv_params[83].value))
#define TPDGINFO_OPTIONS	*((int *)(sv_params[84].value))
#define TPDGINFO_TSDU	*((int *)(sv_params[85].value))
#define TPDGINFO_ETSDU	*((int *)(sv_params[86].value))
#define TPDGINFO_CONNECT	*((int *)(sv_params[87].value))
#define TPDGINFO_DISCON	*((int *)(sv_params[88].value))
#define TPVCINFO_ADDR	*((int *)(sv_params[89].value))
#define TPVCINFO_OPTIONS	*((int *)(sv_params[90].value))
#define TPVCINFO_TSDU	*((int *)(sv_params[91].value))
#define TPVCINFO_ETSDU	*((int *)(sv_params[92].value))
#define TPVCINFO_CONNECT	*((int *)(sv_params[93].value))
#define TPVCINFO_DISCON	*((int *)(sv_params[94].value))
#define TPBADADDR	((char *)(sv_params[95].value))
#define	TPBADLEN		*((int *)(sv_params[96].value))
#define BAD_OPTIONS	((char *)(sv_params[97].value))
#define	BADOPTLEN		*((int *)(sv_params[98].value))
#define	OMAXLEN		*((int *)(sv_params[99].value))
#define	DMAXLEN		*((int *)(sv_params[100].value))

#define	SEMAFILE0		((char *)(sv_params[101].value))
#define SEMAADDR0		((char *)(sv_params[102].value))
#define	SEMAALEN0		*((int *)(sv_params[103].value))
#define SEMACONN0		((char *)(sv_params[104].value))
#define	SEMACLEN0		*((int *)(sv_params[105].value))
#define	SEMAFILE1		((char *)(sv_params[106].value))
#define SEMAADDR1		((char *)(sv_params[107].value))
#define	SEMAALEN1		*((int *)(sv_params[108].value))
#define SEMACONN1		((char *)(sv_params[109].value))
#define	SEMACLEN1		*((int *)(sv_params[110].value))

#define	RF_DOMAIN		((char *)(sv_params[111].value))
#define	RF_NODENAME		((char *)(sv_params[112].value))
#define	RF_CLIENT		((char *)(sv_params[113].value))
#define	RF_NEWDOMAIN		((char *)(sv_params[114].value))

#define	RF_RESOURCE		((char *)(sv_params[115].value))
#define	RF_RD_RESOURCE		((char *)(sv_params[116].value))

#define	RF_DESCRIPTION		((char *)(sv_params[117].value))
#define	RF_RD_DESCRIPTION		((char *)(sv_params[118].value))

#define	RF_REMOTE_RESOURCE		((char *)(sv_params[119].value))

#define	RF_IDLOAD_DIR		((char *)(sv_params[120].value))
#define	RF_U_RULES		((char *)(sv_params[121].value))
#define	RF_G_RULES		((char *)(sv_params[122].value))

#define	RF_BLK_SPEC		((char *)(sv_params[123].value))

#define	DEFAULT_PITCH_DEV	((char *)(sv_params[124].value))
#define	DEFAULT_CATCH_DEV	((char *)(sv_params[125].value))
#define SVVS_RFS		((char *)(sv_params[126].value))
#define	RF_PRIMARY		((char *)(sv_params[127].value))
#define	RF_SLEEP		*((int *)(sv_params[128].value))
#define ARG_MAX			*((int *)(sv_params[129].value))
#define	RF_SECONDARY	((char *)(sv_params[130].value))
#define	CONFIGD			*((int *)(sv_params[131].value))

#ifdef SV_CONFIG
/* Data Types */

#define		UCHAR	1	/*Value points to a uchar */
#define		CHAR	2	/*Value points to a char */
#define		USHORT	3	/*Value points to a ushort */
#define		SHORT	4	/*Value points to a short */
#define		UINT	5	/*Value points to an uint */
#define		INT		6	/*Value points to an int */
#define		ULONG	7	/*Value points to a ulong */
#define		LONG	8	/*Value points to a long */
#define		STRING	9	/*Value points to a string */

#define	DEFAULT "config"
static  char			null_str[]="";
static	int			ZROOT_UID=0;
static	int			ZROOT_GID=0;
static	int			ZBIN_UID=0;
static	int			ZBIN_GID=0;
static	int			ZUID=0;
static	int			ZGID=0;
static	int			ZPROC_LIMIT=0;
static	int			ZPURETEXT=0;
static	int			ZOFF_LINE_DEV=0;
static	int			ZCDEV=0;
static	int			ZBDEV=0;
static	int			ZPR_OFFSET=0;
static	int			ZCHAR_BIT=0;
static	int			ZUID_MAX	=0;
static	int			ZGID_MAX	=0;
static	int			ZPID_MAX	=0;
static	int			ZNAME_MAX=0;
static	int			ZSYS_NMLN=0;
static	int			ZLINK_MAX=0;
static	int			ZCMD_MAX	=0;
static	int			ZCMD_MIN	=0;
static	int			ZSIG_MAX	=0;
static	int			ZSIG_MIN	=0;
static	unsigned	ZUSI_MAX	=0;
static	unsigned short	ZUSHRT_MAX=0;
static	short		ZSHRT_MAX=0;
static	long		ZLONG_MAX=0;
static	int			ZINT_MAX	=0;
static	int			ZINT_MIN	=0;
static	long		ZLONG_MIN=0;
static	short		ZSHRT_MIN=0;
static	int			ZAMAXLEN=0;
static	int			ZTPDGWAIT=0;
static	int			ZTPDGMINOR0=0;
static	int			ZTPDGALEN0=0;
static	int			ZTPDGCLEN0=0;
static	int			ZTPDGMINOR1=0;
static	int			ZTPDGALEN1=0;
static	int			ZTPDGCLEN1=0;
static	int			ZTPVCWAIT=0;
static	int			ZTPVCMINOR0=0;
static	int			ZTPVCALEN0=0;
static	int			ZTPVCCLEN0=0;
static	int			ZTPVCMINOR1=0;
static	int			ZTPVCALEN1=0;
static	int			ZTPVCCLEN1=0;
static	int			ZTPVCMINOR2=0;
static	int			ZTPVCALEN2=0;
static	int			ZTPVCCLEN2=0;
static	int			ZTPVCMINOR3=0;
static	int			ZTPVCALEN3=0;
static	int			ZTPVCCLEN3=0;
static	int			ZTPDGINFO_ADDR=0;
static	int			ZTPDGINFO_OPTIONS=0;
static	int			ZTPDGINFO_TSDU=0;
static	int			ZTPDGINFO_ETSDU=0;
static	int			ZTPDGINFO_CONNECT=0;
static	int			ZTPDGINFO_DISCON=0;
static	int			ZTPVCINFO_ADDR=0;
static	int			ZTPVCINFO_OPTIONS=0;
static	int			ZTPVCINFO_TSDU=0;
static	int			ZTPVCINFO_ETSDU=0;
static	int			ZTPVCINFO_CONNECT=0;
static	int			ZTPVCINFO_DISCON=0;
static	int			ZTPBADLEN=0;
static	int			ZBADOPTLEN=0;
static	int			ZOMAXLEN=0;
static	int			ZDMAXLEN=0;
static	int			ZSEMAALEN0=0;
static	int			ZSEMACLEN0=0;
static	int			ZSEMAALEN1=0;
static	int			ZSEMACLEN1=0;
static	int			ZRF_SLEEP=0;
static	int			ZARG_MAX=0;
static	int			ZCONFIGD=0;	/*Config() status flag */

struct cnfg sv_params[] = {
	{"SVVSPATH",null_str,STRING},
	{"ROOT_UID",(char *)&ZROOT_UID,INT},
	{"ROOT_GID",(char *)&ZROOT_GID,INT},
	{"BIN_UID",(char *)&ZBIN_UID,INT},
	{"BIN_GID",(char *)&ZBIN_GID,INT},
	{"UID",(char *)&ZUID,INT},
	{"GID",(char *)&ZGID,INT},
	{"PROC_LIMIT",(char *)&ZPROC_LIMIT,INT},
	{"PURETEXT",(char *)&ZPURETEXT,INT},
	{"SYSNAME",null_str,STRING},
	{"NODENAME",null_str,STRING},
	{"RELEASE",null_str,STRING},
	{"VERSION",null_str,STRING},
	{"MACHINE",null_str,STRING},
	{"SVVS_ROOT",null_str,STRING},
	{"FSYSNAME",null_str,STRING},
	{"PACKNAME",null_str,STRING},
	{"SVVS_FS",null_str,STRING},
	{"MOUNT_POINT",null_str,STRING},
	{"OFF_LINE_DEV",(char *)&ZOFF_LINE_DEV,INT},
	{"CDEV",(char *)&ZCDEV,INT},
	{"BDEV",(char *)&ZBDEV,INT},
	{"PR_OFFSET",(char *)&ZPR_OFFSET,INT},
	{"CHAR_BIT",(char *)&ZCHAR_BIT,INT},
	{"UID_MAX",(char *)&ZUID_MAX,INT},
	{"GID_MAX",(char *)&ZGID_MAX,INT},
	{"PID_MAX",(char *)&ZPID_MAX,INT},
	{"NAME_MAX",(char *)&ZNAME_MAX,INT},
	{"SYS_NMLN",(char *)&ZSYS_NMLN,INT},
	{"LINK_MAX",(char *)&ZLINK_MAX,INT},
	{"CMD_MAX",(char *)&ZCMD_MAX,INT},
	{"CMD_MIN",(char *)&ZCMD_MIN,INT},
	{"SIG_MAX",(char *)&ZSIG_MAX,INT},
	{"SIG_MIN",(char *)&ZSIG_MIN,INT},
	{"USI_MAX",(char *)&ZUSI_MAX,UINT},
	{"USHRT_MAX",(char *)&ZUSHRT_MAX,USHORT},
	{"SHRT_MAX",(char *)&ZSHRT_MAX,SHORT},
	{"LONG_MAX",(char *)&ZLONG_MAX,LONG},
	{"INT_MAX",(char *)&ZINT_MAX,INT},
	{"INT_MIN",(char *)&ZINT_MIN,INT},
	{"LONG_MIN",(char *)&ZLONG_MIN,LONG},
	{"SHRT_MIN",(char *)&ZSHRT_MIN,SHORT},
	{"AMAXLEN",(char *)&ZAMAXLEN,INT},
	{"TPDGTYPE",null_str,STRING},
	{"TPDGWAIT",(char *)&ZTPDGWAIT,INT},
	{"TPDGFILE0",null_str,STRING},
	{"TPDGMINOR0",(char *)&ZTPDGMINOR0,INT},
	{"TPDGADDR0",null_str,STRING},
	{"TPDGALEN0",(char *)&ZTPDGALEN0,INT},
	{"TPDGCONN0",null_str,STRING},
	{"TPDGCLEN0",(char *)&ZTPDGCLEN0,INT},
	{"TPDGFILE1",null_str,STRING},
	{"TPDGMINOR1",(char *)&ZTPDGMINOR1,INT},
	{"TPDGADDR1",null_str,STRING},
	{"TPDGALEN1",(char *)&ZTPDGALEN1,INT},
	{"TPDGCONN1",null_str,STRING},
	{"TPDGCLEN1",(char *)&ZTPDGCLEN1,INT},

	{"TPVCTYPE",null_str,STRING},
	{"TPVCWAIT",(char *)&ZTPVCWAIT,INT},
	{"TPVCFILE0",null_str,STRING},
	{"TPVCMINOR0",(char *)&ZTPVCMINOR0,INT},
	{"TPVCADDR0",null_str,STRING},
	{"TPVCALEN0",(char *)&ZTPVCALEN0,INT},
	{"TPVCCONN0",null_str,STRING},
	{"TPVCCLEN0",(char *)&ZTPVCCLEN0,INT},
	{"TPVCFILE1",null_str,STRING},
	{"TPVCMINOR1",(char *)&ZTPVCMINOR1,INT},
	{"TPVCADDR1",null_str,STRING},
	{"TPVCALEN1",(char *)&ZTPVCALEN1,INT},
	{"TPVCCONN1",null_str,STRING},
	{"TPVCCLEN1",(char *)&ZTPVCCLEN1,INT},
	{"TPVCFILE2",null_str,STRING},
	{"TPVCMINOR2",(char *)&ZTPVCMINOR2,INT},
	{"TPVCADDR2",null_str,STRING},
	{"TPVCALEN2",(char *)&ZTPVCALEN2,INT},
	{"TPVCCONN2",null_str,STRING},
	{"TPVCCLEN2",(char *)&ZTPVCCLEN2,INT},
	{"TPVCFILE3",null_str,STRING},
	{"TPVCMINOR3",(char *)&ZTPVCMINOR3,INT},
	{"TPVCADDR3",null_str,STRING},
	{"TPVCALEN3",(char *)&ZTPVCALEN3,INT},
	{"TPVCCONN3",null_str,STRING},
	{"TPVCCLEN3",(char *)&ZTPVCCLEN3,INT},
	{"TPDGINFO_ADDR",(char *)&ZTPDGINFO_ADDR,INT},
	{"TPDGINFO_OPTIONS",(char *)&ZTPDGINFO_OPTIONS,INT},
	{"TPDGINFO_TSDU",(char *)&ZTPDGINFO_TSDU,INT},
	{"TPDGINFO_ETSDU",(char *)&ZTPDGINFO_ETSDU,INT},
	{"TPDGINFO_CONNECT",(char *)&ZTPDGINFO_CONNECT,INT},
	{"TPDGINFO_DISCON",(char *)&ZTPDGINFO_DISCON,INT},
	{"TPVCINFO_ADDR",(char *)&ZTPVCINFO_ADDR,INT},
	{"TPVCINFO_OPTIONS",(char *)&ZTPVCINFO_OPTIONS,INT},
	{"TPVCINFO_TSDU",(char *)&ZTPVCINFO_TSDU,INT},
	{"TPVCINFO_ETSDU",(char *)&ZTPVCINFO_ETSDU,INT},
	{"TPVCINFO_CONNECT",(char *)&ZTPVCINFO_CONNECT,INT},
	{"TPVCINFO_DISCON",(char *)&ZTPVCINFO_DISCON,INT},
	{"TPBADADDR",null_str,STRING},
	{"TPBADLEN",(char *)&ZTPBADLEN,INT},
	{"BADOPTIONS",null_str,STRING},
	{"BADOPTLEN",(char *)&ZBADOPTLEN,INT},
	{"OMAXLEN",(char *)&ZOMAXLEN,INT},
	{"DMAXLEN",(char *)&ZDMAXLEN,INT},
 	{"SEMAFILE0",null_str,STRING},
	{"SEMAADDR0",null_str,STRING},
	{"SEMAALEN0",(char *)&ZSEMAALEN0,INT},
	{"SEMACONN0",null_str,STRING},
	{"SEMACLEN0",(char *)&ZSEMACLEN0,INT},
	{"SEMAFILE1",null_str,STRING},
	{"SEMAADDR1",null_str,STRING},
	{"SEMAALEN1",(char *)&ZSEMAALEN1,INT},
	{"SEMACONN1",null_str,STRING},
	{"SEMACLEN1",(char *)&ZSEMACLEN1,INT},
	{"RF_DOMAIN",null_str,STRING},
	{"RF_NODENAME",null_str,STRING},
	{"RF_CLIENT",null_str,STRING},
	{"RF_NEWDOMAIN",null_str,STRING},
	{"RF_RESOURCE",null_str,STRING},
	{"RF_RD_RESOURCE",null_str,STRING},
	{"RF_DESCRIPTION",null_str,STRING},
	{"RF_RD_DESCRIPTION",null_str,STRING},
	{"RF_REMOTE_RESOURCE",null_str,STRING},
	{"RF_IDLOAD_DIR",null_str,STRING},
	{"RF_U_RULES",null_str,STRING},
	{"RF_G_RULES",null_str,STRING},
	{"RF_BLK_SPEC",null_str,STRING},
	{"DEFAULT_PITCH_DEV",null_str,STRING},
	{"DEFAULT_CATCH_DEV",null_str,STRING},
	{"SVVS_RFS",null_str,STRING},
	{"RF_PRIMARY",null_str,STRING},
	{"RF_SLEEP",(char *)&ZRF_SLEEP,INT},
	{"ARG_MAX",(char *)&ZARG_MAX,INT},
	{"RF_SECONDARY",null_str,STRING},
	{"CONFIGD",(char *)&ZCONFIGD},
};

/*
** Do not include CONFIGD in the list of assignable named entries
*/
#define NPARMS	((sizeof(sv_params)/sizeof(struct cnfg))-1)

#endif
