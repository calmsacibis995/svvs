# include <sv_base.h>

# include "main.h"
# include "eval_arg.h"
# include "comb_macro.h"

SCCSID("@(#)comb_wp.c	3.1 (SVID) 12/24/86");

int x_wprintw(win_arg, fmt_arg, var_arg_list)
VAR_ENTRY	*win_arg;
VAR_ENTRY	*fmt_arg;
VAR_ARG_ENTRY	*var_arg_list;
{
	VAR_ENTRY	*var_arg_tbl[3];
	int		var_arg_id;
	int		r;

	var_arg_id = set_var_arg(var_arg_list, var_arg_tbl);

	switch (var_arg_id & 07000)
	{
	  case 00000:
		switch (var_arg_id)
		{
		  case 00000: r = wprintw(X_WIN, X_FMT) ; break;
		  case 00001: r = wprintw(X_WIN, X_FMT, X_CHAR(0)) ; break;
		  case 00002: r = wprintw(X_WIN, X_FMT, X_TEXT(0)) ; break;
		  case 00003: r = wprintw(X_WIN, X_FMT, X_NUM(0)) ; break;
		  case 00004: r = wprintw(X_WIN, X_FMT, X_FLOAT(0)) ; break;
		  case 00011: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1)) ; break;
		  case 00012: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1)) ; break;
		  case 00013: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1)) ; break;
		  case 00014: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1)) ; break;
		  case 00021: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1)) ; break;
		  case 00022: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1)) ; break;
		  case 00023: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1)) ; break;
		  case 00024: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1)) ; break;
		  case 00031: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1)) ; break;
		  case 00032: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1)) ; break;
		  case 00033: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1)) ; break;
		  case 00034: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1)) ; break;
		  case 00041: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1)) ; break;
		  case 00042: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1)) ; break;
		  case 00043: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1)) ; break;
		  case 00044: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1)) ; break;
		  case 00111: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00112: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00113: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00114: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00121: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00122: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00123: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00124: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00131: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00132: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00133: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00134: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00141: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00142: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00143: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00144: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  case 00211: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00212: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00213: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00214: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00221: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00222: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00223: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00224: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00231: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00232: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00233: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00234: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00241: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00242: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00243: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00244: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  case 00311: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00312: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00313: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00314: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00321: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00322: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00323: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00324: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00331: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00332: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00333: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00334: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00341: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00342: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00343: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00344: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  case 00411: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00412: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00413: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00414: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00421: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00422: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00423: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00424: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00431: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00432: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00433: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00434: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00441: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00442: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00443: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00444: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  default: screech("x_wprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 01000:
		switch (var_arg_id)
		{
		  case 01111: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01112: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01113: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01114: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01121: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01122: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01123: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01124: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01131: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01132: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01133: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01134: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01141: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01142: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01143: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01144: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01211: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01212: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01213: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01214: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01221: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01222: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01223: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01224: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01231: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01232: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01233: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01234: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01241: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01242: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01243: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01244: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01311: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01312: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01313: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01314: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01321: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01322: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01323: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01324: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01331: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01332: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01333: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01334: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01341: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01342: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01343: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01344: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01411: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01412: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01413: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01414: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01421: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01422: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01423: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01424: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01431: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01432: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01433: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01434: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01441: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 01442: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 01443: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 01444: r = wprintw(X_WIN, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_wprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 02000:
		switch (var_arg_id)
		{
		  case 02111: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02112: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02113: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02114: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02121: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02122: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02123: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02124: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02131: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02132: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02133: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02134: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02141: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02142: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02143: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02144: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02211: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02212: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02213: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02214: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02221: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02222: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02223: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02224: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02231: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02232: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02233: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02234: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02241: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02242: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02243: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02244: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02311: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02312: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02313: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02314: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02321: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02322: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02323: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02324: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02331: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02332: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02333: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02334: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02341: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02342: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02343: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02344: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02411: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02412: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02413: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02414: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02421: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02422: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02423: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02424: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02431: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02432: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02433: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02434: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02441: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 02442: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 02443: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 02444: r = wprintw(X_WIN, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_wprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 03000:
		switch (var_arg_id)
		{
		  case 03111: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03112: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03113: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03114: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03121: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03122: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03123: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03124: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03131: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03132: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03133: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03134: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03141: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03142: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03143: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03144: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03211: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03212: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03213: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03214: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03221: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03222: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03223: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03224: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03231: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03232: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03233: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03234: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03241: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03242: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03243: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03244: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03311: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03312: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03313: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03314: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03321: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03322: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03323: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03324: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03331: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03332: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03333: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03334: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03341: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03342: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03343: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03344: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03411: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03412: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03413: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03414: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03421: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03422: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03423: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03424: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03431: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03432: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03433: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03434: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03441: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 03442: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 03443: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 03444: r = wprintw(X_WIN, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_wprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 04000:
		switch (var_arg_id)
		{
		  case 04111: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04112: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04113: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04114: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04121: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04122: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04123: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04124: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04131: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04132: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04133: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04134: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04141: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04142: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04143: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04144: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04211: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04212: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04213: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04214: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04221: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04222: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04223: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04224: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04231: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04232: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04233: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04234: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04241: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04242: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04243: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04244: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04311: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04312: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04313: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04314: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04321: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04322: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04323: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04324: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04331: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04332: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04333: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04334: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04341: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04342: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04343: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04344: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04411: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04412: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04413: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04414: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04421: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04422: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04423: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04424: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04431: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04432: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04433: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04434: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04441: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 04442: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 04443: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 04444: r = wprintw(X_WIN, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_wprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}

	  default:
		screech("x_wprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
	}

	return(r);
}

