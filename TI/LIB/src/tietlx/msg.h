/*
** SCCSID("@(#)msg.h	3.2 (SVID) 12/23/86")
*/

# define	MSG_BOS		"BEGIN_SCREEN"
# define	MSG_EOS		"END_SCREEN"

extern BOOLEAN	MsgTouched;
extern BOOLEAN	UmpireError;
extern BOOLEAN	MsgWarning;

extern void	msg_open();
extern void	msg_drop();
extern void	msg_pickup();
extern void	msg_rewind();
extern void	msg_flush();
extern void	msg_name();
