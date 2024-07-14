/*
 *	scanf, fscanf and sscanf test
 *
 *  Data is in a file as follows:
 *	Fformat-string
 *	Rreturn-value
 *	Ddata-to-be-scanned
 *	cchar-value expected to be returned
 *	ddouble-value expected to be returned
 *	ffloat-value expected to be returned
 *	hshort-int-value expected to be returned
 *	iinteger-value expected to be returned
 *	llong-value expected to be returned
 *	sstring-value expected to be returned
 *	uunsigned-value expected to be returned
 *	vunsigned-short-value expected
 *
 *	Numbers must be in decimal.
 *	No whitespace is permitted.
 *
 *  The F, R and D lines are mandatory.
 *  The c d f h i l s u v lines follow as needed.
 *  Ex:
 *	F%d printed an integer
 *	D123 printed an integer
 *	i123
 *	Ftest for %%
 *	Dtest for %
 */


#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#include <fcntl.h>

#ident	"@(#)scanf1.c	1.7"

#undef	strcmp	/*  disable error checking  */

/*
 *  Test for %i %n if version 3 or above
 */
#ifndef	SYSVR_2
#define	DATAFILE	"allcases"
#else
#define	DATAFILE	"mostcases"
#endif

# define	STDIN_FD	0


int	data_fd	= 0;	/*  the file desc. for the data file of test cases  */
char	lookahead = ' ';  /*  assure it isn't EOF at beginning  */
FILE	*Read_Stream;


/*
 *  These are filled in by reading the DATAFILE.
 *  The arguments are places in the ?_exp appropriate for their type.
 *  exp_ptr[] points to the appropriate datum, and
 *  exp_types identifies the data's type.
 *	Ex:the file	Fthis is a format
 *			Dthis is data
 *			R2
 *			i23
 *			f1.2
 *	sets	this_format to "this is a format"
 *		exp_retval = 2
 *		this_data to "this is data"
 *		i_exp [0] = 23
 *		exp_ptrs [0] = &i_exp [0]
 *		exp_types [0] = 'i'
 *		f_exp [1] = 1.2
 *		exp_ptrs [1] = &f_exp [1]
 *		exp_types [1] = 'f'
 *
 *	when lex_line is called repeatedly with args of -2, -1, 0, 1.
 *	lex_line calls getaline to read a line from the DATAFILE.
 *
 *  The limit of 5 arguments is hard-coded.
 *
 */
char	this_format [STRLEN];
char	this_data [STRLEN];
char	*exp_ptrs [5];
char	exp_types [5];
int 	exp_retval;
char	c_exp [5];	/*  expected values  */
char	s_exp [5] [STRLEN];
short int	h_exp [5];
int	i_exp [5];
unsigned int	u_exp [5];
float	f_exp [5];
double	d_exp [5];
long	l_exp [5];
unsigned short	v_exp [5];

/*
 *  These values are returned by scanf, and are compared to the above
 */
char	*ret_ptrs [5];	/*  they are the same type as exp_types  */
char	c_ret [5];
char	s_ret [5] [STRLEN];
short int	h_ret [5];
int	i_ret [5];
unsigned int	u_ret [5];
unsigned short	v_ret [5];
float	f_ret [5];
double	d_ret [5];
long	l_ret [5];


/*
 *  getaline - read a line from datafile into the string
 *	returns 0 for EOF, 1 otherwise
 */
int	getaline (str)
char	*str;
{
static	char	buffer [STRLEN];
static	char	*next_char = buffer + 1;
static	char	*last_char = buffer;
int	i;

do
	{
	if (next_char > last_char)
		{
		/*  fill the buffer  */
		if ((i = read (data_fd, buffer, STRLEN)) < 1)
			return (0);
		last_char = buffer + i - 1;
		next_char = buffer;
		}
	} while ((*str++ = *next_char++) != '\n');
*--str = '\0';
/*  get the lookahead character  */
if (next_char > last_char)
	{
	/*  fill the buffer  */
	if ((i = read (data_fd, buffer, STRLEN)) < 1)
		{
		lookahead = '\0';
		return (1);
		}
	last_char = buffer + i - 1;
	next_char = buffer;
	}
lookahead = *next_char;
return (1);
}
	
