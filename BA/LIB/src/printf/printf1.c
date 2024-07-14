#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)printf1.c	1.8"


extern	FILE	*temp_fopen();
extern	FILE	*init_fprintf();
extern	char	*end_printf();
extern	char	*end_fprintf();

/*
 *  These arrays hold the argument and return value pairs
 */
typedef	struct
	{
	char	*format;
	int	exp_val;
	char	*exp_str;
	} no_argdef;

static	no_argdef	no_args []
={
{"%%",	1,	"%"}
};




typedef	struct
	{
	char	*format;
	int	arg;	/*  one integer argument  */
	int	exp_val;
	char	*exp_str;
	} i_argdef ;


static	i_argdef	i_args []
={

/*
 *  the %i format is not defined for versions earlier than version 3
 */
#ifndef	SYSVR_2  /*  include the %i format testing  */

{"%i",		123,	3,	"123"},
{"%4i",		123,	4,	" 123"},
{"%-4i",	123,	4,	"123 "},
{"%.6i",	123,	6,	"000123"},
{"%+i",		123,	4,	"+123"},
{"%+i",		-123,	4,	"-123"},
{"% i",		123,	4,	" 123"},
{"% i",		-123,	4,	"-123"},
{"% +i",	123,	4,	"+123"},
{"% +i",	-123,	4,	"-123"},
{"%#i",		123,	3,	"123"},
{"%i %%",	123,	5,	"123 %"},	

#endif

/*  the following are from printf6.c  */
{"%d",		123,	3,	"123"},
{"%4d",		123,	4,	" 123"},
{"%-4d",	123,	4,	"123 "},
{"%.6d",	123,	6,	"000123"},
{"%+d",		123,	4,	"+123"},
{"%+d",		-123,	4,	"-123"},
{"% d",		123,	4,	" 123"},
{"% d",		-123,	4,	"-123"},
{"% +d",	123,	4,	"+123"},
{"% +d",	-123,	4,	"-123"},
{"%#d",		123,	3,	"123"},
{"%o",		0123,	3,	"123"},
{"%4o",		0123,	4,	" 123"},
{"%-4o",	0123,	4,	"123 "},
/*  the following are from printf2.c  */
{"%.6o",	0123,	6,	"000123"},
{"%#o",		0123,	4,	"0123"},
{"%4u",		123,	4 ,	" 123"},
{"%-4u",	123,	4 ,	"123 "},
{"%.6u",	123,	6 ,	"000123"},
{"%#u",		123,	3 ,	"123"},
{"%x",		0x12a,	3 ,	"12a"},
{"%4x",		0x12a,	4 ,	" 12a"},
{"%-4x",	0x12a,	4 ,	"12a "},
{"%.6x",	0x12a,	6 ,	"00012a"},
{"%#x",		0x12a,	5 ,	"0x12a"},
{"%X",		0x12a,	3 ,	"12A"},
{"%4X",		0x12a,	4 ,	" 12A"},
{"%-4X",	0x12a,	4 ,	"12A "},
{"%.6X",	0x12a,	6 ,	"00012A"},
{"%#X",		0x12a,	5 ,	"0X12A"},
{"%d %%",	123,	5,	"123 %"},	
/*  the following are from printf7.c  */
{"%o %%",	0123,	5,	"123 %"},
{"%u %%",	123,	5,	"123 %"},
{"%x %%",	0x123,	5,	"123 %"},
/*  the following is from printf8.c  */
{"%X %%",	0x123,	5,	"123 %"},
};


typedef	struct
	{
	char	*format;
	int	arg1;	/*  two integer arguments  */
	int	arg2;
	int	exp_val;
	char	*exp_str;
	} ii_argdef;

