/*LINTLIBRARY*/
/*
 * sv_printf.c - unbuffered printf
 */
#include <stdio.h>
#include <varargs.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_printf.c	1.6"
#endif

static int outfd = 1;
static char *outbuf = (char *)0;
static char *outptr;

sv_vprintf( format, ap )
char *format;
char *ap;
{
	int cnt;
	char *sv_malloc();
	char *no_mem = "SEVERR():\n\tCan't malloc space for output buffer.\n";

	if( !outbuf ) {
		if( !(outbuf=sv_malloc(2048)) ) {
			write( outfd, no_mem, strlen(no_mem) );
			exit( SEVERR );
		} 
		else outptr = outbuf;
	}	
	outptr += vsprintf(outptr, format, ap );
}
sv_flush() { 
	if( outbuf )
		(void) write( outfd, outbuf, outptr-outbuf );
	outptr = outbuf;
}
/*VARARGS1*/
sv_printf(va_alist)
va_dcl
{
	va_list ap;
	char *format;

	va_start( ap );
	format = va_arg(ap, char *);
	sv_vprintf( format, ap );
	va_end( ap );
}
sv_set_outfd( fd )
int fd;
{
	outfd = fd;
}
sv_get_outfd() {
	return( outfd );
}
sv_prntline( str )
char *str;
{
	sv_printf("\n");
	if( sv_xenv->sv_levflg ) {
		sv_prlev();
		sv_printf("\t%s\n\t",str);
		sv_xenv->sv_levflg=0;
	}
	else
		sv_printf("\t");
	va_end( ap );
}
sv_abstract(va_alist)
va_dcl
{
	va_list ap;
	char *format;

	va_start(ap);
	sv_prntline("Abstract");
	format = va_arg(ap, char *);
	sv_vprintf( format, ap );
	va_end(ap);
}
sv_preparation(va_alist)
va_dcl
{
	va_list ap;
	char *format;

	va_start(ap);
	sv_prntline("Preparation");
	format = va_arg(ap, char *);
	sv_vprintf( format, ap );
	va_end(ap);
}
sv_testing(va_alist)
va_dcl
{
	va_list ap;
	char *format;

	va_start(ap);
	sv_prntline("Testing");
	format = va_arg(ap, char *);
	sv_vprintf( format, ap );
	va_end(ap);
}
sv_cleanup(va_alist)
va_dcl
{
	va_list ap;
	char *format;

	va_start(ap);
	sv_prntline("Cleanup");
	format = va_arg(ap, char *);
	sv_vprintf( format, ap );
	va_end(ap);
}
sv_funct(foo)
char *foo;
{
	sv_printf("\n");
	sv_prlev();
	sv_printf("Function:\n\t");
	sv_printf(foo);
	sv_printf("\n");
#ifdef	DEVELOPMENT
	if(sv_xenv!=&sv_deflt){
		warning("funct macro invoked inside test blk");
	}
#endif
}
