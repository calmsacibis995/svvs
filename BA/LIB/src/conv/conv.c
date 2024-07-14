#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<ctype.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	LOWERA		'a'
# define	LOWERZ		'z'
# define	UPPERA		'A'
# define	UPPERZ		'Z'
# define	UPLODELTA	('a' - 'A')
# define	LASTASCII	0x7F


#ident	"@(#)conv.c	1.2"




/*
** conv
**	Test the conv function.
*/
main()
{
	long	ret_val;
	long	i;

setup("conv");
funct("toupper");
testing("toupper(all possible ascii values) should yield");
testing("ascii values corresponding to upper case chars.");
start
	for(i = 0; i <= 255; i++)
	{
		if ( (i < LOWERA) || (i > LOWERZ) )
			expecting (i);
		else
			expecting (i - UPLODELTA);

		toupper(i);
	}

	if ( (ret_val = toupper(EOF)) != EOF )
		error ("toupper(EOF) returned a wrong value of %d",ret_val);
finis


funct("tolower");
testing("tolower(all possible ascii values) should yield");
testing("ascii values corresponding to lower case chars.");
start
	for(i = 0; i <= 255; i++)
	{
		if ( (i < UPPERA) || (i > UPPERZ) )
			expecting (i);
		else
			expecting (i + UPLODELTA);

		tolower(i);
	}

	if ( (ret_val = tolower(EOF)) != EOF )
		error ("toupper(EOF) returned a wrong value of %d",ret_val);
finis


funct("_toupper");
testing("_toupper(all possible ascii values) should yield");
testing("ascii values corresponding to upper case chars.");
start
	for(i = LOWERA; i <= LOWERZ; i++)
	{
		expecting (i-UPLODELTA);
		_toupper(i);
	}
finis


funct("_tolower");
testing(" _tolower(all possible ascii values) should yield");
testing("ascii values corresponding to lower case chars.");
start
	for(i = UPPERA; i <= UPPERZ; i++)
	{
		expecting (i+UPLODELTA);
		_tolower(i);
	}
finis


funct("toascii");
testing("toascii(all possible ascii values) should yield");
testing("ascii values less than or equal to 127.");
start
	expecting (VOID);
	for(i = 0; i <= 255; i++)
	{
		if ( (ret_val = toascii(i)) > LASTASCII )
			    error ("%s'%d'%s'%d'",
			    "toascii returned a wrong ascii value, converting",
			    i, " to ",  ret_val);
	}

	if ( (ret_val = toascii(EOF)) > LASTASCII )
		error ("toascii(EOF) returned a wrong value of %d",ret_val);
finis


done();
/*NOTREACHED*/

}
