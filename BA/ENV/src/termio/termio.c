#include <svvs.h>
#include <sv_env.h>
#include <termio.h>
#include <sv_macro.h>
/*  sv_base.h is not needed since no functions are under scrutiny  */


# ident	"@(#)termio.c	1.4"

# ifdef CCPASS1
# undef CCPASS1
# endif

#define	CCPASS1	1		/*Compiler flag indicating that the 
				control character macros are defined*/

extern	int	errno;

main()
{

struct	termio	tstruct;

setup ("termio");
funct ("termio");

testing ("ioctl requests");
testing ("The definitions of the ioctl command macros from termio.h");
testing ("Definitions for the macros: TCGETA, TCSETA, TCSETAW, TCSETAF,");
testing ("TCSBRK, TCXONC and TCFLSH.");
start

# ifndef TCGETA
	error ("Macro: TCGETA not defined");
# endif
# ifndef TCSETA
	error ("Macro: TCSETA not defined.");
# endif
# ifndef TCSETAW
	error ("Macro: TCSETAW not defined.");
# endif
# ifndef TCSETAF
	error ("Macro: TCSETAF not defined.");
# endif
# ifndef TCSBRK
	error ("Macro: TCSBRK not defined.");
# endif
# ifndef TCXONC
	error ("Macro: TCXONC not defined.");
# endif
# ifndef TCFLSH
	error ("Macro: TCFLSH not defined.");
# endif

finis

testing ("input modes");
testing ("The definitions of the input mode macros from termio.h");
testing ("Definitions for the macros: IGNBRK, BRKINT, IGNPAR,");
testing ("PARMRK, INPCK, ISTRIP, INLCR, IGNCR, ICRNL, IUCLC, IXON,");
testing ("IXANY and IXOFF.");
start

# ifndef IGNBRK
	error ("Macro: IGNBRK not defined.");
# endif
# ifndef BRKINT
	error ("Macro: BRKINT not defined.");
# endif
# ifndef IGNPAR
	error ("Macro: IGNPAR not defined.");
# endif
# ifndef PARMRK
	error ("Macro: PARMRK not defined.");
# endif
# ifndef INPCK
	error ("Macro: INPCK not defined.");
# endif
# ifndef ISTRIP
	error ("Macro: ISTRIP not defined.");
# endif
# ifndef INLCR
	error ("Macro: INLCR not defined.");
# endif
# ifndef IGNCR
	error ("Macro: IGNCR not defined.");
# endif
# ifndef ICRNL
	error ("Macro: ICRNL not defined.");
# endif
# ifndef IUCLC
	error ("Macro: IUCLC not defined.");
# endif
# ifndef IXON
	error ("Macro: IXON not defined.");
# endif
# ifndef IXANY
	error ("Macro: IXANY not defined.");
# endif
# ifndef IXOFF
	error ("Macro: IXOFF not defined.");
# endif

finis

testing ("output modes");
testing ("The definitions of the output mode macros from termio.h");
testing ("Definitions for the macros: OPOST, OLCUC, ONLCR, OCRNL,");
testing ("ONOCR, ONLRET, OFILL and OFDEL.");
start


# ifndef OPOST
	error ("Macro: OPOST not defined.");
# endif
# ifndef OLCUC
	error ("Macro: OLCUC not defined.");
# endif
# ifndef ONLCR
	error ("Macro: ONLCR not defined.");
# endif
# ifndef OCRNL
	error ("Macro: OCRNL not defined.");
# endif
# ifndef ONOCR
	error ("Macro: ONOCR not defined.");
# endif
# ifndef ONLRET
	error ("Macro: ONLRET not defined.");
# endif
# ifndef OFILL
	error ("Macro: OFILL not defined.");
# endif
# ifndef OFDEL
	error ("Macro: OFDEL not defined.");
# endif

finis

testing ("delay types");
testing ("The definitions of the delay type macros from termio.h");
testing ("Definitions for the macros: NLDLY, NL0, NL1, CRDLY, CR0,");
testing ("CR1, CR2, CR3, TABDLY, TAB0, TAB1, TAB2, TAB3, BSDLY,");
testing ("BS0, BS1, VTDLY, VT0, VT1, FFDLY, FF0 and FF1.");
start

# ifndef NLDLY
	error ("Macro: NLDLY not defined.");
# endif
# ifndef NL0
	error ("Macro: NL0 not defined.");
# endif
#ifndef NL1
	error ("Macro: NL1 not defined.");
# endif
# ifndef CRDLY
	error ("Macro: CRDLY not defined.");
#endif
# ifndef CR0
	error ("Macro: CR0 not defined.");
# endif
# ifndef CR1
	error ("Macro: CR1 not defined.");
# endif
# ifndef CR2
	error ("Macro: CR2 not defined.");
# endif
# ifndef CR3
	error ("Macro: CR3 not defined.");
# endif
# ifndef TABDLY
	error ("Macro: TABDLY not defined.");
#endif
# ifndef TAB0
 	error ("Macro: TAB0 not defined.");
# endif
# ifndef TAB1
	error ("Macro: TAB1 not defined.");
# endif
# ifndef TAB2
	error ("Macro: TAB2 not defined.");
# endif
# ifndef TAB3
	error ("Macro: TAB3 not defined.");
# endif
# ifndef BSDLY
	error ("Macro: BSDLY not defined.");
# endif
# ifndef BS0
	error ("Macro: BS0 not defined.");
# endif
# ifndef BS1
	error ("Macro: BS1 not defined.");
# endif
# ifndef VTDLY
	error ("Macro: VTDLY not defined.");
# endif
# ifndef VT0
	error ("Macro: VT0 not defined.");
# endif
# ifndef VT1
	error ("Macro: VT1 not defined.");
# endif
# ifndef FFDLY
	error ("Macro: FFDLY not defined.");
# endif
# ifndef FF0
	error ("Macro: FF0 not defined.");
# endif
# ifndef FF1
	error ("Macro: FF1 not defined.");
# endif

finis

testing ("control modes");
testing ("The definitions of the control mode macros from termio.h");
testing ("Definitions for the macros: CBAUD, B0, B50, B75, B110,");
testing ("B134, B150, B200, B300, B600, B1200, B1800, B2400,");
testing ("B4800, B9600, B19200,  CSIZE, CS5, CS6, CS7, CS8,");
testing ("CSTOPB, CREAD, PARENB, PARODD, HUPCL and CLOCAL.");
start

# ifndef CBAUD
	error ("Macro: CBAUD not defined.");
# endif
# ifndef B0
	error ("Macro: B0 not defined.");
# endif
# ifndef B50
	error ("Macro: B50 not defined.");
# endif
# ifndef B75
	error ("Macro: B75 not defined.");
# endif
# ifndef B110
	error ("Macro: B110 not defined.");
# endif
# ifndef B134
	error ("Macro: B134 not defined.");
# endif
# ifndef B150
	error ("Macro: B150 not defined.");
# endif
# ifndef B200
	error ("Macro: B200 not defined.");
# endif
# ifndef B300
	error ("Macro: B300 not defined.");
# endif
# ifndef B600
	error ("Macro: B600 not defined.");
# endif
# ifndef B1200
	error ("Macro: B1200 not defined.");
# endif
# ifndef B1800
	error ("Macro: B1800 not defined.");
# endif
# ifndef B2400
	error ("Macro: B2400 not defined.");
# endif
# ifndef B4800
	error ("Macro: B4800 not defined.");
# endif
# ifndef B9600
	error ("Macro: B9600 not defined.");
# endif
# ifndef B19200
	error ("Macro: B19200 not defined.");
# endif
# ifndef CSIZE
	error ("Macro: CSIZE not defined.");
# endif
# ifndef CS5
	error ("Macro: CS5 not defined.");
# endif
# ifndef CS6
	error ("Macro: CS6 not defined.");
# endif
# ifndef CS7
	error ("Macro: CS7 not defined.");
# endif
# ifndef CS8
	error ("Macro: CS8 not defined.");
# endif
# ifndef CSTOPB
	error ("Macro: CSTOPB not defined.");
# endif
# ifndef CREAD
	error ("Macro: CREAD not defined.");
# endif
# ifndef PARENB
	error ("Macro: PARENB not defined.");
# endif
# ifndef PARODD
	error ("Macro: PARODD not defined.");
# endif
# ifndef HUPCL
	error ("Macro: HUPCL not defined.");
# endif
# ifndef CLOCAL
	error ("Macro: CLOCAL not defined.");
# endif

finis

testing ("line discipline");
testing ("The definitions of the control mode macros from termio.h");
testing ("Definitions for the macros: ISIG, ICANON, XCASE, ECHO,");
testing ("ECHOE, ECHOK, ECHONL and NOFLSH.");
start

# ifndef ISIG
	error ("Macro: ISIG not defined.");
# endif
# ifndef ICANON
	error ("Macro: ICANON not defined.");
# endif
# ifndef XCASE
	error ("Macro: XCASE not defined.");
# endif
# ifndef ECHO
	error ("Macro: ECHO not defined.");
# endif
# ifndef ECHOE
	error ("Macro: ECHOE not defined.");
# endif
# ifndef ECHOK
	error ("Macro: ECHOK not defined.");
# endif
# ifndef ECHONL
	error ("Macro: ECHONL not defined.");
# endif
# ifndef NOFLSH
	error ("Macro: NOFLSH not defined.");
# endif

finis

testing ("control chars");
testing ("The definitions of the control character macros from termio.h");
testing ("Definitions for the macros: VINTR, VQUIT, VERASE, VKILL,");
testing ("VEOF, VMIN, VEOL and VTIME.");
start


# ifndef VINTR
# undef CCPASS1
	error ("Macro: VINTR not defined.");
# endif
# ifndef VQUIT
# undef CCPASS1
	error ("Macro: VQUIT not defined.");
# endif
# ifndef VERASE
# undef CCPASS1
	error ("Macro: VERASE not defined.");
# endif
# ifndef VKILL
# undef CCPASS1
	error ("Macro: VKILL not defined.");
# endif
# ifndef VEOF
# undef CCPASS1
	error ("Macro: VEOF not defined.");
# endif
# ifndef VMIN
# undef CCPASS1
	error ("Macro: VMIN not defined.");
# endif
# ifndef VEOL
# undef CCPASS1
	error ("Macro: VEOL not defined.");
# endif
# ifndef VTIME
# undef CCPASS1
	error ("Macro: VTIME not defined.");
# endif

finis

# ifdef CCPASS1

testing ("control char vals.");
testing ("The values of the control character macros from termio.h");
testing ("Values of the macros as follows: VINTR=0, VQUIT=1,");
testing ("VERASE=2, VKILL=3, VEOF=4, VMIN=4, VEOL=5, VTIME=5.");
start


	if(VINTR != 0){
		error("Macro: VINTR has incorrect value");
	}
	if(VQUIT != 1){
		error("Macro: VQUIT has incorrect value");
	}
	if(VERASE != 2){
		error("Macro: VERASE has incorrect value");
	}
	if(VKILL != 3){
		error("Macro: VKILL has incorrect value");
	}
	if(VEOF != 4){
		error("Macro: VEOF has incorrect value");
	}
	if(VMIN != 4){
		error("Macro: VMIN has incorrect value");
	}
	if(VEOL != 5){
		error("Macro: VEOL has incorrect value");
	}
	if(VTIME != 5){
		error("Macro: VTIME has incorrect value");
	}
# endif

finis

testing ("struct termio fields");
testing ("The sizes of the struct termio fields from termio.h");
testing ("The following field declarations: unsigned short c_iflag,");
testing ("unsigned short c_oflag, unsigned short c_cflag,");
testing ("unsigned short c_lflag, char c_line, unsigned char c_cc[NCC].");
start

	if(sizeof(tstruct.c_iflag) != sizeof(unsigned short)){
		error("Element c_iflag:invalid size of %d instead of %d.",
		    sizeof(tstruct.c_iflag), sizeof(unsigned short));
	}
	if(sizeof(tstruct.c_oflag) != sizeof(unsigned short)){
		error("Element c_oflag:invalid size of %d instead of %d.",
		    sizeof(tstruct.c_oflag), sizeof(unsigned short));
	}
	if(sizeof(tstruct.c_cflag) != sizeof(unsigned short)){
		error("Element c_cflag:invalid size of %d instead of %d.",
		    sizeof(tstruct.c_cflag), sizeof(unsigned short));
	}
	if(sizeof(tstruct.c_lflag) != sizeof(unsigned short)){
		error("Element c_lflag:invalid size of %d instead of %d.",
		    sizeof(tstruct.c_lflag),sizeof(unsigned short));
	}
	if(sizeof(tstruct.c_line) != sizeof(char)){
		error("Element c_line:invalid size of %d instead of %d.",
		    sizeof(tstruct.c_line), sizeof(char));
	}
# ifdef NCC
	if(sizeof(tstruct.c_cc) != (sizeof(unsigned char) * NCC)){
		error("Element c_cc:invalid size of %d instead of %d.",
		    sizeof(tstruct.c_cc), sizeof(unsigned char)*NCC);
	}
# else
	error ("Macro: NCC not defined.");
#endif

finis

done();
/*NOTREACHED*/
}
