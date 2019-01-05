	.text

	.global	global_sym

	nop
	nop
local_sym:
	nop
	nop

	addi	$4,$5,0x23
	addi	$4,$5,local_sym
	addi	$4,$5,local_sym+0x23
	addi	$4,$5,local_sym-0x23
	addi	$4,$5,global_sym
	addi	$4,$5,global_sym+0x23
	addi	$4,$5,global_sym-0x23
