#include	<sv_base.h> /*replacement macros */
#include	<string.h>
#ifndef	SYSVR_2
#include	<sys/types.h>
#endif
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)string.c	1.4"



/*
** string
**	Test the myriad string functions.
*/
main()
{
void	fill_string ();
void	null_string ();


setup("string");


funct ("strcmp");
start
	tstrcmp();
finis


funct ("strncmp");
start
	tstrncmp();
finis


funct ("strlen");
start
	tstrlen();
finis


funct ("strcat");
start
	tstrcat();
finis


funct ("strncat");
start
	tstrncat();
finis


funct ("strcpy");
start
	tstrcpy();
finis


funct ("strncpy");
start
	tstrncpy();
finis


funct ("strchr");
start
	tstrchr();
finis


funct ("strrchr");
start
	tstrrchr();
finis


funct ("strpbrk");
start
	tstrpbrk();
finis


funct ("strspn");
start
	tstrspn();
finis


funct ("strcspn");
start
	tstrcspn();
finis


funct ("strtok");
start
	tstrtok();
finis


done();
/*NOTREACHED*/
}

/*
** tstrcat
**	Test the strcat routine.
*/
tstrcat()
{
	char	s1[STRLEN];
	char	s2[STRLEN];

testing("strcating two strings together should return the start of");
testing ("the first string, and two concatenated strings.");
start
	fill_string (s1, "ab");

	expecting (s1);
	strcat(s1,"CDEF");

	expecting (VOID);
	if ( strcmp(s1,"abCDEF") )
	{
		error ("strcat did not properly concatenate strings\n\
returned '%s', expected 'abCDEF'",
			s1);
		return;
	}
finis


testing("Concatenating a NULL string to a normal string");
testing("should return the first string, and an unaffected string.");
start
	fill_string (s1, "ac");

	expecting (s1);
	strcat(s1,"");

	expecting (VOID);
	if ( strcmp(s1,"ac") )
	{
		error ("strcat changed the string 'ac' to '%s'", s1);
	}
finis


testing("Concatenating a string to a NULL string");
testing("should return the first string, and copy of the second string.");
start
	fill_string (s1, "");
	fill_string (s2, "ad");

	expecting (s1);
	strcat(s1,s2);

	expecting (VOID);
	if ( strcmp(s1,"ad") )
	{
		error ("strcat did not copy the string\n\
returned '%s', expected 'ad'", s1);
	}
finis


testing("Concatenating two NULL strings");
testing("should return the first string, but nothing to change in either.");
start
	fill_string (s1, "");
	fill_string (s2, "");

	expecting (s1);
	strcat(s1,s2);

	if ( s1[0] )
	{
		error ("strcat changed the string from NULL to '%s'", s1);
	}
finis

}

/*
** tstrcmp
**	Test the strcmp routine.
*/
tstrcmp()
{
	int	ret_val;

testing("Comparing two equal strings should return 0.");
start
	expecting (0);
	strcmp("abcdef","abcdef");

finis

testing("Comparing two strings, the first lexicographically");
testing ("less than the second should return less than 0.");
start
	expecting (VOID);
	ret_val = strcmp("abcdef","bcdefg");
	if ( ret_val >= 0 )
	{
		error ("strcmp returned %d", ret_val);
	}
finis


testing("Comparing two strings, the first lexicographically");
testing("greater than the second should return greater than 0.");
start
	expecting (VOID);
	ret_val = strcmp("bcdefg","abcdef");
	if ( ret_val <= 0 )
	{
		error("strcmp returned %d", ret_val);
	}
finis


testing("Comparing two strings, the first a subset");
testing("of the second, and the second longer");
testing("should return less than 0.");
start
	expecting (VOID);
	ret_val = strcmp("abcdefghijk","abcdefghijklmnopqrstuv");
	if ( ret_val >= 0 )
	{
		error("strcmp returned %d", ret_val);
	}
finis


testing("Comparing two strings, the second a subset");
testing("of the first, and the first longer");
testing("should return greater than 0.");
start
	expecting (VOID);
	ret_val = strcmp("abcdefghijklmnopqrstuv","abcdefghijk");
	if ( ret_val <= 0 )
	{
		error("strcmp returned %d", ret_val);
	}
finis

}

