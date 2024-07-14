#include	<sv_base.h>
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#ident	"@(#)mallopt.c	1.5"

# ifndef SYSVR_1
char my_string[] = "Preservation after a call to free allocated memory.";
char heap[1024*16];
char *sv_malloc( size )
unsigned size;
{
	static char *next = heap;
	char *ret = next;
	next += size;
	return( ret );
}
#endif	

main() {
# ifndef SYSVR_1
	int 	i;
	char	*ptr;

	setup("malloc");
	funct("mallopt");


	abstract("Verify mallopt with the M_MXFAST command.");
	start
		testing("Call mallopt(M_MXFAST,i) for values between 0 and 1000.");
		start
			break_on( ERROR );
			for (i=0; i<1000; i += 100)
				mallopt(M_MXFAST, i);
		finis
		cleanup("Call mallopt(M_MXFAST,512)");
		start
			mallopt(M_MXFAST, 512);
		finis
	finis

	abstract("Verify mallopt with the M_NLBLKS command.");
	start
		testing("Call mallopt(M_NLBLKS,i) for values between 2 and 128.");
		start
			break_on( ERROR );
			for (i=2; i<128; i++)
				mallopt(M_NLBLKS, i);
		finis
		cleanup("Call mallopt(M_NLBLKS,10).");
		start
			mallopt(M_NLBLKS, 10);
		finis
	finis

	abstract("Verify mallopt with the M_GRAIN command.");
	start
		testing("Call mallopt(M_GRAIN,i) for values between 1 and 64.");
		start
			break_on( ERROR );
			for (i=1; i<64; i += 1)
				mallopt(M_GRAIN, i);
		finis
		cleanup("Call mallopt(M_GRAIN,16).");
		start
			mallopt(M_GRAIN, 16);
		finis
	finis

	testing("Call mallopt(M_KEEP,0).");
	start
		mallopt(M_KEEP, 0);
	finis

	abstract("Verify mallopt with illegal sizes and commands.");
	start
		testing("Call mallopt(M_MXFAST,i) for various illegal values.");
		testing("Verify failure.");
		start
			break_on( ERROR );
			expecting(FAILURE);
			for ( i = -1; i > -1000; i -= 100 )
				mallopt(M_MXFAST, i);
		finis

		testing("Call mallopt(M_NLBLKS,i) for various illegal values.");
		testing("Verify failure.");
		start
			break_on( ERROR );
			expecting ( FAILURE );
			for (i=0; i > -1000; i -= 100)
				!mallopt(M_NLBLKS, i);
		finis

		testing("Call mallopt(M_GRAIN,i) for various illegal values.");
		testing("Verify failure.");
		start
			break_on( ERROR );
			expecting ( FAILURE );
			for (i=0; i > -1000; i -= 100)
				mallopt(M_GRAIN, i);
		finis
	
		testing("Call mallopt with illegal commands.");
		testing("Verify failure.");
		start
			break_on( ERROR );
			for (i=0; i < 10; i++ )
			{
				switch (i)
				{
				case M_MXFAST:
				case M_NLBLKS:
				case M_GRAIN:
				case M_KEEP:
					continue;
				default:
					expecting ( FAILURE );
					mallopt(i, 0);
					break;
				}
				break;
			}
		finis
	finis

	abstract("Verify failure of mallopt after an allocation has");
	abstract("taken place.");
	start
		prep("Allocate a buffer.");
		start
			expecting ( SUCCESS );
			exit_on( ERROR );
			ptr=malloc(strlen(my_string)+1);
		finis
		testing("Call mallopt(M_MXFAST,17) after an allocation has");
		testing("taken place.  Verify failure.");
		start
			expecting ( FAILURE );
			mallopt(M_MXFAST, 17);
		finis
		testing("Call mallopt(M_NLBLKS,17) after an allocation has");
		testing("taken place.  Verify failure.");
		start
			expecting ( FAILURE );
			mallopt(M_NLBLKS, 17);
		finis
		testing("Call mallopt(M_GRAIN,17) after an allocation has");
		testing("taken place.  Verify failure.");
		start
			expecting ( FAILURE );
			mallopt(M_GRAIN, 17);
		finis
		testing("Call mallopt(M_KEEP,17) after an allocation has");
		testing("taken place.  Verify failure.");
		start
			expecting ( FAILURE );
			mallopt(M_KEEP, 17);
		finis
	finis

	testing("Verify the M_KEEP command preserves freed storage.");
	testing("Expecting matching memory contents, and no address errors.");
	start
		strcpy(ptr, my_string);
		free(ptr);
		expecting ( VOID );
		if( strcmp(ptr, my_string) ) {
			*(ptr+strlen(my_string)) = '\0'; /* just in case */
			warning("Contents of memory was altered after call to free.\nThe expected string was '%s'.\nThe actual string found was '%s'.\n", my_string, ptr);
		}
	finis
	done();
# else

	unsupported("mallopt is an update included in System V Release 2.");
# endif
	}
