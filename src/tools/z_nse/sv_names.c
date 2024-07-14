#include <svvs.h>
#include <sv_env.h>
#include <tiuser.h>

#ifdef	UNDEF
#ident	"@(#)sv_names.c	1.3"
#endif

char *sname( state )
int state;
{
	char *state_name;
	switch (state) {
	case T_UNBND:
		state_name = "T_UNBND";
		break;	
	case T_IDLE:
		state_name = "T_IDLE";
		break;	
	case T_INCON:
		state_name = "T_INCON";
		break;
	case T_OUTCON:
		state_name = "T_OUTCON";
		break;	
	case T_DATAXFER:
		state_name = "T_DATAXFER";
		break;
	case T_OUTREL:
		state_name = "T_OUTREL";
		break;
	case T_INREL:
		state_name = "T_INREL";
		break;
	default:
#ifdef	DEVELOPMENT
	warning("Invalid state passed to sname.");
#endif
		state_name="";
	}	
	return( state_name );
}
char *ename( event )
int event;
{
	char *event_name;
	switch (event) {
	case T_LISTEN:
		event_name = "T_LISTEN";
		break;	
	case T_CONNECT:
		event_name = "T_CONNECT";
		break;	
	case T_DATA:
		event_name = "T_DATA";
		break;
	case T_EXDATA:
		event_name = "T_EXDATA";
		break;	
	case T_DISCONNECT:
		event_name = "T_DISCONNECT";
		break;
	case T_ERROR:
		event_name = "T_ERROR";
		break;
	case T_UDERR:
		event_name = "T_UDERR";
		break;
	case T_ORDREL:
		event_name = "T_ORDREL";
		break;
	case 0:
		event_name = "0";
		break;
	default:
#ifdef	DEVELOPMENT
	warning("Invalid event passed to ename.");
#endif
		event_name="";
	}	
	return( event_name );
}
