# include <sv_base.h>
# include <svvs.h>
# include <sv_env.h>
# include <sv_macro.h>
# include <sccs.h>

# include <varargs.h>

SCCSID("@(#)tie_error.c	3.1 (SVVS) 12/1/86");

static char	buf[512];
static int	sev;
static int	msg_type;

char		*sv_getlev();
char		*sv_snm();
static void	foo_cpy();

/* VARARGS */

msg_begin(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;
	char	*msg_name;

	va_start(ap);

	buf[0] = '\0';
	sev = va_arg(ap, int);
	msg_name = sv_snm(sev);
	msg_type = sv_sid(sev);
	sv_printf("\n%s (%s):\n\t", msg_name, sv_getlev(buf));
	sv_flush();

	format = va_arg(ap, char *);
	foo_cpy(buf, format);
	sv_vprintf(buf, ap);
	sv_flush();

	va_end(ap);
}

/* VARARGS */

msg_body(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;

	va_start(ap);

	format = va_arg(ap, char *);
	sv_vprintf(format, ap);
	sv_flush();

	va_end(ap);
}

msg_end()
{
	sv_act(sev, sv_xenv->sv_act[msg_type], sv_xenv->sv_jmpb[msg_type]);
}

static void foo_cpy( p1, p2 )
char *p1, *p2;
{
	char *p = p1;
	while( *p1 = *p2 ) {
		if(( p1 - p ) >= 509 ) {
#ifndef PRODUCTION
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
