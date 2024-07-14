/*
**EMACS_MODES: tabstop=4
*/
#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)strdup.c	1.4"

#ifndef	SYSVR_2

char	original[] = "This is the original string which I will duplicate";
char	known[] = "This is the original string which I will duplicate";
char	nullstr[] = "";

char	*strdup();

#define	NREPS	2000

main(argc,argv)
int		argc;
char	*argv[];
{
	char	*duplicate;
	int		i;

	setup("string");
	funct("strdup");
	abstract("There are three test cases in this test.  The first");
	abstract("case attempts to use strdup() to duplicate a string.");
	abstract("The second case passes a null-string (not a null");
	abstract("pointer) to strdup() and expects to get a duplicate");
	abstract("null-string as a return value.  The third case calls");
	abstract("strdup() many times, to ensure that it may be used");
	abstract("more than once. In each of these test cases the");
	abstract("resulting string is tested for correctness (does it");
	abstract("match the original) and independence (is it separate");
	abstract("from the original in memory).  The original is checked");
	abstract("for corruption (did strdup modify the original).");
	start
		testing("duplicate a string and check the results.");
		start
			expecting(VOID);
			duplicate = strdup(original);
			expecting(VOID);
			if(strcmp(duplicate,known)){
				error("Duplicate does not match original");
			}
			expecting(VOID);
			if(strcmp(known,original)){
				exit_on(ERROR);
				error("Original was altered by strdup()");
			}
			while(*duplicate)
				*(duplicate++) = '\0';
			expecting(VOID);
			if(strcmp(known,original)){
				exit_on(ERROR);
				error("Duplicate shares original storage");
			}
		finis
		testing("duplicate a null string and check the results.");
		start
			expecting(VOID);
			duplicate = strdup(nullstr);
			expecting(VOID);
			if(*duplicate){
				error("Duplicate does not match original");
			}
			expecting(VOID);
			if(*nullstr){
				exit_on(ERROR);
				error("Original was altered by strdup()");
			}
			*duplicate = 'a';
			expecting(VOID);
			if(*nullstr){
				exit_on(ERROR);
				error("Duplicate shares original storage");
			}
		finis
		testing("Call strdup() many times without freeing the memory, it");
		testing("must succeed the first time, but may fail after that.");
		start
			i = 0;
			duplicate = strdup(original);
			while(i++ < NREPS && duplicate){
				expecting(VOID);
				if(strcmp(duplicate,known)){
					error("Duplicate does not match original");
				}
				expecting(VOID);
				if(strcmp(known,original)){
					exit_on(ERROR);
					error("Original was altered by strdup()");
				}
				while(*duplicate)
					*(duplicate++) = '\0';

				expecting(VOID);
				if(strcmp(known,original)){
					exit_on(ERROR);
					error("Duplicate shares original storage");
				}
				expecting(VOID);
				duplicate = strdup(original);
		}
		finis
	finis
	done();
}
#else
main(argc,argv)
int		argc;
char	*argv[];
{
	unsupported("strdup not supported in System V Release 2");
}
#endif
