#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<memory.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)crypt.c	1.4"



# define	DESLEN		64	/* Length of the buffer */

# define	DO_ENCRYPT		0

#ifdef CRYPT
extern	char	*crypt();
#endif
#ifdef SETKEY
extern	void	setkey();
#endif
#ifdef ENCRYPT
extern	void	encrypt();
#endif

/*
** test the crypt routines.
*/

main()
{

#ifndef SETKEY
	char *ptr;
	int i;
	void ioerror();
#endif
setup("crypt");
funct("setkey");
funct("crypt");
	t_crypt();
#ifndef SETKEY	
	unsupported("setkey not a supported function");
#endif
funct("encrypt");
funct("setkey");
	t_encrypt();
#ifndef SETKEY
#ifndef CRYPT
#ifndef ENCRYPT
	exit(0);
#endif
#endif
#endif
done();
/*NOTREACHED*/
}

/*
** t_crypt
**	Test the crypt routine.
*/
t_crypt()
{
#ifdef CRYPT
	char	key[STRLEN];
	char	salt[3];
	char	*ret_val;
	char	oldval[STRLEN];

testing("Encrypting a string/password with");
testing("a string not equal to the original string.");
start

	strcpy(key,"Tis time the walrus said");
	salt[0] = 'a';
	salt[1] = 'b';
	salt[2] = '\0';

	exit_on (ERROR);
	expecting (VOID);

	if ((ret_val = crypt(key,salt)) == NULL)
		{
		error ("crypt returned NULL");
		}

	if ( strcmp(key,ret_val) == 0 )
	{
		error ("crypt did not encrypt the string");
		return;
	}

	strcpy(oldval,ret_val);
finis


testing("crypt with the same password and salt should yield");
testing("the same encrypted password back.");
start
	strcpy(key,"Tis time the walrus said");
	salt[0] = 'a';
	salt[1] = 'b';
	salt[2] = '\0';

	expecting (VOID);

	if ((ret_val = crypt(key,salt)) == NULL)
		{
		error ("crypt returned NULL");
		}

	if ( strcmp(key,ret_val) == 0 )
	{
		error ("crypt did not encrypt the string");
	}

	if ( strcmp(oldval,ret_val) )
	{
		error ("crypt did not return the same password");
	}
finis


testing("crypt with a different password, but the same salt should yield");
testing("a different encrypted password back.");
start
	strcpy(key,"Tis NOT time the walrus said");
	salt[0] = 'a';
	salt[1] = 'b';
	salt[2] = '\0';

	expecting (VOID);

	if ((ret_val = crypt(key,salt)) == NULL)
		{
		error ("crypt returned NULL");
		}

	if ( strcmp(key,ret_val) == 0 )
	{
		error ("crypt did not encrypt the string");
	}

	if ( strcmp(oldval,ret_val) == 0 )
	{
		error ("crypt returned the same password");
	}
finis


testing("crypt with a different password, and a different salt should yield");
testing("a different encrypted password back.");
start
	strcpy(key,"Tis NOT time the walrus said");
	salt[0] = 'a';
	salt[1] = 'C';
	salt[2] = '\0';

	expecting (VOID);

	if ((ret_val = crypt(key,salt)) == NULL)
		{
		error ("crypt returned NULL");
		}

	if ( strcmp(key,ret_val) == 0 )
	{
		error ("crypt did not encrypt the string");
	}

	if ( strcmp(oldval,ret_val) == 0 )
	{
		error ("crypt returned the same password");
	}
finis


#else
	char *ptr;
	int i;
	void ioerror();
	
	unsupported  ("crypt not a supported function");

#endif
}

