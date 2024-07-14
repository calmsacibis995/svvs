/*LINTLIBRARY*/
/*
 *	sv_error.c	- svvs error generation
 */
#include <varargs.h>
#include <sv_env.h>

#ifdef	UNDEF
#ident	"@(#)sv_error.c	1.5"
#endif

char *sv_getlev();

/*VARARGS1*/
sv_message(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;

	va_start( ap );
	format = va_arg(ap, char *);
	sv_vprerr( MESSAGE, format, ap );
	va_end( ap );
}
/*VARARGS1*/
sv_unsupported(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;

	va_start( ap );
	format = va_arg(ap, char *);
	sv_vprerr( UNSUPPORTED, format, ap );
	va_end( ap );
}
/*VARARGS1*/
sv_warning(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;

	va_start( ap );
	format = va_arg(ap, char *);
	sv_vprerr( WARNING, format, ap );
	va_end( ap );
}
/*VARARGS1*/
sv_error(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;

	va_start( ap );
	format = va_arg(ap, char *);
	sv_vprerr( ERROR, format, ap );
	va_end( ap );
}
/*VARARGS1*/
sv_severr(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;

	va_start( ap );
	format = va_arg(ap, char *);
	sv_vprerr( SEVERR, format, ap );
	va_end( ap );
}
/*VARARGS2*/
sv_prerr( va_alist )
va_dcl
{
	va_list	ap;
	int sev;
	char *str;

	va_start( ap );
	sev = va_arg(ap, int);
	str = va_arg(ap, char *);
	sv_vprerr( sev, str, ap );
	va_end( ap );
}
sv_vprerr( sev, format, ap )
int sev;
char *format;
char *ap;
{
	char buf[512];
	char *msg_name;
	int	msg_type;

	char	*sv_snm();

	buf[0] = '\0';
	msg_name = sv_snm(sev);
	msg_type = sv_sid(sev);
	sv_printf("\n%s (%s):\n\t",msg_name,sv_getlev( buf ));
	foo_cpy( buf, format );
	sv_vprintf( buf, ap );
	sv_act( sev, sv_xenv->sv_act[msg_type], sv_xenv->sv_jmpb[msg_type] );
}
static foo_cpy( p1, p2 )
char *p1, *p2;
{
	char *p = p1;
	while( *p1 = *p2 ) {
		if(( p1 - p ) >= 509 ) {
#ifdef	DEVELOPMENT
	 		(void) printf("\nWARNING:\n\terror message too long\n");
#endif
			goto done;
		}
		p1++;
		if( *p2++ == '\n' ) 	/* tab out error message		*/
			if(*p2) 			/* no tab if msg ends w/ '\n'	*/
				*p1++ = '\t';	
	}
done:
/*
 *	add '\n' to end of msg if empty msg ("") or does not end with '\n'
 */	
	if( !( p1 - p ) || ( *(p1-1) != '\n' ) )
		*p1++ = '\n';
	*p1 = '\0';
}
