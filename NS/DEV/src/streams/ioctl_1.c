#ident "@(#)ioctl_1.c	1.5"
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

#define rendevous()     (v_sema(), psema())

void ioctl_doit();
void do_open();
void einval2();
void clean();

struct  tbl {
        int     cmd;
        int     arg;
} foo [] = {
                { I_PUSH,0 },
                { I_PUSH,-1 },
                { I_LOOK,0 },
                { I_LOOK,-1 },
                { I_GETSIG,0 },
                { I_GETSIG,-1 },
                { I_FIND,0 },
                { I_FIND,-1 },
                { I_PEEK,0 },
                { I_PEEK,-1 },
                { I_GRDOPT,0 },
                { I_GRDOPT,-1 },
                { I_FDINSERT,0 },
                { I_FDINSERT,-1 },
                { I_STR,0 },
                { I_STR,-1 },
                { I_NREAD,0 },
                { I_NREAD,-1 }
        };
#define ENTRIES ( sizeof( foo ) / sizeof( struct tbl ) )

struct foo2 
{
        int name;
} all [] = {
        I_PUSH, I_POP, I_LOOK, I_FLUSH, I_SETSIG, I_FIND, I_PEEK, I_GETSIG,
        I_SRDOPT, I_GRDOPT, I_NREAD, I_FDINSERT, I_STR, I_LINK, I_UNLINK
};
#define ENTRIES2 ( sizeof( all ) / sizeof( struct foo2 ) )

int     fd, mux, mux_id;
struct  strbuf  sctlbuf,rctlbuf;
struct  strbuf  sdatabuf,rdatabuf;
struct  strfdinsert     strfdbuf;
char    rctlmdata[BUFSIZE],rdatamdata[BUFSIZE];
char    sctlmdata[BUFSIZE],sdatamdata[BUFSIZE];
int     flags;
char    buf[100];

