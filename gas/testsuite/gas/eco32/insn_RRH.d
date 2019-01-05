#as:
#objdump: -dr
#name: RRH instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <.text>:
   0:	1c a4 00 06 	mului \$4,\$5,0x0006
   4:	2c a4 00 06 	divui \$4,\$5,0x0006
   8:	3c a4 00 06 	remui \$4,\$5,0x0006
   c:	44 a4 00 06 	andi \$4,\$5,0x0006
  10:	4c a4 00 06 	ori \$4,\$5,0x0006
  14:	54 a4 00 06 	xori \$4,\$5,0x0006
  18:	5c a4 00 06 	xnori \$4,\$5,0x0006
  1c:	64 a4 00 06 	slli \$4,\$5,0x0006
  20:	6c a4 00 06 	slri \$4,\$5,0x0006
  24:	74 a4 00 06 	sari \$4,\$5,0x0006
