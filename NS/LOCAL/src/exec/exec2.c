#ident	"@(#)exec2.c	1.4"

/*
 * exec2() - used to test ETXTBSY and invalid path,
 *	under normal conditions, will never be executed.
 */
main() {
	exit( 1 );
	/*NOTREACHED*/
}
