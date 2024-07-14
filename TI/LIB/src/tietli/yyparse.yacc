/*
** SCCSID("@(#)yyparse.yacc	3.1 (SVID) 12/1/86");
*/

%union
{
	OPERATOR_TYPE	op_lexval;
	char		*var_lexval;
	TBL_INDEX	const_lexval;
	TBL_INDEX	stmt_lexval;
	struct
	{
		TBL_INDEX	expr_start;
		TBL_INDEX	expr_atom;
	} expr_lexval;
	struct
	{
		TBL_INDEX	arg_start;
		int		arg_count;
	} arg_lexval;
	struct
	{
		TBL_INDEX	for_compare;
		TBL_INDEX	for_last;
	} for_lexval;
}

%start	list

/*
** Precedence list.  Simliar to C precedence
*/

%right	<op_lexval>	'=' ADDASSIGN SUBASSIGN MULTASSIGN DIVASSIGN MODASSIGN
%left	<op_lexval>	OR
%left	<op_lexval>	AND
%left	<op_lexval>	'|'
%left	<op_lexval>	'&'
%left	<op_lexval>	EQ	NE
%left	<op_lexval>	'<'	LE	'>'	GE
%left	<op_lexval>	':'
%left	<op_lexval>	'+'	'-'
%left	<op_lexval>	'*'	'/'	'%'
%right	<op_lexval>	'!'	UMINUS	UPLUS	INCR	DECR
%left	<op_lexval>	'['	']'

%token			PREINCR	POSTINCR
%token			PREDECR	POSTDECR

%token	<var_lexval>	IDENTIFIER

%token	<const_lexval>	NUMERIC	
%token	<const_lexval>	BOOL	
%token	<const_lexval>	TEXT	
%token	<const_lexval>	CHAR	

%term	<stmt_lexval>	WHILE_STMT
%term	<stmt_lexval>	FOR_STMT
%term	<stmt_lexval>	IF_STMT
%term	<stmt_lexval>	ELSE_STMT
%term	<stmt_lexval>	CONTINUE_STMT
%term	<stmt_lexval>	BREAK_STMT
%term	<stmt_lexval>	EXIT_STMT

%type	<stmt_lexval>	stmt
%type	<stmt_lexval>	while_
%type	<stmt_lexval>	if_then
%type	<stmt_lexval>	if_else

%type	<for_lexval>	for_1
%type	<for_lexval>	for_2
%type	<for_lexval>	for_3

%type	<expr_lexval>	_call
%type	<expr_lexval>	call_

%type	<arg_lexval>	args

%type	<expr_lexval>	variable
%type	<expr_lexval>	array
%type	<expr_lexval>	expr
%type	<arg_lexval>	args

%token	<var_lexval>	ABSTRACT_STMT
%token	<var_lexval>	TESTING_STMT
%token	<var_lexval>	PREP_STMT
%token	<var_lexval>	CLEANUP_STMT
%token	<var_lexval>	START_STMT
%token	<var_lexval>	FINIS_STMT

%type	<expr_lexval>	_acall
%type	<expr_lexval>	_tcall
%type	<expr_lexval>	_pcall
%type	<expr_lexval>	_ccall

%type	<stmt_lexval>	_block

%%

/*
** Start symbol
*/

list	:	/* empty */
	|	list ablock
	{
		;
	}
	|	list tblock
	{
		;
	}
	|	list pblock
	{
		;
	}
	|	list cblock
	{
		;
	}
	;

/*
** Abstract headers
*/

_acall	:	ABSTRACT_STMT '('
	{
		$$.expr_atom = add_call_to_expr(sav_call("abstract"));
		$$.expr_start = $$.expr_atom;
	}
	;

acall_	:	_acall args ')' ';'
	{
		rset_call_expr(Prgm.ExprTbl[$1.expr_start].expr_data.expr_var, 
				$2.arg_count);
		assign_stmt($1.expr_start);
		keep_expr();
	}
	;

acalls	:	acall_
	{
		;
	}
	|	acalls acall_
	{
		;
	}
	;

ablock	:	acalls block_
	{
		;
	}
	;

stmt	:	ablock
	{
		;
	}
	;

/*
** Testing headers
*/

