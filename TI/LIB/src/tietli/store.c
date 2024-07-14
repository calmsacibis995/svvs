# include "main.h"
# include "option.h"
# include <ctype.h>

SCCSID("@(#)store.c	3.1 (SVID) 12/1/86");

char	*new_text();
void	esc_filter();

TBL_INDEX sav_var(var_name)
char	*var_name;
{
	NAME_ENTRY	*name_ptr;
	unsigned	name_len;

	VAR_ENTRY	*var_ptr;
	TBL_INDEX	var_index;

	ENTRY		*hash_entry;
	ENTRY		item;

	TBL_INDEX	result;

	name_len = strlen(var_name) + 2;

	if ((name_ptr = malloc(name_len)) == NULL)
	{
		screech("sav_var", 0, "malloc failed\n");
	}

	(void) strcpy(name_ptr, "V");
	(void) strcat(name_ptr, var_name);

	item.key = name_ptr;

	if ((hash_entry = hsearch(item, FIND)) == NULL )
	{
		var_index = tbl_alloc((GENERIC *) &Prgm.VarTbl, 
				      &Prgm.VarStat, 
				      1);
		var_ptr = &Prgm.VarTbl[var_index];

		var_ptr->var_class = CLASS_VAR; 
		var_ptr->var_type = DATA_NUM; 
		var_ptr->var_name.name_ptr = name_ptr; 
		var_ptr->var_data.var_num = 0;

		item.data = (char *) var_index;

		if ((hash_entry = hsearch(item, ENTER)) == NULL)
		{
			screech("sav_var", 0, 
				"Lost new entry = %s\n", item.key);
		}
	}
	result = (TBL_INDEX) hash_entry->data;

	return(result);
}

TBL_INDEX sav_const(const_val, var_type)
char		*const_val;
DATA_TYPE	var_type;
{
	TBL_INDEX	var_index;
	VAR_ENTRY	*var_ptr;

	var_index = tbl_alloc((GENERIC *) &Prgm.VarTbl, &Prgm.VarStat, 1);
	var_ptr = &Prgm.VarTbl[var_index];

	switch (var_type)
	{
	  case DATA_NUM:
		(void) sscanf(const_val, "%d", &var_ptr->var_data.var_num);
		break;

	  case DATA_BOOL:
		var_ptr->var_data.var_bool = (strcmp(const_val, "TRUE") == 0);
		break;

	  case DATA_CHAR:
		esc_filter(const_val);
		var_ptr->var_data.var_chtype = const_val[1];
		break;

	  case DATA_TEXT:
		esc_filter(const_val);
		var_ptr->var_data.var_text_ptr = new_text(const_val);
		break;

	  default:
		(void) fprintf(stderr, "sav_const: Bad type '%c'\n", var_type);
		Option.save_prgm = FALSE;
	}

	var_ptr->var_class = CLASS_CONST;
	var_ptr->var_type = var_type;
	var_ptr->var_name.name_ptr = NULL;

	return(var_index);
}

TBL_INDEX sav_call(call_name)
char	*call_name;
{
	int		name_len;

	TBL_INDEX	call_index;
	CALL_ENTRY	*call_ptr;
	TBL_INDEX	funct_index;

	ENTRY		*hash_entry;
	ENTRY		item;

	name_len = strlen(call_name) + 2;
	if ((item.key = malloc((unsigned) name_len)) == NULL)
	{
		screech("sav_call", 0, "malloc failed\n");
	}
	(void) strcpy(item.key, "F");
	(void) strcat(item.key, call_name);

	if ((hash_entry = hsearch(item, FIND)) == NULL )
	{
		funct_index = NIL;
		(void) fprintf(stderr, "%d: Unknown call '%s'\n", LineNumber, call_name);
		Option.save_prgm = FALSE;
	}
	else
	{
		funct_index = (TBL_INDEX) hash_entry->data;
	}

	call_index = tbl_alloc((GENERIC *) &Prgm.CallTbl, &Prgm.CallStat, 1);
	call_ptr = &Prgm.CallTbl[call_index];

	call_ptr->call_funct_index = funct_index; 
	call_ptr->call_expr = NIL;
	call_ptr->call_expr_count = 0;

	free(item.key);

	return(call_index);
}

