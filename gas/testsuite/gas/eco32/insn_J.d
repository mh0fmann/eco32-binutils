#as:
#objdump: -dr
#name: J instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <label_1>:
   0:	a8 00 00 00 	j 4 <label_2>

00000004 <label_2>:
   4:	b3 ff ff fe 	jal 0 <label_2\+0xfffffffc>
