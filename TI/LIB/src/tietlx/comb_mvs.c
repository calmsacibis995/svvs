# include <sv_base.h>

# include "main.h"
# include "eval_arg.h"
# include "comb_macro.h"

SCCSID("@(#)comb_mvs.c	3.1 (SVID) 12/24/86");

int x_mvscanw(y_arg, x_arg, fmt_arg, var_arg_list)
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
		  case 00000: r = mvscanw(X_Y, X_X, X_FMT) ; break;
		  case 00001: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0)) ; break;
		  case 00002: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0)) ; break;
		  case 00003: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0)) ; break;
		  case 00004: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0)) ; break;
		  case 00011: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1)) ; break;
		  case 00012: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1)) ; break;
		  case 00013: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1)) ; break;
		  case 00014: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1)) ; break;
		  case 00021: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1)) ; break;
		  case 00022: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1)) ; break;
		  case 00023: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1)) ; break;
		  case 00024: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1)) ; break;
		  case 00031: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1)) ; break;
		  case 00032: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1)) ; break;
		  case 00033: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1)) ; break;
		  case 00034: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1)) ; break;
		  case 00041: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1)) ; break;
		  case 00042: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1)) ; break;
		  case 00043: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1)) ; break;
		  case 00044: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1)) ; break;
		  case 00111: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00112: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00113: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00114: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00121: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00122: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00123: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00124: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00131: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00132: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00133: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00134: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00141: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00142: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00143: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00144: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  case 00211: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00212: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00213: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00214: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00221: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00222: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00223: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00224: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00231: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00232: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00233: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00234: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00241: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00242: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00243: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00244: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  case 00311: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00312: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00313: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00314: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00321: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00322: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00323: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00324: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00331: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00332: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00333: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00334: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00341: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00342: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00343: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00344: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  case 00411: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00412: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00413: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00414: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00421: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00422: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00423: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00424: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00431: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00432: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00433: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00434: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00441: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00442: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00443: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00444: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  default: screech("x_mvscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 01000:
		switch (var_arg_id)
		{
		  case 01111: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01112: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01113: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01114: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01121: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01122: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01123: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01124: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01131: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01132: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01133: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01134: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01141: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01142: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01143: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01144: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01211: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01212: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01213: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01214: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01221: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01222: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01223: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01224: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01231: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01232: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01233: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01234: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01241: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01242: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01243: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01244: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01311: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01312: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01313: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01314: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01321: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01322: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01323: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01324: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01331: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01332: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01333: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01334: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01341: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01342: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01343: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01344: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01411: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01412: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01413: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01414: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01421: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01422: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01423: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01424: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01431: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01432: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01433: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01434: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01441: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 01442: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 01443: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 01444: r = mvscanw(X_Y, X_X, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_mvscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 02000:
		switch (var_arg_id)
		{
		  case 02111: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02112: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02113: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02114: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02121: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02122: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02123: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02124: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02131: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02132: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02133: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02134: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02141: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02142: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02143: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02144: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02211: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02212: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02213: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02214: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02221: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02222: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02223: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02224: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02231: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02232: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02233: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02234: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02241: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02242: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02243: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02244: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02311: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02312: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02313: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02314: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02321: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02322: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02323: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02324: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02331: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02332: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02333: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02334: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02341: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02342: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02343: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02344: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02411: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02412: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02413: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02414: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02421: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02422: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02423: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02424: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02431: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02432: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02433: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02434: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02441: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 02442: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 02443: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 02444: r = mvscanw(X_Y, X_X, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_mvscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 03000:
		switch (var_arg_id)
		{
		  case 03111: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03112: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03113: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03114: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03121: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03122: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03123: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03124: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03131: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03132: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03133: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03134: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03141: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03142: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03143: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03144: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03211: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03212: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03213: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03214: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03221: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03222: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03223: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03224: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03231: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03232: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03233: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03234: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03241: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03242: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03243: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03244: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03311: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03312: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03313: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03314: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03321: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03322: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03323: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03324: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03331: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03332: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03333: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03334: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03341: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03342: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03343: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03344: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03411: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03412: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03413: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03414: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03421: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03422: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03423: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03424: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03431: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03432: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03433: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03434: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03441: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 03442: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 03443: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 03444: r = mvscanw(X_Y, X_X, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_mvscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 04000:
		switch (var_arg_id)
		{
		  case 04111: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04112: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04113: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04114: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04121: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04122: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04123: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04124: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04131: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04132: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04133: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04134: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04141: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04142: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04143: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04144: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04211: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04212: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04213: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04214: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04221: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04222: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04223: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04224: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04231: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04232: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04233: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04234: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04241: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04242: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04243: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04244: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04311: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04312: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04313: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04314: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04321: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04322: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04323: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04324: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04331: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04332: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04333: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04334: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04341: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04342: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04343: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04344: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04411: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04412: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04413: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04414: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04421: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04422: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04423: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04424: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04431: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04432: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04433: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04434: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04441: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 04442: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 04443: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 04444: r = mvscanw(X_Y, X_X, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_mvscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}

	  default:
		screech("x_mvscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
	}

	return(r);
}

