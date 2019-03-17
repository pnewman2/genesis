	.text

	/* Initial SSR, PC */
	.long	0xffff00
	.long	reset

	/* All other exceptions spin */
.rept	(0x100-8)/4
	JMP	.
.endr

/*

ROM Cartridge Data

Note that the trailing spaces are deliberate and were carefully counted.

*/
		     /* "012345678901234567890123456789012345678901234567890" */
		     /* "          1         2         3         4         5" */
.org 0x100;	.ascii	"SEGA GENESIS    "
.org 0x110;	.ascii	"(C)AWSM 2019.MAR"
.org 0x120;	.ascii	"TEST GAME 1                                     "
.org 0x150;	.ascii	"TEST GAME 1                                     "
.org 0x180;	.ascii	"GM 12345678-12  "
.org 0x190;	.ascii	"J               "
.org 0x1a0;	.long	0, 0x1000
.org 0x1b0;	.long	0xff0000, 0xffffff
.org 0x1bc;	.ascii	"            "
.org 0x1c8;	.ascii	"AWESOME                                 "
.org 0x1f0;	.ascii	"JUE             "

	.org	0x200
reset:
	JSR	start
	JMP	.
