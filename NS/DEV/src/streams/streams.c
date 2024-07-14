#ident	"@(#)streams.c	1.3"
#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

main() {
	setup("streams");

	funct("ioctl");
	subtest("ioctl_1");
	subtest("ioctl_2");
	funct("open");
	subtest("open");
	funct("close");
	subtest("close");
	funct("read");
	subtest("read");
	funct("write");
	subtest("write");
	done();
	/* NOTREACHED */
}