char	lex_line (sub)
int	sub;
{
char za[STRLEN];
char		*ptr, *ptr2;
int	negative;
double	dec_sf;

int	i;
unsigned int	u;
short int	h;
float	f;
double	d;
long	l;
unsigned short	v;

if (!getaline (za))
	return (0);

/*
 *  are we oversubscripting?
 */
if (sub > 4)
	severr ("line '%s' causes more than 5 arguments", za);

ptr = &za [1];
/*  are the mandatory lines missing?  */
if ((sub < 0) &&
    (za [0] !='F') && (za [0] != 'D') && (za [0] != 'R'))
	{
	severr ("In lex_line: F, D, or R missing before line '%s'\n",za);
	}
if ((sub >= 0) &&
    ((za [0] =='F') || (za [0] == 'D') || (za [0] == 'R')))
	{
	severr ("In lex_line: F line missing before line '%s'", za);
	}
switch (za[0])
  {
  case 'F':ptr2 = this_format; while (*ptr2++ = *ptr++) ;
	break;

  case 'D':ptr2 = this_data; while (*ptr2++ = *ptr++) ;
	break;

  case 'R':exp_retval = 0;
	while (*ptr)
		exp_retval = exp_retval * 10 + *ptr++ - '0';
	break;

  case 'i':
	negative = i = 0;
	if (*ptr == '-')
		{
		ptr++;
		negative = 1;
		}
	while (*ptr)
		i = i * 10 + *ptr++ - '0';
	if (negative)
		i = (-i);
	i_exp [sub] = i;
	exp_ptrs [sub] = (char *) &i_exp [sub];
	exp_types [sub] = 'i';
	ret_ptrs [sub] = (char *) &i_ret [sub];
	break;

  case 'h':
	h = 0;
	while (*ptr)
		h = h * 10 + *ptr++ - '0';
	h_exp [sub] = h;
	exp_ptrs [sub] = (char *) &h_exp [sub];
	exp_types [sub] = 'h';
	ret_ptrs [sub] = (char *) &h_ret [sub];
	break;

  case 'u':
	u = 0;
	while (*ptr)
		u = u * 10 + *ptr++ - '0';
	u_exp [sub] = u;
	exp_ptrs [sub] = (char *) &u_exp [sub];
	exp_types [sub] = 'u';
	ret_ptrs [sub] = (char *) &u_ret [sub];
	break;

  case 'v':
	v = 0;
	while (*ptr)
		v = v * 10 + *ptr++ - '0';
	v_exp [sub] = v;
	exp_ptrs [sub] = (char *) &v_exp [sub];
	exp_types [sub] = 'v';
	ret_ptrs [sub] = (char *) &v_ret [sub];
	break;


  case 'l':
	l = 0L;
	negative = 0;
	if (*ptr == '-')
		{
		ptr++;
		negative = 1;
		}
	while (*ptr)
		l = l * 10 + *ptr++ - '0';
	if (negative)
		l = (-l);
	l_exp [sub] = l;
	exp_ptrs [sub] = (char *) &l_exp [sub];
	exp_types [sub] = 'l';
	ret_ptrs [sub] = (char *) &l_ret [sub];
	break;

  case 'c':
	c_exp [sub] = *ptr;
	exp_ptrs [sub] = (char *) &c_exp [sub];
	exp_types [sub] = 'c';
	ret_ptrs [sub] = (char *) &c_ret [sub];
	break;

  case 's':
	ptr2 = s_exp [sub];
	while (*ptr2++ = *ptr++)
		 ;
	exp_ptrs [sub] = s_exp [sub];
	exp_types [sub] = 's';
	ret_ptrs [sub] = s_ret [sub];
	break;

	
  case 'f':
  case 'd':
	d = 0L;
	negative = 0;
	if (*ptr == '-')
		{
		ptr++;
		negative = 1;
		}
	expecting (VOID); /*  disable error checking */
	while (isdigit (*ptr))
		d = d * 10 + *ptr++ - '0';
	if (*ptr == '.')
		{
		dec_sf = 10.0;
		while (isdigit (*++ptr))
			{
			d += (double)(*ptr - '0') / dec_sf;
			dec_sf *= 10;
			}
		}
	if (*ptr)
		{
		severr ("lex_line: illegal floating point starts at '%s'\n",
			ptr);
		break;
		}
	if (negative)
		d = (-d);

	if (za [0] == 'f')	/*  float arg?  */
		{
		f_exp [sub] = (float) d;
		exp_ptrs [sub] = (char *) &f_exp [sub];
		exp_types [sub] = 'f';
		ret_ptrs [sub] = (char *) &f_ret [sub];
		}
	else
		{	/*  double arg  */
		d_exp [sub] = d;
		exp_ptrs [sub] = (char *) &d_exp [sub];
		exp_types [sub] = 'd';
		ret_ptrs [sub] = (char *) &d_ret [sub];
		}
	break;
	
  default:
	if (!za [0])  /*  don't confuse NULL with EOF return  */
		za [0] = ' ';
	error ("lex_line: cannot recognize line '%s'\n", za);
	break;
  }
return (za [0]);
}

