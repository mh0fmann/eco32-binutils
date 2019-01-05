#as:
#objdump: -dr
#name: RRS instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <.text>:
   0:	04 a4 00 06 	addi \$4,\$5,0x0006
   4:	0c a4 00 06 	subi \$4,\$5,0x0006
   8:	14 a4 00 06 	muli \$4,\$5,0x0006
   c:	24 a4 00 06 	divi \$4,\$5,0x0006
  10:	34 a4 00 06 	remi \$4,\$5,0x0006
