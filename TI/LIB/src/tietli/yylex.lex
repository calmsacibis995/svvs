	/*
	** SCCSID("@(#)yylex.lex	3.1 (SVID) 12/1/86");
	*/
%%
#< {
	SideType = SIDE_PITCHER;
}

#> {
	SideType = SIDE_CATCHER;
}

#[^<>\n].* {
	compile_switch(yytext);
}

#[^<>]$ {
	LineNumber++;
	SideType = SIDE_NULL;
}

#$ {
	LineNumber++;
	SideType = SIDE_NULL;
}

while {
	return(WHILE_STMT);
}

for {
	return(FOR_STMT);
}

if {
	return(IF_STMT);
}

else {
	return(ELSE_STMT);
}

continue {
	return(CONTINUE_STMT);
}

break {
	return(BREAK_STMT);
}

exit {
	return(EXIT_STMT);
}

test {
	return(HEAD_STMT);
}

start {
	;
}

finis {
	;
}

"&&" {
	yylval.op_lexval = AND;
	return((int) yylval.op_lexval);
}

"||" {
	yylval.op_lexval = OR;
	return((int) yylval.op_lexval);
}

"==" {
	yylval.op_lexval = EQ;
	return((int) yylval.op_lexval);
}

"!=" {
	yylval.op_lexval = NE;
	return((int) yylval.op_lexval);
}

"<=" {
	yylval.op_lexval = LE;
	return((int) yylval.op_lexval);
}

">=" {
	yylval.op_lexval = GE;
	return((int) yylval.op_lexval);
}

"++" {
	yylval.op_lexval = INCR;
	return((int) yylval.op_lexval);
}

"--" {
	yylval.op_lexval = DECR;
	return((int) yylval.op_lexval);
}

"+=" {
	yylval.op_lexval = ADDASSIGN;
	return((int) yylval.op_lexval);
}

"-=" {
	yylval.op_lexval = SUBASSIGN;
	return((int) yylval.op_lexval);
}

"*=" {
	yylval.op_lexval = MULTASSIGN;
	return((int) yylval.op_lexval);
}

"/=" {
	yylval.op_lexval = DIVASSIGN;
	return((int) yylval.op_lexval);
}

"%=" {
	yylval.op_lexval = MODASSIGN;
	return((int) yylval.op_lexval);
}

TRUE {
	yylval.const_lexval = store_const(yytext, DATA_BOOL);
	return(BOOL);
}

FALSE {
	yylval.const_lexval = store_const(yytext, DATA_BOOL);
	return(BOOL);
}

[A-Za-z][A-Za-z0-9_\.\$]* {
	yylval.var_lexval = malloc(strlen(yytext) + 1);
	strcpy(yylval.var_lexval, yytext);
	return(IDENTIFIER);
}

[0-9]+ {
	yylval.const_lexval = store_const(yytext, DATA_NUM);
	return(NUMERIC);
}

\"([^\"]|\\\")*\" {
	yylval.const_lexval = store_const(yytext, DATA_TEXT);
	return(TEXT);
}

'(\\?.|\\[0-7]{1,3})' {
	yylval.const_lexval = store_const(yytext, DATA_CHAR);
	return(CHAR);
}

[ \t]+ {
	;
}

\n {
	LineNumber++;
	SideType = SIDE_NULL;
}

. {
	yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
	return((int) yylval.op_lexval);
}
