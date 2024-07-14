#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>

static	int	mode_map;

#ifndef	SYSVR_2

int	omodes[] = {
	S_IXOTH,
	S_IWOTH,
	S_IROTH,
	S_IXGRP,
	S_IWGRP,
	S_IRGRP,
	S_IEXEC,
	S_IWRITE,
	S_IREAD,
	S_ISVTX,
	S_ISGID,
	S_ISUID,
	S_IFIFO,
	S_IFCHR,
	S_IFDIR,
	S_IFREG,
};

#define	NMODES	( sizeof( omodes ) / sizeof( int ) )

#endif

sv_mode(in_mode)
int in_mode;
{
	int	out_mode;
	int	i;

#ifndef	SYSVR_2
	if (mode_map){

		out_mode = 0;
	
		for(i=0; i < NMODES; i++)
			if ( in_mode & (1<<i) )
				out_mode |= omodes[i];
	
		return (out_mode);
	}
#endif
	return (in_mode);
}
sv_mp_mode(in_mode)
int in_mode;
{
	mode_map = in_mode;
}
