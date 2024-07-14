/*
 *	t_getstate.c
 *
 *	a test of t_getstate(NS_LIB), t_sync(NS_LIB), and
 *		t_look(NS_LIB)
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)t_getstate.c	1.6"

#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <signal.h>
#include <svvs.h>
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#define	find_addr( z, y, x )	/*	print_addr( z, y, x )	*/

extern	int	t_errno;

void	chk_unbnd();
void	chk_idle();
void	chk_outcon();
void	chk_incon();
void	chk_dataxfer();
void	chk_outrel();
void	chk_inrel();
void	do_listen();
void	do_connect1();
void	do_connect2();
void	do_connect();
void	do_rcvconnect();
void	do_snddis1();
void	do_snddis2();
void	do_snddis();
void	do_rcvdis1();
void	do_rcvdis2();
void	do_rcvdis3();
void	do_rcvdis();
void	do_snd();
void	do_sndx();
void	do_send();
void	do_rcv();
void	do_rcvx();
void	do_recv();
void	do_sndrel();
void	do_rcvrel();
void	do_accept1();
void	do_accept2();
void	do_accept3();
void	do_accept();
void	do_passcon();
void	do_open();
void	do_close();
void	do_alloc();
void	do_free();
void	do_bind();
void	do_unbind();

void	do_all();
void	do_test();
void	do_case();
void	set_flags();

typedef	void (*vfunctptr)();

struct table {
	char		*name;
	vfunctptr	fn;
	int		sflg;
};

struct table atable[] = {
	"UNBND",		chk_unbnd,		0,
	"IDLE",			chk_idle,		0,
	"OUTCON",		chk_outcon,		0,
	"INCON",		chk_incon,		0,
	"DATAXFER",		chk_dataxfer,	0,
	"OUTREL",		chk_outrel,		0,
	"INREL",		chk_inrel,		0,
	"listen",		do_listen,		1,
	"connect1",		do_connect1,	1,
	"connect2",		do_connect2,	1,
	"rcvconnect",	do_rcvconnect,	1,
	"accept1",		do_accept1,		1,
	"accept2",		do_accept2,		1,
	"passcon",		do_passcon,		1,
	"accept3",		do_accept3,		1,
	"snddis1",		do_snddis1,		1,
	"snddis2",		do_snddis2,		1,
	"rcvdis1",		do_rcvdis1,		1,
	"rcvdis2",		do_rcvdis2,		1,
	"rcvdis3",		do_rcvdis3,		1,
	"snd",			do_snd,			1,
	"sndx",			do_sndx,		1,
	"rcv",			do_rcv,			1,
	"rcvx",			do_rcvx,		1,
	"sndrel",		do_sndrel,		1,
	"rcvrel",		do_rcvrel,		1,
	"open",			do_open,		0,
	"close",		do_close,		0,
	"bind",			do_bind,		0,
	"unbind",		do_unbind,		1,
	"alloc",		do_alloc,		0,
	"free",			do_free,		0,
};

#define	ASIZE	(sizeof(atable)/sizeof(struct table))