/*
** tstrncmp
**	Test the strncmp routine.
*/
tstrncmp()
{
	int	ret_val;

testing("Comparing two equal strings should return 0.");
start
	expecting (0);
	strncmp("abcdef","abcdef",6);
finis


testing("Comparing two strings, the first lexicographically");
testing("less than the second should return less than 0.");
start
	expecting (VOID);
	ret_val = strncmp("abcdef","bcdefg",6);
	if ( ret_val >= 0 )
	{
		error("strncmp returned %d", ret_val);
	}
finis


testing("Comparing two strings, the first lexicographically");
testing("greater than the second should return greater than 0.");
start
	expecting (VOID);
	ret_val = strncmp("bcdefg","abcdef",6);
	if ( ret_val <= 0 )
	{
		error("strncmp returned %d", ret_val);
	}
finis


testing("Comparing two strings, the first a subset");
testing("of the second, and the second longer");
testing("should return less than 0.");
start
	expecting (VOID);
	ret_val = strncmp("abcdefghijk","abcdefghijklmnopqrstuv",22);
	if ( ret_val >= 0 )
	{
		error("strncmp returned %d", ret_val);
	}
finis


testing("Comparing two strings, the second a subset");
testing("of the first, and the first longer");
testing("should return greater than 0.");
start
	expecting (VOID);
	ret_val = strncmp("abcdefghijklmnopqrstuv","abcdefghijk",22);
	if ( ret_val <= 0 )
	{
		error("strncmp returned %d", ret_val);
	}
finis

testing("Compare two unequal strings, but pass a n of 0");
testing("should return 0.");
start
	expecting (0);
	strncmp("abcdef","ghijkl",0);
finis


testing("Compare two unequal strings, but pass a n greater ");
testing("than the length of either");
testing("should return less than 0.");
start
	expecting (VOID);
	ret_val = strncmp("abcdef","ghijkl",20);
	if ( ret_val >= 0 )
	{
		error("strncmp returned %d", ret_val);
	}
finis

testing("Comparing two strings, the first a subset of the second,");
testing("and the second longer, but with n less than the first difference");
testing("should return 0.");
start
	expecting (0);
	strncmp("abcdef","abcdefghij",6);
finis

}

/*
** tstrncat
**	Test the strncat routine.
*/
tstrncat()
{
	char	s1[STRLEN];
	char	s2[STRLEN];

testing("strncating two strings together");
testing("should return the start of the first string,");
testing("and two concatenated strings.");
start
	fill_string (s1, "ab");

	expecting (s1);
	strncat(s1,"CDEF",4);

	expecting (VOID);
	if ( strcmp(s1,"abCDEF") )
	{
		error ("strncat returned '%s', 'abCDEF' was expected", s1);
		return;
	}
finis


testing("Using strncat to concatenate two strings, but passing");
testing("an 'n' less than the second string ");
testing("should return the first string, plus n characters from the second.");
start
	fill_string (s1, "ab");
	fill_string (s2, "CDEFGHI");

	expecting (s1);
	strncat(s1,s2,5);

	expecting (VOID);
	if ( strcmp(s1,"abCDEFG") )
	{
		error ("strncat returned '%s', 'abCDEFG' was expected", s1);
	}
finis



testing("Concatenating a NULL string to a normal string");
testing("should return the first string, and an unaffected string.");
start
	fill_string (s1, "ac");

	expecting (s1);
	strncat(s1,"",1);

	expecting (VOID);
	if ( strcmp(s1,"ac") )
	{
		error ("strncat returned '%s', 'ac' expected");
	}
finis


testing("Using strncat to concatenate two strings,");
testing("'n' exactly equal to the second string");
testing("should return the two strings to be concatenated.");
start
	fill_string (s1, "ab");
	fill_string (s2, "CDEFGHI");

	expecting (s1);
	strncat(s1,s2,7);

	expecting (VOID);
	if ( strcmp(s1,"abCDEFGHI") )
	{
		error ("strncat returned '%s', 'abCDEFFGHI' was expected", s1);
	}
finis


testing("Using strncat to concatenate two strings, but pass a 'n' of 0");
testing("should leave the first string unaffected.");
start
	fill_string (s1, "ab");
	fill_string (s2, "CDEFGHI");

	expecting (s1);
	strncat(s1,s2,0);

	expecting (VOID);
	if ( strcmp(s1,"ab") )
	{
		error ("strncat returned '%s', 'ab' was expected", s1);
	}
finis


testing("Concatenating a string to a NULL string");
testing("should return the first string, and copy of the second string.");
start
	fill_string (s1, "");
	fill_string (s2, "ad");

	expecting (s1);
	strncat(s1,s2,2);

	expecting (VOID);
	if ( strcmp(s1,"ad") )
	{
		error ("strncat returned '%s', 'ad' was expected", s1);
	}
finis


testing("Concatenating two NULL strings");
testing("should return the first string, but nothing to change in either.");
start
	fill_string (s1, "");
	fill_string (s2, "");

	expecting (s1);
	strncat(s1,s2,3);

	if ( s1[0] )
	{
		error ("strncat changed the string from NULL to '%s'", s1);
	}
finis


testing("Use strncat to concatenate two strings,");
testing("'n' larger than either string.");
testing("The two strings should be concatenated together.");
start
	fill_string (s1, "ab");

	expecting (s1);
	strncat(s1,"Walrus",30);

	expecting (VOID);
	if ( strcmp(s1,"abWalrus") )
	{
		error ("strncat returned '%s', 'abWalrus' was expected", s1);
	}
finis

}

