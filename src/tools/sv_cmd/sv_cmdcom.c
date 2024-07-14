#ifdef UNDEF
#ident	"@(#)sv_cmdcom.c	1.4"
#endif

/*LINTLIBRARY*/
#include <fcntl.h>

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#ifndef SEEK_SET
#define	SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define	SEEK_CUR 1
#endif
#ifndef SEEK_END
#define	SEEK_END 2
#endif


#define SECTION -2	/*Used by setsect()*/
#define	TEST	-1	/*Used by setsect()*/
#define	RBUFSZ	512	/*Size of a read buffer*/

extern	long	lseek();
long	svseek();

struct	rdbuf {
	long	bufpos;
	int	pos;
	int	len;
	char	buf[RBUFSZ];
};

static	struct	rdbuf	rbuffs[] = {
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
	{(long)0,RBUFSZ,0,""},
};
#define	MAXFD	((sizeof(rbuffs) -1 )/ sizeof(struct rdbuf))

extern	void	exit();

/*
** Our own printf-like routine.
*/
/*VARARGS1*/
outf(fmt,a,b,c,d,e)
char	*fmt,*a,*b,*c,*d,*e;
{
	char	buff[2048];
	unsigned	len;

	len = (unsigned)sprintf(buff,fmt,a,b,c,d,e);

	if(write(1,buff,len) != len){
		foutf(2,"Write failed to stdout.\n");
		exit(0);
	}
}

/*
** Our own fprintf-like routine using file descriptors instead of
** file pointers.
*/
/*VARARGS2*/
foutf(fd,fmt,a,b,c,d,e)
int		fd;
char	*fmt,*a,*b,*c,*d,*e;
{
	char	buff[2048];
	unsigned	len;
	len = (unsigned)sprintf(buff,fmt,a,b,c,d,e);

	if(write(fd,buff,len) != len){
		foutf(2,"Write failed to file.\n");
		exit(0);
	}
}

int	fgetln(fd,str)
int	fd;
char	*str;
{
	int	rval;
	char	*p;

	p = str;
	while((rval = bytread(fd,str)) && (*str != '\n'))
		++str;
	*str = 0;
	return(rval || !(p == str));
}

int	shell;	/*This flag will be set when a shell escape is taken*/

int	getln(str)
char	*str;
{
	int	rval;
	char	*p,*q;

	shell = 0;
	q = p = str;
	while((rval = read(0,str,1)) && (*str != '\n'))
		++str;
	*str = 0;
	while(*q && ((*q == ' ') || (*q == '	')))
		q++;
	if(*q == '!'){
		shell = 1;
		doshell(++q);
		*p = 0;
	}
	return((rval || (p != str)) ? 1 : 0);
}

static	bytread(fd,ptr)
int	fd;
char	*ptr;
{
	if(fd < 0 ||  fd > MAXFD){
		foutf(2,"Invalid file descriptor for input file\n");
		return(0);
	}
	if(rbuffs[fd].pos < rbuffs[fd].len){
		*ptr = rbuffs[fd].buf[rbuffs[fd].pos++];
	}
	else{
		rbuffs[fd].bufpos += rbuffs[fd].len;
		if(!(rbuffs[fd].len = read(fd,&rbuffs[fd].buf[0],RBUFSZ))){
			return(0);
		}
		rbuffs[fd].pos = 0;
		*ptr = rbuffs[fd].buf[rbuffs[fd].pos++];
	}
	return(1);
}

long	svseek(fd,offset,whence)
int	fd;	/*File descriptor*/
long	offset;	/*Offset in file*/
int	whence;	/*Offset from where?*/
{
	if((fd < 0) || (fd > MAXFD)){
		return((long)-1);
	}
	if(whence == SEEK_CUR){
		offset += ((long)rbuffs[fd].pos + rbuffs[fd].bufpos);
		whence = SEEK_SET;
	}
	rbuffs[fd].len = 0;
	rbuffs[fd].pos = RBUFSZ;
	rbuffs[fd].bufpos = lseek(fd,offset,whence);
	return(rbuffs[fd].bufpos);
}

rstcache(fd)
int	fd;
{
	rbuffs[fd].bufpos = (long)0;
	rbuffs[fd].pos = RBUFSZ;
	rbuffs[fd].len = 0;
	*rbuffs[fd].buf = 0;
}

doshell(str)
char	*str;
{
	int	oldsout;

	oldsout = dup(1);
	close(1);
	if(dup(2) != 1){
		foutf(2,"Dup failed, standard output was lost!\n");
		foutf(2,"Do not use shell escape until dup is fixed!\n");
		exit(-1);
	}
	(void)system(str);
	close(1);
	if(dup(oldsout) != 1){
		foutf(2,"Dup failed, standard output was lost!\n");
		foutf(2,"Do not use shell escape until dup is fixed!\n");
		exit(-1);
	}
	close(oldsout);
}

/*
** our own isspace() like function, returns true for space tab and newline
** false for all others.
*/

int	whitesp(c)
char	c;
{
	return((c == ' ') || (c == '	') || (c == '\n'));
}

/*
** nxtok() gets the next token (delimited by white space) from src
** and places it in dst. It then returns a pointer to the next token
** after the one it got.  If no more tokens, it returns a pointer to
** the end of src.
*/

char	*nxtok(dst,src)
char	*src,*dst;
{

	while(*src && (whitesp(*(src)))) ++src;
	if(*src != '\"')
		while(*src && (!whitesp(*(dst) = *(src++)))) ++dst;
	else{
		++src;
		while(*src &&( (*dst = *(src++)) != '\"')) ++dst;
	}
	while(*src && (whitesp(*(src)))) ++src;
	*dst = 0;
	return(src);
}

/*
** If line has any nonwhite charaters, nonblank() returns true, otherwise
** false.
*/

int	nonblank(str)
char	*str;
{
	while(*str){
		if(!whitesp(*str))
			return(1);
		++str;
	}
	return(0);
}

/*
** Routine to read run numbers from BegJourn and BegRun lines and
** return their integer values
*/

int	runno(str)
char	*str;
{
	char	token[512];
	int	ret;

	(void)nxtok(token,str);
	if(!sscanf(token,"%d",&ret)){
		return(-1);
	}
	return(ret);
}

/*
** Function to reset the setsect function for a new section name.
*/

static	int		havesect = 0;

rstsect()
{
	havesect = 0;
}

/*
** Function to read the section file and return the mappings for a section
** name one at a time until the section is exhausted.  Returns the mapping
** in nxt.  Receives the section name in name.  If the section name does
** not map to anything, the item is assumed to be a testname, and TEST is
** returned, otherwise, SECTION is returned.
*/

setsect (nxt,name,svvs)
char	*nxt;
char	*name;
char	*svvs;		/*SVVSPATH, so we don't need to do a sv_config()*/
{
	static	char	*p,line[512],token[64];
	static	int	sectfile = -1;

	if(!havesect){
		(void)sprintf(line,"%s/data/sections",svvs);
		if(sectfile < 0){
			if((sectfile = open(line,O_RDONLY))< 0){
				foutf(2,"Could not open section list file.\n");
				exit(-1);
			}
		}
		else{
			svseek(sectfile,(long)0,SEEK_SET);
		}
		while(fgetln(sectfile,line)){
			p = &line[0];
			p = nxtok(token,p);
			if(!strcmp(token,name)){
				p = nxtok(nxt,p);
				havesect = 1;
				return(SECTION);
			}
		}
		return(TEST);
	}
	else {
		p = nxtok(nxt,p);
		return(SECTION);
	}
}