static	ii_argdef	ii_args []
={

#ifndef	SYSVR_2  /*  include the %i format testing  */

{"%.*i",	6,	123,	6,	"000123"},
{"%*i",		6,	123,	6,	"   123"},
{"%i %i",	123,	3210,	8,	"123 3210"},
/*  the following are from printf5.c  */
{"%d %i",	123,	456,	7,	"123 456"},
/*  the following are from printf6.c  */
{"%i %o",	123,	0456,	7,	"123 456"},	
{"%i %u",	123,	456,	7,	"123 456"},	
{"%i %x",	123,	0x45a,	7,	"123 45a"},	
{"%i %X",	123,	0x45a,	7,	"123 45A"},	

#endif

/*  the following are from printf6.c  */
{"%d %o",	123,	0456,	7,	"123 456"},	
{"%d %u",	123,	456,	7,	"123 456"},	
{"%d %x",	123,	0x45a,	7,	"123 45a"},	
{"%d %X",	123,	0x45a,	7,	"123 45A"},	
{"%o %u",	0123,	456,	7,	"123 456"},	
{"%o %x",	0123,	0x45a,	7,	"123 45a"},	
{"%o %X",	0123,	0x45a,	7,	"123 45A"},	
{"%.*d",	6,	123,	6,	"000123"},
{"%*d",		6,	123,	6,	"   123"},
{"%d %d",	123,	3210,	8,	"123 3210"},
/*  the following are from printf2.c  */
{"%x %x",	0x12a,	0x32b0,	8 ,	"12a 32b0"},
{"%u %u",	123,	3210,	8 ,	"123 3210"},
{"%o %o",	0123,	03210,	8 ,	"123 3210"},
{"%*x",		6,	0x12a,	6 ,	"   12a"},
{"%.*x",	6,	0x12a,	6 ,	"00012a"},
{"%*u",		6,	123,	6 ,	"   123"},
{"%.*u",	6,	123,	6 ,	"000123"},
{"%*o",		6,	0123,	6 ,	"   123"},
{"%.*o",	6,	0123,	6 ,	"000123"},
/*  the following are from printf3.c  */
{"%.*X",	6,	0x12a,	6,	"00012A"},
{"%*X",		6,	0x12a,	6,	"   12A"},
{"%X %X",	0x12a,	0x32b0,	8,	"12A 32B0"},
/*  the following are from printf7.c  */
{"%u %x",	123,	0x45a,	7,	"123 45a"},
{"%u %X",	123,	0x45a,	7,	"123 45A"},
{"%x %X",	0x123,	0x45a,	7,	"123 45A"}
};


typedef	struct
	{
	char	*format;
	long	arg;	/*  one long argument  */
	int	exp_val;
	char	*exp_str;
	} l_argdef;

static	l_argdef	l_args []
={
{"%ld",	123L,	3,	"123"},
/*  the following are from printf2.c  */
{"%lx",	0x12aL,	3 ,	"12a"},
{"%lu",	123L,	3 ,	"123"},
{"%lo",	0123L,	3,	"123"},
{"%lX",	0x12aL,	3 ,	"12A"}
};


typedef	struct
	{
	char	*format;
	double	arg;	/*  one float argument  */
	int	exp_val;
	char	*exp_str;
	} f_argdef;