/*
** scanf
**	Test the scanf,fscanf and sscanf routines.
**
**	The function tested depends on the name of the file.
**	The executable is linked to three names:
**	scanf1  -- scanf test
**	scanf2  -- sscanf test
**	scanf3  -- fscanf test
**
*/

main(argc, argv)
int	argc;
char	*argv[];
{
int	n_args;
int	loop;
int	ret_val;

char	this_test;	/*  ' ', 'f' or 's'  */
char	*test_name;	/*  "scanf", "fscanf" or "sscanf"  */
char	test_line [STRLEN];	/*  build up the testing line here  */
char	yabuffer [STRLEN];	/* yet another buffer, used to create the above  */
char	*escape ();
char	*unesc1 ();
char	*unesc2 ();

setup ("scanf");

prep ("initialize depending on the test chosen");
start

	if ((data_fd = data_open (DATAFILE, O_RDONLY)) < 3)
		{
		severr ("cannot open datafile '%s'", DATAFILE);
		return (0);
		}
	switch (argv [0] [5])
	{
	    case '1':
		this_test = ' ';  /*  scanf test  */
		test_name = "scanf";
		expecting (SUCCESS);
		fclose (stdin);
		expecting (VOID);
		close (STDIN_FD);
		break;

	    case '2':
		this_test = 'f';  /*  fscanf test  */
		test_name = "fscanf";
		break;

	    case '3':
		this_test = 's';  /*  sscanf test  */
		test_name = "sscanf";
		break;

	    default:
		error ("'%s' is an invalid test name, scanf[123] was expected",
			argv [0]);
		done ();
		/*  NOTREACHED  */

	}
finis


funct (test_name);
abstract ("The arguments are listed in () with their types and");
abstract ("expected values.  For error messages, arg 0 is the");
abstract ("first argument after the format");

/*
 *  loop for all test cases
 */
while (lookahead)
{
/*
 *  read in all lines for this test case
 */
exp_types [0] = exp_types [1] = exp_types [2] = ' ';
exp_types [3] = exp_types [4] = ' ';
n_args = -3;
while (lex_line (n_args++))
	{
	if ((lookahead == 'F') || (lookahead == 0))
		break;
	}
if (n_args < 0)
	{
	severr ("In main: F, D, or R missing after line \"F%s\"\n",
		this_format);
	return;
	}

/*
 *  format the testing line
 */

switch (this_test)
  {
  case 's':strcpy (test_line, "sscanf (\"%s\"");
	break;

  case 'f':strcpy (test_line, "fscanf (fd, \"%s\"");
	break;

  default:strcpy (test_line, "scanf (\"%s\"");
  }

for (loop = 0; loop < 5 ; loop++)
	{
	switch (exp_types [loop])
	  {
	  case 'c':strcat (test_line, ", (char)'");
		strncat (test_line, &c_exp [loop], 1);
		strcat (test_line, "'");
		break;
	  case 'i':sprintf (yabuffer, ", (int)%d", i_exp [loop]);
		strcat (test_line, yabuffer);
		break;
	  case 'h':sprintf (yabuffer, ", (short int)%hd", h_exp [loop]);
		strcat (test_line, yabuffer);
		break;
	  case 'l':sprintf (yabuffer, ", (long)%ld", l_exp [loop]);
		strcat (test_line, yabuffer);
		break;
	  case 'u':sprintf (yabuffer, ", (unsigned)%u", u_exp [loop]);
		strcat (test_line, yabuffer);
		break;
	  case 'v':sprintf (yabuffer, ", (unsigned short)%hu", v_exp [loop]);
		strcat (test_line, yabuffer);
		break;
	  case 's':strcat (test_line, ", (char *)\"");
		strcat (test_line,  s_exp [loop]);
		strcat (test_line, "\"");
		break;
	  case 'f':sprintf (yabuffer, ", (float)%f", f_exp [loop]);
		strcat (test_line, yabuffer);
	 	break;
	  case 'd':sprintf (yabuffer, ", (double )%le", d_exp [loop]);
		strcat (test_line, yabuffer);
		break;
	  default:if (loop < n_args)
		severr ("arg %d is unknown type '%c'",loop,exp_types [loop]);
	  }
	}
strcat (test_line,")\nwith the data \"%s\" should return %d.");

testing (test_line, this_format, this_data, exp_retval);

start

/*
 *  Now that the test has been printed,
 *  translate the escape codes in the format and the data
 *  (ex: \n becomes the newline character)
 */
escape (this_format);
escape (this_data);

/*  COMMENTED OUT - there are no z_functs for scanf, sscanf, fscanf
 *  expecting (VOID);
 */
switch (this_test)
    {
    case 's':
	ret_val = sscanf (this_data, this_format,
		ret_ptrs [0], ret_ptrs [1], ret_ptrs [2],
		ret_ptrs [3], ret_ptrs [4]);
	break;
    case 'f':
	open_file (this_data);
	ret_val = fscanf (Read_Stream, this_format,
		ret_ptrs [0], ret_ptrs [1], ret_ptrs [2],
		ret_ptrs [3], ret_ptrs [4]);
	expecting (SUCCESS);
	fclose (Read_Stream);
	break;
    default:
	open_file (this_data);
	ret_val = scanf (this_format,
		ret_ptrs [0], ret_ptrs [1], ret_ptrs [2],
		ret_ptrs [3], ret_ptrs [4]);
	expecting (SUCCESS);
	fclose (Read_Stream);
    }
if ( ret_val != exp_retval)
	error ("%s returned %d, expecting %d",
		test_name, ret_val, exp_retval);

	
for (loop = 0; loop < n_args ; loop++)
	{
	switch (exp_types [loop])
	  {
	  case 'c':if (c_exp [loop] != c_ret [loop])
		error  ("arg %d: expected char = %c, actual = %c\n",
			loop, c_exp [loop], c_ret [loop]);
		break;
	  case 'i':	if (i_exp [loop] != i_ret [loop])
		error  ("arg %d:expected int = %d, actual = %d\n",
			loop, i_exp [loop], i_ret [loop]);
		break;
	  case 'h':	if (h_exp [loop] != h_ret [loop])
		error  ("arg %d:expected (short int) = %hd, actual = %d\n",
			loop, h_exp [loop], h_ret [loop]);
		break;
	  case 'l':	if (l_exp [loop] != l_ret [loop])
		error  ("arg %d:expected long = %ld, actual = %ld\n",
			loop, l_exp [loop], l_ret [loop]);
		break;
	  case 'u':	if (u_exp [loop] != u_ret [loop])
		error  ("arg %d:expected unsigned = %u, actual = %u\n",
			loop, u_exp [loop], u_ret [loop]);
		break;
	  case 'v':	if (v_exp [loop] != v_ret [loop])
		error  ("arg %d:expected unsigned short = %hu, actual = %u\n",
			loop, v_exp [loop], v_ret [loop]);
		break;
	  case 's':	if (strcmp (escape (s_exp [loop]), s_ret [loop]))
		error  ("arg %d:expected string = %s, actual = %s\n",
			loop, unesc1 (s_exp [loop]), unesc2 (s_ret [loop]));
		break;
	  case 'f':	if (f_exp [loop] != f_ret [loop])
		error  ("arg %d:expected float = %f, actual = %f\n",
			loop, f_exp [loop], f_ret [loop]);
		break;
	  case 'd':	if (d_exp [loop] != d_ret [loop])
		error  ("arg %d:expected double = %lf, actual = %lf\n",
			loop, d_exp [loop], d_ret [loop]);
		break;
	  default:	severr ("match failed for unknown type = '%c'",
			exp_types [loop]);
	  }
	}
finis
}  /*   loop for all test cases  */

done();
/*NOTREACHED*/
}