/*
** tstrcpy
**	Test the strcpy routines.
*/
tstrcpy()
{
	char	s1[STRLEN];
	char	s2[STRLEN];
	char	*sp;

testing("Using strcpy to copy one string onto another");
testing("should return s1, and s1 == s2.");
start
	fill_string (s1, "a");

	expecting (s1);
	strcpy(s1,"My bonnie lies over...");

	expecting (VOID);
	if ( strcmp(s1,"My bonnie lies over...") )
	{
		error ("strcpy returned '%s', 'My bonnie lies over...' was \
expected", s1);
	}
finis


testing("Using strcpy to copy s2 onto a string of NULLS");
testing("should return s1, and s2 should be in s1.");
start
	null_string (s1);
	sp = "Tis time the Walrus said";

	expecting (s1);
	strcpy(s1,sp);

	expecting (VOID);
	if ( strcmp(s1,sp) )
	{
		error ("strcpy returned '%s'\n\
'Tis time the Walrus said' was expected", s1);
	}
finis


testing("Using strcpy to copy two strings, s2 a NULL string");
testing("should return s1, and s1 should be NULL.");
start
	fill_string (s1, "");
	fill_string (s2, "");

	expecting (s1);
	strcpy(s1,s2);

	expecting (VOID);
	if ( strcmp(s1,s2) )
	{
		error ("strcpy returned '%s', a null string was expected", s1);
	}
finis


testing("Using strcpy to copy a string, the string in s1 longer than s2");
testing("should return s1, and s1 should be identical to s2.");
start
	fill_string (s1, "abcdef");

	sp = "Hi";

	expecting (s1);
	strcpy(s1,sp);

	expecting (VOID);
	if ( strcmp(s1,sp) )
	{
		error ("strcpy returned '%s', 'Hi' was expected", s1);
	}
finis
}

