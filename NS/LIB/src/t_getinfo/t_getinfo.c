#ident "@(#)t_getinfo.c	1.3"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>
#include <string.h>


struct	t_info	info;

struct ptype {
	char	*typ; 
	int	val;
} ptypes[] = {
	"T_COTS_ORD",	T_COTS_ORD,
	"T_COTS",	T_COTS,
	"T_CLTS",	T_CLTS,
};

#define	NTYPES	( sizeof( ptypes ) / sizeof( struct ptype ) )

main()
{
	int	tfd;
	int	tpdgtype;
	int	tpvctype;

	setup("t_getinfo");
	funct("t_getinfo");

	prep("Determine expected provider types from config file names.");
	start
		int	i;
		int	got_dg = 0;
		int	got_vc = 0;

		expecting( VOID );
		for( i = 0; i < NTYPES; i++ ) {
			if( !strcmp( TPDGTYPE, ptypes[i].typ )) {
				tpdgtype =  ptypes[i].val;
				got_dg = 1;
			}
			if( !strcmp( TPVCTYPE, ptypes[i].typ )) {
				tpvctype =  ptypes[i].val;
				got_vc = 1;
			}
		}		
		if( !got_vc )
			error("Invalid type '%s' specified for '%s'.",
			  TPVCTYPE, TPVCFILE0);
		if( !got_dg )
			error("Invalid type '%s' specified for '%s'.",
			  TPDGTYPE, TPDGFILE0);
	finis

	abstract("Verify that the information returned for the service ");
	abstract("provider \"%s\" is as expected. ", TPDGFILE0);
	abstract("Expecting values of: ");
	abstract("      t_info.addr ==> %d ", TPDGINFO_ADDR);
	abstract("      t_info.options ==> %d ", TPDGINFO_OPTIONS);
	abstract("      t_info.tsdu ==> %d ", TPDGINFO_TSDU);
	abstract("      t_info.etsdu ==> %d ", TPDGINFO_ETSDU);
	abstract("      t_info.connect ==> %d ", TPDGINFO_CONNECT);
	abstract("      t_info.discon ==> %d ", TPDGINFO_DISCON);
	abstract("      t_info.servtype ==> %s ", TPDGTYPE);
	start
		break_on( ERROR );
		prep(" fd = t_open(\"%s\", O_RDONLY, &info)", TPDGFILE0);
		start     
			tfd = t_open(TPDGFILE0, O_RDONLY, &info);
		finis      

		testing("Call t_getinfo to get the current characteristics of the ");
		testing("service provider.");
		start
			t_getinfo( tfd, &info);
		finis

		testing("Compare the information in the t_info structure ");
		testing("with that in the configuration file. ");
		start     
			continue_on( ERROR );
			compare(TPDGFILE0, TPDGINFO_ADDR, TPDGINFO_OPTIONS,
				TPDGINFO_TSDU, TPDGINFO_ETSDU, TPDGINFO_CONNECT,
					TPDGINFO_DISCON, tpdgtype);
		finis      

		cleanup("Close the service provider.");
		start     
			t_close(tfd);
		finis      
	finis


	expecting( VOID );
	if(strcmp(TPDGFILE0, TPDGFILE1))
	{
		abstract("Verify that the information returned for the service ");
		abstract("provider \"%s\" is as expected. ", TPDGFILE1);
		abstract("Expecting values of: ");
		abstract("      t_info.addr ==> %d ", TPDGINFO_ADDR);
		abstract("      t_info.options ==> %d ", TPDGINFO_OPTIONS);
		abstract("      t_info.tsdu ==> %d ", TPDGINFO_TSDU);
		abstract("      t_info.etsdu ==> %d ", TPDGINFO_ETSDU);
		abstract("      t_info.connect ==> %d ", TPDGINFO_CONNECT);
		abstract("      t_info.discon ==> %d ", TPDGINFO_DISCON);
		abstract("      t_info.servtype ==> %s ", TPDGTYPE);
		start
			break_on( ERROR );
			expecting( SUCCESS );
			prep(" fd = t_open(\"%s\", O_RDONLY, &info)", TPDGFILE1);
			start     
				tfd = t_open(TPDGFILE1, O_RDONLY, &info);
			finis      

			testing("Call t_getinfo to get the current characteristics ");
			testing("of the service provider.");
			start
				t_getinfo( tfd, &info);
			finis

			testing("Compare the information in the t_info structure ");
			testing("with that in the configuration file. ");
			start     
				continue_on( ERROR );
				compare(TPDGFILE1, TPDGINFO_ADDR, 
					TPDGINFO_OPTIONS, TPDGINFO_TSDU,
					TPDGINFO_ETSDU, TPDGINFO_CONNECT,
					TPDGINFO_DISCON, tpdgtype);
			finis      

			cleanup("Close the service provider.");
			start     
				t_close(tfd);
			finis      
		finis
	}


	abstract("Verify that the information returned for the service ");
	abstract("provider \"%s\" is as expected. ", TPVCFILE0);
	abstract("Expecting values of: ");
	abstract("      t_info.addr ==> %d ", TPVCINFO_ADDR);
	abstract("      t_info.options ==> %d ", TPVCINFO_OPTIONS);
	abstract("      t_info.tsdu ==> %d ", TPVCINFO_TSDU);
	abstract("      t_info.etsdu ==> %d ", TPVCINFO_ETSDU);
	abstract("      t_info.connect ==> %d ", TPVCINFO_CONNECT);
	abstract("      t_info.discon ==> %d ", TPVCINFO_DISCON);
	abstract("      t_info.servtype ==> %s ", TPVCTYPE);
	start
		break_on( ERROR );
		prep(" fd = t_open(\"%s\", O_RDONLY, &info)", TPVCFILE0);
		start     
			tfd = t_open(TPVCFILE0, O_RDONLY, &info);
		finis      

		testing("Call t_getinfo to get the current characteristics ");
		testing("of the service provider.");
		start
			t_getinfo( tfd, &info);
		finis

		testing("Compare the information in the t_info structure ");
		testing("with that in the configuration file. ");
		start     
			continue_on( ERROR );
			compare(TPVCFILE0, TPVCINFO_ADDR, TPVCINFO_OPTIONS,
				TPVCINFO_TSDU, TPVCINFO_ETSDU, TPVCINFO_CONNECT,
					TPVCINFO_DISCON, tpvctype);
		finis      

		cleanup("Close the service provider.");
		start     
			t_close(tfd);
		finis      
	finis



	expecting( VOID );
	if(strcmp(TPVCFILE0,TPVCFILE1))
	{
		abstract("Verify that the information returned for the service ");
		abstract("provider \"%s\" is as expected. ", TPVCFILE1);
		abstract("Expecting values of: ");
		abstract("      t_info.addr ==> %d ", TPVCINFO_ADDR);
		abstract("      t_info.options ==> %d ", TPVCINFO_OPTIONS);
		abstract("      t_info.tsdu ==> %d ", TPVCINFO_TSDU);
		abstract("      t_info.etsdu ==> %d ", TPVCINFO_ETSDU);
		abstract("      t_info.connect ==> %d ", TPVCINFO_CONNECT);
		abstract("      t_info.discon ==> %d ", TPVCINFO_DISCON);
		abstract("      t_info.servtype ==> %s ", TPVCTYPE);
		start
			break_on( ERROR );
			expecting( SUCCESS );
			prep(" fd = t_open(\"%s\", O_RDONLY, &info)", TPVCFILE1);
			start     
				tfd = t_open(TPVCFILE1, O_RDONLY, &info);
			finis      

			testing("Call t_getinfo to get the current characteristics ");
			testing("of the service provider.");
			start
				t_getinfo( tfd, &info);
			finis

			testing("Compare the information in the t_info structure ");
			testing("with that in the configuration file. ");
			start     
				continue_on( ERROR );
				compare(TPVCFILE1, TPVCINFO_ADDR, 
					TPVCINFO_OPTIONS, TPVCINFO_TSDU,
					TPVCINFO_ETSDU, TPVCINFO_CONNECT,
					TPVCINFO_DISCON, tpvctype);
			finis      

			cleanup("Close the service provider.");
			start     
				t_close(tfd);
			finis      
		finis

	}

	abstract("Test that t_getinfo returns TBADF if the file descriptor ");
	abstract("does not refer to a service provider. Do this by ");
	abstract("opening the file /dev/tty and passing the file ");
	abstract("descriptor as an argument to t_getinfo. ");
	start     
		prep("Call open(BA_OS) to open /dev/tty. ");
		start    
			exit_on( ERROR );
			tfd = open("/dev/tty", O_RDONLY);
		finis    

		testing("Call t_getinfo and pass the file descriptor for ");
		testing("/dev/tty as an argument. Expecting failure ");
		testing("and t_errno set to TBADF.");
		start    
			expecting ( TBADF );
			t_getinfo(tfd, &info);
		finis    

		cleanup("Use close(BA_OS) to close file /dev/tty");
		start   
			close(tfd);
		finis   
	finis   

	done();
}



