#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)abs.c	1.1"

/*
** abs.c
**	Test the abs function.
*/

main()
{
	int	variable;

	setup("abs");
	funct("abs");
	testing ("abs(0) should return 0");
	start
		expecting (0);
		abs(0);
	finis


	testing ("abs(1) should return 1");
	start
		expecting (1);
		abs(1);
	finis

	testing ("abs(-1) should return 1");
	start
		expecting (1);
		abs(-1);
	finis

	testing ("abs(variable = 0) should return 0");
	start
		expecting (0);
		variable = 0;
		abs(variable);
	finis

	testing ("abs(variable = 1) should return 1");
	start
		expecting (1);
		variable = 1;
		abs(variable);
	finis

	testing ("abs(variable = -1) should return 1");
	start
		expecting (1);
		variable = -1;
		abs(variable);
	finis

	testing ("abs(INT_MIN+1) should return -(INT_MIN +1).");
	start
		expecting ( -(INT_MIN + 1));
		abs(INT_MIN + 1);
	finis

	testing ("abs(INT_MAX) should return INT_MAX.");
	start
		expecting (INT_MAX);
		abs(INT_MAX);
	finis

	testing ("abs (variable = INT_MIN+1) should return -(INT_MIN+1).");
	start
		expecting ( -(INT_MIN + 1));
		variable = (INT_MIN + 1);
		abs(variable);
	finis

	testing ("abs(variable = INT_MAX) should return INT_MAX.");
	start
		expecting (INT_MAX);
		variable = INT_MAX;
		abs(variable);
	finis

	done();
	/*NOTREACHED*/
}