char *active_cases[] = {
	"open(1,2):UNBND(1):alloc(1,2):bind(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcv(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcv(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvx(1,2):OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcv(1,2):OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snd(1,2):INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snd(1,2):INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndx(1,2):INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcv(1,2):OUTREL(1):rcvx(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcv(1,2):OUTREL(1):rcv(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvx(1,2):OUTREL(1):rcv(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snd(1,2):INREL(1):snd(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snd(1,2):INREL(1):sndx(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect1(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snd(1,2):INREL(1):snd(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndrel(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvrel(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndx(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):sndx(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		snd(1,2):DATAXFER(1):snd(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):sndrel(1,2):\
		OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvx(1,2):DATAXFER(1):rcv(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcv(1,2):DATAXFER(1):rcvx(1,2):DATAXFER(1):rcvrel(1,2):\
		INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcv(1,2):OUTREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcv(1,2):OUTREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvx(1,2):OUTREL(1):rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snd(1,2):INREL(1):snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snd(1,2):INREL(1):rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):sndx(1,2):INREL(1):sndrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcv(1,2):OUTREL(1):rcv(1,2):OUTREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcvx(1,2):OUTREL(1):rcv(1,2):OUTREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		sndrel(1,2):OUTREL(1):rcv(1,2):OUTREL(1):rcvx(1,2):OUTREL(1):\
		rcvrel(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snd(1,2):INREL(1):snd(1,2):INREL(1):\
		snddis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):sndx(1,2):INREL(1):snd(1,2):INREL(1):\
		rcvdis1(1,2):IDLE(1)",
	"IDLE(1):connect2(1,2):OUTCON(1):rcvconnect(1,2):DATAXFER(1):\
		rcvrel(1,2):INREL(1):snd(1,2):INREL(1):snd(1,2):INREL(1):\
		sndrel(1,2):IDLE(1)",
	"IDLE(1):unbind(1,2):UNBND(1):free(1,2):close(1,2):",
};
char *passive_cases[] = {
	"open(2,1):UNBND(2):alloc(2,1):bind(2,1):IDLE(2)",
	"open(4,3):UNBND(4):alloc(4,3):bind(4,3):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):rcvdis2(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):sndrel(2,1):OUTREL(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):sndrel(2,1):OUTREL(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):sndrel(2,1):OUTREL(2):\
		rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcvrel(2,1):INREL(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):rcvrel(2,1):INREL(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcvrel(2,1):INREL(2):\
		sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):sndrel(2,1):OUTREL(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):sndrel(2,1):OUTREL(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):sndrel(2,1):OUTREL(2):\
		rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):rcvrel(2,1):INREL(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcvrel(2,1):INREL(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcvrel(2,1):INREL(2):\
		sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcvx(2,1):DATAXFER(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcvx(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):rcvx(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):sndx(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):sndx(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):snddis1(2,1):\
		IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):sndx(2,1):DATAXFER(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):sndx(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndx(2,1):DATAXFER(2):snd(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		snd(2,1):DATAXFER(2):sndx(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcvx(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):sndrel(2,1):\
		OUTREL(2):rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcvx(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcv(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvx(2,1):DATAXFER(2):rcv(2,1):DATAXFER(2):rcvrel(2,1):\
		INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcv(2,1):OUTREL(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcv(2,1):OUTREL(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcvx(2,1):OUTREL(2):\
		rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):snd(2,1):INREL(2):\
		snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):snd(2,1):INREL(2):\
		rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):sndx(2,1):INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcv(2,1):OUTREL(2):rcv(2,1):\
		OUTREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcvx(2,1):OUTREL(2):rcv(2,1):\
		OUTREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		sndrel(2,1):OUTREL(2):rcv(2,1):OUTREL(2):rcvx(2,1):\
		OUTREL(2):rcvrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):snd(2,1):INREL(2):snd(2,1):\
		INREL(2):snddis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):sndx(2,1):INREL(2):snd(2,1):\
		INREL(2):rcvdis1(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept1(2,1):DATAXFER(2):\
		rcvrel(2,1):INREL(2):snd(2,1):INREL(2):snd(2,1):\
		INREL(2):sndrel(2,1):IDLE(2)",
	"IDLE(2):listen(2,1):INCON(2):accept2(2,1,4):IDLE(2):\
		passcon(4,1):DATAXFER(4):snddis1(4,1):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		snddis2(4,3):INCON(4):snddis1(4,1):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		snddis2(4,3):INCON(4):rcvdis1(4,1):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		snddis2(4,3):INCON(4):accept1(4,1):DATAXFER(4):\
		rcvdis1(4,1):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		snddis2(4,3):INCON(4):accept2(4,1,2):IDLE(4):\
		passcon(2,1):DATAXFER(2):snddis1(2,1):IDLE(2)",
 	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		rcvdis3(4,1):INCON(4):snddis1(4,3):IDLE(4)",
 	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		rcvdis3(4,1):INCON(4):rcvdis1(4,3):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		rcvdis3(4,3):INCON(4):accept1(4,1):DATAXFER(4):\
		rcvdis1(4,1):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		rcvdis3(4,3):INCON(4):accept2(4,1,2):IDLE(4):\
		passcon(2,1):DATAXFER(2):snddis1(2,1):IDLE(2)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		accept3(4,1,2):INCON(4):passcon(2,1):DATAXFER(2):\
		rcvdis2(4,3):IDLE(4):snddis1(2,1):IDLE(2)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		accept3(4,1,2):INCON(4):passcon(2,1):DATAXFER(2):\
		snddis1(4,3):IDLE(4):snddis1(2,1):IDLE(2)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		accept3(4,1,2):INCON(4):passcon(2,1):DATAXFER(2):\
		accept1(4,3):DATAXFER(4):snddis1(2,1):IDLE(2):\
		snddis1(4,3):IDLE(4)",
	"IDLE(4):listen(4,1):INCON(4):listen(4,3):INCON(4):\
		accept3(4,1,2):INCON(4):passcon(2,1):DATAXFER(2):\
		snddis1(2,1):IDLE(2):accept2(4,3,2):IDLE(4):\
		passcon(2,3):DATAXFER(2):rcvdis1(2,3):IDLE(2)",
	"IDLE(4):unbind(4,3):UNBND(4):free(4,3):close(4,3)",
	"IDLE(2):unbind(2,1):UNBND(2):free(2,1):close(2,1)",
};

#define NACTIVE		(sizeof( active_cases )/sizeof(char *))
#define NPASSIVE	(sizeof( passive_cases )/sizeof(char *))

