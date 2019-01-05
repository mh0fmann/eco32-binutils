#as:
#objdump: -dr
#name: ECO32_32 relocations

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <local_sym-0x8>:
	...

00000008 <local_sym>:
	...
  10:	00 00 00 24 	nop
  14:	00 00 00 24 	nop
	...
			18: R_ECO32_32	.text\+0x8
			1c: R_ECO32_32	.text\+0x2c
			20: R_ECO32_32	.text-0x1c
			24: R_ECO32_32	global_sym
			28: R_ECO32_32	global_sym\+0x24
			2c: R_ECO32_32	global_sym-0x24