/*
** tstrncpy
**	Test the strncpy routines.
*/
tstrncpy()
{
	char	s1[STRLEN];
	char	s2[STRLEN];
	char	*sp;
#ifndef	SYSVR_2
	size_t	length;
#else
	int	length;
#endif

testing("Using strncpy to copy one string onto another");
testing("should return s1, and s1 == s2.");
start
	fill_string (s1, "a");

	expecting (s1);
	strncpy(s1,"My bonnie lies over...",25);

	expecting (VOID);
	if ( strcmp(s1,"My bonnie lies over...") )
	{
		error ("strncpy returned '%s', '%s' was expected",
			"My bonnie lies over...", s1);
	}
finis


testing("Using strncpy to copy s2 onto a string of NULLS");
testing("should return s1, and s2 should be in s1.");
start
	null_string (s1);

	sp = "Tis time the Walrus said";

	expecting (VOID);
	length = strlen(sp);

	expecting (s1);
	strncpy(s1,sp,length);

	expecting (VOID);
	if ( strcmp(s1,sp) )
	{
		error ("strncpy returned '%s', '%s' was expected",
			s1, sp);
	}
finis


testing("Using strncpy to copy two strings, s2 a NULL string");
testing("should return s1, and s1 should NULL.");
start
	fill_string (s1, "");
	fill_string (s2, "");

	expecting (s1);
	strncpy(s1,s2,2);

	expecting (VOID);
	if ( strcmp(s1,s2) )
	{
		error ("strncpy returned '%s', a null string was expected",
			s1);
	}
finis


testing("Using strncpy to copy a string, the string in s1 longer than s2");
testing("should return s1, and s1 should be identical to s2.");
start
	fill_string (s1, "abcdef");

	sp = "Hi";

	expecting (s1);
	strncpy(s1,sp,2);

	expecting (VOID);
	length = strlen(sp);

	expecting (VOID);
	if ( strncmp(s1, sp, length ))
	{
		s1[length] = '\0';
		error ("strncpy returned '%s', '%s' was expected",
			s1, sp);
	}
finis


testing("Use strncpy with n one less than the length of s2");
testing("should return s1, and s1 should be equal to s2 -1.");
start
	fill_string (s1, "");
	sp = "This is a test";

	expecting (VOID);
	length = strlen(sp) -1;

	expecting (s1);
	strncpy(s1,sp, length);

	expecting (VOID);
	if ( strncmp(sp, s1, length) )
	{
		sp[length] = '\0';
		error ("strncpy returned '%s', '%s' was expected",
			s1, sp);
	}
finis


testing("Using strncpy to copy two strings, with 'n' equal to the");
testing("length of s2");
testing("should return s1, with s1 == s2.");
start
	fill_string (s1, "");
	sp = "Another test";

	expecting (VOID);
	length = strlen(sp);

	expecting (s1);
	strncpy(s1,sp,length);

	expecting (VOID);
	if ( strncmp(sp, s1, length) )
	{
		s1[length] = '\0';
		error ("strncpy returned '%s', '%s' was expected",
			s1, sp);
	}
finis


testing("Using strncpy to copy two strings, 'n' == 0");
testing("should return s1, and s1 should be NULL.");
start
	fill_string (s1, "");
	sp = "This shouldn't be copied";

	expecting (s1);
	strncpy(s1,sp,0);

	if ( s1[0] )
	{
		error ("strncpy returned '%s', a null string was expected",
			s1);
	}
finis


testing("Using strncpy to copy a string, n == strlen(s2) + 2");
testing("should return s1, and s1 == s2, but with two NULLs at the end.");
start
	fill_string (s1, "");
	sp = "This should be copied";

	expecting (VOID);
	length = strlen(sp) + 2;

	expecting (s1);
	strncpy(s1,sp,length);

	expecting (VOID);
	if ( strcmp(s1,sp) )
	{
		error ("strncpy returned '%s', '%s' was expected",
			s1, sp);
	}
	else
		if ( s1[length - 1] != '\0' )
		{
			error ("strncpy did not add two NULL bytes at the end");
		}
finis
}

/*
** tstrlen
**	Test that strlen works.
*/
tstrlen()
{

testing("strlen(string) should return the length of the string.");
start
	expecting (20);
	strlen("12345678901234567890");
finis


testing("strlen(NULL) should return 0.");
start
	expecting (0);
	strlen("");
finis
}

