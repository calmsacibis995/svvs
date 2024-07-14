# include <sv_base.h>
# include <svvs.h>
# include <sv_env.h>
# include <sv_macro.h>
# include <varargs.h>

#ifdef	UNDEF
#ident	"@(#)tie_error.c	1.4"
#endif

int	Msg_type;

/*
** wvs_printf
**	Print out a message.
*/
/*VARARGS*/
wvs_printf( va_alist )
va_dcl
{
	va_list	args;
	char	*fmt;

	va_start(args);
	fmt = va_arg(args, char *);
	sv_vprintf(fmt, args);
	va_end(args);
	sv_flush();
}
/*
** wvs_ew
**	Print out an error about a window.
*/
/*VARARGS*/
wvs_ew( va_alist )
va_dcl
{
	va_list	args;
	char	*fmt;
	char	buf[512];
	char	*msg_name;
	extern	char	*sv_snm();

	msg_name = sv_snm(ERROR);
	Msg_type = sv_sid(ERROR);
	buf[0] = '\0';
	va_start(args);
	fmt = va_arg(args, char *);
	sv_printf("\n%s (%s):\n\t",msg_name,sv_getlev( buf ));
	sv_vprintf(fmt, args);
	va_end(args);
}
/*
** ew_end
**	End the message about an error.
*/
ew_end()
{
	(void) sv_act( ERROR, sv_xenv->sv_act[Msg_type], sv_xenv->sv_jmpb[Msg_type] );
}
