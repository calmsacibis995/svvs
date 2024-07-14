#include	<sv_base.h> /*replacement macros */
#include	<setjmp.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)setjmp.c	1.5"



jmp_buf		Env1, Env2, Env3;
int	expected;	/* value expected from setjmp */
int	ret_val;

static float	flt;
static double	dbl;

/*
** Test the setjmp function.
*/
main()
{
	setup("setjmp");

	abstract ("Test setjmp with static variables, and a local longjmp.");
	abstract ("These all call error with HALT, as the later tests");
	abstract ("depend on what this is testing.");
	start
		local_test();
	finis

	funct("setjmp");
	abstract ("Test setjmp with static variables, and nonlocal longjmp.");
	start
		setjmp_test();
	finis

	funct("longjmp");
	abstract ("Do some boundary tests on long_jmp.");
	start
		lngjmp_test();
	finis
	
	abstract ("Test setjmp/longjmp with multiple environments.");
	start
		env_test();
	finis

	done();
/*
 *	NOTREACHED
 */
}
/*
** local_test
**	Test setjmp with static variables, and a local longjmp.
**	These all call error with HALT, as the later tests
**	depend on what this is testing.
**
** Level:
**	This test is in the normal level (0).
*/
local_test()
{
	static int	a,b,c;
	static long	d,e;
	static short	f,g;

	/* Initialize */
	a = b = c = 0;
	d = e = 0;
	f = g = 0;
	flt = 0.0;
	dbl = 0.0;

	expected = 0;


	testing("Initial call to setjmp should return 0, static variables ");
	testing("should be set to 0.  Setjmp after a call to longjmp");
	testing("should return 1, with the static variables set to 1.");
	start
		errno = 0;
		/* no replacement function for setjmp */
		if ( !(ret_val = setjmp(Env1)) ) {
			if ( expected )
			error("%s%s%d",
				"Setjmp returned zero after a call to longjmp\n",
				"expected: ", expected);
			a = b = 1;
			c = 1;
			d = 1;
			e = 1L;
			f = 1;
			g = 1;
			flt = 1.0;
			dbl = 1.0;

			expected = 1;
			errno = 0;
			longjmp(Env1, 1);  /*  setjmp should return 1  */
			error("longjmp returned at point of call");
			return;
		}
		if ( ret_val != expected )
			error ("%s%d%s%d",
				"unexpected return value from setjmp\nexpected:",
				expected,"\nactual:",ret_val);

		if ( a != 1 )
			error("static variable a is %d, not 1", a);
		if ( b != 1 )
			error("static variable b is %d, not 1", b);
		if ( c != 1 )
			error("static variable c is %d, not 1", c);
		if ( d != 1 )
			error("static variable d is %ld, not 1", d);
		if ( e != 1L )
			error("static variable e is %ld, not 1", e);
		if ( f != 1 )
			error("static variable f is %d, not 1", f);
		if ( g != 1 )
			error("static variable g is %d, not 1", g);
		if ( flt != 1.0 )
			error("static float flt is %f, not 1.0", flt);
		if ( dbl != 1.0 )
			error("static double dbl is %f, not 1.0", dbl);
	finis
}
/*
** setjmp_test
**	Test setjmp with static variables, and nonlocal longjmp.
**
** Level:
**	This test is in the normal level (0).
*/
setjmp_test()
{
	static int	a,b,c;
	static long	d,e;
	static short	f,g;

	/* Initialize */
	a = b = c = 0;
	d = e = 0;
	f = g = 0;
	flt = 0.0;
	dbl = 0.0;
	expected = 0;


	testing("Initial call to setjmp should return 0, static variables ");
	testing("should be set to 0.  Setjmp after a call to longjmp");
	testing("which is located in a separate, called  function");
	testing("should return 1, with the static variables set to 1.");
	start
		errno = 0;
		/* no replacement function for setjmp */
		if ( !(ret_val = setjmp(Env1)) ) {
			if ( expected )
			error("%s%s%d",
				"Setjmp returned zero after a call to longjmp\n",
				"expected: ", expected);
			expected = 1;
			a = b = c = 1;
			d = e = 1;
			f = g = 1;
			flt = 1.0;
			dbl = 1.0;
			errno = 0;
			call_longjmp();
			error("longjmp returned at point of call");
			return;
		}
		if ( ret_val != expected )
			error ("%s%d%s%d",
				"unexpected return value from setjmp\nexpected:",
				expected,"\nactual:",ret_val);

		if ( a != 1 )
			error("static variable a is %d, not 1", a);
		if ( b != 1 )
			error("static variable b is %d, not 1", b);
		if ( c != 1 )
			error("static variable c is %d, not 1", c);
		if ( d != 1 )
			error("static variable d is %ld, not 1", d);
		if ( e != 1 )
			error("static variable e is %ld, not 1", e);
		if ( f != 1 )
			error("static variable f is %d, not 1", f);
		if ( g != 1 )
			error("static variable g is %d, not 1", g);
		if ( flt != 1.0 )
			error("static float flt is %f, not 1.0", flt);
		if ( dbl != 1.0 )
			error("static double dbl is %f, not 1.0", dbl);
	finis
}
/*
** call_longjmp
**
**	Call the longjmp routine, for setjmp_test
*/
call_longjmp()
{
	errno = 0;
	longjmp(Env1,1);
	error(
	  "longjmp returned at point of call within separate called funct");
	return;
}
/*
** lngjmp_test
**	Do some boundary tests on long_jmp.
**
** Level:
**	This test is in the boundary level (1).
*/
lngjmp_test()
{
	static int	ret_val;

	testing("Initial call to setjmp should return 0.");
	testing("First call to longjmp(0) should return 1 from setjmp");
	testing("subsequent setjmp's should return argument passed to longjmp");
	testing("Call longjmp with an argument of 0");
	testing("Check that setjmp returns 1");
	testing("Then call longjmp with arguments of 2,3,5,100,-5, and -17");
	start
		errno = 0;
		expected = 0;
		ret_val = setjmp(Env1);
		if ( expected != ret_val ) /* leave this for subsequent longjumps*/
		{
			error("%s%s%d%s%d",
				"setjmp did not return the value passed by longjmp\n",
				"expected: ",expected,"\nreturned: ",ret_val);
			return;
		}
		switch ( ret_val ) {

		case 0:
			errno = 0;
			expected = 1;
			longjmp(Env1,0);
			error("longjmp returned at point of call");
			return;

		case 1:
			errno = 0;
			expected = 2;
			longjmp(Env1,2);
			error("longjmp returned at point of call");
			return;
	
		case 2:
			errno = 0;
			expected = 3;
			longjmp(Env1,3);
			error("longjmp returned at point of call");
			return;
	
		case 3:
			errno = 0;
			expected = 5;
			longjmp(Env1,5);
			error("longjmp returned at point of call");
			return;
	
		case 5:
			errno = 0;
			expected = 100;
			longjmp(Env1,100);
			error("longjmp returned at point of call");
			return;
	
		case 100:
			errno = 0;
			expected = -5;
			longjmp(Env1,-5);
			error("longjmp returned at point of call");
			return;

	
		case -5:
			errno = 0;
			expected = -17;
			longjmp(Env1,-17);
			error("longjmp returned at point of call");
			return;
	
		case -17:
			break;
	
		default:
			error("%s%s%d%s%d",
			"Setjmp returned an unexpected value\n",
			"expected: ",expected, 
			"\nactual: ",ret_val);
			return;
		}
	finis
}