TBL_INDEX sav_array(var_name)
char	*var_name;
{
	NAME_ENTRY	*name_ptr;
	int		name_len;

	VAR_ENTRY	*var_ptr;
	TBL_INDEX	var_index;

	ENTRY		*hash_entry;
	ENTRY		item;

	TBL_INDEX	result;

	name_len = strlen(var_name) + 2;
	if ((name_ptr = malloc((unsigned) name_len)) == NULL)
	{
		screech("sav_array", 0, "malloc failed\n");
	}
	(void) strcpy(name_ptr, "A");
	(void) strcat(name_ptr, var_name);

	item.key = name_ptr;

	if ((hash_entry = hsearch(item, FIND)) == NULL )
	{
		var_index = tbl_alloc((GENERIC *) &Prgm.VarTbl, 
				      &Prgm.VarStat, 
				      1);
		var_ptr = &Prgm.VarTbl[var_index];

		var_ptr->var_class = CLASS_VAR; 
		var_ptr->var_type = DATA_ARRAY; 
		var_ptr->var_name.name_ptr = name_ptr; 
		var_ptr->var_data.var_array.array_var = NULL;
		var_ptr->var_data.var_array.array_size = 0;

		item.data = (char *) var_index;

		if ((hash_entry = hsearch(item, ENTER)) == NULL)
		{
			screech("sav_array", 0, 
				"Lost new entry = %s\n", item.key);
		}
	}
	result = (TBL_INDEX) hash_entry->data;

	return(result);
}

char *new_text(text_val)
char	*text_val;
{
	char		*text_ptr;
	unsigned	text_len;

	text_len = strlen(text_val) - 1;
	text_val[text_len] = '\0';

	if ((text_ptr = malloc(text_len)) == NULL)
	{
		screech("new_text", 0, "malloc failed\n");
	}

	(void) strcpy(text_ptr, &text_val[1]);

	return(text_ptr);
}

void esc_filter(text_val)
char	*text_val;
{
	char	*seek_ptr;
	char	*hold_ptr;
	int	count;

	seek_ptr = text_val;
	hold_ptr = text_val;

	while (*seek_ptr != '\0')
	{
		if (*seek_ptr != '\\')
		{
			*hold_ptr++ = *seek_ptr++;
			continue;
		}

		switch (*++seek_ptr)
		{
		  case 'n':
			*hold_ptr++ = '\n';
			seek_ptr += 1;
			break;

		  case 't':
			*hold_ptr++ = '\t';
			seek_ptr += 1;
			break;

		  case 'b':
			*hold_ptr++ = '\b';
			seek_ptr += 1;
			break;

		  case 'r':
			*hold_ptr++ = '\r';
			seek_ptr += 1;
			break;

		  case 'f':
			*hold_ptr++ = '\f';
			seek_ptr += 1;
			break;

		  case '"':
			*hold_ptr++ = '"';
			seek_ptr += 1;
			break;

		  case '\'':
			*hold_ptr++ = '\'';
			seek_ptr += 1;
			break;

		  case '\\':
			*hold_ptr++ = '\\';
			seek_ptr += 1;
			break;

		  case '\0':
			break;

		  default:
			if (!isdigit(*seek_ptr))
			{
				*hold_ptr++ = *seek_ptr++;
				break;
			}

			*hold_ptr = '\0';
			count = 0;
			while (isdigit(*seek_ptr) && (count < 3))
			{
				count++;
				*hold_ptr *= 8;
				*hold_ptr += *seek_ptr++ - '0';
			}
			hold_ptr++;
		}
	}
	*hold_ptr = '\0';
}
