/*LINTLIBRARY*/
/*
 * z_execle.c
 */

#ifdef	UNDEF
#ident	"@(#)z_execle.c	1.2"
#endif

#include <varargs.h>
#include <sv_env.h>

#ifndef execle
int execle();
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

static struct ftable Texecle = {
	"execle",
	"no return from the function",
	"a return value of (-1)",
	format,
	compare,
	GONE,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int Zexecle(va_alist)
va_dcl
{
	va_list	ap;
	char	*a[8];
	char	**envp;
	char	*file;
	int		i;
	int		ret_val;

	va_start(ap);
	file = va_arg(ap, char *);
	for( i = 0; i < 8 && (a[i] = va_arg(ap, char *)); i++ ) ;
	envp = va_arg(ap, char **);
	va_end(ap);

	(void) sv_mproc();

	errno = 0;

	switch( i ) {
	case	0:
		ret_val=execle(file,a[0],envp);
		break;
	case	1:
		ret_val=execle(file,a[0],a[1],envp);
		break;
	case	2:
		ret_val=execle(file,a[0],a[1],a[2],envp);
		break;
	case	3:
		ret_val=execle(file,a[0],a[1],a[2],a[3],envp);
		break;
	case	4:
		ret_val=execle(file,a[0],a[1],a[2],a[3],a[4],envp);
		break;
	case	5:
		ret_val=execle(file,a[0],a[1],a[2],a[3],a[4],a[5],envp);
		break;
	case	6:
		ret_val=execle(file,a[0],a[1],a[2],a[3],a[4],a[5],a[6],envp);
		break;
	case	7:
		ret_val=execle(file,a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],envp);
		break;
#ifdef	DEVELOPMENT
	case	8:
		error("Too many args for Zexecle");
		ret_val = -1;
#endif
	}
	evaluate( &Texecle, (pointer)&ret_val );
	return( ret_val );
}