static	f_argdef	f_args []
={
/*  the following are from printf3.c  */
{"%f",		123.45,	10,	"123.450000"},
{"%9f",		1.45,	9,	" 1.450000"},
{"%-11f",	123.45,	11,	"123.450000 "},
{"%.8f",	123.45,	12,	"123.45000000"},
{"%+f",		123.45,	11,	"+123.450000"},
{"%+f",		-123.45,11,	"-123.450000"},
{"% f",		123.45,	11,	" 123.450000"},
{"% f",		-123.45,11,	"-123.450000"},
{"% +f",	123.45,	11,	"+123.450000"},
{"% +f",	-123.45,11,	"-123.450000"},
{"%#f",		123.0,	10,	"123.000000"},
{"%e",		123.45,	12,	"1.234500e+02"},
{"%13e",	145.0,	13,	" 1.450000e+02"},
{"%-13e",	123.45,	13,	"1.234500e+02 "},
{"%.8e",	123.45,	14,	"1.23450000e+02"},
{"%+e",		123.45,	13,	"+1.234500e+02"},
{"%+e",		-123.45,13,	"-1.234500e+02"},
{"% e",		123.45,	13,	" 1.234500e+02"},
{"% e",		-123.45,13,	"-1.234500e+02"},
{"% +e",	123.45,	13,	"+1.234500e+02"},
{"% +e",	-123.45,13,	"-1.234500e+02"},
{"%#e",		100.0,	12,	"1.000000e+02"},
{"%E",		123.45,	12,	"1.234500E+02"},
/*  the following are from printf4.c  */
{"%13E",	145.0,	13,	" 1.450000E+02"},
{"%-13E",	123.45,	13,	"1.234500E+02 "},
{"%.8E",	123.45,	14,	"1.23450000E+02"},
{"%+E",		123.45,	13,	"+1.234500E+02"},
{"%+E",		-123.45,13,	"-1.234500E+02"},
{"% E",		123.45,	13,	" 1.234500E+02"},
{"% E",		-123.45,13,	"-1.234500E+02"},
{"% +E",	123.45,	13,	"+1.234500E+02"},
{"% +E",	-123.45,13,	"-1.234500E+02"},
{"%#E",		100.0,	12,	"1.000000E+02"},
{"%g",		1.2345e8,10,	"1.2345e+08"},
{"%g",		1.2345e-7,10,	"1.2345e-07"},
{"%g",		1.2345e2,6,	"123.45"},
{"%13g",	145.1,	13,	"        145.1"},
{"%13g",	1.45e7,	13,	"     1.45e+07"},
{"%13g",	1.45e-9,13,	"     1.45e-09"},
{"%-13g",	145.1,	13,	"145.1        "},
{"%-13g",	1.45e7,	13,	"1.45e+07     "},
{"%-13g",	1.45e-9,13,	"1.45e-09     "},
{"%.8g",	123.123456789,	9,	"123.12346"},
{"%.8g",	1.2345e9,	10,	"1.2345e+09"},
{"%.8g",	1.2345e-6,	10,	"1.2345e-06"},
{"%+g",		1.2345e8,	11,	"+1.2345e+08"},
{"%+g",		-1.2345e8,	11,	"-1.2345e+08"},
{"% g",		123.45,		7,	" 123.45"},
{"% g",		-123.45,	7,	"-123.45"},
{"% +g",	1.2345,		7,	"+1.2345"},
{"% +g",	-1.2345,	7,	"-1.2345"},
{"%#g",		1.0e8,		11,	"1.00000e+08"},
/*  the following are from printf5.c  */
{"%G",		1.2345e8,	10,	"1.2345E+08"},
{"%G",		1.2345E-7,	10,	"1.2345E-07"},
{"%G",		1.2345e2,	6,	"123.45"},
{"%13G",	145.1,		13,	"        145.1"},
{"%13G",	1.45e7,		13,	"     1.45E+07"},
{"%13G",	1.45E-9,	13,	"     1.45E-09"},
{"%-13G",	145.1,		13,	"145.1        "},
{"%-13G",	1.45e7,		13,	"1.45E+07     "},
{"%-13G",	1.45E-9,	13,	"1.45E-09     "},
{"%.8G",	123.123456789,	9,	"123.12346"},
{"%.8G",	1.2345e9,	10,	"1.2345E+09"},
{"%.8G",	1.2345E-6,	10,	"1.2345E-06"},
{"%+G",		1.2345e8,	11,	"+1.2345E+08"},
{"%+G",		-1.2345e8,	11,	"-1.2345E+08"},
{"% G",		123.45,		7,	" 123.45"},
{"% G",		-123.45,	7,	"-123.45"},
{"% +G",	1.2345,		7,	"+1.2345"},
{"% +G",	-1.2345,	7,	"-1.2345"},
{"%#G",		1.0e8,		11,	"1.00000E+08"},
/*  the following are from printf8.c  */
{"%f %%",	123.0,	12,	"123.000000 %"},
{"%e %%",	123.0,	14,	"1.230000e+02 %"},
{"%E %%",	123.0,	14,	"1.230000E+02 %"},
{"%g %%",	1.23e9,	10,	"1.23e+09 %"},
{"%G %%",	1.23e9,	10,	"1.23E+09 %"}
};



typedef	struct
	{
	char	*format;
	int	arg1;	/*  one int argument  */
	double	arg2;	/*  floating argument  */
	int	exp_val;
	char	*exp_str;
	} if_argdef;

