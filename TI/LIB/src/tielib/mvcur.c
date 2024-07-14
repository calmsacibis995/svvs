# include <curses.h>
# include <sccs.h>
# include <svvs.h>
# include <sv_env.h>
# include <tiuser.h>

SCCSID("@(#)mvcur.c	3.1 (SVVS) 12/1/86");

void test_mvcur(result, len, oldy, oldx, newy, newx)
char	*result;
int	len;
int	oldy, oldx, newy, newx;
{
	static int	y,x;

	if ((y != oldy) && (x != oldx))
		error("Expected oldy to be %d and oldx %d, but they were %d and %d.", y, x, oldy, oldx);
	else
	if (y != oldy)
		error("Expected oldy to be %d, but it was %d.",y, oldy);
	else
	if (x != oldx)
		error("Expected oldx to be %d, but it was %d.",x, oldx);
	else 
	{
	y = newy;
	x = newx;
	}
}
