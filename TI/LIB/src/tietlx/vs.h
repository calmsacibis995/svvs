/*
** SCCSID("@(#)vs.h	3.2 (SVID) 12/23/86")
*/

extern void	vs_setup();
extern void	vs_done();
extern void	vs_expecting();
extern void	vs_no_expecting();
extern void	vs_abstract();
extern void	vs_testing();
extern void	vs_prep();
extern void	vs_cleanup();
extern void	vs_error();
extern void	vs_warning();
extern FILE	*vs_t_fopen();
extern FILE	*vs_d_fopen();
