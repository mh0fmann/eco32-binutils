#as:
#objdump: -dr
#name: ECO32_L16 relocations

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <local_sym-0x8>:
	...

00000008 <local_sym>:
	...
  10:	04 a4 00 23 	addi \$4,\$5,0x0023
  14:	7c 01 00 00 	ldhi \$1,0x00000000
			14: R_ECO32_H16	.text\+0x8
  18:	4c 21 00 00 	ori \$1,\$1,0x0000
			18: R_ECO32_L16	.text\+0x8
  1c:	00 a1 20 00 	add \$4,\$5,\$1
  20:	7c 01 00 00 	ldhi \$1,0x00000000
			20: R_ECO32_H16	.text\+0x2b
  24:	4c 21 00 00 	ori \$1,\$1,0x0000
			24: R_ECO32_L16	.text\+0x2b
  28:	00 a1 20 00 	add \$4,\$5,\$1
  2c:	7c 01 00 00 	ldhi \$1,0x00000000
			2c: R_ECO32_H16	.text-0x1b
  30:	4c 21 00 00 	ori \$1,\$1,0x0000
			30: R_ECO32_L16	.text-0x1b
  34:	00 a1 20 00 	add \$4,\$5,\$1
  38:	7c 01 00 00 	ldhi \$1,0x00000000
			38: R_ECO32_H16	global_sym
  3c:	4c 21 00 00 	ori \$1,\$1,0x0000
			3c: R_ECO32_L16	global_sym
  40:	00 a1 20 00 	add \$4,\$5,\$1
  44:	7c 01 00 00 	ldhi \$1,0x00000000
			44: R_ECO32_H16	global_sym\+0x23
  48:	4c 21 00 00 	ori \$1,\$1,0x0000
			48: R_ECO32_L16	global_sym\+0x23
  4c:	00 a1 20 00 	add \$4,\$5,\$1
  50:	7c 01 00 00 	ldhi \$1,0x00000000
			50: R_ECO32_H16	global_sym-0x23
  54:	4c 21 00 00 	ori \$1,\$1,0x0000
			54: R_ECO32_L16	global_sym-0x23
  58:	00 a1 20 00 	add \$4,\$5,\$1
