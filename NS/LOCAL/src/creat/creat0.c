
#include	<sv_base.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)creat0.c	1.7"

#ifndef SYSVR_2
# define 	WORLD	S_IXOTH | S_IWOTH | S_IROTH | S_IXGRP | S_IWGRP | S_IRGRP | S_IEXEC | S_IWRITE | S_IREAD | S_ISVTX | S_ISGID | S_ISUID
# else
# define	WORLD	07777
# endif

# define	PROT	0777

/*
** creat
**	Test the creat function.
**
*/

main()
{
	char	path[PATH_MAX];

	int	fildes;
	int	mode;
	int	i;
	struct	stat buf;
	int	creat();
	int	fstat();
	int	stat();
	int	unlink();
	int	close();

	setup("creat");

	funct("creat");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

/*
**	Set umask explicitly to zero since driver may not do so....
**	Call unlink on the fly in order not to leave around lots and lots
**	of empty files....
**	close must be called so that we don't run out of file descriptors
**	either...
*/

	abstract("All possible values of mode.");
	start

		prep("Reset umask");
		start
			expecting( VOID );
			(void) umask(0);
			umask(0);
		finis

		testing("Testing all modes for creat.");
		start
			for ( mode = 00; mode <= 0777; mode++){

				sprintf(path, "%s%d", temp_path(), mode);

				expecting ( SUCCESS );
				fildes = creat(path, mode);

				expecting ( SUCCESS );
				stat(path, &buf);

				if (( buf.st_mode & WORLD ) !=   ( mode & WORLD ) )	/* creat explains all */
					error("creat: stat reports mode in error? mode %d buf.st_mode %d\n", mode, (int) buf.st_mode);

				expecting ( SUCCESS );
				unlink(path);

				expecting ( SUCCESS );
				close(fildes);
			}
		finis

	finis


	prep("Writing to a file that was created with a no write mode.");
	prep("This file is to be used below.");
	start

		expecting(SUCCESS);
		fildes = temp_creat("creat_write", PROT);

		expecting( 1 );
		write(fildes, "a", 1);

		expecting(SUCCESS);
		close(fildes);
	finis


	abstract("Creat for an existing file that has data in it.");
	abstract("Creat should truncate the file to zero.");
	start

		testing("Issue the call to creat.");
		start
			expecting ( SUCCESS );
			creat(path, PROT);
		finis

		testing("Verify the call with a call to fstat.");
		start
			expecting ( SUCCESS );
			fstat(fildes, &buf);

			if ( buf.st_size != (ushort) 0)
				error("creat: fstat reports that file was not truncated. buf.st_size %d\n", (int) buf.st_size);

		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis
	finis


	testing("Creat for a file along a path which does not exist.");
	start

		strcpy(path, temp_path());
		strcat(path, "_creat/_creat");

		expecting( ENOENT );
		creat(path, PROT);
	finis


	testing("Creat -- search permission along the path is denied.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm.dir");
		strcat(path, "/creat_tryme");

		expecting( EACCES );
		creat(path, PROT);
	finis


	testing("Creat for a file that does not exist and the directory does not permit writing.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm1.dir");
		strcat(path, "/creat_tryme");

		expecting( EACCES );
		creat(path, PROT);
	finis


	testing("Creat for a file that exists but write permission is denied.");
	start

		strcpy(path, data_path());
		strcat(path, "nowrite");

		expecting( EACCES );
		creat(path, PROT);
	finis


	abstract("Creat -- argument has a component that is not a directory.");
	start

		prep("Open a temporary file and close it.");
		start
			expecting( SUCCESS );
			fildes = temp_creat("creatbad_dir", PROT);

			expecting ( SUCCESS );
			close(fildes);
		finis

		strcpy(path, temp_path());
		strcat(path, "creat");
		strcat(path, "bad_dir");
		strcat(path, "/dummy_point");

		testing("Issue the call to creat.");
		start
			expecting( ENOTDIR );
			creat(path, PROT);
		finis
	finis


	testing("Creat -- path is an existing directory.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm.dir");

		expecting( EISDIR );
		creat(path, PROT);
	finis


	testing("Creat repetitively to consume OPEN_MAX+1 file descriptors.");
	start

		prep("Consume OPEN_MAX-3 descriptors before we try for the error.");
		start
			for ( i = 0; i != OPEN_MAX-3; i++){	/* stdio system eats first 3 */

				expecting( SUCCESS );
				sprintf(path, "%s%s%d", temp_path(), "m", i);

				expecting ( SUCCESS );
				creat(path, PROT);
			}

		finis

		expecting ( SUCCESS );
		sprintf(path, "%s%s%d", temp_path(), "m", i);

		testing("Issue the call to creat.");
		start
			expecting(EMFILE);
			creat(path, PROT);
		finis

	finis

	done();
/*
 *	NOTREACHED
*/
}
