#ident "@(#)ioctl_2.c	1.6"
#include <stdio.h>      
#include <fcntl.h>
#include <sv_base.h>
#include <sv_nse.h>
#include <svvs.h>       
#include <sv_env.h>
#include <sv_macro.h>
#include <sys/stropts.h>
#include <sv_streams.h>
#include <string.h>
#include <values.h>
#include <poll.h>

void sigcatch();
void sethang();
void setsig_doit();
void do_open();
void clean();

int     sig[] = {S_INPUT,S_OUTPUT,S_HIPRI,S_MSG,S_INPUT|S_OUTPUT|S_HIPRI|S_MSG};

int     fd, fd1, mux, mux_id;
int     flags;
unsigned        int     qvalue,nqval;
int     *iptr;
struct  foo2 {
        char    *mode_nm;
        int     mode;
} modes[] = {{"RNORM", RNORM},
             {"RMSGD", RMSGD},
             {"RMSGN", RMSGN}};
#define MODE_SZ (sizeof(modes)/sizeof(struct foo2))
int     rtnmode;
main()
{
        struct  strioctl        istr;
        char    errval;
        char    badname[512];
        char    buf[100];
        char    modbuf[10];
        register int    i;
        struct  strbuf  sctlbuf,rctlbuf;
        struct  strbuf  sdatabuf,rdatabuf;
        struct  strfdinsert     strfdbuf;
        char    rctlmdata[BUFSIZE],rdatamdata[BUFSIZE];
        char    sctlmdata[BUFSIZE],sdatamdata[BUFSIZE];
        struct  strpeek         strpeekbuf;

        setup("streams");

        do_open();

/* 
** verify all the ENXIO errors....
*/


        abstract("Verify that ioctl returns ENXIO if command is set to");
        abstract("I_PUSH and the open of a new module failed.");
        start
                        
                testing("Call ioctl with command set to I_PUSH and arg set");
                testing("to %s. Expecting failure with errno set to ENXIO.",
                        MODR);
                start
                        expecting( ENXIO );
                        ioctl(fd,I_PUSH,MODR);
                finis
        finis

        abstract("Verify that ioctl returns ENXIO when command is set to");
        abstract("I_FDINSERT.");
        start
                sethang();
                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=fd;
                strfdbuf.offset=0;

                testing("Call ioctl with command set to I_FDINSERT. Expecting");
                testing("failure with errno set ENXIO.");
                start
                        expecting( ENXIO );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis
        finis

        abstract("Verify that ioctl returns ENXIO when command is set to");
        abstract("I_STR.");
        start
                sethang();
                istr.ic_cmd = I_SETERR;
                istr.ic_len = sizeof(char);
                istr.ic_timout = 0;
                errval = (char)ENODEV;
                istr.ic_dp = &errval;

                testing("Call ioctl with command set to I_STR. Expecting");
                testing("failure with errno set ENXIO.");
                start
                        expecting( ENXIO );
                        ioctl(fd,I_STR,&istr);
                finis
        finis

        abstract("Verify that ioctl returns ENXIO when command is set to");
        abstract("I_POP.");
        start
                prep("Call ioctl to push module %s onto the stream.", MODB);
                start
                        expecting( VOID );
                        ioctl(fd,I_PUSH,MODB);
                finis

                sethang(fd);
                
                testing("Call ioctl with command set to I_POP. Expecting");
                testing("failure with errno set to ENXIO.");
                start
                        expecting( ENXIO );
                        ioctl(fd,I_POP,0);
			clean();
			do_open();
                finis
        finis

/*
** verify all the functionalities.
*/

        abstract("Verify the functionality of I_PUSH, I_POP and I_LOOK.");
        abstract("Push a module, look to make sure it was pushed then pop the");
        abstract("module.");
        start
                testing("Call ioctl to push the module %s.", MODB);
                start
                        expecting( 0 );
                        ioctl(fd,I_PUSH,MODB);
                finis

                testing("Call ioctl with command set to I_LOOK. Check that");
                testing("module returned is the one just pushed.");
                start
                        expecting( 0 );
                        ioctl(fd,I_LOOK,modbuf);
                        expecting( VOID );
                        if(strcmp(modbuf,MODB))
                                error("First module is not same.");
                finis

                testing("Call ioctl to push module %s.", MODT);
                start
                        expecting( 0 );
                        ioctl(fd,I_PUSH,MODT);
                finis

                testing("Call ioctl with command set to I_LOOK. Check that");
                testing("module returned is the one just pushed.");
                start
                        expecting( 0 );
                        ioctl(fd,I_LOOK,modbuf);
                        expecting( VOID );
                        if(strcmp(modbuf,MODT))
                                error("Second module is not same.");
                finis

                testing("Call ioctl to pop the first module.");
                start
                        expecting( 0 );
                        ioctl(fd,I_POP,0);
                finis

                testing("Call ioctl with command set to I_LOOK. Check that");
                testing("module returned is the first one pushed.");
                start
                        expecting( 0 );
                        ioctl(fd,I_LOOK,modbuf);
                        expecting( VOID );
                        if(strcmp(modbuf,MODB))
                                error("First module is not same.");
                finis
        finis

	abstract("Verify the functionality of I_LINK and I_UNLINK. Link");
	abstract("a driver to a multiplexor then unlink it. ");
	start
		testing("Call ioctl to link the loop back driver under the");
		testing("multiplexor driver.");
		start
			expecting( VOID );
			mux_id = ioctl(mux,I_LINK,fd);
		finis

		testing("Verify the driver was linked by calling ioctl with");
		testing("cmd set to I_PUSH and arg set to %s. Expecting",MODB);
		testing("failure with errno set to EINVAL.");
		start
			expecting( EINVAL );
			ioctl(fd, I_PUSH, MODB);
		finis

		prep("Close the loop back and multiplex derivers and open");
		prep("them for read/write. Link the driver to the multiplex.");
		start
			close( fd );
			close( mux );
			fd = open(DRIVER,O_RDWR);
			mux = open(MUX,O_RDWR);
			expecting( VOID );
			mux_id = ioctl(mux,I_LINK,fd);
		finis

		testing("Verify the functionality of I_UNLINK by calling ");
		testing("ioctl to unlink the driver.");
		start
			expecting( VOID );
			ioctl(fd,I_UNLINK,mux_id);
		finis

	finis
		clean();
		do_open();

	abstract("Verify the functionality of I_FLUSH. ");
	start
		testing("Call ioctl with cmd set to I_FLUSH and arg set");
		testing("to FLUSHR");
		start
			expecting( 0 );
			ioctl(fd,I_FLUSH,FLUSHR);
		finis
		testing("Call ioctl with cmd set to I_FLUSH and arg set");
		testing("to FLUSHW");
		start
			expecting( 0 );
			ioctl(fd,I_FLUSH,FLUSHW);
		finis
		testing("Call ioctl with cmd set to I_FLUSH and arg set");
		testing("to FLUSHRW");
		start
			expecting( 0 );
			ioctl(fd,I_FLUSH,FLUSHRW);
		finis
	finis


	abstract("Verify the functionality of I_SETSIG and I_GETSIG. Call");
	abstract("ioctl with cmd set to I_SETSIG and arg set to S_INPUT,");
	abstract("S_OUTPUT, S_HIPRI, S_MSG or S_INPUT|S_OUTPUT|S_HIPRI|S_MSG.");
	abstract("Call ioctl with cmd set to I_GETSIG to verify the signal");
	abstract("retreived is same as that sent.");
        start
                prep("Close the loop back driver.");
                start
                        close( fd );
                finis

                for( i = 0; i < (sizeof(sig)/sizeof(int)); i++)
                        setsig_doit(sig[i]);

                cleanup("Open the loop back driver for read/write.");
                start
                        fd = open(DRIVER, O_RDWR);
                finis

        finis

	abstract("Verify the functionality of I_FIND");
	start
                prep("Close the loop back driver and open it for read/write");
                prep("with O_NDELAY flag set.");
                start
                        close( fd );
                        fd = open(DRIVER, O_RDWR|O_NDELAY);
                finis

                testing("Call ioctl with command set to I_FIND and arg");
                testing("set to %s. Expecting ioctl to return 0.", MODB);
                start
                        expecting( 0 );
                        ioctl(fd,I_FIND,MODB);
                finis


                prep("Call ioctl with command set to I_PUSH and the module");
                prep("set to %s.", MODB);
                start
                        expecting( 0 );
                        ioctl(fd,I_PUSH,MODB);
                finis

                testing("Call ioctl with command set to I_FIND and arg");
                testing("set to %s. Expecting ioctl to return 1.", MODB);
                start
                        expecting( 1 );
                        ioctl(fd,I_FIND,MODB);
                finis

                cleanup("Close the loop back driver and open it for");
                cleanup("read/write.");
                start
                        close( fd );
                        fd = open(DRIVER, O_RDWR);
                finis
        finis

        abstract("Verify that I_PEEK returns the data requested and leaves ");
        abstract("the messages on the queue.");
        start
                filldata(SND,&sctlbuf,&sdatabuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
                filldata(RCV,&rctlbuf,&rdatabuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);
                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                prep("Call putmsg with flags set to 0.");
                start
                        putmsg(fd,&sctlbuf,&sdatabuf,0);
                finis

                testing("Call ioctl with command set to I_PEEK. Expecting a");
                testing("return value of 1.");
                start
                        expecting( 1 );
                        ioctl(fd,I_PEEK,&strpeekbuf);
                finis

                checkdata(&sctlbuf,&strpeekbuf.ctlbuf,
                        &sdatabuf,&strpeekbuf.databuf);

                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                flags=0;
                prep("Call getmsg to retrieve the message.");
                start
                        getmsg(fd,&rctlbuf,&rdatabuf,&flags);
                finis

                checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);
        finis


        abstract("Verify that I_PEEK returns 0 is a priority message is asked");
        abstract("for and there are none.");
        start
                filldata(SND,&sctlbuf,&sdatabuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
                filldata(RCV,&rctlbuf,&rdatabuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);
                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                prep("Call putmsg with flags set to 0.");
                start
                        putmsg(fd,&sctlbuf,&sdatabuf,0);
                finis

                strpeekbuf.flags = 1;

                testing("Call ioctl with commands set to I_PEEK. Expecting");
                testing("a return value of zero.");
                start
                        expecting( 0 );
                        ioctl(fd,I_PEEK,&strpeekbuf);
                finis

                strpeekbuf.flags = 0;

                checkdata(&rctlbuf,&strpeekbuf.ctlbuf,
                        &rdatabuf,&strpeekbuf.databuf);

                flags=0;

                cleanup("Call getmsg to retrieve that message.");
                start
                        getmsg(fd,&rctlbuf,&rdatabuf,&flags);
                finis

                checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);
        finis


        abstract("Verify that I_PEEK returns EFAULT if a bad address is ");
        abstract("passed inside the ctlbuf structure.");
        start
                filldata(SND,&sctlbuf,&sdatabuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
                filldata(RCV,&rctlbuf,&rdatabuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);
                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                prep("Call putmsg with flag set to 0.");
                start
                        putmsg(fd,&sctlbuf,&sdatabuf,0);
                finis

                strpeekbuf.ctlbuf.buf=0;

                testing("Call ioctl with command set to I_PEEK. Expecting ");
                testing("failure with errno set to EFAULT.");
                start
                        expecting( EFAULT );
                        ioctl(fd,I_PEEK,&strpeekbuf);
                finis
        finis

        abstract("Verify that I_PEEK returns the priority data requested and");
        abstract("leaves the priority and other non-priority messages on the");
        abstract("queue.");
        start
                prep("Close the loop back driver and open it for read/write");
                prep("with O_NDELAY set.");
                start
                        close( fd );
                        fd = open(DRIVER,O_RDWR|O_NDELAY);
                finis

                filldata(SND,&sctlbuf,&sdatabuf,
                        BUFSIZE/2,BUFSIZE/2,sctlmdata,sdatamdata);
                filldata(RCV,&rctlbuf,&rdatabuf,
                        BUFSIZE/2,BUFSIZE/2,rctlmdata,rdatamdata);
                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE/2,BUFSIZE/2,rctlmdata,rdatamdata);

                prep("Call putmsg with flag set to 0.");
                start
                        putmsg(fd,&sctlbuf,&sdatabuf,0);
                finis
                
                prep("Call putmsg with flag set to RS_HIPRI.");
                start
                        putmsg(fd,&sctlbuf,&sdatabuf,RS_HIPRI);
                finis

                strpeekbuf.flags=RS_HIPRI;

                testing("Call ioctl with command set to I_PEEK. Check that it");
                testing("returns RS_HIPRI.");
                start
                        expecting( RS_HIPRI);
                        ioctl(fd,I_PEEK,&strpeekbuf);
                finis

                if(strpeekbuf.flags == 1)
                        strpeekbuf.flags = 0;

                checkdata(&sctlbuf,&strpeekbuf.ctlbuf,
                        &sdatabuf,&strpeekbuf.databuf);

                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE/2,BUFSIZE/2,rctlmdata,rdatamdata);

                flags=1;
                testing("Call getmsg to retrieve the priority message.");
                start
                        getmsg(fd,&rctlbuf,&rdatabuf,&flags);
                finis

                checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);

                        flags=0;
                testing("Call getmsg to retrieve the non priority message.");
                start
                        getmsg(fd,&rctlbuf,&rdatabuf,&flags);
                finis

                checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);

                cleanup("Close the loop back driver and open for read/write.");
                start
                        close( fd );
                        fd = open(DRIVER, O_RDWR);
                finis
        finis


        abstract("Verify that I_PEEK returns the priority data requested and");
        abstract("leaves the messages on the queue.");
        start

                filldata(SND,&sctlbuf,&sdatabuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
                filldata(RCV,&rctlbuf,&rdatabuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);
                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                prep("Call putmsg with flags set to RS_HIPRI.");
                start
                        putmsg(fd,&sctlbuf,&sdatabuf,1);
                finis

                strpeekbuf.flags = 1;

                testing("Call ioctl with commands set to I_PEEK. Expecting a");
                testing("return value of RS_HIPRI.");
                start
                        expecting( RS_HIPRI);
                        ioctl(fd,I_PEEK,&strpeekbuf);
                finis

                checkdata(&sctlbuf,&strpeekbuf.ctlbuf,
                        &sdatabuf,&strpeekbuf.databuf);

                filldata(RCV,&strpeekbuf.ctlbuf,&strpeekbuf.databuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                flags=1;

                prep("call getmsg to retrieve the message.");
                start
                        getmsg(fd,&rctlbuf,&rdatabuf,&flags);
                finis

                checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);
        finis

	abstract("Verify the functionality of I_FINDINSERT when offset=zero.");
        start
		prep("Open the second loop back driver for read/write");
		start
			fd1 = open(DRIVER2,O_RDWR);
		finis
                (int) sigset(SIGALRM,sigcatch);

                break_on(ERROR);

	prep("Get the pointer to the second loop back driver's read queue");
	prep("structure. Call ioctl with command set to I_STR. ");
        start
                istr.ic_cmd = I_QPTR;
                istr.ic_len = sizeof (int);
                istr.ic_timout = 0;
                istr.ic_dp = (char *)&flags;

                expecting( VOID );
                if((qvalue = ioctl(fd1,I_STR,&istr)) == -1)
                        error("Ioctl failed with errno set to %d.", errno);
        finis

        filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                4,BUFSIZE,sctlmdata,sdatamdata);

        strfdbuf.flags=0;
        strfdbuf.fildes=fd1;
        strfdbuf.offset=0;

        alarm(30);

	testing("Verify that if flags and offset are set to zero, the");
	testing("correct message is sent down stream.");
        testing("Call ioctl with command set to I_FDINSERT and arg pointing");
        testing("to strfdinsert structure. The flags field is set to 0, the ");
        testing("fildes field points to the second loop back driver and the ");
        testing("offset field is set to 0.");
        start
                expecting( 0 );
                ioctl(fd,I_FDINSERT,&strfdbuf);
        finis

        filldata(SND,&sctlbuf,&sdatabuf,
                4,BUFSIZE,sctlmdata,sdatamdata);
        filldata(RCV,&rctlbuf,&rdatabuf,
                4,BUFSIZE,rctlmdata,rdatamdata);

        flags=0;
        prep("Call getmsg to retrieve the message sent down stream.");
        start
                getmsg(fd,&rctlbuf,&rdatabuf,&flags);
        finis

	testing("Check to see that a pointer to the second driver's read");
	testing("queue eas returned from the getmsg call.");
	start
                iptr = (int *)rctlbuf.buf;
                nqval = *iptr;

                if(nqval != qvalue)
                        error("bad queue value returned");
	finis

        testing("Skip past the queue pointer returned and compare rest of ");
        testing("data.");
        start
                checkdata(0,0,&sdatabuf, &rdatabuf);
        finis
        cleanup("Close the second loop back driver.");
        start
                close(fd1 );
        finis

        finis

        abstract("Check that if flags is set to zero and offset is set to");
        abstract("200, the correct message is sent down stream.");
        start
                sigset(SIGALRM,sigcatch);

		prep("Open the second loop back driver for read/write.");
		start
			fd1 = open(DRIVER2,O_RDWR);
		finis

                istr.ic_cmd = I_QPTR;
                istr.ic_len = sizeof (int);
                istr.ic_timout = 0;
                istr.ic_dp = (char *)&flags;

		prep("Get the pointer to the second loop back driver's read ");
		prep("queue structure. Call ioctl with command set to I_STR. ");
		start
			expecting( VOID );
			if((qvalue = ioctl(fd1,I_STR,&istr)) == -1)
			     error("Ioctl failed with errno set to %d.", errno);
		finis

                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        204,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=fd1;
                strfdbuf.offset=200;

                alarm(30);


        testing("Call ioctl with command set to I_FDINSERT and arg pointing");
        testing("to strfdinsert structure. The flags field is set to 0, the ");
        testing("fildes field points to the second loop back driver and the ");
        testing("offset field is set to 200.");
        start
                expecting( 0 );
                ioctl(fd,I_FDINSERT,&strfdbuf);
        finis
                        
        filldata(SND,&sctlbuf,&sdatabuf,
                204,BUFSIZE,sctlmdata,sdatamdata);
        filldata(RCV,&rctlbuf,&rdatabuf,
                204,BUFSIZE,rctlmdata,rdatamdata);

                flags=0;
        prep("Call getmsg to retrieve the message sent down stream.");
        start
                getmsg(fd,&rctlbuf,&rdatabuf,&flags);
        finis

	testing("Check that the read queue pointer for the second loop back");
	testing("driver was placed after byte 200 of the buffer returned");
	testing("from getmsg call.");
	start

                iptr = (int *)&rctlbuf.buf[200];
                nqval = *iptr;

                if(nqval != qvalue)
                        error("bad queue value returned");
	finis

        testing("Skip past the queue pointer returned and compare rest of ");
        testing("data.");
        start
                checkdata(0,0,&sdatabuf, &rdatabuf);
        finis

        finis

        abstract("Verify that if call ioctl with command set to I_SRDOPT");
        abstract("and modes set to RNORM, RMSGD or RMSGN that it succeeds");
        abstract("successfully. Also call ioctl with command set to I_GRDOPT");
        abstract("and check that the modes were properly set.");
        start
                for(i=0; i<MODE_SZ; i++)
                {
                testing("Call ioctl with command set to I_SRDOPT and arg");
                testing("set to %s.", modes[i].mode_nm);
                start
                        expecting( 0 );
                        ioctl(fd,I_SRDOPT,modes[i].mode);
                finis

                testing("Call ioctl with command set to I_GRDOPT and arg");
                testing("set to %s.", modes[i].mode_nm);
                start
                        expecting( 0 );
                        ioctl(fd,I_GRDOPT,&rtnmode);
                finis

                testing("Check that the mode returned was that which was");
                testing("expected.");
                start
                        if(rtnmode != modes[i].mode)
                                error("Incorrect mode returned");
                finis
                }
        finis

        clean();
        done();
}
void
do_open()
{
        abstract("Open two drivers. ");
        start
                exit_on( ERROR );
                prep("Open the loop back streams based driver");
                prep("\"%s\" for reading and writing.", DRIVER);
                start
                        fd = open(DRIVER,O_RDWR);
                finis
                        
                prep("Open the streams based multiplex driver \"%s\"", MUX);
                prep("for reading and writing.");
                start
                        mux = open(MUX, O_RDWR);
                finis

        finis
}
void
sethang()
{
        struct  strioctl        istr;
        int     rtn;

        istr.ic_cmd = I_SETHANG;
        istr.ic_len = istr.ic_timout = 0;
        prep("Call ioctl with command set to I_STR. This will generate a");
        prep("hangup request at the stream head.");
        start
                expecting( VOID );
                ioctl(fd,I_STR,&istr);
        finis
}
void
clean()
{
        abstract("Close the multiplexor and the loop back drivers.");
        start
                close( fd );
                close( mux );
        finis
}

