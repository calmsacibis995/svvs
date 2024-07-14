# include	<bless.h>
# include	<common.h>
# include	<tie.h>

SCCSID("@(#)main.c	3.1 (SVID) 12/1/86");

/*
** main
**	Wrapper function for testing the bless functionality.
**	This is used only for debugging purposes, not for 
**	real testing.
*/
main(argc,argv)
int	argc;
char	**argv;
{
	int	x,y;
	int	ret_val;
	TERMINAL	*term;
	char	*termtype;
	char	*sp;
	char	*string;
	long	array[20];
	extern	char	*optarg;
	extern	int	optind;
	extern	ARGS	*apply();
	extern	char	*getenv();
	extern	int	putenv();
	extern	TRACE_MAP	Bless_map[];

	termtype = "SVVS3";
	clr_trace(Bless_trace);

	if ( argc > 1 )
	{
		while ( (ret_val = getopt(argc,argv,"d:t:o:")) != EOF )
		{
			switch ( ret_val )
			{
			  case 'd':
				set_trace(Bless_trace,optarg,Bless_map);
				setbuf(stdout,NULL);
				break;

			  case 't':
				termtype = optarg;
				break;

			  case 'o':
				if ( freopen(optarg,"w",stdout) == NULL )
				{
					perror(optarg);
					exit(1);
				}
				break;

			  case '?':
				fprintf(stderr,"bless [-d values] [-t termtype] [-o outfile] [filename]\n");
				exit(1);
			}
		}
		if ( optind < argc )
			init_data(argv[optind]);

	}

	string = "TERM=";
	strcat(string,termtype);
	putenv(string);

	term = init_terminfo(termtype);

	reset_shell_mode();

	if ( term == (TERMINAL *) NULL )
	{
		print("init_terminfo returned error.\n");
		exit(1);
	}

	sp = (char *) NULL;
	sp = get_data(sp);
	while ( *sp )
	{
		bless(term,sp,0);
		sp = get_data((char *) NULL);
	}
	dump_term(term,NULL);
	exit(0);
	/*NOTREACHED*/
}
scom(a,b)
COMM	*a, *b;
{
	return ( strcmp(a->comm_name,b->comm_name) );
}
