#as:
#objdump: -dr
#name: ECO32_R26 relocations

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <local_sym-0x8>:
	...

00000008 <local_sym>:
	...
  10:	a8 00 00 00 	j 14 <local_sym\+0xc>
			10: R_ECO32_R26	\*ABS\*\+0x20
  14:	a8 00 00 00 	j 18 <local_sym\+0x10>
			14: R_ECO32_R26	\*ABS\*\+0x20
  18:	ab ff ff fb 	j 8 <const_sym\+0xffffffe4>
  1c:	a8 00 00 03 	j 2c <local_sym\+0x24>
  20:	ab ff ff f0 	j ffffffe4 <const_sym\+0xffffffc0>
  24:	a8 00 00 00 	j 28 <local_sym\+0x20>
			24: R_ECO32_R26	global_sym-0x4
  28:	a8 00 00 00 	j 2c <local_sym\+0x24>
			28: R_ECO32_R26	global_sym\+0x20
  2c:	a8 00 00 00 	j 30 <const_sym\+0xc>
			2c: R_ECO32_R26	global_sym-0x28
