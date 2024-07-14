# include <stdio.h>
# include <malloc.h>
# include <sccs.h>

SCCSID("@(#)tietl.c	3.1 (SVID) 12/1/86");

extern	int	pipe();
extern	int	fork();
extern	int	execvp();

void screech(msg)
char	*msg;
{
	perror("tietl");
	fprintf(stderr, "tietl: %s", msg);
	exit(1);
}

void test_dup(file_des)
int	file_des;
{
	if (dup(file_des) == -1)
	{
		screech("dup failed\n");
	}
}

void test_close(file_des)
int	file_des;
{
	if (close(file_des) == -1)
	{
		screech("close failed\n");
	}
}

char **prep_arg(new_arg, argv, argc)
char	*new_arg;
char	**argv;
int	argc;
{
	char	**result;
	int	i;

	result = (char **) malloc((unsigned) ((argc + 2) * sizeof(char **)));
	if (!result)
	{
		screech("malloc failed\n");
	}
	result[0] = argv[0];
	result[1] = new_arg;
	for (i = 1; i <= argc; i++)
	{
		result[i + 1] = argv[i];
	}

	return(result);
}

main(argc, argv)
int	argc;
char	**argv;
{
	int	pipe_des[2];
	char	**new_argv;
	int	result;

	if (pipe(pipe_des))
	{
		screech("pipe failed\n");
	}

	switch (fork())
	{
	  case -1:
		screech("fork failed\n");
		break;

	  case 0:
		test_close(1);
		test_dup(pipe_des[1]);
		test_close(pipe_des[0]);
		test_close(pipe_des[1]);

		new_argv = prep_arg("-", argv, argc);
		new_argv = prep_arg("-o", new_argv, argc + 1);
		execvp("tietli", new_argv);
		screech("exec of tietli failed\n");
		break;

	  default:
		switch (fork())
		{
		  case -1:
			screech("fork failed\n");
			break;

		  case 0:
			test_close(0);
			test_dup(pipe_des[0]);
			test_close(pipe_des[0]);
			test_close(pipe_des[1]);

			new_argv = prep_arg("-i", argv, argc);
			execvp("tietlx", new_argv);
			screech("exec of tietlx failed\n");
			break;

		  default:
			test_close(pipe_des[0]);
			test_close(pipe_des[1]);
			while ((result = wait((int *) 0)) != -1)
			{
				/* Do nothing */
			}
			break;
		}
	}
}
