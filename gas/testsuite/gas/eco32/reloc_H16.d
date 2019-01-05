#as:
#objdump: -dr
#name: ECO32_H16 relocations

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <local_sym-0x8>:
	...

00000008 <local_sym>:
	...
  10:	7c 04 00 00 	ldhi \$4,0x00000000
  14:	7c 04 00 00 	ldhi \$4,0x00000000
			14: R_ECO32_H16	.text\+0x8
  18:	7c 04 00 00 	ldhi \$4,0x00000000
			18: R_ECO32_H16	.text\+0x2b
  1c:	7c 04 00 00 	ldhi \$4,0x00000000
			1c: R_ECO32_H16	.text-0x1b
  20:	7c 04 00 00 	ldhi \$4,0x00000000
			20: R_ECO32_H16	global_sym
  24:	7c 04 00 00 	ldhi \$4,0x00000000
			24: R_ECO32_H16	global_sym\+0x23
  28:	7c 04 00 00 	ldhi \$4,0x00000000
			28: R_ECO32_H16	global_sym-0x23
