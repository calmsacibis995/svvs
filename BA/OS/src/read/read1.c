#ident	"@(#)read1.c	1.2"
#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2
#include	<unistd.h>
#endif
#ifndef	SEEK_SET
#define	SEEK_SET 0
#endif
#ifndef	SEEK_CUR
#define	SEEK_CUR 1
#endif
#ifndef	SEEK_END
#define	SEEK_END 2
#endif
#ifndef	S_IREAD
#define	S_IREAD	0400
#endif
#ifndef	S_IWRITE
#define	S_IWRITE 0200
#endif
#ifndef	S_IRGRP
#define	S_IRGRP 040
#endif
#ifndef	S_IWGRP
#define	S_IWGRP 020
#endif

static	char	*buffer = "This buffer contains 34 characters";
static	char	inbuf[256];
static	char	path[512];

static	char	*gbg = "GARBAGE";	/*Something to stuff inbuf with*/

#define	HOLE_SIZE	50			/*Make a 50 character hole*/
#define	MODE	(S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP)

main(argc,argv)
int	argc;
char	*argv[];
{
	int		fd,bsiz;
	int		fsiz,i;
	char	*p,*g;

	setup("read");
	funct("read");

	for(bsiz = 0; buffer[bsiz]; ++bsiz);
	fsiz = bsiz + HOLE_SIZE;
	abstract("Test whether read fills with '\\0' when it encounters");
	abstract("a hole in a file.  In this test we create a file and");
	abstract("make a hole in it.  We then read the file back from");
	abstract("the beginning, expecting to find both the hole filled");
	abstract("with '\\0' and the data we wrote after the hole.");
	start
		prep("Create a temporary file.");
		start
			strcpy(path,temp_path());
			strcat(path,"read_hole");
			fd = creat(path,MODE);
		finis
		prep("Make a hole in the file.");
		start
			expecting((long)HOLE_SIZE);
			lseek(fd,(long)HOLE_SIZE,SEEK_END);
			expecting(bsiz);
			write(fd,buffer,bsiz);
		finis
		prep("Close and reopen the file for reading.");
		start
			close(fd);
			open(path,O_RDONLY);
		finis
		testing("Read back the data we just wrote, including the hole");
		testing("and make sure the hole was filled with '\\0'");
		start
			trash(inbuf,sizeof(inbuf));
			expecting(fsiz);
			read(fd,inbuf,fsiz);
			for(i = 0; i < HOLE_SIZE; ++i){
				if(inbuf[i]){
					error("read() failed to fill hole with '\\0'");
					break;
				}
			}
		finis
		testing("Make sure the data following the hole is also correct.");
		start
			p = buffer;
			for(i = HOLE_SIZE; i < fsiz; ++i){
				if(*(p++) != inbuf[i]){
					error("Incorrect data read after the hole.");
					break;
				}
			}
		finis
	finis
	done();
/*NOTREACHED*/
}

int	trash(buf,len)
char	*buf;
int		len;
{
	char	*g;

	g = gbg;
	while((len--) > 0){
		if(!*g){
			g = gbg;
		}
		*(buf++) = *(g++);
	}
}
