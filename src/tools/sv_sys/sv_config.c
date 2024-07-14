/*LINTLIBRARY*/
#include <fcntl.h>
#define	SV_CONFIG
#include <sv_conf.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_config.c	1.6"
#endif

#define	EOLN	'\n'

/*	The sv_config() function reads a file which is made up of lines with
 *	keywords on the beginning of each line and processes that file
 *	according to the rules defined in svtune.h.  Svtune.h defines the
 *	array "params" which is an array of structures each containing a
 *	keyword and a pointer to a function which performs the desired
 *	action. The static nparms is the number of keywords
 *	recognized by the parser.
 *
 *	In this case sv_config() is used to set up the tunable values for
 *	the SVVS modules.  It receives a pointer to an array of
 *	data structures defined in svtune.h as an argument.  It then
 *	loads that array based on the configuration file.
 */

static	struct	cnfg	*params;
static	int		nparms;
char	*getenv();
char	*sv_malloc();
unsigned	long	val();

sv_config() {
	char	curlin[512];
	char	*path;
	int	fd;

	params = &sv_params[0];
	nparms = NPARMS;

/*
 *	CONFIGD is resolved as a pointer.  Lint will complain, no problem
 */
	if(CONFIGD)
		return;

	if( !(path = getenv("CONFIG")) || !*path )
		path = DEFAULT;
	if((fd = open(path,O_RDONLY)) < 0)
		severr("Cannot open config file: '%s'.",path);
	while(getln(fd,curlin)){
		stripcom(curlin);
		parse(curlin);
	}
	if(close(fd))
		severr("Cannot close config file.");
/*
 *	CONFIGD is resolved as a pointer, lint will complain, no problem
 */
	CONFIGD = 1;	/*Set CONFIGD */
	reset_cache();			/* in case called twic */
}
static int	parse(curlin)
char	*curlin;
{
	int	i;
	char	*str;
	char	def[512];
	char	*nxtok();

	str = curlin;					/* check for blank line */
	while( isspace(*str) ) str++;
	if(*str){
		curlin = nxtok(def,curlin,(int *)0);
		for(i = 0; i < nparms; i++ )
			if(!strcmp(def,params[i].name)) {
				assign(i,curlin);
				return;
			}				
		error("Unrecognized symbol: '%s' in config file",def);
	}
}
/*
 *	lint will complain about pointer alignments for assignment functions
 */
static assign(pos,line)
int		pos;
char	*line;
{
	char	tmp_buf[512];
	int		strsize;
	int		vtyp;
	char	*nxtok();
	char	*target;
	unsigned long thing;

	if( (vtyp=params[pos].typ) == STRING ) {
		(void) nxtok(tmp_buf,line,&strsize);
		if( !(params[pos].value = target = sv_malloc(strsize)) )
			severr("Can't malloc space for config variables");
		memcpy(target,tmp_buf,strsize);
		return;
	}
	target = params[pos].value;
	thing = val(line);

	switch( vtyp ) {
	case UCHAR:
 		*(unsigned char *)target=(unsigned char)thing;
		break;
	case CHAR:
		*target=(char)thing;
		break;
	case USHORT:
		*(unsigned short *)target=(unsigned short)thing;
		break;
	case SHORT:
		*(short *)target=(short)thing;
		break;
	case UINT:
		*(unsigned int *)target=(unsigned int)thing;
		break;
	case INT:
		*(int *)target=(int)thing;
		break;
	case ULONG:
		*(unsigned long *)target=thing;
		break;
	case LONG:
		*(long *)target=(long)thing;
		break;
#ifdef	DEVELOPMENT
	default:
		warning("unknown type in sv_config.h");
		break;
#endif
	}
}		
static char *nxtok(dst,src,len)
char	*src,*dst;
int	*len;
{
	char *c_string();
	int	foo;

	if( !len ) len = &foo;
	
	*len = 1;	/* trailing '\0' ---> 1 char minimum in target */
	while( isspace(*src) ) src++;
	if(*src == '"')
		src = c_string( dst, src, len );/* quoted characters in str */
	else {
		while(*src && !isspace(*dst = *src++)) {
			dst++;
			(*len)++;
		}
		*dst = 0;
	}
	while( isspace(*src) ) src++;
	return(src);
}
static int isspace(c)
char	c;
{
	return((c == ' ') || (c == '\t') || (c == '\n'));
}
static unsigned long val(str)
char *str;
{
	char	num[512];
	unsigned	long	retval = 0l;
	char *sstr, *styp, *p;

	(void) nxtok(num,str,(int *)0);
	commas(num);
	p=num;
	switch( *num ) {
	case '+': case '-': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9':
		sstr="%ld";
		styp="decimal";
		break;
	case '0':
		if( *(num+1) == 'x' || *(num+1) == 'X' ) {
			p += 2;
			sstr="%lx";
			styp="hexadecimal";
			break;
		}
		sstr="%lo";
		styp="octal";
		break;
	default:
		error("Found: %s when expecting a number in config file",num);
		return( retval );
	}
	if(!sscanf(p,sstr,&retval))
		error("Bad or missing %s value: '%s' in config file",styp,num);
	return(retval);
}
static int stripcom(str)
char	*str;
{
	static	int	comment = 0;
	char	*st,*end;
	int	quote;

	end = st = str;
	while(*end){
		if(comment){
			while( *end && (*end != '*' || *(end + 1) != '/')) ++end;
			if(!*end){
				*str = 0;
				return;
			}
			comment = 0;
			if(st != str)
				*st++ = ' ';
			end +=2;
		}
		quote = 0;
		while( *end && (*end != '/' || *(end + 1) != '*' || quote)){
			if( *end == '"')
				quote = !quote;
			*st++ = *end++;
		}
		if( *end ){
			comment = 1;
			end += 2;
		}
	}
	*st = 0;
}

