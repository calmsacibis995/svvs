/*
** SCCSID("@(#)option.h	3.1 (SVID) 12/1/86");
*/

/*
** Default values
*/

# define DEFAULT_SHOW_STRUCT	FALSE
# define DEFAULT_SHOW_VAR	FALSE
# define DEFAULT_SIZE_FILE	"/usr2/mcfong/Size/size"
# define DEFAULT_SAVE_PRGM	TRUE

/*
** Options
*/

typedef struct
{
	char		*src_file;
	char		*obj_file;
	BOOLEAN		show_struct;
	BOOLEAN		show_var;
	char		*size_file;
	BOOLEAN		save_prgm;
} OPTION_TYPE;

extern OPTION_TYPE	Option;