static	if_argdef	if_args []
={

#ifndef	SYSVR_2  /*  include the %i format testing  */

/*  the following are from printf6.c  */
{"%i %f",	123,	98.765,	13,	"123 98.765000"},	
{"%i %e",	123,	98.765,	16,	"123 9.876500e+01"},	
{"%i %E",	123,	98.765,	16,	"123 9.876500E+01"},	
{"%i %g",	123,	9.8765e8,	14,	"123 9.8765e+08"},	
{"%i %G",	123,	9.8765e8,	14,	"123 9.8765E+08"},	

#endif

/*  the following are from printf3.c  */
{"%.*f",	6,	123.45,	10,	"123.450000"},
{"%*f",		11,	123.45,	11,	" 123.450000"},
{"%.*e",	8,	123.45,	14,	"1.23450000e+02"},
{"%*e",		13,	123.45,	13,	" 1.234500e+02"},
/*  the following are from printf4.c  */
{"%.*E",	8,	123.45,	14,	"1.23450000E+02"},
{"%*E",		13,	123.45,	13,	" 1.234500E+02"},
{"%.*g",	8,	123.45,	6,	"123.45"},
{"%*g",		13,	1.2345e16,	13,	"   1.2345e+16"},
/*  the following are from printf5.c  */
{"%.*G",	8,	123.45,	6,	"123.45"},
{"%*G",		13,	1.2345e16,	13,	"   1.2345E+16"},
/*  the following are from printf6.c  */
{"%d %f",	123,	98.765,	13,	"123 98.765000"},	
{"%d %e",	123,	98.765,	16,	"123 9.876500e+01"},	
{"%d %E",	123,	98.765,	16,	"123 9.876500E+01"},	
{"%d %g",	123,	9.8765e8,	14,	"123 9.8765e+08"},	
{"%d %G",	123,	9.8765e8,	14,	"123 9.8765E+08"},	
{"%o %f",	0123,	98.765,	13,	"123 98.765000"},	
{"%o %e",	0123,	98.765,	16,	"123 9.876500e+01"},	
{"%o %E",	0123,	98.765,	16,	"123 9.876500E+01"},	
/*  the following are from printf7.c  */
{"%o %g",	0123,	9.8765e8,	14,	"123 9.8765e+08"},
{"%o %G",	0123,	9.8765e8,	14,	"123 9.8765E+08"},
{"%u %f",	123,	98.765,	13,	"123 98.765000"},
{"%u %e",	123,	98.765,	16,	"123 9.876500e+01"},
{"%u %E",	123,	98.765,	16,	"123 9.876500E+01"},
{"%u %g",	123,	9.8765e8,	14,	"123 9.8765e+08"},
{"%u %G",	123,	9.8765e8,	14,	"123 9.8765E+08"},
{"%x %f",	0x123,	98.765,	13,	"123 98.765000"},
{"%x %e",	0x123,	98.765,	16,	"123 9.876500e+01"},
{"%x %E",	0x123,	98.765,	16,	"123 9.876500E+01"},
{"%x %g",	0x123,	9.8765e8,	14,	"123 9.8765e+08"},
{"%x %G",	0x123,	9.8765e8,	14,	"123 9.8765E+08"},
{"%X %f",	0x123,	98.765,	13,	"123 98.765000"},
{"%X %e",	0x123,	98.765,	16,	"123 9.876500e+01"},
{"%X %E",	0x123,	98.765,	16,	"123 9.876500E+01"},
{"%X %g",	0x123,	9.8765e8,	14,	"123 9.8765e+08"},
{"%X %G",	0x123,	9.8765e8,	14,	"123 9.8765E+08"}
};


typedef	struct
	{
	char	*format;
	double	arg1;	/*  floating argument  */
	double	arg2;	/*  floating argument  */
	int	exp_val;
	char	*exp_str;
	} ff_argdef;

static	ff_argdef	ff_args []
={
/*  the following are from printf3.c  */
{"%f %f",	123.45,	3210.1,	22,	"123.450000 3210.100000"},
{"%e %e",	123.45,	3210.1,	25,	"1.234500e+02 3.210100e+03"},
/*  the following are from printf4.c  */
{"%E %E",	123.45,	3210.1,	25,	"1.234500E+02 3.210100E+03"},
{"%g %g",	1.2345e9,	3210.1,	17,	"1.2345e+09 3210.1"},
/*  the following are from printf5.c  */
{"%G %G",	1.2345e9,	3210.1,	17,	"1.2345E+09 3210.1"},
/*  the following are from printf8.c  */
{"%f %e",	123.0,98.765,	23,	"123.000000 9.876500e+01"},
{"%f %E",	123.0,98.765,	23,	"123.000000 9.876500E+01"},
{"%f %g",	123.0,9.8765e8,	21,	"123.000000 9.8765e+08"},
{"%f %G",	123.0,9.8765e8,	21,	"123.000000 9.8765E+08"},
{"%e %E",	123.0,98.765,	25,	"1.230000e+02 9.876500E+01"},
{"%e %g",	123.0,9.8765e8,	23,	"1.230000e+02 9.8765e+08"},
{"%e %G",	123.0,9.8765e8,	23,	"1.230000e+02 9.8765E+08"},
{"%E %g",	123.0,9.8765e8,	23,	"1.230000E+02 9.8765e+08"},
{"%E %G",	123.0,9.8765e8,	23,	"1.230000E+02 9.8765E+08"},
{"%g %G",	1.23e9,9.8765e8,	19,	"1.23e+09 9.8765E+08"}
};


typedef	struct
	{
	char	*format;
	char	arg;	/*  char argument  */
	int	exp_val;
	char	*exp_str;
	} c_argdef;

