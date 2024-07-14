# include <sv_base.h>

# include "main.h"
# include "eval_arg.h"
# include "comb_macro.h"

SCCSID("@(#)comb_mvwp.c	3.1 (SVID) 12/24/86");

int x_mvwprintw(win_arg, y_arg, x_arg, fmt_arg, var_arg_list)
VAR_ENTRY	*win_arg;
VAR_ENTRY	*y_arg;
VAR_ENTRY	*x_arg;
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
		  case 00000: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT) ; break;
		  case 00001: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0)) ; break;
		  case 00002: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0)) ; break;
		  case 00003: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0)) ; break;
		  case 00004: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0)) ; break;
		  case 00011: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1)) ; break;
		  case 00012: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1)) ; break;
		  case 00013: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1)) ; break;
		  case 00014: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1)) ; break;
		  case 00021: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1)) ; break;
		  case 00022: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1)) ; break;
		  case 00023: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1)) ; break;
		  case 00024: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1)) ; break;
		  case 00031: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1)) ; break;
		  case 00032: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1)) ; break;
		  case 00033: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1)) ; break;
		  case 00034: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1)) ; break;
		  case 00041: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1)) ; break;
		  case 00042: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1)) ; break;
		  case 00043: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1)) ; break;
		  case 00044: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1)) ; break;
		  case 00111: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00112: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00113: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00114: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00121: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00122: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00123: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00124: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00131: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00132: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00133: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00134: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00141: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00142: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00143: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00144: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  case 00211: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00212: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00213: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00214: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00221: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00222: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00223: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00224: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00231: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00232: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00233: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00234: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00241: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00242: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00243: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00244: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  case 00311: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00312: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00313: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00314: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00321: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00322: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00323: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00324: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00331: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00332: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00333: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00334: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00341: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00342: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00343: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00344: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  case 00411: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2)) ; break;
		  case 00412: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2)) ; break;
		  case 00413: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2)) ; break;
		  case 00414: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2)) ; break;
		  case 00421: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2)) ; break;
		  case 00422: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00423: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2)) ; break;
		  case 00424: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2)) ; break;
		  case 00431: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2)) ; break;
		  case 00432: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2)) ; break;
		  case 00433: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2)) ; break;
		  case 00434: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2)) ; break;
		  case 00441: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2)) ; break;
		  case 00442: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2)) ; break;
		  case 00443: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2)) ; break;
		  case 00444: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2)) ; break;
		  default: screech("x_mvwprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 01000:
		switch (var_arg_id)
		{
		  case 01111: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01112: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01113: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01114: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 01121: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01122: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01123: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01124: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 01131: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01132: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01133: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01134: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 01141: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01142: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01143: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01144: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 01211: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01212: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01213: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01214: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 01221: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01222: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01223: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01224: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01231: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01232: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01233: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01234: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 01241: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01242: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01243: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01244: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 01311: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01312: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01313: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01314: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 01321: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01322: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01323: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01324: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 01331: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01332: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01333: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01334: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 01341: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01342: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01343: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01344: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 01411: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01412: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01413: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01414: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 01421: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01422: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01423: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01424: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 01431: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01432: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01433: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01434: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 01441: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 01442: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 01443: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 01444: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_CHAR(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_mvwprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 02000:
		switch (var_arg_id)
		{
		  case 02111: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02112: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02113: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02114: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 02121: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02122: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02123: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02124: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 02131: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02132: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02133: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02134: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 02141: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02142: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02143: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02144: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 02211: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02212: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02213: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02214: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 02221: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02222: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02223: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02224: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02231: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02232: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02233: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02234: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 02241: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02242: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02243: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02244: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 02311: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02312: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02313: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02314: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 02321: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02322: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02323: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02324: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 02331: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02332: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02333: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02334: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 02341: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02342: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02343: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02344: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 02411: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02412: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02413: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02414: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 02421: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02422: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02423: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02424: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 02431: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02432: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02433: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02434: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 02441: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 02442: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 02443: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 02444: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_TEXT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_mvwprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 03000:
		switch (var_arg_id)
		{
		  case 03111: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03112: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03113: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03114: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 03121: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03122: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03123: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03124: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 03131: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03132: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03133: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03134: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 03141: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03142: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03143: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03144: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 03211: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03212: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03213: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03214: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 03221: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03222: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03223: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03224: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03231: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03232: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03233: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03234: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 03241: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03242: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03243: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03244: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 03311: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03312: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03313: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03314: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 03321: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03322: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03323: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03324: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 03331: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03332: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03333: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03334: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 03341: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03342: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03343: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03344: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 03411: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03412: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03413: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03414: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 03421: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03422: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03423: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03424: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 03431: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03432: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03433: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03434: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 03441: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 03442: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 03443: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 03444: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_NUM(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_mvwprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 04000:
		switch (var_arg_id)
		{
		  case 04111: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04112: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04113: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04114: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_CHAR(2), X_CHAR(3)) ; break;
		  case 04121: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04122: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04123: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04124: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_TEXT(2), X_CHAR(3)) ; break;
		  case 04131: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04132: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04133: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04134: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_NUM(2), X_CHAR(3)) ; break;
		  case 04141: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04142: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04143: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04144: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_CHAR(1), X_FLOAT(2), X_CHAR(3)) ; break;
		  case 04211: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04212: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04213: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04214: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_CHAR(2), X_TEXT(3)) ; break;
		  case 04221: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04222: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04223: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04224: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04231: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04232: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04233: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04234: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_NUM(2), X_TEXT(3)) ; break;
		  case 04241: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04242: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04243: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04244: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_TEXT(1), X_FLOAT(2), X_TEXT(3)) ; break;
		  case 04311: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04312: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04313: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04314: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_CHAR(2), X_NUM(3)) ; break;
		  case 04321: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04322: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04323: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04324: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_TEXT(2), X_NUM(3)) ; break;
		  case 04331: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04332: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04333: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04334: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_NUM(2), X_NUM(3)) ; break;
		  case 04341: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04342: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04343: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04344: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_NUM(1), X_FLOAT(2), X_NUM(3)) ; break;
		  case 04411: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04412: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04413: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04414: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_CHAR(2), X_FLOAT(3)) ; break;
		  case 04421: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04422: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04423: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04424: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_TEXT(2), X_FLOAT(3)) ; break;
		  case 04431: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04432: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04433: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04434: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_NUM(2), X_FLOAT(3)) ; break;
		  case 04441: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 04442: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 04443: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  case 04444: r = mvwprintw(X_WIN, X_Y, X_X, X_FMT, X_FLOAT(0), X_FLOAT(1), X_FLOAT(2), X_FLOAT(3)) ; break;
		  default: screech("x_mvwprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}

	  default:
		screech("x_mvwprintw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
	}

	return(r);
}

