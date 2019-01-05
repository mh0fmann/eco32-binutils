#as:
#objdump: -dr
#name: N instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <.text>:
   0:	78 00 00 07 	cctl 7
   4:	b8 00 00 00 	trap
   8:	bc 00 00 00 	rfx
   c:	e8 00 00 00 	tbs
  10:	ec 00 00 00 	tbwr
  14:	f0 00 00 00 	tbri
  18:	f4 00 00 00 	tbwi
  1c:	00 00 00 00 	nop