static	c_argdef	c_args []
={
/*  the following are from printf5.c  */
{"%c",	'X',	1,	"X"},
{"%4c",	'Y',	4,	"   Y"},
{"%-4c",	'Y',	4,	"Y   "},
{"%lc",	'Z',	1,	"Z"},
{"%#c",	'Z',	1,	"Z"},
/*  the following is from printf8.c  */
{"%c %%",	'J',	3,	"J %"}
};

typedef	struct
	{
	char	*format;
	char	*arg;	/*  string argument  */
	int	exp_val;
	char	*exp_str;
	} s_argdef;

static	s_argdef	s_args []
={
/*  the following are from printf5.c  */
{"%s",	"SVVS",	4,	"SVVS"},
{"%4s",	"JTK",	4,	" JTK"},
{"%-4s",	"JTK",	4,	"JTK "},
{"%.6s",	"Unique!",	6,	"Unique"},
{"%ls",	"Tis time",	8,	"Tis time"},
{"%#s",	"The Walrus Said",	15,	"The Walrus Said"},
/*  the following is from printf8.c  */
{"%s %%",	"SVVS",	6,	"SVVS %"}
};

typedef	struct
	{
	char	*format;
	char	arg1;	/*  char argument  */
	char	arg2;	/*  char argument  */
	int	exp_val;
	char	*exp_str;
	} cc_argdef;

static	cc_argdef	cc_args []
={
/*  the following are from printf5.c  */
{"%c %c",	'J',	'K',	3,	"J K"}
};

typedef	struct
	{
	char	*format;
	char	*arg1;	/*  string argument  */
	char	*arg2;	/*  string argument  */
	int	exp_val;
	char	*exp_str;
	} ss_argdef;

static	ss_argdef	ss_args []
={
{"%s %s",	"JTK",	"STO",	7,	"JTK STO"},
};


typedef	struct
	{
	char	*format;
	int	arg1;	/*  int argument  */
	char	arg2;	/*  char argument  */
	int	exp_val;
	char	*exp_str;
	} ic_argdef;

static	ic_argdef	ic_args []
={

#ifndef	SYSVR_2  /*  include the %i format testing  */

/*  the following are from printf6.c  */
{"%i %c",	123,	'P',	5,	"123 P"},

#endif

/*  the following are from printf6.c  */
{"%d %c",	123,	'P',	5,	"123 P"},
/*  the following are from printf7.c  */
{"%o %c",	0123,	'P',	5,	"123 P"},
{"%u %c",	123,	'P',	5,	"123 P"},
{"%x %c",	0x123,	'P',	5,	"123 P"},
{"%X %c",	0x123,	'P',	5,	"123 P"}
};


typedef	struct
	{
	char	*format;
	int	arg1;	/*  int argument  */
	char	*arg2;	/*  string argument  */
	int	exp_val;
	char	*exp_str;
	} is_argdef;

static	is_argdef	is_args []
={

#ifndef	SYSVR_2  /*  include the %i format testing  */

/*  the following are from printf6.c  */
{"%i %s", 123, "Bang the Drum Slowly", 24, "123 Bang the Drum Slowly"},	

#endif

/*  the following are from printf6.c  */
{"%d %s", 123, "Bang the Drum Slowly", 24, "123 Bang the Drum Slowly"},	
/*  the following are from printf7.c  */
{"%o %s", 0123, "Bang the Drum Slowly", 24, "123 Bang the Drum Slowly"},
{"%u %s", 123, "Bang the Drum Slowly", 24, "123 Bang the Drum Slowly"},
{"%x %s", 0x123, "Bang the Drum Slowly", 24, "123 Bang the Drum Slowly"},
/*  the following is from printf8.c  */
{"%X %s", 0x123,"Bang the Drum Slowly", 24, "123 Bang the Drum Slowly"}
};


typedef	struct
	{
	char	*format;
	double	arg1;	/*  float argument  */
	char	arg2;	/*  char argument  */
	int	exp_val;
	char	*exp_str;
	} fc_argdef;

static	fc_argdef	fc_args []
={
/*  the following are from printf8.c  */
{"%f %c",	123.0,'P',	12,	"123.000000 P"},
{"%e %c",	123.0,'P',	14,	"1.230000e+02 P"},
{"%E %c",	123.0,'P',	14,	"1.230000E+02 P"},
{"%g %c",	1.23e9,'P',	10,	"1.23e+09 P"},
{"%G %c",	1.23e9,'P',	10,	"1.23E+09 P"}
};