void
setsig_doit(Arg)
int     Arg;
{
        char    arg[50];
        int     flags;

        switch(Arg)
        {
        case S_HIPRI:
                strcpy(arg, "S_HIPRI");
                break;
        
        case S_INPUT:
                strcpy(arg, "S_INPUT");
                break;
        
        case S_OUTPUT:
                strcpy(arg, "S_OUTPUT");
                break;
        
        case S_MSG:
                strcpy(arg, "S_MSG");
                break;
        
        default:
                strcpy(arg, "S_INPUT|S_OUTPUT|S_HIPRI|S_MSG");
                break;
        }
        

        prep("Open the loopback driver for read/write.");
        start
                fd = open(DRIVER, O_RDWR);
        finis

        testing("Call ioctl with command set to I_SETSIG and arg set ");
        testing("to %s.", arg);
        start
                expecting( 0 );
                ioctl(fd,I_SETSIG, Arg);
        finis

        testing("Call ioctl with command set to I_GETSIG and arg set ");
        testing("to %s.", arg);
        start
                expecting( 0 );
                ioctl(fd,I_GETSIG,&flags);
        finis

        testing("Verify the signal retrieved was %s.", arg);
        start
                if(flags != Arg)
                        error("Incorrect value returned from getsig.");
        finis

        cleanup("Close the driver.");
        start
                close(fd);
        finis
}
void
sigcatch(sig)
int     sig;
{
        gotsig = sig;
}
