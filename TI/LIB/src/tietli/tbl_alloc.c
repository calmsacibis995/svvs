# include "main.h"
# include "option.h"
# include <fcntl.h>

SCCSID("@(#)tbl_alloc.c	3.1 (SVID) 12/1/86");

TBL_INDEX tbl_alloc(tbl_data, tbl_stat, need)
GENERIC		*tbl_data;
TBL_STAT_TYPE	*tbl_stat;
int		need;
{
	GENERIC		hold_ptr;
	unsigned	new_size;
	TBL_INDEX	result;

	result = tbl_stat->next_free;
	tbl_stat->next_free += need;

	while (tbl_stat->next_free > tbl_stat->cur_size)
	{
		if (tbl_stat->cur_size == 0) 
		{
			tbl_stat->cur_size = tbl_stat->grow_size;
			new_size = tbl_stat->cur_size * tbl_stat->elem_size;
			hold_ptr.char_ptr = malloc(new_size);
			if (hold_ptr.char_ptr == NULL)
			{
				screech("tbl_alloc", 0, "malloc failed\n");
			}
		}
		else
		{
			tbl_stat->cur_size += tbl_stat->grow_size;
			new_size = tbl_stat->cur_size * tbl_stat->elem_size;
			hold_ptr.char_ptr = realloc(*tbl_data, new_size);
			if (hold_ptr.char_ptr == NULL)
			{
				screech("tbl_alloc", 0, "realloc failed\n");
			}
		}
		*tbl_data = hold_ptr;
	}

	return(result);
}
