# include "main.h"

SCCSID("@(#)block_dump.c	3.1 (SVID) 12/1/86");

# define	SIZE	800

extern	void	perror();

block_dump()
{
/*
	char	buff[SIZE];
	int	x;
	char	ch = '0';

	for(x = 0; x < SIZE; x++)
	{
		buff[x] = ch;
		if (ch == '9')
		{
			ch = '0';
		}
		else
		{
			ch++;
		}
	}
	ch = 1;
	for (x = 0; x < SIZE; x++)
	{
		if (write(1, buff, SIZE) != SIZE)
		{
			(void) fprintf(stderr, "Not enough\n");
		}
		if ( ch == 0 && ioctl(1,TCSBRK,1) == -1 )
		{
			ch = 1;
			perror("ioctl");
		}
	}
*/
}
