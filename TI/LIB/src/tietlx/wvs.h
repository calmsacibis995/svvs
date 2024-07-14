/*
** SCCSID("@(#)wvs.h	3.2 (SVID) 12/23/86")
*/

extern void		set_svvs_option();
extern void		wvs_setup();
extern void		wvs_done();
extern void		wvs_expecting();
extern void		wvs_no_expecting();
extern void		wvs_abstract();
extern void		wvs_testing();
extern void		wvs_prep();
extern void		wvs_cleanup();
extern void		wvs_error();
extern void		wvs_message();
extern void		wvs_warning();
extern STMT_TYPE	exec_block();
extern FILE		*wvs_t_fopen();
extern char		*wvs_t_path();
extern FILE		*wvs_d_fopen();
extern char		*wvs_d_path();
extern char		*wvs_advisory();
extern int		wvs_msg_type();
