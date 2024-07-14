# include "main.h"
# include "option.h"
# include <exec_head.h>
# include <fcntl.h>
extern int	unlink();

SCCSID("@(#)read_prgm.c	3.1 (SVID) 12/1/86");

void index_to_ptr();
void read_prgm();
void read_tbl();
void read_stat();
void read_exec_head();

void read_prgm()
{
	int	fd;
	char	junk[1];

	if (Option.obj_file)
	{
		if ((fd = open(Option.obj_file, O_RDONLY)) == -1)
		{
			screech("read_prgm", 1, 
				"failed to open %s\n", Option.obj_file);
		}
	}
	else
	{
		fd = 0;
	}

	read_exec_head(fd);

	/*
	read_stat(fd, &Prgm.HeadStat, "HeadStat");
	*/
	read_stat(fd, &Prgm.ExprStat, "ExprStat");
	read_stat(fd, &Prgm.VarStat,  "VarStat");
	read_stat(fd, &Prgm.CallStat, "CallStat");
	read_stat(fd, &Prgm.NameStat, "NameStat");
	read_stat(fd, &Prgm.StmtStat, "StmtStat");
	read_stat(fd, &Prgm.TextStat, "TextStat");

	/*
	read_tbl(fd, (GENERIC *) &Prgm.HeadTbl, Prgm.HeadStat, "Head");
	*/
	read_tbl(fd, (GENERIC *) &Prgm.ExprTbl, Prgm.ExprStat, "Expr");
	read_tbl(fd, (GENERIC *) &Prgm.VarTbl,  Prgm.VarStat,  "Var");
	read_tbl(fd, (GENERIC *) &Prgm.CallTbl, Prgm.CallStat, "Call");
	read_tbl(fd, (GENERIC *) &Prgm.NameTbl, Prgm.NameStat, "Name");
	read_tbl(fd, (GENERIC *) &Prgm.StmtTbl, Prgm.StmtStat, "Stmt");
	read_tbl(fd, (GENERIC *) &Prgm.TextTbl, Prgm.TextStat, "Text");

	if (read(fd, junk, (unsigned) 1) > 0)
	{
		screech("read_prgm", 0, "left over input\n");
	}

	index_to_ptr();

	if (Option.show_struct)
	{
		(void) fprintf(stderr, "STRUCTURE:\n");
		shw_stmt();
	}

	if (Option.size_file)
	{
		shw_size();
	}

	switch (ExecHead.timeout_type)
	{
	  case TIMEOUT_MOD:
		Option.TIME += ExecHead.timeout_mod;
		break;

	  case TIMEOUT_SET:
		Option.TIME = ExecHead.timeout_mod;
		break;

	  case TIMEOUT_NULL:
		/* Do Nothing */
		break;

	  default:
		screech("no_device_delay", 0, 
			"Bad timeout_type = %d\n", ExecHead.timeout_type);
	}

	if ((Option.use_pipe) && (Option.TIME))
	{
		Option.TIME = (Option.TIME / 10) + 1;
	}
}

void read_exec_head(fd)
int	fd;
{
	unsigned	data_size;

	data_size = sizeof(EXEC_HEAD_TYPE);

	if (read(fd, (char *) &ExecHead, data_size) != data_size)
	{
		screech("exec_head_save", 1, "read ExecHead failed\n");
	}
}

void read_stat(fd, stat_data, stat_name)
int		fd;
TBL_STAT_TYPE	*stat_data;
char		*stat_name;
{
	unsigned	stat_size;

	stat_size = sizeof(TBL_STAT_TYPE);

	if (read(fd, (char *) stat_data, stat_size) != stat_size)
	{
		screech("read_stat", 1, "read %s failed\n", stat_name);
	}
}

# define	READ_SIZE	0

void read_tbl(fd, tbl_data, stat_data, tbl_name)
int		fd;
GENERIC		*tbl_data;
TBL_STAT_TYPE	stat_data;
char		*tbl_name;
{
	unsigned	num_byte;
	unsigned	request;
	int		load_count;
	int		read_count;
	char		*load_ptr;

	num_byte = stat_data.next_free * stat_data.elem_size;

	if (num_byte == 0)
	{
		tbl_data->char_ptr = NULL;
	}
	else if (num_byte < 0)
	{
		screech("prgm_restore", 0, "%sTbl num_byte = %d\n", 
			tbl_name, num_byte);
	}
	else
	{
		if ((tbl_data->char_ptr = malloc(num_byte)) == NULL)
		{
			screech("prgm_restore", 0, 
				"malloc %sTbl failed\n", tbl_name);
		}

		load_ptr = tbl_data->char_ptr;
		load_count = 0;

		while (load_count < num_byte)
		{
			request = num_byte - load_count;

			if ((read_count = read(fd, load_ptr, request)) <= 0)
			{
				screech("read_prgm", 1, 
					"read %sTbl[] failed. Count = %d Should = %d\n",
					tbl_name, read_count, request);
			}

			load_ptr += read_count;
			load_count += read_count;
		}

		if (load_count > num_byte)
		{
			screech("read_prgm", 0, 
				"read too much of %sTbl[]\n", tbl_name);
		}
	}
}

void index_to_ptr()
{
	VAR_ENTRY	*guide;
	VAR_ENTRY	*end_list;
	char		*tmp_ptr;

	end_list = &Prgm.VarTbl[Prgm.VarStat.next_free];

	for (guide = &Prgm.VarTbl[0]; guide != end_list; guide++)
	{
		if (guide->var_name.name_index == NIL)
		{
			guide->var_name.name_ptr = NULL;
		}
		else
		{
			tmp_ptr = &Prgm.NameTbl[guide->var_name.name_index];
			guide->var_name.name_ptr = tmp_ptr;
		}

		if ((guide->var_class == CLASS_CONST) && 
		    (guide->var_type == DATA_TEXT))
		{
			tmp_ptr = &Prgm.TextTbl[guide->var_data.var_text_index];
			guide->var_data.var_text_ptr = tmp_ptr;
		}
	}
}
