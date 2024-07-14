# include	<bless.h>
# include	<common.h>
# include	<varargs.h>
# include	<stack.h>

SCCSID("@(#)push.c	3.2 (SVID) 12/30/86");


/*
** The stack structure. The terminfo param. string system
** works on a RPN notation stack. This is our version of
** the same stack.
*/
struct	stack
{
	int	stk_value;	/* int values */
	int	stk_type;	/* What this element is */
	char	*stk_str;	/* string values. */
};


struct	stack	Stack[STACKSIZE];
struct	stack	*Top = Stack;

/*
** stack_init
**	Initialize the stack, and Top.
*/
stack_init()
{
	struct	stack	*stk;

	stk = &Stack[STACKSIZE - 1];
	Top = Stack;

	while ( Top != stk )
	{
		Top->stk_type = STK_NADA;
		Top++;
	}
	Top = Stack;
}

/*
** push
**	Push something onto the stack.
**
** Arguments
**	arg1	The type of the argument to push.
**	arg2	The value of the argument (if constant),
**		or the parameter number if a parameter.
**
** Return Value
**	OK	If the argument could be pushed
**	ERR	If there is no space to push
*/
int
push(va_alist)
va_dcl
{
	va_list	ap;
	char	*str;
	int	type;

	if ( Top > &Stack[STACKSIZE] )
		return (ERR);

	va_start(ap);
	type = va_arg(ap,int);

	switch(type)
	{
	  case STK_SUB:
	  case STK_MUL:
	  case STK_DIV:
	  case STK_ADD:
	  case STK_COMP:
	  case STK_NEG:
# ifdef	TRACE
		if ( Trace(30,0) )
			printf("push: math arg\n");
# endif	/* TRACE */
		break;

	  case STK_PARAM:
		if ( Top != Stack )
		{
			va_end(ap);
# ifdef	TRACE
			if ( Trace(30,0) )
				printf("push: Attempt to push a parameter, not at the bottom of the stack\n");
# endif	/* TRACE */
			return (ERR);
		}
		Top->stk_value = va_arg(ap,int);
# ifdef	TRACE
		if ( Trace(30,0) )
			printf("push: parameter, value = %d\n",Top->stk_value);
# endif	/* TRACE */
		break;

	  case STK_CONST:
		Top->stk_value = va_arg(ap,int);
# ifdef	TRACE
		if ( Trace(30,0) )
			printf("push: Constant, value = %d\n",Top->stk_value);
# endif	/* TRACE */
		break;

	  case STK_STR:
		str = va_arg(ap,char *);
		if ( (Top->stk_str = malloc(strlen(str) + 1)) == NULL )
		{
			va_end(ap);
# ifdef	TRACE
			if ( Trace(30,0) )
				printf("malloc of STK_STR failed\n");
# endif	/* TRACE */
			return (ERR);
		}
		(void) strcpy(Top->stk_str,str);
		break;

	  default:
# ifdef	TRACE
		if ( Trace(30,0) )
			printf("push: Got an illegal type %d\n",type);
# endif	/* TRACE */
		va_end(ap);
		return (ERR);
	}
	Top->stk_type = type;
	Top++;
	va_end(ap);
	return (OK);
}

/*
** b_eval
**	Given a value, evaluate the stack to see what the
**	original parameter was.
**
** Arguments
**	value	The value to start the evaluation with.
**	orig	A pointer to return the original value with.
**
** Return Value
**	OK	If the original parameter could be found.
**	ERR	If the parameter can't be figured out.
*/
int
b_eval(value,orig)
int	value;
int	*orig;
{
	int	op;
	int	seen;

# ifdef	TRACE
	if ( Trace(35,0) )
		printf("b_eval:( %d, %d)\n",value,*orig);
# endif	/* TRACE */
	if ( Top == Stack )
	{
# ifdef	TRACE
		if ( Trace(35,0) )
			printf("Nothing on the stack\n");
# endif	/* TRACE */
		return (ERR);
	}
	Top--;
	seen = 0;

	do
	{
		switch( Top->stk_type )
		{
		  case STK_SUB:
		  case STK_MUL:
		  case STK_DIV:
		  case STK_ADD:
		  case STK_COMP:
		  case STK_NEG:
			op = Top->stk_type;
			break;

		  case STK_PARAM:
			if ( Top != Stack )
			{
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("Parameter not on the bottom of the stack\n");
# endif	/* TRACE */
				return (ERR);
			}
			*orig = value;
			seen = 1;
			break;

		  case STK_CONST:
			if ( op == STK_NADA )
			{
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("No operator on the stack\n");
# endif	/* TRACE */
				return (ERR);
			}
			switch ( op )
			{
			  case STK_SUB:
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("stk (%d) + value (%d)\n",value,Top->stk_value);
# endif	/* TRACE */
				value = value + Top->stk_value;
				break;

			  case STK_MUL:
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("stk (%d) / value (%d)\n",value,Top->stk_value);
# endif	/* TRACE */
				value = value / Top->stk_value;
				break;

			  case STK_DIV:
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("stk (%d) * value (%d)\n",value,Top->stk_value);
# endif	/* TRACE */
				value = value * Top->stk_value;
				break;

			  case STK_ADD:
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("stk (%d) - value (%d)\n",value,Top->stk_value);
# endif	/* TRACE */
				value = value - Top->stk_value;
				break;

			  case STK_COMP:
				value = ~value;
				break;

			  case STK_NEG:
				value = !value;
				break;

			  default:
# ifdef	TRACE
				if ( Trace(35,0) )
					printf("Unknown operator %d\n",op);
# endif	/* TRACE */
				return (ERR);
			}
			break;

		  default:
# ifdef	TRACE
			if ( Trace(35,0) )
				printf("Unknown stack element %d\n",Top->stk_type);
# endif	/* TRACE */
			return (ERR);
		}
		Top--;
	} while (Top >= Stack);

	if ( seen == 0 )
	{
# ifdef	TRACE
		if ( Trace(35,0) )
			printf("No parameter on the stack\n");
# endif	/* TRACE */
		return (ERR);
	}
	return (OK);
}
