# include "main.h"
# include "wvs.h"

SCCSID("@(#)vs.c	3.2 (SVID) 12/23/86");

void vs_setup(file_name)
char	*file_name;
{
	char	*tail;
	extern	char	*getenv();

	if (file_name == NULL)
	{
		if ( (file_name = getenv("SVVS_TEST")) == NULL )
			file_name = "tietl";
	}
	else if ((tail = strrchr(file_name, '/')) != NULL)
	{
		file_name = tail++;
	}

	wvs_setup(file_name);
}

void vs_done()
{
	wvs_done();
}

void vs_expecting()
{
	wvs_expecting();
}

void vs_no_expecting()
{
	wvs_no_expecting();
}

void vs_abstract(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		wvs_abstract(tmp_str);
		free(tmp_str);
	}
}

void vs_testing(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		wvs_testing(tmp_str);
		free(tmp_str);
	}
}

void vs_prep(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		wvs_prep(tmp_str);
		free(tmp_str);
	}
}

void vs_cleanup(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		wvs_cleanup(tmp_str);
		free(tmp_str);
	}
}

void vs_error(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		wvs_error(tmp_str);
		free(tmp_str);
	}
}

void vs_warning(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		wvs_warning(tmp_str);
		free(tmp_str);
	}
}

FILE *vs_t_fopen(sub_path, type)
char	*sub_path;
char	*type;
{
	return(wvs_t_fopen(sub_path, type));
}

FILE *vs_d_fopen(sub_path, type)
char	*sub_path;
char	*type;
{
	return(wvs_d_fopen(sub_path, type));
}
