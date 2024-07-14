#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)sys2.c	1.1"

# define isdiggy(z)	(z>='0'&&z<='9')

main(argc,argv)
int argc;
char *argv[];
{
	int i;

	if (argc > 1)
	{
		for(i=1;i<argc;i++)
		{
			if (!isdiggy(*argv[i]) || *(argv[i]+1))
			{
				severr("bad argument to sys2");
			}
			doit(*argv[i]-'0');
		}
	}
	done();
}
doit(fd)
int fd;
{
	struct stat st_buf;

	expecting ( EBADF );
	fstat(fd, &st_buf);
}