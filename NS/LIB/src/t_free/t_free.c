/*
 *	t_free.c
 *
 *	a test of t_free(NS_LIB)
 *
 *	This test may adversely affect other processes
 *	using malloc space in a paging system with a
 *	"small" swap device.
 *	
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)t_free.c	1.2"

#define max(a,b)	((a>b)?a:b)

#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

extern int t_errno;

main() {
/*
 *	This test attempts to verify that the space freed 
 *	with t_free is available for further re-allocation.
 *
 *	T_alloc(NS_LIB) and t_free(NSLIB) are a pair
 *	of complementary functions.  The t_alloc test 
 *	allocates all possible types of structures 
 *	and with all possible combinations of fields.
 *	The t_alloc test will fail if the structures 
 *	cannot be freed with t_free.
 *
 *	Note that some sanity checking on the transport provider is 
 *	done, in case some of the fields buffers are not supported.
 *	Problems of this sort will generate warnings.
 *	
 */
	struct	t_info	info;
	struct	t_info	*ifptr = &info;
	int				tfp;				/* xport provider file des	*/
	long			hi_h20;				/* cnt of successful allocs */
	long			old_hi_h20;			/* cnt of successful allocs */
	char			*aptr;				/* return val from t_alloc	*/
	int				fields;				/* fields to alloc			*/
	char			fname[32];			/* field name				*/
	int				type;				/* type of struct to alloc	*/
	char			tname[32];			/* struct type name			*/
	FILE			*tmp;				/* temp file pointer		*/
	long			l;					/* junk variable			*/

	char			*t_alloc();
	FILE			*temp_fopen();

	setup("t_free");
	funct("t_free");

 	abstract("Call t_alloc repeatedly until no more space can");
	abstract("be allocated.  The amount of space allocated is");
	abstract("considered a high water mark.  Call t_free repeatedly");
	abstract("to free the allocated space, and then call t_alloc again");
	abstract("in an attempt to allocate back to the high water mark.");
	abstract("If the same amount of space cannot be allocated,");
	abstract("consider the amount of space allocated the second");
	abstract("time as a new high water mark.  This procedure is");
	abstract("repeated until the high water mark is reached or");
	abstract("surpassed, or until the high water mark becomes 0.");
	abstract("A high water mark of 0 indicates that t_free does");
	abstract("not free all of allocated space for further allocation.");
	abstract("If t_free is broken in this fashion, a system error is");
	abstract("likely to occur before the high water mark reaches 0.");
	start
		prep("open the transport provider: \"%s\"", TPVCFILE0);
		start
			tfp = t_open( TPVCFILE0, O_RDWR, ifptr );
		finis
		prep("Check the struct info returned from t_open to see which");
		prep("of the addr, opt, and udata fields can be allocated.  If");
		prep("none of these fields can be allocated, generate a warning.");
		start
			expecting( VOID );	/* turn off ret val checking for strcpy */
			type = T_CALL;
			strcpy( tname, "T_CALL" );
			fields = 0;
			fname[0] = '\0';
			if( info.addr > 0 ) {
				strcat( fname, "T_ADDR" );
				fields |= T_ADDR;
			}
			if( info.options > 0 ) {
				if( fields ) strcat( fname, " | ");
				strcat( fname, "T_OPT" );
				fields |= T_OPT;
			}
			if( info.connect > 0 && info.discon > 0 ) {
				if( fields ) strcat( fname, " | ");
				strcat( fname, "T_UDATA" );
				fields |= T_UDATA;
			}
			else if (info.tsdu > 1 ){/*try t_unitdata if can't use t_call*/
				if( fields ) strcat( fname, " | ");
				strcat( fname, "T_UDATA" );
				type = T_UNITDATA;
				strcpy( tname, "T_UNITDATA" );
				fields |= T_UDATA;
			}
			if (!fields)
				warning("%s%s%s%s%s",
				  "tranport provider: '",
				  TPVCFILE0,
				  "' does not support fields\n",
				  "This test should be run again with ",
				  "a different provider");
		finis
		prep("Try to allocate a large amount of memory with malloc");
		prep("in an attempt to expedite a t_alloc failure.  Assume");
		prep("that if t_alloc does not use malloc space that a failure");
		prep("can be easily generated.");
		start
			unsigned lim;
			unsigned find_limit();

			expecting( VOID );
			lim = find_limit();
			while( malloc( lim - 1000 ) );
		finis
		abstract("Call t_alloc until memory is exhausted.  Set the ");
		abstract("high water mark to the number of structures allocated.");
		abstract("Save the pointers in a temp file.  Read the temp file");
		abstract("and free the allocated memory.");
		start
			prep("Open a temp file to save the pointers returned");
			prep("by t_alloc.");
			start
				exit_on( ERROR );
				tmp = temp_fopen("t_free.tmp","w");
			finis
			prep("Call t_alloc( fd, %s, %s ) until failure.",tname, fname);
			prep("Save the pointers in a temp file.  Check that the");
			prep("failure is due to exhausted memory.  This may take");
			prep("a significant amount of time to execute.");
			start
				expecting( VOID );
				exit_on( ERROR );
				hi_h20 = 0l;
				while( aptr = t_alloc( tfp, type, fields ) ) {
					hi_h20++;
					expecting( 1 );
					fwrite( &aptr, sizeof( char * ), 1, tmp );
					expecting( VOID );
				}
				exit_on( WARNING );
				if ( t_errno != TSYSERR )
					warning(
					  "t_alloc failed to exhaust memory, test terminated");
			finis
			prep("Close the temp file, and re-open for reading");
			start
				exit_on( ERROR );
				fclose( tmp );
				tmp = temp_fopen("t_free.tmp","r");
			finis
			testing("Read the pointers from the temp file, and free the");
			testing("allocated space.");
			start
				exit_on( ERROR );
				for( l = 0l; l < hi_h20; l++ ) {
					expecting( 1 );
					fread( &aptr, sizeof( char * ), 1, tmp );
					t_free( aptr, type );
				}
			finis
			cleanup("Close the temp file.");
			start
				fclose( tmp );
			finis
		finis
		abstract("Verify that the space which was previously allocated");
		abstract("and then freed, is again available for allocation.");
		abstract("Call t_alloc until the previously allocated amount");
		abstract("of space has been re-allocated.  Save the pointers");
		abstract("returned by t_alloc in a temp file so that the");
		abstract("space may be freed.  Free the space.  If the high ");
		abstract("water mark is not reached, set the high water mark");
		abstract("to the number of structures allocated on this attempt,");
		abstract("and repeat this step.  If the high water mark reaches");
		abstract("0, generate an error");
		start
			break_on( ERROR );
			old_hi_h20 = hi_h20 + 1;
			while( hi_h20 < old_hi_h20 ) {
				old_hi_h20 = hi_h20;
				prep("Open a temp file to save the pointers returned");
				prep("by t_alloc.");
				start
					exit_on( ERROR );
					tmp = temp_fopen("t_free.tmp","w");
				finis
				testing("Check that the high water mark is non-zero");
				start
					if( !old_hi_h20 )
						error("%s%s",
						  "t_free does not free space ",
						  "for further allocation");
				finis
				testing("Attempt calling t_alloc( fd, %s, %s ) %ld times.",
				  tname,fname,hi_h20);
				testing("Save the pointers returned by t_alloc in a temp");
				testing("file for later use.  This may a significant ");
				testing("amount of time to execute.");
				start
					break_on( ERROR );
					for( hi_h20=0l ; hi_h20 < old_hi_h20; hi_h20++ ) {
						expecting( VOID );
						if( !(aptr = t_alloc( tfp, type, fields )) )
							break;
						expecting( 1 );
						fwrite( &aptr, sizeof( char * ), 1, tmp );
					}
					if( !aptr ) {
						exit_on( WARNING );
						if ( t_errno != TSYSERR )
						warning("%s%s",
						  "t_alloc failed to exhaust memory, ",
						  "test terminated");
					}
				finis
				prep("Close the temp file, and re-open for reading");
				start
					exit_on( ERROR );
					fclose( tmp );
					tmp = temp_fopen("t_free.tmp","r");
				finis
				testing("Read the pointers from the temp file, and free the");
				testing("allocated space.");
				start
					exit_on( ERROR );
					for( l = 0l; l < hi_h20 ; l++ ) {
						expecting( 1 );
						fread( &aptr, sizeof( char * ), 1, tmp );
						t_free( aptr, type );
					}
				finis
				cleanup("Close the temp file.");
				start
					fclose( tmp );
				finis
			}
		finis
	finis
	done();
}
