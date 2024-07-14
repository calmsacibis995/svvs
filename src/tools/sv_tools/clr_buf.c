/*LINTLIBRARY*/
#ifdef	UNDEF
#ident	"@(#)clr_buf.c	1.2"
#endif
void clr_buf(buf, cnt)
char *buf;
int cnt;
{
	int		i;

	for(i=0; i<cnt; i++) *buf++ = '\0';
}