/*
** t_encrypt
**	Test the encryption routines
*/
t_encrypt()
{
#ifdef ENCRYPT
	char	block[DESLEN];
	char	newblock[DESLEN];
	char	newkey[DESLEN];
	char	origblock[DESLEN];
	char	origdata[9];

	int	i;
	int	j;

testing("encrypt should yield");
testing("an encrypted buffer back.");
start
	expecting (VOID);

	j = 'a';
	for ( i = 0 ; i < CHAR_BIT ; i++ )
	{
		origdata[i] = j;
		if ( j == 'z' )
			j = 'A';
		else
			if ( j == 'Z' )
				j = 'a';
			else
				j++;
	}

	origdata[CHAR_BIT] = '\0';
	makebits(origdata,block);

	memcpy(newblock,block,DESLEN);
	encrypt(block,DO_ENCRYPT);
	if ( memcmp(newblock,block,DESLEN) == 0 )
	{
		bintoas(block,DESLEN);
		error ("encrypt did not change the block, it remained %.*s",
		    DESLEN, block);
	}
finis


testing("Encrypt two different blocks of data should yield");
testing("The two encrypted blocks to be different.");
start
	expecting (VOID);
	
	for ( i = 0 ; i < CHAR_BIT ; i++ )
		origdata[i] = 'Z';

	makebits(origdata,block);
	encrypt(block,DO_ENCRYPT);

	for ( i = 0 ; i < CHAR_BIT ; i++ )
		origdata[i] = '0';

	makebits(origdata,newblock);
	encrypt(newblock,DO_ENCRYPT);

	if ( memcmp(newblock,block,DESLEN) == 0)
	{
		bintoas(block,DESLEN);
		bintoas(newblock,DESLEN);
		block[DESLEN -1] = '\0';
		newblock[DESLEN -1] = '\0';
		error ("encrypt encrypted two different blocks to the same string");
	}
finis


#ifdef SETKEY
testing("Use setkey to change the encoding key, then encrypt some text");
testing("should yield a differently encoded text back.");
start
	expecting (VOID);

	for ( i = 0 ; i < CHAR_BIT ; i++ )
		origdata[i] = '1';

	makebits(origdata,newkey);
	makebits(origdata,block);
	memcpy(origblock,block,DESLEN);
	makebits(origdata,newblock);
	encrypt(block,DO_ENCRYPT);

	setkey(newkey);
	encrypt(newblock,DO_ENCRYPT);

	if ( memcmp(block,newblock,DESLEN) == 0 )
	{
		bintoas(block,DESLEN);
		bintoas(newblock,DESLEN);
		block[DESLEN -1] = '\0';
		newblock[DESLEN -1] = '\0';
		error ("setkey had no effect on encrypt");
	}
finis


#endif
testing("Encrypting two different blocks of data should yield");
testing("two different encrypted blocks.");
start
	expecting (VOID);
	
	for ( i = 0 ; i < CHAR_BIT ; i++ )
		origdata[i] = 'Z';

	makebits(origdata,block);
	encrypt(block,DO_ENCRYPT);

	for ( i = 0 ; i < CHAR_BIT ; i++ )
		origdata[i] = '0';

	makebits(origdata,newblock);
	encrypt(newblock,DO_ENCRYPT);

	if ( memcmp(newblock,block,DESLEN) == 0)
	{
		bintoas(block,DESLEN);
		bintoas(newblock,DESLEN);
		block[DESLEN -1] = '\0';
		newblock[DESLEN -1] = '\0';
		error ("encrypt encrypted two different blocks to the same string\%s'%s'\n%s'%s'",
			"first block = ",newblock,
			"other block = ",block);
	}
finis


#else
	char *ptr;
	int i;
	void ioerror();
	
	unsupported ("encrypt not a supported function");

#endif

}

/*
** makebits
**	Turn a string of bytes into a series of bits.
*/
makebits(origdata,block)
char	*origdata;
char	*block;
{
	int	i,j;

	i = 0;

	/*
	** Put each bit of each byte of the origdata into
	** a byte of 'block'.
	*/
	while ( i < CHAR_BIT )
	{
		for ( j = 0 ; j < CHAR_BIT ; j++ )
		{
			if ( (i * CHAR_BIT) + j >= DESLEN )
				break;

			block[ (i * CHAR_BIT) + j] = (origdata[i] >> j) & 01;
		}
		i++;
	}
}

/*
** bintoas
**	Change a simple binary 0 into -> '0', and a 1 -> '1'.
**	I.e. this is a binary to ascii routine. It does all
**	changes in place.
*/
bintoas(buf,len)
char	*buf;
int	len;
{
	while ( len-- )
	{
		buf[len] += '0';
	}
}