_tcall	:	TESTING_STMT '('
	{
		$$.expr_atom = add_call_to_expr(sav_call("testing"));
		$$.expr_start = $$.expr_atom;
	}
	;

tcall_	:	_tcall args ')' ';'
	{
		rset_call_expr(Prgm.ExprTbl[$1.expr_start].expr_data.expr_var, 
				$2.arg_count);
		assign_stmt($1.expr_start);
		keep_expr();
	}
	;

tcalls	:	tcall_
	{
		;
	}
	|	tcalls tcall_
	{
		;
	}
	;

tblock	:	tcalls block_
	{
		;
	}
	;

stmt	:	tblock
	{
		;
	}
	;

/*
** Prep headers
*/

_pcall	:	PREP_STMT '('
	{
		$$.expr_atom = add_call_to_expr(sav_call("prep"));
		$$.expr_start = $$.expr_atom;
	}
	;

pcall_	:	_pcall args ')' ';'
	{
		rset_call_expr(Prgm.ExprTbl[$1.expr_start].expr_data.expr_var, 
				$2.arg_count);
		assign_stmt($1.expr_start);
		keep_expr();
	}
	;

pcalls	:	pcall_
	{
		;
	}
	|	pcalls pcall_
	{
		;
	}
	;

pblock	:	pcalls block_
	{
		;
	}
	;

stmt	:	pblock
	{
		;
	}
	;

/*
** Cleanup headers
*/

_ccall	:	CLEANUP_STMT '('
	{
		$$.expr_atom = add_call_to_expr(sav_call("cleanup"));
		$$.expr_start = $$.expr_atom;
	}
	;

ccall_	:	_ccall args ')' ';'
	{
		rset_call_expr(Prgm.ExprTbl[$1.expr_start].expr_data.expr_var, 
				$2.arg_count);
		assign_stmt($1.expr_start);
		keep_expr();
	}
	;

ccalls	:	ccall_
	{
		;
	}
	|	ccalls ccall_
	{
		;
	}
	;

cblock	:	ccalls block_
	{
		;
	}
	;

stmt	:	cblock
	{
		;
	}
	;

/*
** Statement blocks
*/

_block	:	START_STMT
	{
		$$ = set_block_stmt();
	}
	;

block_	:	_block stmts FINIS_STMT
	{
		set_misc_stmt(STMT_FINIS);
		rset_block_stmt($1, NEXT_FREE_STMT);
	}
	;

stmts	:	stmt
	{
		;
	}
	|	stmts stmt
	{
		;
	}
	;

stmt	:	'{' stmts '}'
	{
		;
	}
	;

/*
** Parse error rule
*/

stmt	:	error ';'
	{
		Option.save_prgm = FALSE;
		junk_expr();
		yyerrok;
	}
	;

/*
** While loop
*/

while_	:	WHILE_STMT '(' expr ')'
	{
		$$ = set_loop_stmt($3.expr_start);
		keep_expr();
	}
	;

stmt	:	while_ stmt
	{
		set_misc_stmt(STMT_CONT_LOOP);
		rset_loop_stmt($1, NEXT_FREE_STMT, NIL);
	}
	;

/*
** If-then and if-then-else
*/

if_then	:	IF_STMT '(' expr ')'
	{
		$$ = set_compare_stmt($3.expr_start);
		keep_expr();
	}
	;

if_else	:	if_then	stmt ELSE_STMT
	{
		$$ = set_goto_stmt(NULL, TRUE);
		rset_compare_stmt($1, NEXT_FREE_STMT);
	}
	;

stmt	:	if_then stmt
	{
		rset_compare_stmt($1, NEXT_FREE_STMT);
		(void) set_goto_stmt(NEXT_FREE_STMT, FALSE);
	}
	;

stmt	:	if_else stmt
	{
		rset_goto_stmt($1, NEXT_FREE_STMT);
		(void) set_goto_stmt(NEXT_FREE_STMT, FALSE);
	}
	;

/*
** Expression statements
*/

stmt	:	expr ';'
	{
		assign_stmt($1.expr_start);
		keep_expr();
	}
	;

/*
** Function calls
*/

