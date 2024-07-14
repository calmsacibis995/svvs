#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)link0.c	1.5"

# define	BLOCK_SPEC	060777
# define	PROT		0222
# define	READ_ONLY	1

/*
** link
**	Test the link function.
**
*/

main()
{
	char	path1[PATH_MAX];
	char	path2[PATH_MAX];
	int	lncnt;
	int	link();

	setup("link");
	funct("link");

	exit_on ( ERROR );

	testing("A legal invocation of link.");
	testing("Expect a return value of zero.");
	start

		expecting ( SUCCESS );
		temp_creat("link_1", PROT);

		strcpy(path1, temp_path());
		strcpy(path2, temp_path());
		strcat(path1, "link_1");
		strcat(path2, "link_2");

		expecting ( SUCCESS );
		link(path1, path2);

	finis

	testing("Linking a file (path1) which does not exist.");
	testing("Expect a return value of -1 and errno of ENOENT");
	start

		strcpy(path1, temp_path());
		strcat(path1, "_link");

		expecting ( ENOENT );
		link(path1, path2);

	finis

	testing("Linking a file -- search permission along the path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path1, data_path());
		strcat(path1, "noperm.dir");
		strcat(path1, "/link_tryme");

		expecting ( EACCES );
		link(path1, path2);

	finis

	testing("Linking a file -- write permission along the path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		expecting ( SUCCESS );
		temp_creat("link_2", PROT);


		strcpy(path1, temp_path());
		strcat(path1, "link_2");
		strcpy(path2, data_path());
		strcat(path2, "noperm1.dir");
		strcat(path2, "/link_tryme");

		expecting ( EACCES );
		link(path1, path2);

	finis

	testing("Link -- argument has a component that is not a directory.");
	testing("Expect a return value of -1 and errno of ENOTDIR");
	start

		expecting ( SUCCESS );
		temp_creat("linkbad_dir", PROT);

		strcpy(path1, temp_path());
		strcat(path1, "link");
		strcat(path1, "bad_dir");
		strcat(path1, "/dummy_point");

		expecting ( ENOTDIR );
		link(path1, path2);

		expecting ( SUCCESS );
		strcpy(path1, temp_path());
		strcat(path1, "link");
		strcat(path1, "bad_dir");

		expecting ( SUCCESS );
		unlink(path1);


	finis

	testing("To fail link a directory while not superuser.");
	testing("Expect a return value of -1 and errno of EPERM.");
	start

		strcpy(path1, temp_path());
		path1[strlen(path1)-1] = 0;
		strcpy(path2, temp_path());
		strcat(path2, "suid");

		expecting ( EPERM );
		link(path1, path2);

	finis

	testing("Double a link to path2.");
	testing("Expect a return value of -1 and errno of EEXIST.");
	start

		expecting ( SUCCESS );
		temp_creat("link_3", PROT);

		strcpy(path1, temp_path());
		strcpy(path2, temp_path());
		strcat(path1, "link_3");
		strcat(path2, "link_4");

		expecting ( SUCCESS );
		link(path1, path2);

		expecting ( EEXIST );
		link(path1, path2);

	finis

	testing("To exceed the maximum number of links a file is allowed.");
	testing("Expect a return value of -1 and errno of EMLINK.");
	start

		expecting ( SUCCESS );
		temp_creat("link_target", PROT);

		strcpy(path1, temp_path());
		strcat(path1, "link_target");

		for(lncnt = 1; lncnt < LINK_MAX; lncnt++){
			expecting ( SUCCESS );
			sprintf(path2, "%s%d", temp_path(), lncnt);

			expecting ( VOID );
			(void) unlink(path2); /* make driver's life easier */

			expecting ( SUCCESS );
			link(path1, path2);

		}

		lncnt++;

		expecting ( SUCCESS );
		sprintf(path2, "%s%d", temp_path(), lncnt);

		expecting ( EMLINK );
		link(path1, path2);

/*
**		remove links from temp directory,
**		rm command may not be able to handle
**		LINK_MAX arguments.
**/
		for(lncnt = 1; lncnt < LINK_MAX; lncnt++){
			expecting ( SUCCESS );
			sprintf(path2, "%s%d", temp_path(), lncnt);
			(void) unlink(path2); /* make driver's life easier */
		}
		finis
	done();
/*NOTREACHED*/
}
