#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)strtol.c	1.3"



char	Alpha[] = "0123456789abcdefghijklmnopqrstuvwxyz";

extern	long	strtol();
extern	long	atol();
extern	int	atoi();

/*
** strtol
**	Test the string to long routines.
*/
main()
{
setup("strtol");

funct ("strtol");
start
	t_strtol();
finis

funct ("atol");
start
	t_atol();
finis

funct ("atoi");
start
	t_atoi();
finis

done();
/*NOTREACHED*/
}

/*
** t_strtol
**	Check the string to long conversions.
*/
t_strtol()
{
	long	ret_val;
	char	*ptr;
	char	*optr;
	int	base;
	char	errstr[STRLEN];

testing("strtol(123456,&ptr,10) should return");
testing("123456, with ptr pointing at the NULL byte.");
start
	expecting (123456L);
	strtol("123456",&ptr,10);

	if ( ptr == NULL )
	{
		error ("ptr is NULL");
	}

	if ( *ptr != '\0' )
	{
		error ("ptr points to '%c', should be a NULL", *ptr);
	}
finis


testing("strtol(&^%$,&ptr,10) should return");
testing("0, with ptr pointing to the '&'.");
start
	expecting (0L);
	strtol("&^%$",&ptr,10);

	if ( ptr == NULL )
	{
		error ("ptr is NULL");
	}

	if ( *ptr != '&' )
	{
		error ("ptr points to '%c', should be a '&'", *ptr);
	}
finis


testing("strtol(0,(char **) 0,10) should return 0.");
start
	expecting (0L);
	strtol("0",(char **) 0,10);
finis


testing("strtol(123,(char **) 0,10) should return 123.");
start
	expecting (123L);
	strtol("123",(char **) 0,10);
finis


testing("strtol(+123,(char **) 0,10) should return +123.");
start
	expecting (123L);
	strtol("+123",(char **) 0,10);
finis


testing("strtol( 123,(char **) 0,10) should return 123.");
start
	expecting (123L);
	strtol(" 123",(char **) 0,10);
finis


testing("strtol(-123,(char **) 0,10) should return -123.");
start
	expecting (-123L);
	strtol("-123",(char **) 0,10);
finis


testing("0x123 with a base of 16 should return 123 (hex).");
start
	expecting (0x123L);
	strtol("0x123",(char **) 0,16);
finis


testing("0X123 with a base of 16 should return 123 (hex).");
start
	expecting (0x123L);
	strtol("0X123",(char **) 0,16);
finis


testing("123 with a base of 0 should return 123.");
start
	expecting (123L);
	strtol("123",(char **) 0,0);
finis


testing("0123 with a base of 0 should return 0123 (octal).");
start
	expecting (0123L);
	strtol("0123",(char **) 0,0);
finis


testing("0x123 with a base of 0 should return 0x123 (hex).");
start
	expecting (0x123L);
	strtol("0x123",(char **) 0,0);
finis


testing("0X123 with a base of 0 should return 0X123 (hex).");
start
	expecting (0x123L);
	strtol("0X123",(char **) 0,0);
finis


testing("strtol(!@#$,&ptr,0)");
testing("should return 0, with ptr pointing to the '!'.");
start
	expecting (0L);
	strtol("!@#$",&ptr,0);

	if ( ptr == NULL )
	{
		error ("ptr is NULL");
	}
	if ( *ptr != '!' )
	{
		error ("ptr points to '%c', should be a '!'", *ptr);
	}
finis


testing("strtol(321&,&ptr,0)");
testing("should return 321, with ptr pointing to the '&'.");
start
	expecting (321L);
	strtol("321&",&ptr,0);

	if ( ptr == NULL )
	{
		error ("ptr is NULL");
	}
	if ( *ptr != '&' )
	{
		error ("ptr points to '%c', should be a '&'", *ptr);
	}
finis


testing("0123456789abcdefghijklmnopqrstuvwxyz, with bases ranging from 2");
testing("to 36 work should move ptr along one character at a time.");
start
	optr = &Alpha[1];
	for ( base = 2 ; base <= 36 ; base++ )
	{
		expecting (VOID);  /* don't care about return values  */
		ret_val = strtol(Alpha,&ptr,base);
		optr++;
		if ( ptr != optr )
		{
			error ("for base %d, ptr points to '%c', should be '%c'",
				base, *ptr, *optr);
		}

	}
finis

}

/*
** t_atol
**	Check the string to long conversions.
*/
t_atol()
{
	long	ret_val;

testing("atol(123456) should return 123456.");
start
	expecting (123456L);
	atol("123456");
finis



testing("atol(&^%$) should return 0.");
start
	expecting (0L);
	atol("&^%$");
finis


testing("atol(0) should return 0.");
start
	expecting (0L);
	atol("0");
finis


testing("atol(123) should return 123.");
start
	expecting (123L);
	atol("123");
finis


testing("atol(+123) should return +123.");
start
	expecting (123L);
	atol("+123");
finis


testing("atol( 123) should return 123.");
start
	expecting (123L);
	atol(" 123");
finis


testing("atol(-123) should return -123.");
start
	expecting (-123L);
	atol("-123");
finis
}

/*
** t_atoi
**	Check the string to int conversions.
*/
t_atoi()
{
	long	ret_val;

testing("atoi(1234) should return 1234.");
start
	expecting (1234);
	atoi("1234");
finis


testing("atoi(&^%$) should return 0.");
start
	expecting (0);
	atoi("&^%$");
finis


testing("atoi(0) should return 0.");
start
	expecting (0);
	atoi("0");
finis


testing("atoi(123) should return 123.");
start
	expecting (123);
	atoi("123");
finis


testing("atoi(+123) should return +123.");
start
	expecting (123);
	atoi("+123");
finis


testing("atoi( 123) should return 123.");
start
	expecting (123);
	atoi(" 123");
finis


testing("atoi(-123) should return -123.");
start
	expecting (-123);
	atoi("-123");
finis
}
