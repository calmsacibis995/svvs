#include	<sv_base.h> /*replacement macros */
#include	<memory.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)memory.c	1.6"



/*
** memory
**	Test the memory operations.
*/
main()
{
setup("memory");

funct ("memccpy");
start
	t_memccpy();
finis

funct ("memchr");
start
	t_memchr();
finis

funct ("memcmp");
start
	t_memcmp();
finis

funct ("memcpy");
start
	t_memcpy();
finis

funct ("memset");
start
	t_memset();
finis


done();
/*NOTREACHED*/
}

/*
** t_memccpy
**
**	Test the memccpy routine.
*/
t_memccpy()
{
	char	inbuf[STRLEN + 1];
	char	outbuf[STRLEN + 1];
	char	errstr[STRLEN];
	char	*sp;
	int	i;
	int	j;

testing("copying a block where the operation is terminated by the count");
testing ("rather than by finding the character");
testing("should return NULL, and a copied block.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
	{
		inbuf[i] = (i % 2) ? 'a' : 'b';
	}
	inbuf[STRLEN] = outbuf[STRLEN] = '\0';

	expecting (NULL);
	memccpy(outbuf,inbuf,(int) 'c',STRLEN);

	if ( (i = compare(outbuf,inbuf,STRLEN)) != STRLEN )
		error ("memccpy did not correctly copy two strings,\n\
mismatch at character %d", i);
finis


testing("memccpy should properly stop copying characters when");
testing("an occurrence of the character 'c' occurs, in the middle of s2");
testing("returning the address of s2[STRLEN / 2 + 1],");
testing("and a partly copied string.");
start

	for ( i = 0 ; i < STRLEN ; i++ )
	{
		inbuf[i] = (i % 2) ? 'd' : 'e';
	}
	inbuf[STRLEN / 2] = 'c';

	expecting (&outbuf [(STRLEN / 2) + 1]);
	sp = memccpy(outbuf,inbuf,(int) 'c',STRLEN);

	if ( (i = compare(inbuf,outbuf,STRLEN)) != (STRLEN / 2 + 1) )
	{
		inbuf[(STRLEN/ 2) + 2] = '\0';
		outbuf[(STRLEN/ 2) + 2] = '\0';
		error ("The block '%s'\nwas incorrectly copied as '%s'",
			inbuf, outbuf);
	}
finis


testing("a string only of 'c' will only copy a 'c'");
testing("returning the address of sp1[1], and one character copied.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
	{
		inbuf[i] = 'c';
	}

	expecting (&outbuf[1] );
	sp = memccpy(outbuf,inbuf,(int) 'c',STRLEN);

	if ( (i = compare(inbuf,outbuf,STRLEN)) != 1 )
	{
		inbuf[(STRLEN/ 2) + 2] = '\0';
		outbuf[(STRLEN/ 2) + 2] = '\0';
		error ("The block '%s'\nwas incorrectly copied as '%s'",
			inbuf, outbuf);
	}
finis


testing("memory copies to various boundaries");
testing("should yield a copied string.");
start

	for ( i = 0 ; i < 30 ; i++ )
	{

		for ( j = 0 ; j < STRLEN ; j++ )
			inbuf[j] = (i % 2) ? 'a' : 'b';

		expecting (VOID);
		sp = memccpy(&outbuf[i],inbuf,'c',STRLEN - i);

		if ( sp != NULL )
		{
			error ("On a copy to outbuf[%d], memccpy did not \
return NULL",i);
		}
		if ( compare(&outbuf[i],inbuf,STRLEN - i) != (STRLEN - i) )
		{
			error ("on a copy to outbuf[%d], memccpy did not \
copy correctly",i);
		}
	}
finis

}

/*
** t_memchr
**	Test the memory comparison function.
*/
t_memchr()
{
	char	buffer[STRLEN];
	char	*sp;
	int	j;
	int	i;

testing("memchr should return NULL if a character is not in a block.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
		buffer[i] = 'a';

	expecting (NULL);
	memchr(buffer,(int) 'b', STRLEN);
finis


testing("memchr with a character embedded in a string");
testing("should return the location of the character.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
		buffer[i] = 'a';

	buffer[STRLEN / 2] = 'b';
	expecting (VOID);
	sp = memchr(buffer,(int) 'b', STRLEN);

	if ( sp == NULL )
	{
		error ("memchr returned NULL when the character \
it was searching for was in the block");
	}
	else
		if ( sp != &buffer[STRLEN / 2] )
		{
			error ("memchr returned %o instead of %o",
				sp, &buffer [STRLEN /2]);
		}
finis


testing("memchr with a character at the start of a string");
testing("should return the location of the character.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
		buffer[i] = 'a';

	buffer[0] = 'b';
	expecting (VOID);
	sp = memchr(buffer,(int) 'b', STRLEN);

	if ( sp == NULL )
	{
		error ("memchr returned NULL");
	}
	else
		if ( sp != buffer )
		{
			error ("memchr returned %o instead of %o",
				sp, buffer);
		}
finis


testing("memchr on a number of different boundaries");
testing("should return the location of the character.");
start
	for ( i = 0 ; i < 30 ; i++ )
	{
		for ( j = 0 ; j < STRLEN ; j++ )
			buffer[j] = 'a';

		buffer[STRLEN - 1] = 'b';
		expecting (VOID);
		sp = memchr(&buffer[i],(int) 'b', STRLEN - i);

		if ( sp == NULL )
		{
			error ("memchr returned NULL");
		}

		if ( sp != &buffer[STRLEN - 1] )
		{
			error ("memchr returned %o instead of %o",
				sp, &buffer [STRLEN - 1]);
		}
	}
finis

}

/*
** t_memcmp
**	Test the memory comparison routine.
*/
t_memcmp()
{
	char	inbuf[STRLEN];
	char	outbuf[STRLEN];
	int	i;

testing("Comparing two identical blocks should return 0.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
		inbuf[i] = outbuf[i] = 'Z';

	expecting (0);
	memcmp(inbuf,outbuf,STRLEN);
finis


testing("comparison of two unequal blocks, with the first ");
testing("lexically greater than the second should return > 0.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
	{
		inbuf[i] = 'a';
		outbuf[i] = 'b';
	}

	expecting (VOID);
	if ( (i = memcmp(outbuf,inbuf,STRLEN)) <= 0 )
	{
		error("memcmp returned <= 0");
	}
finis


testing("comparison of two unequal blocks, with the first ");
testing("lexically less than the second should return < 0.");
start
	expecting (VOID);
	if ( (i = memcmp(inbuf,outbuf,STRLEN)) >= 0 )
		error("memcmp returned >= 0");
finis


testing("Comparison of two blocks with only the first character different");
testing("should return > 1.");
start
	for ( i = 0 ; i < STRLEN ; i++ )
	{
		inbuf[i] = outbuf[i] = '5';
	}
	inbuf[0] = '6';

	expecting (VOID);
	if ( (i = memcmp(inbuf,outbuf,STRLEN)) <= 0 )
		error("memcmp returned <= 0");
finis


testing("Comparison of two blocks with only the last character different");
testing("should return > 1.");
start
	inbuf[0] = '5';
	inbuf[STRLEN - 1] = '6';

	expecting (VOID);
	if ( (i = memcmp(inbuf,outbuf,STRLEN)) <= 0 )
		error("memcmp returned <= 0");
finis


testing("Comparison of two blocks with only a middle character different");
testing("should return > 1.");
start
	inbuf[STRLEN - 1] = '5';
	inbuf[STRLEN / 2] = '6';

	expecting (VOID);
	if ( (i = memcmp(inbuf,outbuf,STRLEN)) <= 0 )
		error("memcmp returned <= 0");
finis


testing("Comparison of a block to itself should return 0.");
start
	expecting (VOID);
	if ( (i = memcmp(inbuf,inbuf,STRLEN)) != 0 )
		error("memcmp did not return 0");
finis


testing("Comparison of overlapping blocks should return 0.");
start
	expecting (VOID);
	if ( (i = memcmp(outbuf,&outbuf[3],STRLEN / 2)) != 0 )
		error("memcmp did not return 0");
finis

}

/*
** t_memcpy
**	Test the memory copy routine
*/
t_memcpy()
{
	char	inbuf[STRLEN];
	char	outbuf[STRLEN];
	char	errstr[STRLEN];
	char	*sp;
	int	i,j;

testing("memcpy a block should");
testing("copy a block, and return the first block.");
start

	for ( i = 0 ; i < STRLEN ; i++ )
	{
		inbuf[i] = 'a';
		outbuf[i] = 'z';
	}

	expecting (outbuf);
	memcpy(outbuf,inbuf,STRLEN);

	if ( compare(inbuf,outbuf,STRLEN) != STRLEN )
	{
		inbuf[STRLEN - 1] = '\0';
		outbuf[STRLEN - 1] = '\0';
		error ("memcpy incorrectly copied the block '%s'\n as '%s'",
			inbuf,outbuf);
	}
finis


testing("Copy of adjacent blocks");
testing("should yield copied blocks, and a return value of the first block.");
start
	for ( i = 0 ; i < STRLEN / 2 ; i++ )
		inbuf[i] = 'a';
	for ( i = (STRLEN / 2) + 1 ; i < STRLEN ; i++ )
		inbuf[i] = 'b';
	expecting (inbuf);
	memcpy(inbuf,&inbuf[(STRLEN / 2) + 1], 10);
finis


testing("Copies to various memory alignments should");
testing("copy blocks, returning the beginning of the first block.");
start
	for ( i = 0 ; i < 30 ; i++ )
	{

		for ( j = 0 ; j < STRLEN ; j++ )
			inbuf[j] = (i % 2) ? 'a' : 'b';

		expecting (&outbuf[i]);
		memcpy(&outbuf[i],inbuf,STRLEN - i);

		if ( compare(&outbuf[i],inbuf,STRLEN - i) != (STRLEN - i) )
		{
			error ("The block '%s'\nwas incorrectly copied as '%s'",
				inbuf, &outbuf [i]);
		}
	}
finis


}

/*
** t_memset
**	Test the memory setting routine.
*/
t_memset()
{
	char	buffer[STRLEN];
	char	errstr[STRLEN];
	char	*sp;
	int	i,j;

testing("'memset'ing a block");
testing("should return the block, setting the block to a character.");
start
	for ( i = 0 ; i < sizeof (buffer)  ; i++ )
		buffer[i] = 's';
	expecting (buffer);
	memset(buffer,'f',sizeof (buffer));

	for ( i = 0 ; i < sizeof (buffer) ; i++ )
	{
		if ( buffer[i] != 'f' )
		{
			error ("memset did not set character %d in the \
block correctly",i);
		}
	}
finis


testing("memset on various memory alignments");
testing("should return the beginning of the block, and a 'set' block.");
start
	for ( i = 0 ; i < 30 ; i++ )
	{
		for ( j = 0 ; j < sizeof (buffer) ; j++ )
			buffer[j] = 'k';

		expecting (&buffer [i]);
		memset(&buffer[i],'l',(sizeof (buffer)) - i);

		for ( j = i ; j < sizeof (buffer) ; j++ )
			if ( buffer[j] != 'l' )
			{
				error ("memset did not set character %d \
in the block correctly,\nwhen called with &buffer[%d]",j,i);
			}
	}
finis

}

/*
** compare
**	Compare two byte streams
**
** Returns
**	The offset to the character if there was a difference [0,...,count-1],
**	'count' if equal
*/
compare(a,b,count)
char	*a, *b;
int	count;
{
	int	number;

	number = 0;
	while ( count-- > 0 )
	{
		if ( *a++ != *b++ )
			return ( number );
		number++;
	}
	return ( number );
}
