# include	<bless.h>
# include	<common.h>
# include	<varargs.h>

SCCSID("@(#)wrapper.c	3.2 (SVID) 12/30/86");


# define	BUFSIZE		17000

char	Buffer1[BUFSIZE];
char	Buffer2[BUFSIZE];
static	char	*Current = Buffer2;
static	FILE	*Datafp = NULL;
/*
** init_data
**	Initialize the data file.
*/
init_data(file)
char	*file;
{
	if ( Datafp != NULL )
		(void) fclose(Datafp);

	Datafp = fopen(file,"r");
# ifdef	TRACE
	if ( Trace(7,0) )
		printf("init_data: fopen of %s == %o\n",file,Datafp);
# endif	/* TRACE */
}

/*
** get_data
**	Get data from the input device.
**
** Arguments
**	str	pointer into the buffer where you currently are.
** Return Value
**	Returns a pointer to the start of a buffer.
**	NULL	on an error.
*/
char	*
get_data(str)
char	*str;
{
	int		len;
	int		i;

	if ( Datafp == NULL )
	{
		if ( (Datafp = fopen("data","r")) == NULL )
		{
			perror("data, can't open");
			return (NULL);
		}
		(void) fseek(Datafp,0L,0);
	}
	if ( feof(Datafp) )
		return (str);
	if ( low(str) == ERR )
		return (str);
	if ( Current == Buffer2 )
		Current = Buffer1;
	else
		Current = Buffer2;
	if ( str == (char *) NULL )
		len = 0;
	else
	{
		len = strlen(str);
		(void) memcpy(Current,str,len);
	}
	i = fread(&Current[len],1,(BUFSIZE - 1) - len,Datafp);
	if ( i <= 0 )
		return (str);
	Current[i + len] = '\0';
# ifdef	TRACE
	if ( Trace(7,0) )
		printf("get_data: read %d, %10s\n",i,&Current[len]);
# endif	/* TRACE */
	return (Current);
}
/*
** low
**	Indicate if it is time to read more data.
**
** Arguments
**	str	A pointer into a buffer.
**	
** Return value
**	OK	If low, or a NULL string.
**	ERR	If not low.
*/
int
low(str)
char	*str;
{
	if ( str == (char *) NULL )
		return (OK);
	if ( strlen(str) <= 20 )
		return (OK);
	return (ERR);
}

/*
** error
**	this is a holder, as the REAL error function
**	is in the SVVS lib, but the bless program doesn't
**	use the lib so we have this skeleton.
*/
error(fmt,va_alist)
va_dcl
{
	va_list	ap;

	va_start(ap);
	vprintf(fmt,ap);
	va_end(ap);
}

/*
** msg_drop
**	this is a holder, as the REAL msg_drop function
**	is in the TIETLX lib, but the bless program doesn't
**	use the lib so we have this skeleton.
*/
msg_drop(va_alist)
va_dcl
{
}