typedef	struct
	{
	char	*format;
	double	arg1;	/*  float argument  */
	char	*arg2;	/*  string argument  */
	int	exp_val;
	char	*exp_str;
	} fs_argdef;

static	fs_argdef	fs_args []
={
/*  the following are from printf8.c  */
{"%f %s", 123.0,"Bang the Drum Slowly", 31, "123.000000 Bang the Drum Slowly"},
{"%e %s", 123.0,"Bang the Drum Slowly", 33, "1.230000e+02 Bang the Drum Slowly"},
{"%E %s", 123.0,"Bang the Drum Slowly", 33, "1.230000E+02 Bang the Drum Slowly"},
{"%g %s", 1.23e9,"Bang the Drum Slowly", 29, "1.23e+09 Bang the Drum Slowly"},
{"%G %s", 1.23e9,"Bang the Drum Slowly", 29, "1.23E+09 Bang the Drum Slowly"}
};


typedef	struct
	{
	char	*format;
	char	arg1;	/*  char argument  */
	char	*arg2;	/*  string argument  */
	int	exp_val;
	char	*exp_str;
	} cs_argdef;

static	cs_argdef	cs_args []
={
/*  the following is from printf8.c  */
{"%c %s", 'J',"Bang the Drum Slowly", 22, "J Bang the Drum Slowly"}
};



