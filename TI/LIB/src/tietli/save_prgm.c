# include "main.h"
# include "option.h"
# include <show.h>
# include <exec_head.h>
# include <fcntl.h>

SCCSID("@(#)save_prgm.c	3.1 (SVID) 12/1/86");

void ptr_to_index();
void exec_head_save();
void stat_save();
void tbl_save();

void save_prgm()
{
	int	fd;

	if (strcmp(Option.obj_file, "-") == 0)
	{
		fd = 1;
	}
	else
	{
		if ((fd = creat(Option.obj_file, 0644)) == -1)
		{
			screech("save_prgm", 1, 
				"failed to open %s\n", Option.obj_file);
		}
	}

	if (Option.show_struct)
	{
		(void) fprintf(stderr, "STRUCTURE:\n");
		shw_stmt();
	}

	if (Option.size_file)
	{
		shw_size(Option.size_file);
	}

	ptr_to_index();

	exec_head_save(fd);

	/*
	stat_save(fd, &Prgm.HeadStat,	"HeadStat");
	*/
	stat_save(fd, &Prgm.ExprStat,	"ExprStat");
	stat_save(fd, &Prgm.VarStat,	"VarStat");
	stat_save(fd, &Prgm.CallStat,	"CallStat");
	stat_save(fd, &Prgm.NameStat,	"NameStat");
	stat_save(fd, &Prgm.StmtStat,	"StmtStat");
	stat_save(fd, &Prgm.TextStat,	"TextStat");

	/*
	tbl_save(fd, (GENERIC *) &Prgm.HeadTbl, Prgm.HeadStat, "Head");
	*/
	tbl_save(fd, (GENERIC *) &Prgm.ExprTbl, Prgm.ExprStat, "Expr");
	tbl_save(fd, (GENERIC *) &Prgm.VarTbl,  Prgm.VarStat,  "Var");
	tbl_save(fd, (GENERIC *) &Prgm.CallTbl, Prgm.CallStat, "Call");
	tbl_save(fd, (GENERIC *) &Prgm.NameTbl, Prgm.NameStat, "Name");
	tbl_save(fd, (GENERIC *) &Prgm.StmtTbl, Prgm.StmtStat, "Stmt");
	tbl_save(fd, (GENERIC *) &Prgm.TextTbl, Prgm.TextStat, "Text");

	if (close(fd))
	{
		screech("save_prgm", 1, 
			"close of %s failed\n", Option.obj_file);
	}
}

void exec_head_save(fd)
int	fd;
{
	unsigned	data_size;

	data_size = sizeof(EXEC_HEAD_TYPE);

	if (write(fd, (char *) &ExecHead, data_size) != data_size)
	{
		screech("exec_head_save", 1, "write ExecHead failed\n");
	}
}

void tbl_save(fd, tbl_data, stat_data, tbl_name)
int		fd;
GENERIC		*tbl_data;
TBL_STAT_TYPE	stat_data;
char		*tbl_name;
{
	unsigned	num_byte;

	if ((num_byte = stat_data.next_free * stat_data.elem_size) > 0)
	{
		if (write(fd, tbl_data->char_ptr, num_byte) != num_byte)
		{
			screech("save_prgm", 1, 
				"write %sTbl[] failed\n", tbl_name);
		}
	}
}

void stat_save(fd, stat_data, stat_name)
int		fd;
TBL_STAT_TYPE	*stat_data;
char		*stat_name;
{
	unsigned	data_size;

	data_size = sizeof(TBL_STAT_TYPE);

	if (write(fd, (char *) stat_data, data_size) != data_size)
	{
		screech("stat_save", 1, "write %s failed\n", stat_name);
	}
}

void ptr_to_index()
{
	VAR_ENTRY	*guide;
	VAR_ENTRY	*end_list;
	char		*tmp_ptr;
	int		text_len;
	int		text_index;
	int		name_len;
	int		name_index;

	end_list = &Prgm.VarTbl[Prgm.VarStat.next_free];

	for (guide = &Prgm.VarTbl[0]; guide != end_list; guide++)
	{
		if (guide->var_name.name_ptr == NULL)
		{
			guide->var_name.name_index = NIL;
		}
		else
		{
			name_len = strlen(guide->var_name.name_ptr) + 1;
			name_index = tbl_alloc((GENERIC *) &Prgm.NameTbl, 
					       &Prgm.NameStat, 
					       name_len);

			tmp_ptr = &Prgm.NameTbl[name_index];
			(void) strcpy(tmp_ptr, guide->var_name.name_ptr);
			free(guide->var_name.name_ptr);
			guide->var_name.name_index = name_index;
		}

		if ((guide->var_class == CLASS_CONST) && 
		    (guide->var_type == DATA_TEXT))
		{
			text_len = strlen(guide->var_data.var_text_ptr) + 1;
			
			text_index = tbl_alloc((GENERIC *) &Prgm.TextTbl, 
					       &Prgm.TextStat, 
					       text_len);

			tmp_ptr = &Prgm.TextTbl[text_index];
			(void) strcpy(tmp_ptr, guide->var_data.var_text_ptr);
			free(guide->var_data.var_text_ptr);
			guide->var_data.var_text_index = text_index;
		}
	}
}
