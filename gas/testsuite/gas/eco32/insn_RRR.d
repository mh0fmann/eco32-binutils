#as:
#objdump: -dr
#name: RRR instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <.text>:
   0:	00 a6 20 00 	add \$4,\$5,\$6
   4:	08 a6 20 00 	sub \$4,\$5,\$6
   8:	10 a6 20 00 	mul \$4,\$5,\$6
   c:	18 a6 20 00 	mulu \$4,\$5,\$6
  10:	20 a6 20 00 	div \$4,\$5,\$6
  14:	28 a6 20 00 	divu \$4,\$5,\$6
  18:	30 a6 20 00 	rem \$4,\$5,\$6
  1c:	38 a6 20 00 	remu \$4,\$5,\$6
  20:	40 a6 20 00 	and \$4,\$5,\$6
  24:	48 a6 20 00 	or \$4,\$5,\$6
  28:	50 a6 20 00 	xor \$4,\$5,\$6
  2c:	58 a6 20 00 	xnor \$4,\$5,\$6
  30:	60 a6 20 00 	sll \$4,\$5,\$6
  34:	68 a6 20 00 	slr \$4,\$5,\$6
  38:	70 a6 20 00 	sar \$4,\$5,\$6
