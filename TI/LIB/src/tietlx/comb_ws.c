# include <sv_base.h>

# include "main.h"
# include "eval_arg.h"
# include "comb_macro.h"

SCCSID("@(#)comb_ws.c	3.1 (SVID) 12/24/86");

int x_wscanw(win_arg, fmt_arg, var_arg_list)
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
		  case 00000: r = wscanw(X_WIN, X_FMT) ; break;
		  case 00001: r = wscanw(X_WIN, X_FMT, X_C_PTR(0)) ; break;
		  case 00002: r = wscanw(X_WIN, X_FMT, X_TEXT(0)) ; break;
		  case 00003: r = wscanw(X_WIN, X_FMT, X_N_PTR(0)) ; break;
		  case 00004: r = wscanw(X_WIN, X_FMT, X_F_PTR(0)) ; break;
		  case 00011: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1)) ; break;
		  case 00012: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1)) ; break;
		  case 00013: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1)) ; break;
		  case 00014: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1)) ; break;
		  case 00021: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1)) ; break;
		  case 00022: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1)) ; break;
		  case 00023: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1)) ; break;
		  case 00024: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1)) ; break;
		  case 00031: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1)) ; break;
		  case 00032: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1)) ; break;
		  case 00033: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1)) ; break;
		  case 00034: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1)) ; break;
		  case 00041: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1)) ; break;
		  case 00042: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1)) ; break;
		  case 00043: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1)) ; break;
		  case 00044: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1)) ; break;
		  case 00111: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00112: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00113: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00114: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00121: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00122: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00123: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00124: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00131: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00132: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00133: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00134: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00141: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00142: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00143: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00144: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  case 00211: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00212: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00213: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00214: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00221: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00222: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00223: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00224: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00231: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00232: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00233: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00234: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00241: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00242: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00243: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00244: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  case 00311: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00312: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00313: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00314: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00321: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00322: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00323: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00324: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00331: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00332: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00333: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00334: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00341: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00342: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00343: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00344: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  case 00411: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2)) ; break;
		  case 00412: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2)) ; break;
		  case 00413: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2)) ; break;
		  case 00414: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2)) ; break;
		  case 00421: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2)) ; break;
		  case 00422: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2)) ; break;
		  case 00423: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2)) ; break;
		  case 00424: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2)) ; break;
		  case 00431: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2)) ; break;
		  case 00432: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2)) ; break;
		  case 00433: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2)) ; break;
		  case 00434: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2)) ; break;
		  case 00441: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2)) ; break;
		  case 00442: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2)) ; break;
		  case 00443: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2)) ; break;
		  case 00444: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2)) ; break;
		  default: screech("x_wscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 01000:
		switch (var_arg_id)
		{
		  case 01111: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01112: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01113: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01114: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 01121: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01122: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01123: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01124: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 01131: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01132: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01133: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01134: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 01141: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01142: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01143: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01144: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 01211: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01212: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01213: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01214: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 01221: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01222: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01223: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01224: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 01231: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01232: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01233: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01234: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 01241: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01242: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01243: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01244: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 01311: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01312: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01313: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01314: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 01321: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01322: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01323: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01324: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 01331: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01332: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01333: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01334: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 01341: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01342: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01343: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01344: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 01411: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01412: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01413: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01414: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 01421: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01422: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01423: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01424: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 01431: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01432: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01433: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01434: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 01441: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 01442: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 01443: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 01444: r = wscanw(X_WIN, X_FMT, X_C_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_wscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 02000:
		switch (var_arg_id)
		{
		  case 02111: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02112: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02113: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02114: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 02121: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02122: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02123: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02124: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 02131: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02132: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02133: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02134: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 02141: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02142: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02143: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02144: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 02211: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02212: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02213: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02214: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 02221: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02222: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02223: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02224: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 02231: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02232: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02233: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02234: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 02241: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02242: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02243: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02244: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 02311: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02312: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02313: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02314: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 02321: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02322: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02323: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02324: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 02331: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02332: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02333: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02334: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 02341: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02342: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02343: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02344: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 02411: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02412: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02413: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02414: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 02421: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02422: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02423: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02424: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 02431: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02432: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02433: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02434: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 02441: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 02442: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 02443: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 02444: r = wscanw(X_WIN, X_FMT, X_TEXT(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_wscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 03000:
		switch (var_arg_id)
		{
		  case 03111: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03112: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03113: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03114: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 03121: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03122: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03123: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03124: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 03131: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03132: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03133: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03134: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 03141: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03142: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03143: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03144: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 03211: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03212: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03213: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03214: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 03221: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03222: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03223: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03224: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 03231: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03232: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03233: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03234: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 03241: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03242: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03243: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03244: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 03311: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03312: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03313: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03314: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 03321: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03322: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03323: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03324: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 03331: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03332: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03333: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03334: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 03341: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03342: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03343: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03344: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 03411: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03412: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03413: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03414: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 03421: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03422: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03423: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03424: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 03431: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03432: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03433: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03434: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 03441: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 03442: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 03443: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 03444: r = wscanw(X_WIN, X_FMT, X_N_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_wscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}
		break;

	  case 04000:
		switch (var_arg_id)
		{
		  case 04111: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04112: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04113: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04114: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_C_PTR(2), X_C_PTR(3)) ; break;
		  case 04121: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04122: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04123: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04124: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_TEXT(2), X_C_PTR(3)) ; break;
		  case 04131: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04132: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04133: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04134: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_N_PTR(2), X_C_PTR(3)) ; break;
		  case 04141: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04142: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04143: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04144: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_C_PTR(1), X_F_PTR(2), X_C_PTR(3)) ; break;
		  case 04211: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04212: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04213: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04214: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_C_PTR(2), X_TEXT(3)) ; break;
		  case 04221: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04222: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04223: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04224: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_TEXT(2), X_TEXT(3)) ; break;
		  case 04231: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04232: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04233: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04234: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_N_PTR(2), X_TEXT(3)) ; break;
		  case 04241: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04242: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04243: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04244: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_TEXT(1), X_F_PTR(2), X_TEXT(3)) ; break;
		  case 04311: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04312: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04313: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04314: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_C_PTR(2), X_N_PTR(3)) ; break;
		  case 04321: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04322: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04323: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04324: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_TEXT(2), X_N_PTR(3)) ; break;
		  case 04331: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04332: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04333: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04334: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_N_PTR(2), X_N_PTR(3)) ; break;
		  case 04341: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04342: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04343: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04344: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_N_PTR(1), X_F_PTR(2), X_N_PTR(3)) ; break;
		  case 04411: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04412: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04413: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04414: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_C_PTR(2), X_F_PTR(3)) ; break;
		  case 04421: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04422: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04423: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04424: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_TEXT(2), X_F_PTR(3)) ; break;
		  case 04431: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04432: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04433: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04434: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_N_PTR(2), X_F_PTR(3)) ; break;
		  case 04441: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 04442: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 04443: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  case 04444: r = wscanw(X_WIN, X_FMT, X_F_PTR(0), X_F_PTR(1), X_F_PTR(2), X_F_PTR(3)) ; break;
		  default: screech("x_wscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
		}

	  default:
		screech("x_wscanw", 0, "Bad var_arg_id = %d\n", var_arg_id); 
	}

	return(r);
}

