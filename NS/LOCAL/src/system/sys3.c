#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)sys3.c	1.2"

main()
{
	struct stat st_stdin;
	struct stat st_devnull;

	if (stat("/dev/null",  &st_devnull))
	{
		severr("cannot stat /dev/null");
	}

	expecting ( VOID );
	if (fstat(fileno(stdin),  &st_stdin))
	{
		severr("cannot stat stdin");
	}

	compare_stat( &st_devnull,  &st_stdin );
}

compare_stat( st1,  st2 )
struct stat *st1;
struct stat *st2;
{
	if (st2->st_mode != st1->st_mode )
	{
error("File modes do not match.\nThe expected value was %d.\nThe actual value was %d.\n", (int)st1->st_mode, (int)st2->st_mode);	/*  ERROR */
	}

	if (st2->st_ino != st1->st_ino )
	{
error("File inodes do not match.\nThe expected value was %d.\nThe actual value was %d.\n", (int)st1->st_ino, (int)st2->st_ino);	/*  ERROR */
	}

	if (st2->st_dev != st1->st_dev )
	{
error("Directory device id's do not match.\nThe expected value was %d.\nThe actual value was %d.\n", (int)st1->st_dev, (int)st2->st_dev);	/*  ERROR */
	}

	if (st2->st_nlink != st1->st_nlink )
	{
error("Link reference counts do not match.\nThe expected value was %d.\nThe actual value was %d.\n", (int)st1->st_nlink, (int)st2->st_nlink);	/*  ERROR */
	}

	if (st2->st_uid != st1->st_uid )
	{
error("File user id's do not match.\nThe expected value was %d.\nThe actual value was %d.\n", (int)st1->st_uid, (int)st2->st_uid);	/*  ERROR */
	}

	if (st2->st_gid != st1->st_gid )
	{
error("File group id's do not match.\nThe expected value was %d.\nThe actual value was %d.\n", (int)st1->st_gid, (int)st2->st_gid);	/*  ERROR */
	}

	if (st2->st_size != st1->st_size )
	{
error("File sizes do not match.\nThe expected value was %l.\nThe actual value was %l.\n", st1->st_size, st2->st_size);	/*  ERROR */
	}
}