/*
** env_test
**
**	Test the setjmp/longjmp routines with multiple environments.
**
** Level:
**	This test is in the robustness level (2).
*/
env_test()
{
	static int	exp1, exp2, exp3;

	testing ("Initial calls to setjmp should return 0.");
	testing ("Subsequent calls to setjmp should return to correct");
	testing ("locations with correct return values");
	start
		exp1 = exp2 = exp3 = 0;
		errno = 0;
		ret_val = setjmp(Env1);
		if ( ret_val != exp1 ) {
			error("%s\n%s%d\n%s%d",
				"Unexpected return value from setjmp",
				"expected: ",exp1,
				"actual: ", ret_val);
			return;
		}
		exp1 = 1;
		errno = 0;
		if ( !ret_val ) {
			ret_val = setjmp(Env2);
			if (ret_val != exp2) {
				error("%s\n%s%d\n%s%d",
					"Unexpected return value from setjmp",
					"expected: ",exp2,
					"actual: ", ret_val);
				return;
			}
			exp2 = 2;
			errno = 0;
			if( !ret_val ) {
				ret_val  = setjmp(Env3);
				if ( ret_val != exp3 ) {
					error("%s\n%s%d\n%s%d",
						"Unexpected return value from setjmp",
						"expected: ",exp3,
						"actual: ", ret_val);
					return;
				}
				exp3 = 3;
				errno = 0;
				if( !ret_val ) {
					longjmp(Env3,3);
					error("longjmp returned at point of call");
					return;
				} else {
					longjmp(Env2,2);
					error("longjmp returned at point of call");
					return;
				}
			} else {
				longjmp(Env1,1);
				error("longjmp returned at point of call");
				return;
			}
		}
	finis
}
