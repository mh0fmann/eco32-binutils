#as:
#objdump: -dr
#name: RLS instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <.text>:
   0:	c0 a4 00 06 	ldw \$4,\$5,0x0006
   4:	c4 a4 00 06 	ldh \$4,\$5,0x0006
   8:	c8 a4 00 06 	ldhu \$4,\$5,0x0006
   c:	cc a4 00 06 	ldb \$4,\$5,0x0006
  10:	d0 a4 00 06 	ldbu \$4,\$5,0x0006
  14:	d4 a4 00 06 	stw \$4,\$5,0x0006
  18:	d8 a4 00 06 	sth \$4,\$5,0x0006
  1c:	dc a4 00 06 	stb \$4,\$5,0x0006
  20:	f8 a4 00 06 	ldlw \$4,\$5,0x0006
  24:	fc a4 00 06 	stcw \$4,\$5,0x0006
