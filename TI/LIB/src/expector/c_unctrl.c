# include "main.h"
# include <curses.h>

SCCSID("@(#)c_unctrl.c	3.1 (SVID) 12/1/86");

char *c_unctrl(chvar)
VAR_ENTRY *chvar;
{
/*
** This routine returns a printable representation of the control character
** in chvar.
*/

	static	char	*unctrltbl[]	= {
		"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G", "^H", "^I", "^J", "^K",
		"^L", "^M", "^N", "^O", "^P", "^Q", "^R", "^S", "^T", "^U", "^V", "^W",
		"^X", "^Y", "^Z", "^[", "^\\", "^]", "^~", "^_",
		" ", "!", "\"", "#", "$",  "%", "&", "'", "(", ")", "*", "+", ",", "-",
		".", "/", "0",  "1", "2",  "3", "4", "5", "6", "7", "8", "9", ":", ";",
		"<", "=", ">",  "?", "@",  "A", "B", "C", "D", "E", "F", "G", "H", "I",
		"J", "K", "L",  "M", "N",  "O", "P", "Q", "R", "S", "T", "U", "V", "W",
		"X", "Y", "Z",  "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e",
		"f", "g", "h",  "i", "j",  "k", "l", "m", "n", "o", "p", "q", "r", "s",
		"t", "u", "v",  "w", "x",  "y", "z", "{", "|", "}", "~", "^?"
	};
	char ch;

	ch = (chvar->var_data.var_chtype & A_CHARTEXT);
	
	return(unctrltbl[(unsigned) ch]);
	
}