_call	:	IDENTIFIER '('
	{
		$$.expr_atom = add_call_to_expr(sav_call($1));
		$$.expr_start = $$.expr_atom;
		free($1);
	}
	;

call_	:	_call args ')'
	{
		rset_call_expr(Prgm.ExprTbl[$1.expr_start].expr_data.expr_var, 
				$2.arg_count);
		$$.expr_start = $1.expr_start;
	}
	;

/*
** Arguments for function calls
*/

args	:	/* empty list */
	{
		$$.arg_start = NIL;
		$$.arg_count = 0;
	}
	|	expr
	{
		keep_expr();
		$$.arg_start = $1.expr_start;
		$$.arg_count = 1;
	}
	|	args ',' expr
	{
		keep_expr();
		$$.arg_start = $1.arg_start;
		$$.arg_count++;
	}
	;

/*
** For loop
*/

for_1	:	FOR_STMT '(' expr ';'
	{
		assign_stmt($3.expr_start);
		keep_expr();
	}
	|	FOR_STMT '(' ';'
	{
		;
	}

for_2	:	for_1 expr ';'
	{
		$$.for_compare = set_loop_stmt($2.expr_start);
		keep_expr();
	}
	|	for_1 ';'
	{
		$$.for_compare = set_loop_stmt(NIL);
	}

for_3	:	for_2 expr ')'
	{
		keep_expr();
		$$.for_compare = $1.for_compare;
		$$.for_last = $2.expr_start;
	}
	|	for_2 ')'
	{
		$$.for_compare = $1.for_compare;
		$$.for_last = NIL;
	}

stmt	:	for_3 stmt
	{
		set_misc_stmt(STMT_CONT_LOOP);
		rset_loop_stmt($1.for_compare, NEXT_FREE_STMT, $1.for_last);
	}

/*
** Misc. simple statements
*/

stmt	:	CONTINUE_STMT
	{
		set_misc_stmt(STMT_CONTINUE);
	}

stmt	:	BREAK_STMT
	{
		set_misc_stmt(STMT_BREAK);
	}

stmt	:	EXIT_STMT
	{
		set_misc_stmt(STMT_EXIT);
	}
	;

/*
** Null statement
*/

stmt	:	';'
	{
		;
	}
	;

/*
** Expression groups
*/

expr	:	'(' expr ')'
	{
		$$ = $2;
	}
	|	call_
	{
		$$ = $1;
	}
	;

/*
** Binary and unary expressions
*/

