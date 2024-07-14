/*LINTLIBRARY*/
/*
 * z_execl.c
 */

#ifdef	UNDEF
#ident	"@(#)z_execl.c	1.4"
#endif

#include <varargs.h>
#include <sv_env.h>

#ifndef execl
int execl();
#endif

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
}

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%d", *val );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}

int *sv_geti();

static struct ftable Texecl = {
	"execl",
	"no return from the function",
	"a return value of (-1)",
	format,
	compare,
	GONE,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int Zexecl(va_alist)
va_dcl
{
	va_list	ap;
	char	*a[8];
	char	*file;
	int		i;
	int		ret_val;

	va_start(ap);
	file = va_arg(ap, char *);
	for( i = 0; i < 8 && (a[i] = va_arg(ap, char *)); i++ ) ;
	va_end(ap);

	(void) sv_mproc();

	errno = 0;

	switch( i ) {
	case	0:
		ret_val=execl(file,a[0]);
		break;
	case	1:
		ret_val=execl(file,a[0],a[1]);
		break;
	case	2:
		ret_val=execl(file,a[0],a[1],a[2]);
		break;
	case	3:
		ret_val=execl(file,a[0],a[1],a[2],a[3]);
		break;
	case	4:
		ret_val=execl(file,a[0],a[1],a[2],a[3],a[4]);
		break;
	case	5:
		ret_val=execl(file,a[0],a[1],a[2],a[3],a[4],a[5]);
		break;
	case	6:
		ret_val=execl(file,a[0],a[1],a[2],a[3],a[4],a[5],a[6]);
		break;
	case	7:
		ret_val=execl(file,a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
		break;
#ifdef	DEVELOPMENT
	case	8:
		error("Too many args for Zexecl");
		ret_val = -1;
#endif
	}
	evaluate( &Texecl, (pointer)&ret_val );
	return( ret_val );
}
