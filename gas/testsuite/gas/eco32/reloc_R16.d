#as:
#objdump: -dr
#name: ECO32_R16 relocations

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <local_sym-0x8>:
	...

00000008 <local_sym>:
	...
  10:	80 85 00 00 	beq \$4,\$5,14 <local_sym\+0xc>
			10: R_ECO32_R16	\*ABS\*\+0x20
  14:	80 85 00 00 	beq \$4,\$5,18 <local_sym\+0x10>
			14: R_ECO32_R16	\*ABS\*\+0x20
  18:	80 85 ff fb 	beq \$4,\$5,8 <const_sym\+0xffffffe4>
  1c:	80 85 00 03 	beq \$4,\$5,2c <local_sym\+0x24>
  20:	80 85 ff f0 	beq \$4,\$5,ffffffe4 <const_sym\+0xffffffc0>
  24:	80 85 00 00 	beq \$4,\$5,28 <local_sym\+0x20>
			24: R_ECO32_R16	global_sym-0x4
  28:	80 85 00 00 	beq \$4,\$5,2c <local_sym\+0x24>
			28: R_ECO32_R16	global_sym\+0x20
  2c:	80 85 00 00 	beq \$4,\$5,30 <const_sym\+0xc>
			2c: R_ECO32_R16	global_sym-0x28