/*
** open_file()
**	This function will open the stdin for writing, write a char
**	to it, and close the write file.  At the end it opens the
**	stdin for reading and exits.
*/
open_file(data_ptr)
char		*data_ptr;
{
	FILE		*write_stream;
	extern	FILE	*temp_fopen ();
	int	length;

	expecting (SUCCESS);
	write_stream = temp_fopen("tmpfscanf", "w");
	expecting (VOID);
	length = strlen (data_ptr);
	expecting (length);
	fwrite(data_ptr, 1, length, write_stream);
	expecting (SUCCESS);
	fclose(write_stream);
	Read_Stream = temp_fopen("tmpfscanf", "r");
}

char *
escape(str)
char *str;
{
char *src, *dest;
char	it;
int	hasaslash= 0;

src = dest = str;

while ( *src )
	{
	if (*src == '\\')
		{
		hasaslash = 1;
		++src;
		it = *src++;
		switch (it)
			{
			case '\\':
				*dest++ = '\\';
				break;
			case 'b':
				*dest++ = '\b';
				break;
			case 'f':
				*dest++ = '\f';
				break;
			case 'n':
				*dest++ = '\n';
				break;
			case 'r':
				*dest++ = '\r';
				break;
			case 't':
				*dest++ = '\t';
				break;
			case 'v':
				*dest++ = '\v';
				break;
			default:
				*dest++ = it;
			}
		}
	else
		if (hasaslash)
			*dest++ = *src++;
		else
			{
			dest++;
			src++;
			}
	}
*dest = '\0';
return (str);
}

/*
 *  Map control characters into printable characters.
 *  Two copies are needed since two buffers must be allocated in order
 *  to print two strings in one call to error().
 */
char *
unesc1 ( str)
char *str;
{
static char buffer[100];
char *dest = buffer;
while (*str)
	{
	if (*str < ' ')
		{
		*dest++ = '^';
		*dest++ = *str++ + '@';
		}
	else
		*dest++ = *str++;
	}
*dest = '\0';
return (buffer);
}


char *
unesc2 ( str)
char *str;
{
static char buffer[100];
char *dest = buffer;
while (*str)
	{
	if (*str < ' ')
		{
		*dest++ = '^';
		*dest++ = *str++ + '@';
		}
	else
		*dest++ = *str++;
	}
*dest = '\0';
return (buffer);
}
