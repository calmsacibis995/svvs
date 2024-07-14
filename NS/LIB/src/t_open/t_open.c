#ident "@(#)t_open.c	1.1"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>
#include <string.h>

#define	BAD_PATH	"badpath.data/"

struct ptype {
	char	*typ; 
	int	val;
} ptypes[] = {
	"T_COTS_ORD",	T_COTS_ORD,
	"T_COTS",	T_COTS,
	"T_CLTS",	T_CLTS,
};

#define	NTYPES	( sizeof( ptypes ) / sizeof( struct ptype ) )
char	*flag_nms[] = {"O_RDONLY", "O_RDONLY|O_NDELAY", "O_WRONLY",
		    "O_WRONLY|O_NDELAY", "O_RDWR", "O_RDWR|O_NDELAY" };

int	flags[] = { O_RDONLY, O_RDONLY|O_NDELAY, O_WRONLY, 
		    O_WRONLY|O_NDELAY, O_RDWR, O_RDWR|O_NDELAY };

main()
{
	struct	t_info	info;
	int	tfd;
	int	tpdgtype;
	int	i;

	setup("t_open");
	funct("t_open");
	prep("Determine expected provider types from config file names.");
	start
		expecting( VOID );
		for( i = 0; i < NTYPES; i++ ) {
			if( !strcmp( TPVCTYPE, ptypes[i].typ )) {
				tpdgtype =  ptypes[i].val;
				break;
			}
		}		
		if( i==NTYPES )
			error("Invalid type '%s' specified for '%s'.",
			  TPVCTYPE, TPVCFILE0);
	finis


	abstract("Open a valid service provider and pass an address to a ");
	abstract("t_info structure. Check that the open successfully returns.");
	start     
		exit_on( ERROR );
		testing(" tfd = t_open(\"%s\", O_RDONLY, &info)", TPVCFILE0);
		start     
			tfd = t_open(TPVCFILE0, O_RDONLY, &info);
		finis      
	finis      


	abstract("Check that the current state for service provider ");
	abstract("\"%s\" is T_UNBND and that the correct ",TPVCFILE0);
	abstract("information was passed to the t_info structure.");
	abstract("Expecting the values of the t_info structure to be as ");
	abstract("follows:");
	abstract("      t_info.addr ==> %d ", TPVCINFO_ADDR);
	abstract("      t_info.options ==> %d ", TPVCINFO_OPTIONS);
	abstract("      t_info.tsdu ==> %d ", TPVCINFO_TSDU);
	abstract("      t_info.etsdu ==> %d ", TPVCINFO_ETSDU);
	abstract("      t_info.connect ==> %d ", TPVCINFO_CONNECT);
	abstract("      t_info.discon ==> %d ", TPVCINFO_DISCON);
	abstract("      t_info.servtype ==> %s ", tpdgtype);
	start     
		continue_on( ERROR );

		testing("Call t_getstate to get the current state of the");
		testing("provider. Expecting t_getstate() to return a state ");
		testing("of T_UNBND.");
		start     
			expecting( T_UNBND );
			t_getstate( tfd );
		finis      

		testing("Compare the information in the t_info structure ");
		testing("with that in the configuration file. ");
		testing("Check that the address fields are equal.");
		start
			if(info.addr != TPVCINFO_ADDR)
			    error("%s %d %s %d",
				    "Actual address returned was", info.addr,
				    "while expecting a value of",
				    TPVCINFO_ADDR);
		finis

		testing("Check that the options fields are equal.");
		start
			if(info.options != TPVCINFO_OPTIONS)
			    error("%s %d %s %d",
				    "Actual options returned was", info.options,
				    "while expecting a value of",
				    TPVCINFO_OPTIONS);
		finis

		testing("Check that the maximum size of the transport service");
		testing("data unit are equal.");
		start
			if(info.tsdu != TPVCINFO_TSDU)
			    error("%s %s %d %s %d",
				    "The maximum size of the transport service",
				    "data unit returned was", info.tsdu,
				    "while expecting a value of",
				    TPVCINFO_TSDU);
		finis

		testing("Check that the maximum size of the expedited transport");
		testing("service data unit are equal.");
		start
			if(info.etsdu != TPVCINFO_ETSDU)
			    error("%s %s %d %s %d",
				  "The maximum size of the expedited transport",
				  "service data unit returned was", info.etsdu,
				  "while expecting a value of", TPVCINFO_ETSDU);
		finis

		testing("Check that the maximum size of data to be transfered");
		testing("while establishing a connection, are equal");
		start
			if(info.connect != TPVCINFO_CONNECT)
			    error("%s %s %d %s %d",
			      "The maximum size of data to be transfered",
			      "while establishing a connection was returned as",
			      info.connect,
			      "while expecting a value of", TPVCINFO_CONNECT);
		finis

		testing("Check that the maximum amount of data to be transfered");
		testing("while executing the t_snddis and t_rcvdis calls, are");
		testing("equal.");
		start
			if(info.discon != TPVCINFO_DISCON)
			    error("%s %s %s %d %s %d",
				"The value returned for the maximum amount of",
				"data to be transfered while executing the ",
				" t_snddis and t_rcvdis calls was", info.discon,
				    "while expecting", TPVCINFO_DISCON);
		finis

		testing("Check that the servtype fields are equal.");
		start
			if(info.servtype != tpdgtype)
			    error("%s %d %s %d",
				  "Actual servtype returned was", info.servtype,
				  "while expecting a value of", tpdgtype);
		finis


		testing("Check that if the service provider supports a");
		testing("connectionless-mode service, the etsdu, connect and"); 
		testing("discon fields of the t_info structure equal -2.");
		start
			if((info.servtype == T_CLTS) &&
				((info.etsdu != (long) -2) ||
				(info.connect != (long) -2) ||
				(info.discon != (long) -2)))
			error("%s\n%s %d\n%s %d\n%s %d",
	                  "The service provider supports a connectionless-mode",
			  "service, however, the etsdu field is equal to",
			  info.etsdu,
		 	  "the connect field is equal to", info.connect,
			  "and the discon field is equal to", info.discon);
		finis      

		cleanup("Close the service provider.");
		start     
			t_close(tfd);
		finis      
	finis       
		

	abstract("Open a valid service provider with the info field equal");
	abstract("to NULL and check that it does not fail");
	start     
		exit_on ( ERROR );

		testing("Call t_open(\"%s\", O_RDONLY, NULL)",TPVCFILE0);
		start     
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis      

		cleanup("Close the service provider.");
		start     
			t_close(tfd);
		finis      
	finis      



	abstract("Open a file that is not a transport provider. ");
	abstract("Expecting failure with t_errno set to TSYSERR.");
	start  

		testing("Call t_open(\"/dev/tty\", O_RDONLY, NULL)");
		start     
			expecting ( TSYSERR );
			tfd = t_open("/dev/tty", O_RDONLY, NULL);
		finis      
	finis  

	abstract("Open a file that does not exist. Expecting failure ");
	abstract("with t_errno set to TSYSERR.");
	start   
		testing("call t_open(\"%s\" , O_RDONLY, NULL)", BAD_PATH);
		start     
			expecting ( TSYSERR );
			tfd = t_open(BAD_PATH,  O_RDONLY, NULL);
		finis      
	finis   

	abstract("Open service provider \"%s\" verifying the", TPVCFILE0);
	abstract("use of the oflags. Possible oflag values can be O_NDELAY");
	abstract("or-ed with O_RDONLY, O_WRONLY or O_RDWR.");
	start   
		for(i=0; i < (sizeof(flags)/sizeof(int)); i++)
		{
			testing("Call t_open with an oflag of %s.",flag_nms[i]);
			start     
				break_on ( ERROR );
				tfd = t_open(TPVCFILE0,  flags[i], NULL);
			finis

			cleanup("Close the opened file.");
			start     
				t_close( tfd );
			finis      
		}

	finis  
}
