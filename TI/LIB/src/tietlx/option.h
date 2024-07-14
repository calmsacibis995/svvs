/*
** SCCSID("@(#)option.h	3.2 (SVID) 12/23/86");
*/

typedef struct
{
	char*		obj_file;
	BOOLEAN		use_stdin;

	BOOLEAN		show_struct;
	BOOLEAN		show_var;

	char		*term_type;
	char		*pitch_dev_name;
	char		*catch_dev_name;
	BOOLEAN		use_pipe;
	char		*pipe_name;

	BOOLEAN		hard;
	unsigned	MIN;
	unsigned	TIME;

	unsigned short	baud_rate;

	BOOLEAN		block_dump;

	BOOLEAN		run_bless;
	BOOLEAN		run_mimic;
	BOOLEAN		run_umpire;

	BOOLEAN		direct;
	char*		flush_flag;

	BOOLEAN		output_bless;
	BOOLEAN		output_mimic;
	BOOLEAN		output_umpire;

	BOOLEAN		respond;

	BOOLEAN		bell;
	BOOLEAN		noisy;
	BOOLEAN		core_dump;

	char		*size_file;

	BOOLEAN		dump_error;
} OPTION_TYPE;

extern OPTION_TYPE	Option;
