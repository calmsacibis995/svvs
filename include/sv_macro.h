/*
 *	sv_macro.h - macro definitions for macro replacement
 */

#ifdef	UNDEF
ident	"@(#)sv_macro.h	1.7"
#endif

#include <sv_errno.h>

#undef	ferror	
#undef	feof	
#undef	clearerr	
#undef	fileno	
#undef	_toupper	
#undef	_tolower	
#undef	toascii	
#undef	isalpha	
#undef	isupper	
#undef	islower	
#undef	isdigit	
#undef	isxdigit	
#undef	isalnum	
#undef	isspace	
#undef	ispunct	
#undef	isprint	
#undef	isgraph	
#undef	iscntrl	
#undef	isascii	
#undef	getc	
#undef	getchar	
#undef	putc	
#undef	putchar	
#undef	rewinddir

#define	ferror			Zferror
#define	feof			Zfeof
#define	clearerr		Zclearerr
#define	fileno			Zfileno
#define	_toupper		Z_toupper
#define	_tolower		Z_tolower
#define	toascii			Ztoascii
#define	isalpha			Zisalpha
#define	isupper			Zisupper
#define	islower			Zislower
#define	isdigit			Zisdigit
#define	isxdigit		Zisxdigit
#define	isalnum			Zisalnum
#define	isspace			Zisspace
#define	ispunct			Zispunct
#define	isprint			Zisprint
#define	isgraph			Zisgraph
#define	iscntrl			Ziscntrl
#define	isascii			Zisascii
#define	getc			Zgetc
#define	getchar			Zgetchar
#define	putc			Zputc
#define	putchar			Zputchar
#define	rewinddir		Zrewinddir