/*
** tstrchr
**	Test the strchr routine.
*/
tstrchr()
{
	char	*ret_val;
	char	*sp;

testing("Using strchr to scan a string for a character");
testing("should return an index into the string.");
start
	sp = "This is a test";

	expecting (VOID);
	ret_val = strchr(sp,'a');
	if ( ret_val == NULL )
	{
		error ("strchr returned NULL");
	}
	else
		if ( ret_val != &sp[8] )
		{
			error("strchr returned the wrong offset");
		}
finis


testing("Using strchr to scan a string for a character not in the string");
testing("should return NULL.");
start
	sp = "abcdef";

	expecting (NULL);
	strchr(sp,'0');
finis

testing("Using strchr to scan a NULL string should return NULL.");
start
	sp = "";

	expecting (NULL);
	strchr(sp,'0');
finis


testing("Scanning a string with the character we are looking");
testing("for as the first character");
testing("should return the entire string.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strchr(sp,'a');
	if ( ret_val == NULL )
	{
		error("strchr returned NULL");
	}
	else
	{
		if ( ret_val != sp )
		{
			error ("strchr did not return the correct string");
		}
	}
finis


testing("Scanning a string with the character we are looking");
testing("for as the last character");
testing("should return the end of the string.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strchr(sp,'g');
	if ( ret_val == NULL )
	{
		error("strchr returned NULL");
	}
	else
	{
		if ( strcmp(ret_val,"g") )
		{
			error ("strchr returned '%s', '%s' was expected",
				ret_val, "g");
		}
	}
finis


testing("Scanning a string looking for a NULL byte");
testing("should return the NULL byte at the end of the string.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strchr(sp,NULL);
	if ( ret_val == NULL )
	{
		error("strchr returned NULL");
	}
	else
	{
		if ( ret_val != &sp[strlen(sp)] )
		{
			error ("strchr returned '%s', '%s' was expected",
				ret_val, &sp[strlen(sp)] );
		}
	}
finis


testing("Using strchr to scan a string with multiple occurrences");
testing("of the character");
testing("should return the first character.");
start
	sp = "abacabae";

	expecting (VOID);
	ret_val = strchr(sp,'b');
	if ( ret_val == NULL )
	{
		error ("strchr returned NULL");
	}
	else
	{
		if ( strcmp(ret_val,"bacabae") )
		{
			error ("strchr returned '%s', '%s' was expected",
				ret_val,"bacabae");
		}
	}
finis
}


/*
** tstrrchr
**	Test the strrchr routine.
*/
tstrrchr()
{
	char	*ret_val;
	char	*sp;

testing("Using strrchr to scan a string for a character");
testing("should return an index into the string.");
start
	sp = "This is a test";

	expecting (VOID);
	ret_val = strrchr(sp,'a');
	if ( ret_val == NULL )
	{
		error ("strrchr returned NULL");
	}
	else
		if ( ret_val != &sp[8] )
		{
			error ("strrchr returned '%s', '%s' was expected",
				ret_val, &sp[8] );
		}
finis


testing("Using strrchr to scan a string for a character not in the string");
testing("should return NULL.");
start
	sp = "abcdef";

	expecting (NULL);
	strrchr(sp,'0');
finis


testing("Using strrchr to scan a NULL string");
testing("should return NULL.");
start
	sp = "";

	expecting (NULL);
	strrchr(sp,'0');
finis

testing("Scanning a string with the character we are looking");
testing("for as the first character");
testing("should return the entire string.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strrchr(sp,'a');
	if ( ret_val == NULL )
	{
		error("strrchr returned NULL");
	}
	else
	{
		if ( ret_val != sp )
		{
			error ("strrchr returned '%s', '%s' was expected",
				ret_val, sp);
		}
	}
finis


testing("Scanning a string with the character we are looking");
testing("for as the last character");
testing("should return the end of the string.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strrchr(sp,'g');
	if ( ret_val == NULL )
	{
		error("strrchr returned NULL");
	}
	else
	{
		if ( strcmp(ret_val,"g") )
		{
			error ("strrchr returned '%s', '%s' was expected",
				ret_val,"g");
		}
	}
finis


testing("Scanning a string looking for a NULL byte");
testing("should return the NULL byte at the end of the string.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strrchr(sp,NULL);
	if ( ret_val == NULL )
	{
		error("strrchr returned NULL");
	}
	else
	{
		if ( ret_val != &sp[strlen(sp)] )
		{
			error ("strrchr returned '%s', '%s' was expected",
				ret_val, &sp[strlen(sp)] );
		}
	}
finis


testing("Using strrchr to scan a string with multiple occurrences");
testing("of the character.");
testing("should return the last character.");
start
	sp = "abacabae";

	expecting (VOID);
	ret_val = strrchr(sp,'b');
	if ( ret_val == NULL )
	{
		error ("strrchr returned NULL");
	}
	else
	{
		if ( strcmp(ret_val,"bae") )
		{
			error ("strrchr returned '%s', '%s' was expected",
				ret_val,"bae");
		}
	}
finis
}

/*
** tstrpbrk
**	Test the strpbrk routine.
*/
tstrpbrk()
{
	char	*ret_val;
	char	*sp;

testing("Using strpbrk to search a string for a character.");
testing("should return a substring of s1.");
start
	sp = "Tis time the other Walrus said";

	expecting (VOID);
	ret_val = strpbrk(sp,"W");
	if ( ret_val == NULL )
		error ("strpbrk returned NULL");
	else
		if ( strcmp (ret_val,"Walrus said") )
			error ("strpbrk returned '%s', '%s' was expected",
				ret_val,"Walrus said");
finis


testing("Using strpbrk to search a string for a character not in the string");
testing("should return NULL.");
start
	sp = "Hi there kids";

	expecting (NULL);
	strpbrk(sp,"Z");
finis


testing("Using strpbrk to look for the second character in s2");
testing("should return the index into s1.");
start
	sp = "I have seen the enemy, and he is us.";

	expecting (VOID);
	ret_val = strpbrk(sp,"Z,.");
	if ( ret_val == NULL )
		error ("strpbrk returned NULL");
	else
		if ( strcmp(", and he is us.",ret_val) )
			error ("strpbrk returned '%s', '%s' was expected",
				ret_val, ", and he is us.");
finis


testing("Using strpbrk to look for a character at the end of s1");
testing("should return the end of s1.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strpbrk(sp,"PgL");
	if ( ret_val == NULL )
		error ("strpbrk returned NULL");
	else
		if ( strcmp("g",ret_val) )
			error ("strpbrk returned '%s', '%s' was expected",
				ret_val, "g");
finis


testing("Using strpbrk to look for a character at the beginning of s1");
testing("should return the beginning of s1.");
start
	sp = "abcdefg";

	expecting (VOID);
	ret_val = strpbrk(sp,"PaL");
	if ( ret_val == NULL )
		error ("strpbrk returned NULL");
	else
		if ( strcmp(sp,ret_val) )
			error ("strpbrk returned '%s', '%s' was expected",
				ret_val, sp);
finis


testing("Using strpbrk to look for a character occurring multiple times in s1");
testing("should return the first occurrence of the character.");
start
	sp = "abcbebg";

	expecting (VOID);
	ret_val = strpbrk(sp,"PbL");
	if ( ret_val == NULL )
		error ("strpbrk returned NULL");
	else
		if ( strcmp("bcbebg",ret_val) )
			error ("strpbrk returned '%s', '%s' was expected",
				"bcbebg",ret_val);
finis
}

/*
** tstrspn
**	test the strspn routine...
*/
tstrspn()
{
	char	*sp;

testing("Using strspn(Hi there, how are the kids?,Hi there)");
testing("should return 8.");
start
	sp = "Hi there, how are the kids?";

	expecting (8);
	strspn(sp,"Hi there");
finis


testing("strspn(abcdefg,bcdefg) should return 0.");
start
	expecting (0);
	strspn("abcdefg","bcdefg");
finis


testing("strspn(aaabbbabcaabb,ab) should return 9.");
start
	expecting (9);
	strspn("aaabbbbabcaabb","ab");
finis


testing("strspn(\"\",a) should return 0");
start
	expecting (0);
	strspn("","a");
finis
}


/*
** tstrcspn
**	test the strcspn routine...
*/
tstrcspn()
{
	char	*sp;

testing("strcspn(Hi there, how are the kids?,\",\") should return 8.");
start
	expecting (8);
	sp = "Hi there, how are the kids?";
	strcspn(sp,",");
finis


testing("strcspn(abcdefg,bcdefg) should return 1.");
start
	expecting (1);
	strcspn("abcdefg","bcdefg");
finis


testing("strcspn(cdefghijkaaabbbabcaabb,ab) should return 9.");
start
	expecting (9);
	strcspn("cdefghijkaaabbbbabcaabb","ab");
finis


testing("strcspn(\"\",a) should return 0.");
start
	expecting (0);
	strcspn("","a");
finis
}

/*
** tstrtok
**	Test the strtok routine.
*/
tstrtok()
{
	char	*ret_val;
	char	*s1;

testing("Using strtok to scan a string with separators of ' ' and ':',");
testing("make sure each return value is correct.  Returned should be");
testing("a pointer to the start of each token, a NULL byte written");
testing("after each token, and a return of NULL at the end.");
start
	s1 = "First Second:Third";

	expecting (s1);
	strtok(s1," :");

	if ( s1[5] != '\0' )
	{
		error ("strtok did not put a NULL byte at the end of the token");
		return;
	}

	expecting (VOID);
	ret_val = strtok(NULL," :");
	if ( ret_val == NULL )
	{
		error ("strtok returned NULL the second time.");
		return;
	}

	if ( strncmp(ret_val,"Second",6) )
	{
		error ("strpbrk returned '%s' the second time, \
'%s' was expected", ret_val,"Second",6);
		return;
	}

	if ( strlen(ret_val) != 6 )
	{
		error ("strtok did not put a NULL byte at the end\n\
of the token, the second time, length = %d", strlen (ret_val));
		return;
	}

	expecting (VOID);
	ret_val = strtok(NULL," :");
	if ( ret_val == NULL )
	{
		error ("strtok returned NULL the third time");
		return;
	}

	if ( strncmp(ret_val,"Third",5) )
	{
		error ("strtok returned the wrong token the third time\n\
'%s' was returned, 'Third' was expected", ret_val);
		return;
	}

	if ( strlen(ret_val) != 5 )
	{
		error ("strtok did not put a NULL byte at the end of the\n\
token, the second time.");
		return;
	}

	ret_val = strtok(NULL," :");
	if ( ret_val != NULL )
	{
		error("strtok returned '%s' the last time, expecting NULL\n%s",
			ret_val, "at the end of the string");
		return;
	}
finis


testing("Using strtok to scan a string, but having the");
testing("separator string consist of character not in s1");
testing("should return the string back the first time, NULL back the second.");
start
	s1 = "Hi there:How are the,kids?";

	expecting (VOID);
	ret_val = strtok(s1,";!@#");
	if ( ret_val != s1 )
		error ("strtok returned '%s', '%s' was expected",
			ret_val, s1);
	else
		if ( strcmp("Hi there:How are the,kids?",ret_val) )
			error("strtok changed the token from '%s'\nto '%s'",
				"Hi there:How are the,kids?",ret_val);
	ret_val = strtok(NULL,";!@#");
	if ( ret_val != NULL )
		error ("the second call to strtok returned '%s', NULL expected",
			ret_val);
finis



testing("Using strtok to scan a NULL string should return NULL.");
start
	expecting (NULL);
	strtok(""," :;");
finis



testing("Using strtok to scan a string, the separator characters are NULL");
testing("should return the string back the first time, NULL back the second.");
start
	s1 = "Hi there:How are the,kids?";

	expecting (s1);
	ret_val = strtok(s1,"");

	expecting (VOID);
	if ( strcmp("Hi there:How are the,kids?",ret_val) )
		error("strtok changed the token from '%s'\nto '%s'",
			"Hi there:How are the,kids?",ret_val);
				
	expecting (NULL);
	strtok(NULL,"");
finis



testing("Scanning a string with strtok, but giving it different separator");
testing("characters at different calls should return the tokens back.");
start
	s1 = "First Second:Third";

	expecting (s1);
	strtok(s1," ");

	if ( s1[5] != '\0' )
	{
		error ("strtok did not put a NULL byte at the end of the token");
	}

	expecting (VOID);
	ret_val = strtok(NULL,":");
	if ( ret_val == NULL )
	{
		error ("strtok returned NULL the second time");
	}

	expecting (VOID);
	if ( strncmp(ret_val,"Second",6) )
	{
		error ("strtok returned '%s' the second time, expecting '%s'",
			ret_val, "Second");
	}
	if ( strlen(ret_val) != 6 )
	{
		error ("strtok did not put a NULL byte at the end of the token,\
the second time.");
	}

	expecting (VOID);
	ret_val = strtok(NULL,":");
	if ( ret_val == NULL )
	{
		error ("strtok returned NULL the third time");
	}

	if ( strncmp(ret_val,"Third",5) )
	{
		error ("strtok returned '%s' the third time, expecting '%s'",
			ret_val, "Third");
	}
	if ( strlen(ret_val) != 5 )
	{
		error ("strtok did not put a NULL byte at the end of\n\
the token, the second time.");
	}

	expecting (VOID);
	ret_val = strtok(NULL," :");

	if ( ret_val != NULL )
		error ("strtok did not return NULL at the end of the string");
finis

}

void
fill_string (str1, str2)
register	char	*str1, *str2;
{
	register int len;

	len = STRLEN;
	do
		len--;
	while ((*str1++ = *str2++) != '\0');
	while (len > 0)
		{
		*str1++ = '\377';
		len--;
		}
}



void
null_string (s)
register char	*s;
{
	register int	len;

	len = STRLEN;
	while (len > 0)
		{
		*s++ = '\0';
		len--;
		}
}

