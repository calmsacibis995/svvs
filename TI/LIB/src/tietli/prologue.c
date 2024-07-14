# include "main.h"
# include "option.h"
# include <exec_head.h>
# include <fcntl.h>

SCCSID("@(#)prologue.c	3.2 (SVID) 12/23/86");

void	set_exec_head();
void	set_option();
void	reset_option();
void	process_args();
void	init_tables();
void	load_funct();
void	hash_funct_entry();
void	shw_funct();
void	usage();
void	to_stderr();
void	init_alloc();
void	pull_file_name();

void prologue(argc, argv)
int	argc;
char	*argv[];
{
	set_exec_head();
	set_option();
	process_args(argc, argv);
	reset_option();
	
	if (!hcreate(HASH_TABLE_LEN))
	{
		screech("prologue", 0, "hcreate() failed\n");
	}

	init_tables();
	load_funct();
	load_var();
	scan_init();
}

void set_exec_head()
{
	ExecHead.do_initscr = DEFAULT_DO_INITSCR;
	ExecHead.do_endwin  = DEFAULT_DO_ENDWIN;
	ExecHead.do_xon_xoff  = DEFAULT_DO_XON_XOFF;
	ExecHead.do_check = DEFAULT_DO_CHECK;
	ExecHead.do_protocal = DEFAULT_DO_PROTOCAL;

	ExecHead.timeout_mod  = DEFAULT_TIMEOUT_MOD;
	ExecHead.timeout_type  = DEFAULT_TIMEOUT_TYPE;

	ExecHead.file_name[0] = '\0';
}

void set_option()
{
	Option.src_file = 	NULL;
	Option.obj_file = 	NULL;
	Option.show_struct = 	DEFAULT_SHOW_STRUCT;
	Option.show_var = 	DEFAULT_SHOW_VAR;
	Option.size_file =	NULL;
	Option.save_prgm =	DEFAULT_SAVE_PRGM;
}

void reset_option()
{
	unsigned	length;
	char		*tail;

	if (!Option.obj_file)
	{
		if (tail = strrchr(Option.src_file, '/'))
		{
			tail++;
		}
		else
		{
			tail = Option.src_file;
		}

		if (!*tail)
		{
			screech("reset_option", 0,
				"Bad file name %s\n", Option.src_file);
		}

		length = strlen(tail) + 3;
		if ((Option.obj_file = malloc(length)) == NULL)
		{
			screech("reset_option", 0, "malloc failed\n");
		}

		(void) strcpy(Option.obj_file, tail);
		(void) strcat(Option.obj_file, ".t");
	}
}