main()
{
        int     retn;
        char    badname[512];
        register int    i;


        setup("streams");

        do_open();

        
        abstract("Verify that ioctl returns EINVAL if an invalid module name");
        abstract("is specified (I_PUSH option).");
        start
                prep("Fill a buffer with 255 Xs.");
                start
                        for(i=0; i < 256; i++)
                                badname[i] = 'X';
                        badname[i]=0;
                finis

                testing("Call ioctl with the buffer name as the argument");
                testing("and I_PUSH as the command. Expecting failure with");
                testing("errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_PUSH,badname);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if there is no module");
        abstract("present in the stream and command is set to I_LOOK.");
        start
                testing("Call ioctl with the command set to I_LOOK. Expecting");
                testing("failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_LOOK, buf);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if fildes stream does not");
        abstract("support multiplexing and command is set to I_LINK.");
        start
                testing("Call ioctl with command set to I_LINK, fildes and");
                testing("arg set to file descriptor of the loop back driver.");
                testing("Expecting failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_LINK,fd);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if arg is not a stream");
        abstract("and command is set to I_LINK.");
        start
                prep("Open /etc/passwd for reading.");
                start
                        retn = open("/etc/passwd", O_RDONLY);
                finis

                testing("Call ioctl with fildes set to the file descriptor");
                testing("of the multiplexing driver, command set to I_LINK ");
                testing("and fd set to the file descriptor of /etc/passwd.");
                testing("Expecting failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(mux,I_LINK,retn);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if arg is already linked");
        abstract("under a multiplexor.");
        start
                prep("Call ioctl with fildes set to the file descriptor");
                prep("of the multiplexing driver, command set to I_LINK ");
                prep("and fd set to the file descriptor of the loop back");
                testing("driver.");
                start
                        expecting( VOID );
                        mux_id = ioctl(mux,I_LINK,fd);
                finis

                testing("Call ioctl with fildes set to the file descriptor");
                testing("of the multiplexing driver, command set to I_LINK ");
                testing("and fd set to the file descriptor of the loop");
                testing("back driver. Expecting failure with errno set to ");
                testing("EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(mux,I_LINK,fd);
                finis

                cleanup("Unlink the loop back driver.");
                start
                        expecting( VOID );
                        ioctl(fd,I_UNLINK,mux_id);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if arg is an invalid");
        abstract("multiplexor id.");
        start
                prep("Call ioctl with fildes set to the file descriptor");
                prep("of the multiplexing driver, command set to I_LINK ");
                prep("and fd set to the file descriptor of the loop back");
                testing("driver.");
                start
                        expecting( VOID );
                        mux_id = ioctl(mux,I_LINK,fd);
                finis

                testing("Call ioctl with fildes set to the file descriptor");
                testing("of the loop back driver, command set to I_UNLINK");
                testing("and mux_id set to its complement. Expecting failure");
                testing("with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_UNLINK,~mux_id);
                finis

                cleanup("Unlink the loop back driver.");
                start
                        expecting( VOID );
                        ioctl(fd,I_UNLINK,mux_id);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if an invalid module name");
        abstract("is specified (I_FIND option).");
        start
                prep("Fill a buffer with 255 Xs.");
                start
                        for(i=0; i < 256; i++)
                                badname[i] = 'X';
                        badname[i]=0;
                finis

                testing("Call ioctl with the buffer name as the argument");
                testing("and I_FIND as the command. Expecting failure with");
                testing("errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FIND,badname);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if an invalid value is");
        abstract("specified for arg. while the command is I_FLUSH.");
        start

                        flags = ~(FLUSHR|FLUSHW|FLUSHRW);

                testing("Call ioctl with the command set to I_FLUSH and the");
                testing("argument set to the complement of ");
                testing("(FLUSHR|FLUSHW|FLUSHRW). Expecting failure with");
                testing("errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FLUSH,flags);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if an invalid value is");
        abstract("specified for arg. while the command is I_SETSIG.");
        start
                        flags = ~(S_INPUT|S_HIPRI|S_OUTPUT|S_MSG);

                testing("Call ioctl with the command set to I_SETSIG and the");
                testing("argument set to the complement of ");
                testing("(S_INPUT|S_HIPRI|S_OUTPUT|S_MSG). Expecting failure");
                testing("with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_SETSIG,flags);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if an invalid value is");
        abstract("specified for arg. while the command is I_SETSIG.");
        start

                testing("Call ioctl with the command set to I_SETSIG and the");
                testing("argument set to zero. Expecting failure");
                testing("with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_SETSIG,0);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if an invalid value is");
        abstract("specified for arg. while the command is I_SRDOPT.");
        start
                        flags = ~(RNORM|RMSGD|RMSGN);

                testing("Call ioctl with the command set to I_SRDOPT and the");
                testing("argument set to the complement of (RNORM|RMSGD|RMSGN)");
                testing("Expecting failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_SRDOPT,flags);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL while the command is");
        abstract("I_FDINSERT.");
        start
                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=0;
                strfdbuf.offset=0;

                testing("Call ioctl with flags, fildes and offset fields of");
                testing("the strfdinsert structure set to 0 and with the ");
                testing("command set to I_FDINSERT. Expecting failure with ");
                testing("errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis


                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=MAXINT;
                strfdbuf.offset=0;

                testing("Call ioctl with the I_FDINSERT command and with the");
                testing("elements of the strfdinsert structure set to:");
                testing("flags=0, fildes=MAXINT and offset=0. Expecting");
                testing("failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis

                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags= ~RS_HIPRI;
                strfdbuf.fildes=fd;
                strfdbuf.offset=0;

                testing("Call ioctl with the I_FDINSERT command and with the");
                testing("elements of the strfdinsert structure set to:");
                testing("flags=~RS_HIPRI, fildes=fd and offset=0. Expecting");
                testing("failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis

                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=0;
                strfdbuf.offset=MAXINT;

                testing("Call ioctl with the I_FDINSERT command and with the");
                testing("elements of the strfdinsert structure set to:");
                testing("flags=0, fildes=0 and offset=MAXINT. Expecting");
                testing("failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis

                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=0;
                strfdbuf.offset=1;

                testing("Call ioctl with the I_FDINSERT command and with the");
                testing("elements of the strfdinsert structure set to:");
                testing("flags=0, fildes=0 and offset=1. Expecting");
                testing("failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis

                filldata(SND,&strfdbuf.ctlbuf,&strfdbuf.databuf,
                        BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                strfdbuf.flags=0;
                strfdbuf.fildes=0;
                strfdbuf.offset= ~0;

                testing("Call ioctl with the I_FDINSERT command and with the");
                testing("elements of the strfdinsert structure set to:");
                testing("flags=0, fildes=0 and offset=~0. Expecting");
                testing("failure with errno set to EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_FDINSERT,&strfdbuf);
                finis
        finis

        abstract("Verify that ioctl returns EINVAL if the command is set");
        abstract("to I_GETSIG and the process is not registered to receive a");
        abstract("SIGPOLL.");
        start
                testing("Call ioctl. Expecting failure with errno set to");
                testing("EINVAL.");
                start
                        expecting( EINVAL );
                        ioctl(fd,I_GETSIG,&flags);
                finis
        finis

        abstract("Verify that ioctl returns EBADF if arg is not an open stream");
        abstract("and command is set to I_LINK.");
        start
                prep("Close the loop back driver.");
                start
                        close( fd );
                finis

                testing("Call ioctl with fildes set to the file descriptor");
                testing("of the multiplexing driver, command set to I_LINK ");
                testing("and fd set to the file descriptor of the closed loop");
                testing("back driver. Expecting failure with errno set to ");
                testing("EBADF.");
                start
                        expecting( EBADF );
                        ioctl(mux,I_LINK,fd);
                finis

                cleanup("Open the loop back driver for read/write.");
                start
                        fd = open(DRIVER, O_RDWR);
                finis
        finis

        abstract("Verify ioctl will returns EINVAL when command is set to");
        abstract("any of the valid commands and fd points to a file linked");
        abstract("under a multiplexor.");
        start
                for( i = 0; i < ENTRIES2; i++ )
		{
                prep("Call ioctl to link the loop back driver under the");
                prep("multiplexor driver.");
                start
                        expecting( VOID );
                        mux_id = ioctl(mux,I_LINK,fd);
                finis

                        einval2(all[i].name);
		
		clean();
		do_open();
		}

        finis
        
        abstract("Verify ioctl returns EFAULT when command is set to");
        abstract("I_PUSH, I_LOOK, I_GETSIG, I_FIND, I_PEEK, I_GRDOPT, I_POP,");
        abstract("I_FDINSERT, I_STR, I_SETSIG or I_NREAD and arg is set to");
	abstract("0 or -1.");
        start
                for( i = 0; i < ENTRIES; i++ )
                        ioctl_doit(foo[i].cmd, foo[i].arg);
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
ioctl_doit(command, arg)
int     command, arg;
{
        char    cmd[25];
        
        switch(command)
        {
        case I_PUSH:
                strcpy(cmd, "I_PUSH");
                break;

        case I_LOOK:
                strcpy(cmd, "I_LOOK");
                prep("Call ioctl to push the %s module.", MODB);
                start
                        expecting( 0 );
                        ioctl(fd,I_PUSH,MODB);
                finis
                break;

        case I_POP:
                strcpy(cmd, "I_POP");
                break;

        case I_SETSIG:
                strcpy(cmd, "I_SETSIG");
                break;

        case I_GETSIG:
                strcpy(cmd, "I_GETSIG");
                prep("Call ioctl with command set to I_SETSIG and arg set");
                prep("to S_INPUT.");
                start
                        expecting( 0 );
                        ioctl(fd,I_SETSIG,S_INPUT);
                finis
                break;

        case I_FIND:
                strcpy(cmd, "I_FIND");
                break;

        case I_PEEK:
                strcpy(cmd, "I_PEEK");
                break;

        case I_GRDOPT:
                strcpy(cmd, "I_GRDOPT");
                break;

        case I_FDINSERT:
                strcpy(cmd, "I_FDINSERT");
                break;

        case I_STR:
                strcpy(cmd, "I_STR");
                break;

        case I_NREAD:
                strcpy(cmd, "I_NREAD");
                        filldata(SND,&sctlbuf,&sdatabuf,
                                BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                        prep("Call putmsg to send a message down stream.");
                        start
                                putmsg(fd,&sctlbuf,&sdatabuf,0);
                        finis

                break;

        default:
                break;
        }


        testing("Call ioctl with command set to %s and arg set to %d.",cmd,arg);
        testing("Expecting failure with errno set to EFAULT.");
        start
                expecting( EFAULT );
                ioctl(fd, command, arg);
        finis


        switch(command)
        {
        case I_LOOK:
                cleanup("Call ioctl to pop module %s.", MODB);
                start
                        expecting( 0 );
                        ioctl(fd, I_POP, 0);
                finis
                break;

        case I_GETSIG:
                cleanup("Call ioctl with command set to I_SETSIG and arg set");
                cleanup("to zero.");
                start
                        expecting( 0 );
                        ioctl(fd,I_SETSIG,0);
                finis
                break;

        case I_NREAD:
                filldata(RCV,&rctlbuf,&rdatabuf,
                        BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

                cleanup("Call getmsg to retrieve the message from the stream");
                cleanup("head.");
                start
                        flags=0;
                        getmsg(fd,&rctlbuf,&rdatabuf,&flags);
                finis
                break;

        default:
                break;
        }
                
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
einval2(command)
int     command;
{
        char    cmd[25];
        
        switch(command)
        {
        case I_PUSH:
                strcpy(cmd, "I_PUSH");
                break;

        case I_LOOK:
                strcpy(cmd, "I_LOOK");
                break;

        case I_POP:
                strcpy(cmd, "I_POP");
                break;

        case I_SETSIG:
                strcpy(cmd, "I_SETSIG");
                break;

        case I_GETSIG:
                strcpy(cmd, "I_GETSIG");
                break;

        case I_FIND:
                strcpy(cmd, "I_FIND");
                break;

        case I_PEEK:
                strcpy(cmd, "I_PEEK");
                break;

        case I_GRDOPT:
                strcpy(cmd, "I_GRDOPT");
                break;

        case I_FDINSERT:
                strcpy(cmd, "I_FDINSERT");
                break;

        case I_STR:
                strcpy(cmd, "I_STR");
                break;

        case I_NREAD:
                strcpy(cmd, "I_NREAD");
                        filldata(SND,&sctlbuf,&sdatabuf,
                                BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

                        prep("Call putmsg to send a message down stream.");
                        start
                                putmsg(fd,&sctlbuf,&sdatabuf,0);
                        finis

                break;

        case I_FLUSH:
                strcpy(cmd, "I_FLUSH");
                break;

        case I_SRDOPT:
                strcpy(cmd, "I_SRDOPT");
                break;

        case I_LINK:
                strcpy(cmd, "I_LINK");
                break;

        case I_UNLINK:
                strcpy(cmd, "I_UNLINK");
                break;

        default:
                break;
        }

        switch(command)
        {
        case I_PUSH:
        testing("Call ioctl with command set to I_PUSH and argument set to ");
        testing("%s. Expecting failure with errno set to EINVAL.", MODB);
        start
                expecting( EINVAL );
                ioctl(fd, I_PUSH, MODB);
        finis
                break;

        case I_POP:
        testing("Call ioctl with command set to I_POP and argument set to ");
        testing("%s. Expecting failure with errno set to EINVAL.", MODB);
        start
                expecting( EINVAL );
                ioctl(fd, I_POP, MODB);
        finis
                break;

        case I_LOOK:
        case I_FIND:
        case I_PEEK:
        case I_GRDOPT:
        case I_FDINSERT:
        case I_STR:
        case I_NREAD:
        case I_FLUSH:
        testing("Call ioctl with command set to %s and argument pointing", cmd);
        testing("to a buffer. Expecting failure with errno set to EINVAL.");
        start
                expecting( EINVAL );
                ioctl(fd, command, buf);
        finis
                break;

        case I_SETSIG:
        testing("Call ioctl with command set to I_SETSIG and argument set to");
        testing("S_INPUT. Expecting failure with errno set to EINVAL.");
        start
                expecting( EINVAL );
                ioctl(fd, I_SETSIG, S_INPUT);
        finis
                break;

        case I_GETSIG:
        testing("Call ioctl with command set to I_GETSIG and argument set to");
        testing("S_INPUT. Expecting failure with errno set to EINVAL.");
        start
                expecting( EINVAL );
                ioctl(fd, I_GETSIG, S_INPUT);
        finis
                break;

        case I_SRDOPT:
        testing("Call ioctl with command set to I_SRDOPT and argument set to");
        testing("RNORM. Expecting failure with errno set to EINVAL.");
        start
                expecting( EINVAL );
                ioctl(fd, I_SRDOPT, RNORM);
        finis
                break;

        default:
                break;
        }
}