compare(provider, addr, options, tsdu, etsdu, connect, discon, servtype)
char	*provider;
long	addr, options, tsdu, etsdu, connect, discon, servtype;
{

	testing("Check that the address fields are equal.");
	start
		if(info.addr != addr)
		    error("%s %d %s %d",
			    "Actual address returned was", info.addr,
			    "while expecting a value of", addr);
	finis

	testing("Check that the options fields are equal.");
	start
		if(info.options != options)
		    error("%s %d %s %d",
			    "Actual options returned was", info.options,
			    "while expecting a value of", options);
	finis

	testing("Check that the maximum size of the transport service data");
	testing("units are equal.");
	start
		if(info.tsdu != tsdu)
		    error("%s %s %d %s %d",
	    		    "The maximum size of the transport service data",
			    "unit returned was", info.tsdu,
			    "while expecting a value of", tsdu);
	finis

	testing("Check that the maximum size of the expedited transport");
	testing("service data units are equal.");
	start
		if(info.etsdu != etsdu)
		    error("%s %s %d %s %d",
	    		    "The maximum size of the expedited transport",
			    "service data unit returned was", info.etsdu,
			    "while expecting a value of", etsdu);
	finis

	testing("Check that the maximum size of data to be transferred while");
	testing("establishing a connection are equal");
	start
		if(info.connect != connect)
		    error("%s %s %d %s %d",
	    		    "The maximum size of data to be transferred while",
			    "establishing a connection was returned as",
			    info.connect,
			    "while expecting a value of", connect);
	finis

	testing("Check that the maximum amount of data to be transferred");
	testing("while executing the t_snddis and t_rcvdis calls are equal.");
	start
		if(info.discon != discon)
		    error("%s %s %s %d %s %d",
			    "The value returned for the maximum amount of data",
			    "to be transferred while executing the t_snddis",
			    "and t_rcvdis calls was", info.discon,
			    "while expecting", discon);
	finis

	testing("Check that the servtype fields are equal.");
	start
		if(info.servtype != servtype)
		    error("%s %d %s %d",
			    "Actual servtype returned was", info.servtype,
			    "while expecting a value of", servtype);
	finis

}