void process_args(argc, argv)
int		argc;
char		*argv[];
{
	char	*s;

	while (--argc > 0 && (*++argv)[0] == '-')
	{
		s = argv[0]+1;
		if (*s == '\0')
		{
			usage();
			screech(NULL, 0, NULL);
		}

		for (; *s != '\0'; s++)
		{
			switch (*s)
			{
			  case '?':
				usage();
				screech(NULL, 0, NULL);
				break;

			  case 'i':
				break;

			  case 'o':
				if (--argc > 0)
				{
					Option.obj_file = *++argv;
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'x':
				if (--argc > 0)
				{
					++argv;
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'd':
				break;

			  case 'O':
				if (--argc > 0)
				{
					++argv;
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'h':
				break;

			  case 's':
			  case 'M':
			  case 'T':
			  case 't':
			  case 'p':
			  case 'c':
				if (--argc > 0)
				{
					++argv;
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'P':
			  case 'r':
			  case 'g':
			  case 'n':
			  case 'C':
				break;

			  case 'l':
				Option.show_struct = TRUE;
				break;

			  case 'S':
				Option.show_var = TRUE;
				break;

			  case 'f':
				shw_funct();
				screech(NULL, 0, NULL);
				break;

			  case 'F':
			  case 'B':
			  case 'I':
				if (--argc > 0)
				{
					++argv;
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'L':
				Option.size_file = DEFAULT_SIZE_FILE;
				break;

			  case 'D':
				break;

			  default:
				(void) fprintf(stderr, 
					       "tietl: illegal option %c\n", 
					       *s);
				argc = -1;
				break;
			}
		}
	}

	switch (argc)
	{
	  case 1:
		Option.src_file = argv[0];

		if (close(0) != NULL)
		{
			screech("process_args", 1, "close()");
		}

		if (open(Option.src_file, O_RDONLY) != 0)
		{
			screech("process_args", 1, 
				"opening %s\n", Option.src_file);
		}

		pull_file_name(ExecHead.file_name, Option.src_file);

		break;

	  default:
		usage();
		screech(NULL, 0, NULL);
	}
}

void to_stderr(str)
char	*str;
{
	(void) fprintf(stderr, str);
}

void usage()
{
	to_stderr("Usage: tietl [options] file\n");
	to_stderr("Option Arg      Default    Description\n");
	to_stderr("\n");
	to_stderr("    -o file     *.t        t-code is output to file\n");
	to_stderr("    -l          FALSE      show lines in program\n");
	to_stderr("    -S          FALSE      show symbol table\n");
	to_stderr("    -f          FALSE      show legal TIE functions\n");
	to_stderr("    -L          TRUE       report program length\n");
}

void init_tables()
{
	Prgm.HeadStat.elem_size = sizeof(HEAD_ENTRY);
	Prgm.HeadStat.next_free = 0;
	Prgm.HeadStat.cur_size  = 0;
	Prgm.HeadStat.grow_size = HEAD_GROW_SIZE;
	Prgm.HeadStat.restart   = 0;

	Prgm.ExprStat.elem_size = sizeof(EXPR_ENTRY);
	Prgm.ExprStat.next_free = 0;
	Prgm.ExprStat.cur_size  = 0;
	Prgm.ExprStat.grow_size = EXPR_GROW_SIZE;
	Prgm.ExprStat.restart   = 0;

	Prgm.VarStat.elem_size = sizeof(VAR_ENTRY);
	Prgm.VarStat.next_free = 0;
	Prgm.VarStat.cur_size  = 0;
	Prgm.VarStat.grow_size = VAR_GROW_SIZE;
	Prgm.VarStat.restart   = 0;

	Prgm.CallStat.elem_size = sizeof(CALL_ENTRY);
	Prgm.CallStat.next_free = 0;
	Prgm.CallStat.cur_size  = 0;
	Prgm.CallStat.grow_size = CALL_GROW_SIZE;
	Prgm.CallStat.restart   = 0;

	Prgm.NameStat.elem_size = sizeof(NAME_ENTRY);
	Prgm.NameStat.next_free = 0;
	Prgm.NameStat.cur_size  = 0;
	Prgm.NameStat.grow_size = NAME_GROW_SIZE;
	Prgm.NameStat.restart   = 0;

	Prgm.StmtStat.elem_size = sizeof(STMT_ENTRY);
	Prgm.StmtStat.next_free = 0;
	Prgm.StmtStat.cur_size  = 0;
	Prgm.StmtStat.grow_size = STMT_GROW_SIZE;
	Prgm.StmtStat.restart   = 0;

	Prgm.TextStat.elem_size = sizeof(TEXT_ENTRY);
	Prgm.TextStat.next_free = 0;
	Prgm.TextStat.cur_size  = 0;
	Prgm.TextStat.grow_size = TEXT_GROW_SIZE;
	Prgm.TextStat.restart   = 0;

	init_alloc((GENERIC *) &Prgm.HeadTbl, 
		   &Prgm.HeadStat, 
		   HEAD_INIT_SIZE);
	init_alloc((GENERIC *) &Prgm.ExprTbl, 
		   &Prgm.ExprStat, 
		   EXPR_INIT_SIZE);
	init_alloc((GENERIC *) &Prgm.VarTbl,  
		   &Prgm.VarStat,  
		   VAR_INIT_SIZE);
	init_alloc((GENERIC *) &Prgm.CallTbl, 
		   &Prgm.CallStat, 
		   CALL_INIT_SIZE);
	init_alloc((GENERIC *) &Prgm.NameTbl, 
		   &Prgm.NameStat, 
		   NAME_INIT_SIZE);
	init_alloc((GENERIC *) &Prgm.StmtTbl, 
		   &Prgm.StmtStat, 
		   STMT_INIT_SIZE);
	init_alloc((GENERIC *) &Prgm.TextTbl, 
		   &Prgm.TextStat, 
		   TEXT_INIT_SIZE);
}

void init_alloc(tbl_data, tbl_stat, need)
GENERIC		*tbl_data;
TBL_STAT_TYPE	*tbl_stat;
int		need;
{
	unsigned	new_size;

	tbl_stat->cur_size = need;
	new_size = tbl_stat->cur_size * tbl_stat->elem_size;
	if ((tbl_data->char_ptr = malloc(new_size)) == NULL)
	{
		screech("init_alloc", 0, "malloc failed\n");
	}
}

void load_funct()
{
	TBL_INDEX	i;

	for(i = 0; Funct[i].funct_name != NULL; i++)
	{
		hash_funct_entry(i);
	}
}

void hash_funct_entry(funct_index)
TBL_INDEX	funct_index;
{
	NAME_ENTRY	*name_ptr;
	unsigned	name_len;

	FUNCT_ENTRY	*funct_ptr;

	ENTRY		item;
	ENTRY		*find_ptr;

	funct_ptr = &Funct[funct_index];

	name_len = strlen(funct_ptr->funct_name) + 2;

	if ((name_ptr = malloc(name_len)) == NULL)
	{
		screech("hash_funct_name", 0, "malloc failed\n");
	}

	(void) strcpy(name_ptr, "F");
	(void) strcat(name_ptr, funct_ptr->funct_name);

	item.key = name_ptr;

	find_ptr = hsearch(item, FIND);

	if (!find_ptr)
	{
		item.data = (char *) funct_index;

		if (hsearch(item, ENTER) == NULL)
		{
			screech("hash_funct_entry", 0, 
				"Lost new entry = %s\n", item.key);
		}
	}
	else
	{
		screech("load_funct", 0, 
			"Duplicate function name '%s'\n",
			funct_ptr->funct_name);
	}
}

void shw_funct()
{
	FUNCT_ENTRY	*funct_ptr;

	for (funct_ptr = &Funct[0]; funct_ptr->funct_name != NULL; funct_ptr++)
	{
		(void) printf("%s\n", funct_ptr->funct_name);
	}
}

void pull_file_name(dest_str, src_str)
char	*dest_str;
char	*src_str;
{
	char	*tmp_str;

	if ((tmp_str = strrchr(src_str, '/')) == NULL)
	{
		strcpy(dest_str, src_str);
	}
	else
	{
		if (++tmp_str == '\0')
		{
			dest_str = '\0';
		}
		else
		{
			strcpy(dest_str, tmp_str);
		}
	}
}