struct endpoint {
	char	*provider;
	char	*addr;
	int		alen;
	char	*conn;
	int		clen;
	int		qlen;
	int		fd;
} tp[4] = {
	{	NULL,	NULL,	0,	NULL,	0,	0,	0,	},
	{	NULL,	NULL,	0,	NULL,	0,	1,	0,	},
	{	NULL,	NULL,	0,	NULL,	0,	0,	0,	},
	{	NULL,	NULL,	0,	NULL,	0,	2,	0,	},
};

#define NPROVIDERS	(sizeof( tp )/sizeof(struct endpoint))

struct	t_info	info[ NPROVIDERS ];			
struct	t_bind	*req[ NPROVIDERS ];
struct	t_bind	*ret[ NPROVIDERS ];
struct	t_call	*rcall[ NPROVIDERS ];
struct	t_call	*scall[ NPROVIDERS ];

int		id1;
int		id2;
int		id3;
int		under_test;
int		active;
int		ord_fudge;
char	*fnct;
char	tmpfyle[128];

int		(*fn)() = NULL;
int		t_getstate();
int		t_sync();
int		t_look();

#define	GETSTATE	(fn == t_getstate)
#define	SYNC		(fn == t_sync)
#define	LOOK		(fn == t_look)

main(argc,argv) 
int	argc;
char	*argv[];
{
	int		cpid;

	for( fnct = argv[0]; *fnct; fnct++ );
	while( fnct > argv[0] && *(fnct-1) != '/' ) fnct--;

	setup(fnct);
	funct(fnct);

	prep("Initialize variables and structures.");
	start
		expecting( VOID );
		if( !strcmp( fnct, "t_getstate" ) )
			fn = t_getstate;
		else if( !strcmp( fnct, "t_sync" ) )
			fn = t_sync;
		else if( !strcmp( fnct, "t_look" ) )
			fn = t_look;
		else
			severr("%s%s%s%s",
			  "Unknown name '",fnct,"'\n",
			  "expected:	t_getstate, t_sync, or t_look.");
		strcpy(tmpfyle,fnct);
		strcat(tmpfyle,".tmp");
		tp[0].provider = TPVCFILE0;
		tp[0].addr = TPVCADDR0;
		tp[0].alen = TPVCALEN0;
		tp[0].conn = TPVCCONN0;
		tp[0].clen = TPVCCLEN0;
		
		tp[1].provider = TPVCFILE1;
		tp[1].addr = TPVCADDR1;
		tp[1].alen = TPVCALEN1;
		tp[1].conn = TPVCCONN1;
		tp[1].clen = TPVCCLEN1;
		
		tp[2].provider = TPVCFILE2;
		tp[2].addr = TPVCADDR2;
		tp[2].alen = TPVCALEN2;
		tp[2].conn = TPVCCONN2;
		tp[2].clen = TPVCCLEN2;
		
		tp[3].provider = TPVCFILE3;
		tp[3].addr = TPVCADDR3;
		tp[3].alen = TPVCALEN3;
		tp[3].conn = TPVCCONN3;
		tp[3].clen = TPVCCLEN3;
	finis	

	prep("Initialize the semaphore mechanism.");
	start
		seminit( 2 );
	finis

	abstract("Establish communications between two processes.");
	abstract("At each state transition in the active user session ");
	abstract("call %s to verify the %s returned is the", 
		fnct, LOOK?"event":"state");
	abstract("current %s.  The parent process acts as the active",
		LOOK?"event":"state");
	abstract("user while the child process acts as the passive user.");
	abstract("Endpoint id no's are given within parentheses for each");
	abstract("individual test case, where the first id refers to the");
	abstract("active user endpoint, and the second id refers to the");
	abstract("passive user endpoint.");
	start
		break_on( ERROR );
		cpid = fork();
		abort_on( ERROR );
		if( !cpid ) {
			under_test = active = 0;
			do_all( active_cases, NACTIVE );
			if( SYNC )
				p_sema();
			done();
		}
		else {
			under_test = active = 1;
			do_all( active_cases, NACTIVE );
			if( SYNC )
				v_sema();
			expecting( cpid );
			wait( NULL );
		}
	finis

	abstract("Establish communications between two processes.");
	abstract("At each state transition in the passive user session ");
	abstract("call %s to verify the %s returned is the",
		fnct, LOOK?"event":"state");
	abstract("current %s.  The parent process acts as the passive",
		LOOK?"event":"state");
	abstract("user while the child process acts as the active user.");
	abstract("Endpoint id no's are given within parentheses for each");
	abstract("individual test case, where the first id refers to the");
	abstract("passive user endpoint, and the second id refers to the");
	abstract("active user endpoint.  The third id for accept2 and");
	abstract("accept3 refers to the passive user endpoint to which");
	abstract("the connection is passed.");
	start
		break_on( ERROR );
		cpid = fork();
		abort_on( ERROR );
		if( !cpid ) {
			under_test = 0;
			active = 1;
			do_all( passive_cases, NPASSIVE );
			if( SYNC )
				p_sema();
			done();
		}
		else {
			under_test = 1;
			active = 0;
			do_all( passive_cases, NPASSIVE );
			if( SYNC )
				v_sema();
			expecting( cpid );
			wait( NULL );
		}
	finis
	abstract("Open a temp file.  Call %s with the file",fnct);
	abstract("file descriptor of the temp file.  Verify that");
	abstract("%s fails and sets t_errno to TBADF",fnct);
	start
		break_on( ERROR );
		bad_file_des();
	finis
	done();
}
bad_file_des() {
	int	fd;				/* xport provider file des	*/

	prep("Open a temp file: '%s'", tmpfyle);
	start
		fd = temp_open( tmpfyle, O_RDWR | O_CREAT, 0664 );
	finis
	testing("Call %s with the file descriptor for the temp file.",fnct);
	testing("Verify %s returns (-1) and sets t_errno to TBADF.",fnct);
	start
		
		if( SYNC ) {
			expecting( FAILURE );
			if( t_sync( fd ) == -1 ) {
				if ( t_errno == TSYSERR )
					warning( "%s\nt_sync set t_errno to TSYSERR",ADVISORY );
				else {
					expecting( TBADF );
					t_sync( fd );
				}
			}
		} else {
			expecting( TBADF );
			(*fn)( fd );
		}
	finis
	cleanup("Close the temp file.");
	start
		close(fd);
	finis
}