expr	:	variable '=' expr
	{
		$$.expr_atom = add_op_to_expr(OP_ASSIGN);
		$$.expr_start = $1.expr_start;
	}
	|	variable ADDASSIGN expr
	{
		$$.expr_atom = add_op_to_expr(OP_ADDASSIGN);
		$$.expr_start = $1.expr_start;
	}
	|	variable SUBASSIGN expr
	{
		$$.expr_atom = add_op_to_expr(OP_SUBASSIGN);
		$$.expr_start = $1.expr_start;
	}
	|	variable MULTASSIGN expr
	{
		$$.expr_atom = add_op_to_expr(OP_MULTASSIGN);
		$$.expr_start = $1.expr_start;
	}
	|	variable DIVASSIGN expr
	{
		$$.expr_atom = add_op_to_expr(OP_DIVASSIGN);
		$$.expr_start = $1.expr_start;
	}
	|	variable MODASSIGN expr
	{
		$$.expr_atom = add_op_to_expr(OP_MODASSIGN);
		$$.expr_start = $1.expr_start;
	}
	|	expr '+' expr
	{
		$$.expr_atom = add_op_to_expr(OP_ADD);
		$$.expr_start = $1.expr_start;
	}
	|	expr '-' expr
	{
		$$.expr_atom = add_op_to_expr(OP_SUB);
		$$.expr_start = $1.expr_start;
	}
	|	expr '*' expr
	{
		$$.expr_atom = add_op_to_expr(OP_MULT);
		$$.expr_start = $1.expr_start;
	}
	|	expr '/' expr
	{
		$$.expr_atom = add_op_to_expr(OP_DIV);
		$$.expr_start = $1.expr_start;
	}
	|	expr '%' expr
	{
		$$.expr_atom = add_op_to_expr(OP_MOD);
		$$.expr_start = $1.expr_start;
	}
	|	expr ':' expr
	{
		$$.expr_atom = add_op_to_expr(OP_CAT);
		$$.expr_start = $1.expr_start;
	}
	|	expr AND expr
	{
		$$.expr_atom = add_op_to_expr(OP_AND);
		$$.expr_start = $1.expr_start;
		Prgm.ExprTbl[$1.expr_atom].expr_short_circuit = $$.expr_atom;
	}
	|	expr OR expr
	{
		$$.expr_atom = add_op_to_expr(OP_OR);
		$$.expr_start = $1.expr_start;
		Prgm.ExprTbl[$1.expr_atom].expr_short_circuit = $$.expr_atom;
	}
	|	expr '&' expr
	{
		$$.expr_atom = add_op_to_expr(OP_BIT_AND);
		$$.expr_start = $1.expr_start;
	}
	|	expr '|' expr
	{
		$$.expr_atom = add_op_to_expr(OP_BIT_OR);
		$$.expr_start = $1.expr_start;
	}
	|	expr EQ expr
	{
		$$.expr_atom = add_op_to_expr(OP_EQ);
		$$.expr_start = $1.expr_start;
	}
	|	expr NE expr
	{
		$$.expr_atom = add_op_to_expr(OP_NE);
		$$.expr_start = $1.expr_start;
	}
	|	expr '<' expr
	{
		$$.expr_atom = add_op_to_expr(OP_LT);
		$$.expr_start = $1.expr_start;
	}
	|	expr LE expr
	{
		$$.expr_atom = add_op_to_expr(OP_LE);
		$$.expr_start = $1.expr_start;
	}
	|	expr '>' expr
	{
		$$.expr_atom = add_op_to_expr(OP_GT);
		$$.expr_start = $1.expr_start;
	}
	|	expr GE expr
	{
		$$.expr_atom = add_op_to_expr(OP_GE);
		$$.expr_start = $1.expr_start;
	}
	|	'-' expr %prec UMINUS
	{
		$$.expr_atom = add_op_to_expr(OP_UMINUS);
		$$.expr_start = $2.expr_start;
	}
	|	'+' expr %prec UPLUS
	{
		$$.expr_start = $2.expr_start;
	}
	|	'!' expr
	{
		$$.expr_atom = add_op_to_expr(OP_NOT);
		$$.expr_start = $2.expr_start;
	}
	|	DECR variable
	{
		(void) add_op_to_expr(OP_PREDECR);
		$$.expr_start = $2.expr_start;
	}
	|	variable DECR
	{
		(void) add_op_to_expr(OP_POSTDECR);
		$$.expr_start = $1.expr_start;
	}
	|	INCR variable
	{
		(void) add_op_to_expr(OP_PREINCR);
		$$.expr_start = $2.expr_start;
	}
	|	variable INCR
	{
		(void) add_op_to_expr(OP_POSTINCR);
		$$.expr_start = $1.expr_start;
	}
	;

/*
** Variables, function names, arrays, constants
*/

expr	:	variable
	{
		;
	}
	|	NUMERIC
	{
		$$.expr_atom = add_var_to_expr($1);
		$$.expr_start = $$.expr_atom;
	}
	|	BOOL
	{
		$$.expr_atom = add_var_to_expr($1);
		$$.expr_start = $$.expr_atom;
	}
	|	TEXT
	{
		$$.expr_atom = add_var_to_expr($1);
		$$.expr_start = $$.expr_atom;
	}
	|	CHAR
	{
		$$.expr_atom = add_var_to_expr($1);
		$$.expr_start = $$.expr_atom;
	}
	;

variable:	IDENTIFIER
	{
		$$.expr_atom = add_var_to_expr(sav_var($1));
		$$.expr_start = $$.expr_atom;
		free($1);
	}
	|	array expr ']'
	{
		$$.expr_atom = add_op_to_expr(OP_ARRAY);
		$$.expr_start = $1.expr_start;
	}
	;

array	:	IDENTIFIER '['
	{
		$$.expr_atom = add_var_to_expr(sav_array($1));
		$$.expr_start = $$.expr_atom;
		free($1);
	}
	;
%%