static int commas(str)
char	*str;
{
	char	*d;

	d = str;
	while(*str){
		if(*str != ',')
			*d++ = *str;
		str++;
	}
	*d = 0;
}
static int getln(fd,buf)
int fd;
char *buf;
{
	int getbyte();
	int i = 0;

	while( getbyte(fd,buf) ) {
		if( *buf == '\n' ) {
			if (i) break;
			continue;	/* strip empty lines */
		}
		if(i++ > 512)
			severr("sv_config: line too long in config file");
		buf++;
	}
	*buf = '\0';
	return(i);
}
static int cnt = 512;
static int my_fd = -1;
static int i = 512;
static char *buf;
static int reset_cache() {
	cnt = 512;
	my_fd = -1;
	i = 512;
	if( buf ) free( buf );
	buf = (char *)0;
}	
static int getbyte(fd,p)
int fd;
char *p;
{
	if( !buf && !(buf=sv_malloc(512))) 
		severr("sv_config: cannot malloc space for read buffer");
	if(my_fd > 0 && fd != my_fd)
		severr("sv_config: internal error");
	else my_fd = fd;
	if( i >= cnt) {
		cnt = read(fd,buf,512);
		i = 0;
	}	
	if ( i < cnt ) {
		*p = buf[i++];
		return(1);
	}
	return(0);
}	
char *c_string( dst, src, len )
char *dst, *src;
int *len;
{
	int	val;

	*len = 1;
	if( *src != '"' )	/* should never happen */
		severr("missing '\"' at start of string in config file.");
	src++;
	while( *src ) {
		switch( *src ) {
		case 	'"':
			*dst='\0';
			return( ++src );
		case	'\n':
			severr("Imbedded newline in config file string value.");
		case	'\\':
			src++;
			switch( *src ) {
			case	'\0':
				severr("Unterminated string in config file.");
			case	'n':
				*dst++ = '\n';
				(*len)++;
				src++;
				break;
			case	't':
				*dst++ = '\t';
				(*len)++;
				src++;
				break;
			case	'b':
				*dst++ = '\b';
				(*len)++;
				src++;
				break;
			case	'r':
				*dst++ = '\r';
				(*len)++;
				src++;
				break;
			case	'0':
			case	'1':
			case	'2':
			case	'3':
			case	'4':
			case	'5':
			case	'6':
			case	'7':
			case	'8':	/* accept 8 and 9 even though not octal */
			case	'9':
				val = 0;
				while( *src >= '0' && *src <= '9' ) {
					val *= 8;
					val += *src - '0';
					src++;
				}
				if( !*src )				
					severr("Unterminated string in config file.");
				*dst++ = (char) val;
				(*len)++;
				break;
			default:
				*dst++ = *src++;
				(*len)++;
				break;
			}
			break;
		default:			
			*dst++ = *src++;
			(*len)++;
			break;
		}
	}
	severr("Unterminated string in config file.");
}
