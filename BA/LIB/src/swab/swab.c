#include	<sv_base.h> /*replacement macros */
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)swab.c	1.3"



void	swab();

/*
** swab
**	Swap bytes...
*/
main()
{
setup("swab");
funct("swab");

abstract ("Test swab by swapping some bytes.");
start
	norm_test();
finis

abstract ("Test swab with a couple of strange nbyte counts.");
start
	nbyte_test();
finis

abstract ("Test swab on various boundaries.");
start
	boundary_test();
finis

done();
/*NOTREACHED*/
}

/*
** nbyte_test
**	Test swab with a couple of strange nbyte counts.
*/
nbyte_test()
{
	int	i;
	int dummy1;
	char	inbuf[5];
	int dummy2;
	int dummy3;
	char	outbuf[5];

testing("swab with an odd byte count");
testing("should return: (byte count - 1), only the first two bytes swapped.");
start

	inbuf[0] = 'a';
	inbuf[1] = 'b';
	inbuf[2] = 'c';
	inbuf[3] = 'd';
	inbuf[4] = '\0';
	for (i = 0; i < 5; i++)
		outbuf[i] = '\0';

	expecting (VOID);
	swab(inbuf,outbuf,3);

	expecting (VOID);
	if ( strcmp(outbuf,"ba") != 0 )
	{
		error ("the string returned is '%s', 'bacd' expected",
			outbuf);
	}
finis


testing("swab(x,y,0) should cause no change to the input array.");
start

	inbuf[0] = 'a';
	inbuf[1] = 'b';
	inbuf[2] = 'c';
	inbuf[3] = 'd';
	inbuf[4] = '\0';
	for (i = 0; i < 5; i++)
		outbuf[i] = '\0';

	expecting (VOID);
	swab(inbuf,outbuf,0);

	expecting (VOID);
	if ( strncmp(outbuf,inbuf,4) == 0 )
	{
		error ("swab swapped some bytes, returning '%s', \
'%s' was expected",
			outbuf, inbuf);
	}
finis


testing("swab(x,y,-1) should cause no change to the input array.");
start

	inbuf[0] = 'a';
	inbuf[1] = 'b';
	inbuf[2] = 'c';
	inbuf[3] = 'd';
	inbuf[4] = '\0';
	for (i = 0; i < 5; i++)
		outbuf[i] = '\0';

	expecting (VOID);  /*  swab does nothing if nbytes is negative  */
	swab(inbuf,outbuf,-1);

	expecting (VOID);
	if ( strncmp(outbuf,inbuf,4) == 0 )
	{
		error ("swab swapped some bytes, returning '%s', \
'%s' was expected",
			outbuf, inbuf);
	}
finis
}

/*
** norm_test
**	Test swab by swapping some bytes.
**
** Level
**	This routine is in normal level (0).
*/
norm_test()
{
	int	i;
	int dummy1;
	char	buffer[5];
	int dummy2;
	int dummy3;
	char	into[5];

testing("Swapping a 4 byte array consisting of 'abcd' should return 'badc'.");
start
	buffer[0] = 'a';
	buffer[1] = 'b';
	buffer[2] = 'c';
	buffer[3] = 'd';
	buffer[4] = '\0';
	for (i = 0; i < 5; i++)
		into[i] = '\0';

	expecting (VOID);
	swab(buffer,into,4);

	if ( strncmp(into,"badc",4) != 0 )
	{
		error ("swab returned '%s'", into);
	}
finis
}

/*
** boundary_test
**
**	Test swab on various boundaries.
*/
boundary_test()
{
	int dummy1;/* for alignement purpose*/
	int dummy2;
	char	inbuf[STRLEN];
	int dummy3;/* for alignement purpose*/
	int dummy4;
	char	outbuf[STRLEN];
	int dummy5;/* for alignement purpose*/
	int dummy6;
	char	errstr[STRLEN];
	int	i;
	int	j;


testing("swab on a variety of boundaries");
testing("should return 'badc' from each call to swab.");
start

	for (i = 0 ; i < (STRLEN - 5) ; i=i+4 )
	{
		for (j = 0; j < STRLEN; j++)
		{
			outbuf[j] = '\0';
			inbuf[j] = '\0';
		}
		strcpy(&inbuf[i],"abcd");

		expecting (VOID);
		swab(&inbuf[i],&outbuf[i],4);
		if ( strncmp(&outbuf[i],"badc",4) )
		{
			error ("swab returned '%s'", &outbuf [i]);
			return;
		}
	}
finis
}