/*
** printf
**	Test the printf,fprintf and sprintf routines.
**
**	The function tested depends on the name of the file.
**	The executable is linked to three names:
**	printf1  -- printf test
**	printf2  -- sprintf test
**	printf3  -- fprintf test
**
*/
main(argc, argv)
int	argc;
char	*argv[];
{
	int		ret_val;
	char		*sp1;
	long		long1;
	int	i;	/*  loop index  */
	int	num;	/*  number of test cases, determined by array size  */
	FILE		*fp;  /*  used for fprintf test  */
	char	this_test;	/*  ' ', 'f' or 's'  */
	char	buffer [STRLEN];	/*  buffer for sprintf tests  */

setup("printf");

sp1 = buffer;	/*  For the sprintf test, sp1 must point to an allocated buffer,
			the other tests return a pointer	*/
switch (argv [0] [6])
{
    case '1':
	funct("printf");
	this_test = ' ';  /*  printf test  */
	break;

    case '2':
	funct("fprintf");
	this_test = 'f';  /*  fprintf test  */
	break;

    case '3':
	funct("sprintf");
	this_test = 's';  /*  sprintf test  */
	break;

    default:
	error ("'%s' is an invalid test name, printf[123] was expected",
		argv [0]);
	done ();
	/*  NOTREACHED  */
}


abstract ("%cprintf with no arguments", this_test);
start
num = sizeof (no_args) / sizeof (no_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\")\nshould return %d, printing \"%s\"",
		this_test, no_args [i].format,
		no_args [i].exp_val, no_args [i].exp_str);
	start
		/*  COMMENTED OUT - no z_funct for printf, sprintf, fprintf
		 *  expecting (VOID);
		 */
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1, no_args [i].format);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp, no_args [i].format);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (no_args [i].format);
			sp1 = end_printf ();
		    }
		if ( ret_val != no_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, no_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,no_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, no_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with an integer argument", this_test);
start
num = sizeof (i_args) / sizeof (i_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %d)\nshould return %d, printing \"%s\"",
		this_test, i_args [i].format, i_args [i].arg,
		i_args [i].exp_val, i_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				i_args [i].format, i_args [i].arg);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				i_args [i].format, i_args [i].arg);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (i_args [i].format, i_args [i].arg);
			sp1 = end_printf ();
		    }
		if ( ret_val != i_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, i_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,i_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, i_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with two integer arguments", this_test);
start
num = sizeof (ii_args) / sizeof (ii_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %d, %d)\nshould return %d, printing \"%s\"",
		this_test,
		ii_args [i].format, ii_args [i].arg1, ii_args [i].arg2,
		ii_args [i].exp_val, ii_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				ii_args [i].format, ii_args [i].arg1,
				ii_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				ii_args [i].format, ii_args [i].arg1,
				ii_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				ii_args [i].format, ii_args [i].arg1,
				ii_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != ii_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, ii_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,ii_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, ii_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with a long argument", this_test);
start
num = sizeof (l_args) / sizeof (l_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %ld)\nshould return %d, printing \"%s\"",
		this_test, l_args [i].format, l_args [i].arg,
		l_args [i].exp_val, l_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				l_args [i].format, l_args [i].arg);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				l_args [i].format, l_args [i].arg);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				l_args [i].format, l_args [i].arg);
			sp1 = end_printf ();
		    }
		if ( ret_val != l_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, l_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,l_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, l_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with a float argument", this_test);
start
num = sizeof (f_args) / sizeof (f_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %g)\nshould return %d, printing \"%s\"",
		this_test, f_args [i].format, f_args [i].arg,
		f_args [i].exp_val, f_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				f_args [i].format, f_args [i].arg);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				f_args [i].format, f_args [i].arg);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				f_args [i].format, f_args [i].arg);
			sp1 = end_printf ();
		    }
		if ( ret_val != f_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, f_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,f_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, f_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with an integer and float argument", this_test);
start
num = sizeof (if_args) / sizeof (if_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %d, %g)\nshould return %d, printing \"%s\"",
		this_test,
		if_args [i].format, if_args [i].arg1, if_args [i].arg2,
		if_args [i].exp_val, if_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				if_args [i].format, if_args [i].arg1,
				if_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				if_args [i].format, if_args [i].arg1,
				if_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				if_args [i].format, if_args [i].arg1,
				if_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != if_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, if_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,if_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, if_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with two float arguments", this_test);
start
num = sizeof (ff_args) / sizeof (ff_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %g, %g)\nshould return %d, printing \"%s\"",
		this_test, 
		ff_args [i].format, ff_args [i].arg1, ff_args [i].arg2,
		ff_args [i].exp_val, ff_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				ff_args [i].format, ff_args [i].arg1,
				ff_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				ff_args [i].format, ff_args [i].arg1,
				ff_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				ff_args [i].format, ff_args [i].arg1,
				ff_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != ff_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, ff_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,ff_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, ff_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with a char argument", this_test);
start
num = sizeof (c_args) / sizeof (c_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", '%c')\nshould return %d, printing \"%s\"",
		this_test, c_args [i].format, c_args [i].arg,
		c_args [i].exp_val, c_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				c_args [i].format, c_args [i].arg);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				c_args [i].format, c_args [i].arg);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				c_args [i].format, c_args [i].arg);
			sp1 = end_printf ();
		    }
		if ( ret_val != c_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, c_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,c_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, c_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with string argument", this_test);
start
num = sizeof (s_args) / sizeof (s_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", \"%s\")\nshould return %d, printing \"%s\"",
		this_test, s_args [i].format, s_args [i].arg,
		s_args [i].exp_val, s_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				s_args [i].format, s_args [i].arg);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				s_args [i].format, s_args [i].arg);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				s_args [i].format, s_args [i].arg);
			sp1 = end_printf ();
		    }
		if ( ret_val != s_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, s_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,s_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, s_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with two char arguments", this_test);
start
num = sizeof (cc_args) / sizeof (cc_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", '%c', '%c')\nshould return %d, \
printing \"%s\"",
		this_test, 
		cc_args [i].format, cc_args [i].arg1, cc_args [i].arg2,
		cc_args [i].exp_val, cc_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				cc_args [i].format, cc_args [i].arg1,
				cc_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				cc_args [i].format, cc_args [i].arg1,
				cc_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				cc_args [i].format, cc_args [i].arg1,
				cc_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != cc_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, cc_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,cc_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, cc_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with two string arguments", this_test);
start
num = sizeof (ss_args) / sizeof (ss_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", \"%s\", \"%s\")\nshould return %d, \
printing \"%s\"",
		this_test, 
		ss_args [i].format, ss_args [i].arg1, ss_args [i].arg2,
		ss_args [i].exp_val, ss_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				ss_args [i].format, ss_args [i].arg1,
				ss_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				ss_args [i].format, ss_args [i].arg1,
				ss_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				ss_args [i].format, ss_args [i].arg1,
				ss_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != ss_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, ss_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,ss_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, ss_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with integer and character arguments", this_test);
start
num = sizeof (ic_args) / sizeof (ic_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %d, '%c')\nshould return %d, printing \"%s\"",
		this_test, 
		ic_args [i].format, ic_args [i].arg1, ic_args [i].arg2,
		ic_args [i].exp_val, ic_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				ic_args [i].format, ic_args [i].arg1,
				ic_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				ic_args [i].format, ic_args [i].arg1,
				ic_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				ic_args [i].format, ic_args [i].arg1,
				ic_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != ic_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, ic_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,ic_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, ic_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with integer and string arguments", this_test);
start
num = sizeof (is_args) / sizeof (is_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %d, \"%s\")\nshould return %d, \
printing \"%s\"",
		this_test, 
		is_args [i].format, is_args [i].arg1, is_args [i].arg2,
		is_args [i].exp_val, is_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				is_args [i].format, is_args [i].arg1,
				is_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				is_args [i].format, is_args [i].arg1,
				is_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				is_args [i].format, is_args [i].arg1,
				is_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != is_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, is_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,is_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, is_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with floating and char arguments", this_test);
start
num = sizeof (fc_args) / sizeof (fc_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %g, '%c')\nshould return %d, printing \"%s\"",
		this_test,
		fc_args [i].format, fc_args [i].arg1, fc_args [i].arg2,
		fc_args [i].exp_val, fc_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				fc_args [i].format, fc_args [i].arg1,
				fc_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				fc_args [i].format, fc_args [i].arg1,
				fc_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				fc_args [i].format, fc_args [i].arg1,
				fc_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != fc_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, fc_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,fc_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, fc_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with float and string arguments", this_test);
start
num = sizeof (fs_args) / sizeof (fs_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", %g, \"%s\")\nshould return %d, \
printing \"%s\"",
		this_test,
		fs_args [i].format, fs_args [i].arg1, fs_args [i].arg2,
		fs_args [i].exp_val, fs_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				fs_args [i].format, fs_args [i].arg1,
				fs_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				fs_args [i].format, fs_args [i].arg1,
				fs_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				fs_args [i].format, fs_args [i].arg1,
				fs_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != fs_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, fs_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,fs_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, fs_args [i].exp_str);
	finis
	}
finis



abstract ("%cprintf with char and string arguments", this_test);
start
num = sizeof (cs_args) / sizeof (cs_argdef);
for (i=0 ; i<num ; i++)
	{
	testing ("%cprintf (\"%s\", '%c', \"%s\")\nshould return %d, \
printing \"%s\"",
		this_test,
		cs_args [i].format, cs_args [i].arg1, cs_args [i].arg2,
		cs_args [i].exp_val, cs_args [i].exp_str);
	start
		switch (this_test)
		    {
		    case 's':
			ret_val = sprintf (sp1,
				cs_args [i].format, cs_args [i].arg1,
				cs_args [i].arg2);
			break;
		    case 'f':
			fp = init_fprintf ();
			ret_val = fprintf (fp,
				cs_args [i].format, cs_args [i].arg1,
				cs_args [i].arg2);
			sp1 = end_fprintf (fp);
			break;
		    default:
			init_printf();
			ret_val = printf (
				cs_args [i].format, cs_args [i].arg1,
				cs_args [i].arg2);
			sp1 = end_printf ();
		    }
		if ( ret_val != cs_args [i].exp_val )
			error ("%cprintf returned %d, expecting %d",
				this_test, ret_val, cs_args [i].exp_val);
		expecting (VOID);
		if ( strcmp(sp1,cs_args [i].exp_str) )
			error ("%cprintf printed \"%s\", expecting \"%s\"",
				this_test, sp1, cs_args [i].exp_str);
	finis
	}
finis


done();
/*NOTREACHED*/
}

/*
** init_printf
**	Initialize  stdout for printf.
*/
init_printf()
{
	push_stdout("tmpprintf","w");
}

/*
** end_printf
**	Close stdout, and return the string written into the file.
*/
char	*
end_printf()
{
	static	char	buffer[STRLEN];
	int		i;
	FILE		*fp;

	expecting (VOID);
	pop_stdout();
	if ( (fp = temp_fopen("tmpprintf","r")) == NULL )
		error ("can not open temp file 'printf'");
	if ( (i = fread(buffer,1,STRLEN,fp)) == 0 )
		error ("fread returned 0");

	expecting (SUCCESS);
	if ( fclose(fp) )
		error ("fclose returned non-zero");
	buffer[i] = '\0';
	return ( buffer );
}

/*
** init_fprintf
**	Initialize stdout for fprintf.
*/
FILE	*
init_fprintf()
{
	FILE	*fp;

	if ( (fp = temp_fopen("tmpfprintf","w")) == NULL )
		error ("can not open temp file 'fprintf'");
	return ( fp );
}

/*
** end_fprintf
**	Close stdout, and return the string written into the file.
*/
char	*
end_fprintf(ofp)
FILE	*ofp;
{
	static	char	buffer[STRLEN];
	int		i;
	FILE		*fp;

	expecting (SUCCESS);
	fclose(ofp);
	fp = temp_fopen("tmpfprintf","r");
	i = fread(buffer,1,STRLEN,fp);
	fclose(fp);
	buffer[i] = '\0';
	return ( buffer );
}
