#as:
#objdump: -dr
#name: RRB instructions

.*: +file format elf32-eco32


Disassembly of section .text:

00000000 <label_1>:
   0:	80 85 ff ff 	beq \$4,\$5,0 <label_2\+0xffffffd8>
   4:	84 85 00 08 	bne \$4,\$5,28 <label_2>
   8:	88 85 ff fd 	ble \$4,\$5,0 <label_2\+0xffffffd8>
   c:	8c 85 00 06 	bleu \$4,\$5,28 <label_2>
  10:	90 85 ff fb 	blt \$4,\$5,0 <label_2\+0xffffffd8>
  14:	94 85 00 04 	bltu \$4,\$5,28 <label_2>
  18:	98 85 ff f9 	bge \$4,\$5,0 <label_2\+0xffffffd8>
  1c:	9c 85 00 02 	bgeu \$4,\$5,28 <label_2>
  20:	a0 85 ff f7 	bgt \$4,\$5,0 <label_2\+0xffffffd8>
  24:	a4 85 00 00 	bgtu \$4,\$5,28 <label_2>