void do_all( tsts, ntsts )
char *tsts[];
int ntsts;
{
	int	i;

	for( i = 0; i < ntsts; i++ ) {
		do_test( tsts[i] );
	}
}
void do_test( teststr ) 
char *teststr;
{
	char		*str;
	char		*fmt_string();

	if( under_test ) {
		abstract("Check the following sequence:");
		while( str = fmt_string( teststr ) )
			abstract("%s",str);
		start
			do_case( teststr );
		finis
	}
	else {
		do_case( teststr );
	}
}
void do_case( teststr )
char *teststr;
{
	vfunctptr	fnct;
	vfunctptr	get_step();
	int		semflg;

	while( fnct = get_step( teststr, &semflg ) ) {
		ord_fudge = 0;
#define FAST
#ifdef	FAST
		if( semflg ) {
#endif
			v_sema();
			p_sema();
#ifdef	FAST
		}
#endif
		expecting( SUCCESS );
		(*fnct)();
#ifndef	FAST
		v_sema();
		p_sema();
#endif
		if( ord_fudge ) 
			break;
	}
}
vfunctptr get_step( ptr, semflg )
char *ptr;
int  *semflg;
{
	static	char	*oldptr = NULL;
	static	char	*loc = NULL;
	char	token[ 64 ];
	char	*tptr;
	int		i;
	if( ptr != oldptr )
		oldptr = loc = ptr;
	tptr = token;

	if( !*loc )
		return( NULL );	/* end of string	*/
	while( *loc && *loc != '(' ) {
#ifdef	DEVELOPMENT
		if( *loc == ':' )
			warning("error in test data");
#endif
		if( *loc != '\t' ) 
			*tptr++ = *loc++;
		else
			loc++;
	}
	*tptr = '\0';
#ifdef	DEVELOPMENT
		if( *loc != '(' )
			warning("error in test data");
#endif
	loc++;
	if( under_test )	
		id1 = *loc++ - '1';
	else
		id2 = *loc++ - '1';
	if( *loc == ',' ) {
		loc++;	/* skip over ',' */
		if( under_test )	
			id2 = *loc++ - '1';
		else
			id1 = *loc++ - '1';
	}
	if( *loc == ',' ) {
		loc++;	/* skip over ',' */
		id3 = *loc++ - '1';
	}
	loc++;	/* skip over ')' */

	if( *loc ) loc++; 	/* skip over ':'	*/

	for( i=0; i<ASIZE; i++ ) {
		expecting( VOID );
		if( !strcmp( token, atable[i].name ) ) {
			*semflg = atable[i].sflg;
			return( atable[i].fn );
		}
	}
	warning("unrecognized token in test data");
	return( NULL );
}
void do_listen() {
	if ( active ) {
#ifdef	DEVELOPMENT
		if( under_test )
			warning("do_listen called for active test");
#endif
		scall[id2]->addr.len = tp[id2].clen;
		memcpy( scall[id2]->addr.buf, tp[id2].conn, tp[id2].clen );
		set_flags();
		expecting( TNODATA );
		t_connect( tp[id1].fd, scall[id2], rcall[id2] );
		v_sema();
	} 
	else {		/* passive user */
		p_sema();
#ifdef	DEVELOPMENT	
		if( !under_test ) {
			warning("do_listen called for active test");
			t_listen( tp[id1].fd, rcall[id2] );
			return;
		}
#endif	
		chk_event( tp[id1].fd, T_LISTEN );
		prep("Call t_listen");
		start
			t_listen( tp[id1].fd, rcall[id2] );
		finis
		prep("Check the addr returned by t_listen corresponds to");
		prep("the active user.");
		start
#ifdef	NPACK
			if( rcall[id2]->addr.len == 12 ) {
				rcall[id2]->addr.buf[10] = '\0';
				rcall[id2]->addr.buf[11] = '\0';
			}
#endif	
			find_addr("list req",tp[id2].conn,tp[id2].clen );

			find_addr("list ret",rcall[id2]->addr.buf, rcall[id2]->addr.len );
			expecting( VOID );
			if( tp[id2].clen != rcall[id2]->addr.len ||
			  memcmp( rcall[id2]->addr.buf, tp[id2].conn, tp[id2].clen ) )
				error("Wrong addr returned by t_listen.");
		finis

	}
}
void do_connect1()
{
	do_connect( 0 );
}
void do_connect2()
{
	int	oflag;

	if( active ) {
		if( under_test ) {
			prep("Call fcntl to get the file status flags.  If the");
			prep("O_NDELAY file status flag is not set, call fcntl");
			prep("again to set it.");
			start
				set_flags();
			finis
		}
		else {
			set_flags();
		}
	}
	do_connect( 1 );
}
void set_flags() {
	int	oflag;

	expecting( VOID );
	oflag = fcntl( tp[id1].fd, F_GETFL, NULL );
	if( !(oflag & O_NDELAY ) ) {
		expecting( SUCCESS );
		fcntl( tp[id1].fd, F_SETFL, oflag | O_NDELAY );
	}
}
void do_connect( async )
int async;
{
	if ( active ) {
		if ( under_test ) {

			prep("Initialize snd[rcv]call structures.  Call t_connect.");
			start
				scall[id2]->addr.len = tp[id2].clen;
				memcpy( scall[id2]->addr.buf, tp[id2].conn, 
				  tp[id2].clen );
	
				if ( async ) 
					expecting( TNODATA );
				t_connect( tp[id1].fd, scall[id2], rcall[id2] );
#ifdef	NPACK
				if( rcall[id2]->addr.len == 12 ) {
						rcall[id2]->addr.buf[10] = '\0';
						rcall[id2]->addr.buf[11] = '\0';
				}
#endif

				if( !async )  {
					prep("Check that the address returned by t_connect");
					prep("corresponds to the passive user.");
					start
					
					find_addr("conn req",scall[id2]->addr.buf,scall[id2]->addr.len);
					find_addr("conn ret",rcall[id2]->addr.buf,rcall[id2]->addr.len);
						expecting( VOID );
						if( tp[id2].clen != rcall[id2]->addr.len ||
						  memcmp( rcall[id2]->addr.buf, tp[id2].conn, 
						  tp[id2].clen ) )
							error("Wrong addr returned by t_connect");
					finis
				}									
			finis
		}
#ifdef	DEVELOPMENT
		else {
				warning("do_connect called when active and !under test");

				scall[id2]->addr.len = tp[id2].clen;
				memcpy( scall[id2]->addr.buf, tp[id2].conn, 
				  tp[id2].clen );
	
				if ( async ) 
					expecting( TNODATA );
				t_connect( tp[id1].fd, scall[id2], rcall[id2] );
#ifdef	NPACK
				if( rcall[id2]->addr.len == 12 ) {
						rcall[id2]->addr.buf[10] = '\0';
						rcall[id2]->addr.buf[11] = '\0';
				}
#endif

		}
#endif
	}
	else {		/* passive user won't call do_connect under test */
#ifdef	DEVELOPMENT
		if (under_test)
			warning("do_connect called for passive test");
#endif

		t_listen( tp[id1].fd, rcall[id2] );
#ifdef	NPACK
		if( rcall[id2]->addr.len == 12 ) {
			rcall[id2]->addr.buf[10] = '\0';
			rcall[id2]->addr.buf[11] = '\0';
		}
#endif

		find_addr("list req",tp[id2].conn,tp[id2].clen );

		find_addr("list ret",rcall[id2]->addr.buf, rcall[id2]->addr.len );
		expecting( VOID );
		if( tp[id2].clen != rcall[id2]->addr.len ||
		  memcmp( rcall[id2]->addr.buf, tp[id2].conn, tp[id2].clen ) )
			error("Wrong addr returned by t_listen.");

		if ( !async ) {	/* in asynch accept @ rcvconnect */
			t_accept( tp[id1].fd, tp[id1].fd, rcall[id2] );
		}
	}
}
void do_rcvconnect() {
	if ( active ) {
		p_sema();
#ifdef	DEVELOPMENT
		if( !under_test ) {
			warning("do_rcvconnect called for passive test");
			t_rcvconnect( tp[id1].fd, rcall[id2] );
			return;
		}
#endif
		chk_event( tp[id1].fd, T_CONNECT );
		prep("Call t_rcvconnect.");
		start
			t_rcvconnect( tp[id1].fd, rcall[id2] );
		finis
#ifdef	NPACK
		if( rcall[id2]->addr.len == 12 ) {
			rcall[id2]->addr.buf[10] = '\0';
			rcall[id2]->addr.buf[11] = '\0';
		}
#endif
	prep("Check that the address returned by t_rcvconnect");
		prep("corresponds to the passive user.");
		start
			find_addr("conn req",scall[id2]->addr.buf,scall[id2]->addr.len);
			find_addr("rcvc ret",rcall[id2]->addr.buf,rcall[id2]->addr.len);
			expecting( VOID );
			if( tp[id2].clen != rcall[id2]->addr.len ||
			  memcmp( rcall[id2]->addr.buf, tp[id2].conn, 
			  tp[id2].clen ) )
				error("Wrong addr returned by t_rcvconnect");
		finis

	} 
	else {		/* passive user */
#ifdef	DEVELOPMENT
		if( under_test )
			warning("do_rcvconnect called for passive test");
#endif
/*
 *		always want to do accept1 when testing active side
 */
		t_accept( tp[id1].fd, tp[id1].fd, rcall[id2] );
		v_sema();
	}
}
void do_passcon() {
	return;
	/* passcon is actually done by accept2 */
}
void do_accept1() {
	do_accept( id1, id2 );
}
void do_accept2() {
	do_accept( id3, id3 );
}
void do_accept3() {
	do_accept( id3, id3 );
}
void do_accept( resid, conid )
int	resid;
int	conid;
{
	if( !active ) {
#ifdef	DEVELOPMENT
		if ( !under_test ) {
			warning("do_accept called for active test");
			t_accept( tp[id1].fd, tp[resid].fd, rcall[id2] );
			v_sema();
			return;
		}			
#endif

		prep("Call t_accept.");
		start
			t_accept( tp[id1].fd, tp[resid].fd, rcall[id2] );
		finis
		v_sema();
	}
	else {
#ifdef	DEVELOPMENT
		if ( under_test ) 
			warning("do_accept called for active test");
#endif
		p_sema();
		t_rcvconnect( tp[id1].fd, rcall[conid] );
#ifdef	NPACK
		if( rcall[conid]->addr.len == 12 ) {
			rcall[conid]->addr.buf[10] = '\0';
			rcall[conid]->addr.buf[11] = '\0';
		}
#endif
		find_addr("conn req",tp[conid].conn,tp[conid].clen);
		find_addr("rcvc ret",rcall[conid]->addr.buf,rcall[conid]->addr.len);
		expecting( VOID );
		if( tp[conid].clen != rcall[conid]->addr.len ||
		  memcmp( rcall[conid]->addr.buf, tp[conid].conn, 
		  tp[conid].clen ) )
			error("Wrong addr returned by t_rcvconnect");
	}
}
void do_snddis1() {
	do_snddis();
}
void do_snddis2() {
	do_snddis();
}
void do_snddis() {
	if( under_test ) {

		prep("Call t_snddis.");
		start
			t_snddis( tp[id1].fd, rcall[id2] );
		finis
		v_sema();
	}
	else {
		p_sema();
		t_rcvdis( tp[id1].fd, rcall[id2] );
	}
}
void do_rcvdis1() {
	do_rcvdis();
}
void do_rcvdis2() {
	do_rcvdis();
}
void do_rcvdis3() {
	do_rcvdis();
}
void do_rcvdis() {
	if ( under_test ) {
		p_sema();

		prep("Call t_rcvdis.");
		start
			t_rcvdis( tp[id1].fd, rcall[id2] );
		finis
	}
	else {
		t_snddis( tp[id1].fd, rcall[id2] );
		v_sema();
	}
}
void do_sndrel() {
	
	if( info[id1].servtype != T_COTS_ORD ) {
		warning("%s%s%s%s",
		  "Orderly release not supported by this provider.\n",
		  "Tests of T_OUTREL and T_INREL will be suppressed.\n",
		  "This test should be re-run with a provider that ",
		  "supports orderly release.");
		do_snddis1();
		ord_fudge = 1;
		return;
	}
	if ( under_test ) {

		prep("Call t_sndrel.");
		start
			t_sndrel( tp[id1].fd );
		finis
		v_sema();
	}
	else {
		p_sema();
		t_rcvrel( tp[id1].fd );
	}
}
void do_rcvrel() {

	if( info[id1].servtype != T_COTS_ORD ) {
		warning("%s%s%s%s",
		  "Orderly release not supported by this provider.\n",
		  "Tests of T_OUTREL and T_INREL will be suppressed.\n",
		  "This test should be re-run with a provider that ",
		  "supports orderly release.");
		do_rcvdis1();
		ord_fudge = 1;
		return;
	}
	if ( under_test ) {
		p_sema();
		chk_event( tp[id1].fd, T_ORDREL );
		prep("Call t_rcvrel.");
		start
			t_rcvrel( tp[id1].fd );
		finis
	}
	else {
		t_sndrel( tp[id1].fd );
		v_sema();
	}
}
void do_rcvx() {
	do_recv( T_EXPEDITED );
}
void do_rcv() {
	do_recv( 0 );
}
void do_recv( ex )
int ex;
{
	if( under_test ) {
		p_sema();
		if( ex ) 
			chk_event( tp[id1].fd, T_EXDATA );
		else
			chk_event( tp[id1].fd, T_DATA );
		prep("Call t_rcv for %s.  Check the received data.",
		  ex?"expedited data":"normal data");
		start
			int	z;
			int	flags;

			t_rcv( tp[id1].fd, &z, sizeof( int ), &flags );
			continue_on( ERROR );
			if( z != 0x2317 )
				error("Received incorrect data.");
		finis
	} 
	else {
		int	z;

		z = 0x2317;
		t_snd( tp[id1].fd, &z, sizeof( int ), ex );
		v_sema();
	}
}
void do_sndx() {
	do_send( T_EXPEDITED );
}
void do_snd() {
	do_send( 0 );
}
void do_send( ex ) 
int ex;
{
	if ( under_test ) {

		prep("Call t_snd with %s.",ex?"expedited data":"normal data");
		start
			int z;

			z = 0x1723;
			t_snd( tp[id1].fd, &z, sizeof(int), ex );	
			v_sema();
		finis
	} 
	else {
		int	z;
		int	flags;

		p_sema();
		t_rcv( tp[id1].fd, &z, sizeof( int ), &flags );
		if( z != 0x1723 )
			error("Received incorrect data.");

	}
}
void do_open() {
	if( under_test ) {
		prep("Call t_open to open the transport provider: '%s'.",
		  tp[id1].provider );
		start
			tp[id1].fd=t_open(tp[id1].provider,O_RDWR,&info[id1]);
		finis
	}
	else
			tp[id1].fd=t_open(tp[id1].provider,O_RDWR,&info[id1]);
}
void do_alloc() {
	if( under_test ) {
		prep("Call t_alloc for some struct t_binds and struct t_calls.");
		start
			grab_space( id1 );
			grab_space( id2 );
		finis
	}
	else
		grab_space( id1 );
		grab_space( id2 );
}
grab_space( id )
int id;
{
	char	*t_alloc();

	ret[id] = (struct t_bind *) t_alloc( tp[id1].fd, T_BIND, T_ADDR );
	req[id] = (struct t_bind *) t_alloc( tp[id1].fd, T_BIND, T_ADDR );
	rcall[id] = (struct t_call *) t_alloc( tp[id1].fd, T_CALL, T_ADDR );
	scall[id] = (struct t_call *) t_alloc( tp[id1].fd, T_CALL, 
	  T_ADDR | T_UDATA );
}
void do_bind() {
	if( under_test ) {
		prep("Copy the address to be bound into the addr buffer, and");
		prep("call t_bind.");
		start
			req[id1]->addr.len = tp[id1].alen;
			memcpy( req[id1]->addr.buf, tp[id1].addr, tp[id1].alen );
			req[id1]->qlen = tp[id1].qlen;
			t_bind(tp[id1].fd,req[id1],ret[id1]);
		finis
		prep("Check that the requested address was the one bound.");
		start
			find_addr("bind req",req[id1]->addr.buf,req[id1]->addr.len);
			find_addr("bind ret",ret[id1]->addr.buf,ret[id1]->addr.len);
			expecting( VOID );
			if( tp[id1].alen != ret[id1]->addr.len ||
			  memcmp( ret[id1]->addr.buf, tp[id1].addr, 
			  tp[id1].alen ) )
				error("Wrong addr returned by t_bind");
		finis
	}
	else {
		req[id1]->addr.len = tp[id1].alen;
		memcpy( req[id1]->addr.buf, tp[id1].addr, tp[id1].alen );
		req[id1]->qlen = tp[id1].qlen;
		t_bind(tp[id1].fd,req[id1],ret[id1]);
		find_addr("bind req",req[id1]->addr.buf,req[id1]->addr.len);
		find_addr("bind ret",ret[id1]->addr.buf,ret[id1]->addr.len);
		expecting( VOID );
		if( tp[id1].alen != ret[id1]->addr.len ||
		  memcmp( ret[id1]->addr.buf, tp[id1].addr, 
		  tp[id1].alen ) )
			error("Wrong addr returned by t_bind");
	}
}
void do_unbind() {
	if ( under_test ) {
		prep("call t_unbind.");
		start
			t_unbind(tp[id1].fd);
		finis
	}
	else
		t_unbind(tp[id1].fd);
}
void do_free() {
	if( under_test ) {
		cleanup("Call t_free to free the allocated structures.");
		start
			chuck_space( id1 );
			chuck_space( id2 );
		finis
	}
	else {
		chuck_space( id1 );
		chuck_space( id2 );
	}
}
chuck_space( id )
int id;
{
	t_free( ret[id], T_BIND );
	t_free( req[id], T_BIND );
	t_free( rcall[id], T_CALL );
	t_free( scall[id], T_CALL );
}
void do_close() {
	if( under_test ) {
		cleanup("Call t_close to close the transport provider.");
		start
			t_close(tp[id1].fd);
		finis
	}
	else
		t_close(tp[id1].fd);
}
char *fmt_string( str )
char *str;
{
	static	char	*oldptr = NULL;
	static	char	*loc = NULL;
	static	char	line[ 128 ];
	char			*tptr;
	int				i;

	if( str != oldptr )
		oldptr = loc = str;
	tptr = line;

	if( !*loc )
		return( NULL );	/* end of string	*/
	i = 0; 
	while( *loc && ( i < 57 || *loc != ':' ) ) {
		if( *loc != '\t' ) {
			*tptr++ = *loc;
			i++;
		}
		loc++;
	}
	*tptr = '\0';
	return( line );
}
void chk_unbnd() {
	chk_state( tp[id1].fd, T_UNBND );
}
void chk_idle() {
	chk_state( tp[id1].fd, T_IDLE );
}
void chk_incon() {
	chk_state( tp[id1].fd, T_INCON );
}
void chk_outcon() {
	chk_state( tp[id1].fd, T_OUTCON );
}
void chk_dataxfer() {
	chk_state( tp[id1].fd, T_DATAXFER );
}
void chk_outrel() {
	chk_state( tp[id1].fd, T_OUTREL );
}
void chk_inrel() {
	chk_state( tp[id1].fd, T_INREL );
}
chk_state( fd, state ) 
int fd;
int state;
{
	char *st_name;
	char *sname();
	static int	kludge = 0;

	if( !under_test )
		return;
	if (LOOK)
		chk_event( fd, 0 );
	else {
		st_name = sname( state );
		testing("Call %s, verify that the state returned is %s.",
		  fnct, st_name);
		start
			continue_on( ERROR );
			if( SYNC && ( state == T_INREL || state == T_OUTREL )) {
				static int inflag = 0;
				static int outflag = 0;
				int st;

				expecting( VOID );
				st = t_sync( fd );
				if( st != state ) {
					if( st == T_OUTREL && state == T_INREL ) {
						if( !inflag ) {
							inflag=1;
							kludge=1;
							warning("%s\nt_sync returned T_OUTREL instead of T_INREL",ADVISORY);
						}
						else kludge = 0;
					}
					else if( st = T_INREL && state == T_OUTREL ) {
						if( !outflag ) {
							outflag=1;
							warning("%s\nt_sync returned T_INREL instead of T_OUTREL",ADVISORY);
							kludge=1;
						}
						else kludge = 0;
					}
					else {
						expecting( state );
						t_sync( fd );
					}
				}
			}
			else {
				expecting( state );
				(*fn)( fd );
			}
		finis
		if ( SYNC ) {
			testing("Fork and exec a subtest, which calls t_sync");
			testing("expecting a return state of %s.",st_name);
			start
				prep("Write the file descriptor and state to a tmp file.");
				start
					int	tfd;
					break_on ( ERROR );		
					tfd = temp_open( tmpfyle, O_RDWR | O_CREAT, 0664 );
					expecting( sizeof( int ) );
					write( tfd, &fd, sizeof( int ) );
					expecting( sizeof( int ) );
					write( tfd, &state, sizeof( int ) );
					expecting( sizeof( int ) );
					write( tfd, &kludge, sizeof( int ) );
					close( tfd );
				finis
				continue_on( ERROR );
				subtest( "t_sync0" );
			finis
		}
	}
}
chk_event( fd, event ) 
int fd;
int event;
{
	char *ev_name;
	char *ename();

	if( !under_test )
		return;

	ev_name = ename( event );	
	testing("Call t_look, verify that the event returned is %s.",ev_name);
	start
		continue_on( ERROR );
		expecting( event );
		t_look( fd );
	finis
}
#ifdef	DEVELOPMENT
print_addr( str, addr, len ) 
char *str;
char *addr;
int	len;
{
	int	i;
	sv_printf("%s: ",str);
	for( i=0; addr && i<len ; i++ )	
		sv_printf("\\%o",(addr[i]&0x00ff));
	sv_printf("[%d]\n",len);
}
#endif
