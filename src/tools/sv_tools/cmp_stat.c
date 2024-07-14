/*LINTLIBRARY*/
#include	<sv_base.h>

#include	<sys/types.h>
#include	<sys/stat.h>

#include	<sv_env.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)cmp_stat.c	1.6"
#endif

void cmp_stat( st1, st2, trunc_flag )
struct stat *st1;
struct stat *st2;
int trunc_flag;
{

	continue_on ( ERROR | WARNING );

	if ( st2->st_mode != st1->st_mode )
		error("%s\n%s%d\n%s%d",
			"file modes do not match",
			"expected value:", (int)st1->st_mode,
			"actual value:", (int)st2->st_mode );

	if ( st2->st_ino != st1->st_ino )
		error("%s\n%s%d\n%s%d",
			"file inodes do not match",
			"expected value:", (int)st1->st_ino,
			"actual value:", (int)st2->st_ino );

	if (st2->st_dev != st1->st_dev )
		error("%s\n%s%d\n%s%d",
			"directory device id's do not match",
			"expected value:", (int)st1->st_dev,
			"actual value:", (int)st2->st_dev );

	if (st2->st_nlink != st1->st_nlink )
		error("%s\n%s%d\n%s%d",
			"link reference counts do not match",
			"expected value:", (int)st1->st_nlink,
			"actual value:", (int)st2->st_nlink );

	if (st2->st_uid != st1->st_uid )
		error("%s\n%s%d\n%s%d",
			"file user id's do not match",
			"expected value:", (int)st1->st_uid,
			"actual value:", (int)st2->st_uid );

	if (st2->st_gid != st1->st_gid )
		error("%s\n%s%d\n%s%d",
			"file group id's do not match",
			"expected value:", (int)st1->st_gid,
			"actual value:", (int)st2->st_gid );

	if ( trunc_flag == TRUNC ) {
		if (st2->st_size != 0l )
			error("%s\n%s%ld\n%s%ld",
				"file size is non-zero",
				"expected value:", 0l,
				"actual value:", (long)st2->st_size );
	}
	else {
		if (st2->st_size != st1->st_size )
			error("%s\n%s%ld\n%s%ld",
				"file sizes do not match",
				"expected value:", (long)st1->st_size,
				"actual value:", (long)st2->st_size );
	}
	if ( trunc_flag == ACCESS ) {
		if (st2->st_atime < st1->st_atime )
			warning("%s\n%s%ld\n%s%ld",
				"file access time was not updated",
				"expected value greater than or equal to:",
				(long)st1->st_atime,
				"actual value:", (long)st2->st_atime );
	}
	else {
		if (st2->st_atime != st1->st_atime )
			warning("%s\n%s%ld\n%s%ld",
				"file access times do not match",
				"expected value:", (long)st1->st_atime,
				"actual value:", (long)st2->st_atime );
	}

	if ( trunc_flag == TRUNC ) {
		if (st2->st_mtime < st1->st_mtime )
			warning("%s\n%s%ld\n%s%ld",
				"file modification time was not updated",
				"expected value greater than or equal to:",
				(long)st1->st_mtime,
				"actual value:", (long)st2->st_mtime );

		if (st2->st_ctime < st1->st_ctime )
			warning("%s\n%s%ld\n%s%ld",
				"file status time was not updated",
				"expected value greater than or equal to:",
				(long)st1->st_ctime,
				"actual value:", (long)st2->st_ctime );
	}
	else {
		if (st2->st_mtime != st1->st_mtime )
			warning("%s\n%s%ld\n%s%ld",
				"file modification times do not match",
				"expected value:", (long)st1->st_mtime,
				"actual value:", (long)st2->st_mtime );

		if (st2->st_ctime != st1->st_ctime )
			warning("%s\n%s%ld\n%s%ld",
				"file status times do not match",
				"expected value:",(long)st1->st_ctime,
				"actual value:", (long)st2->st_ctime );
	}
}
