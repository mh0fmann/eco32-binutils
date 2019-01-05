	.text

	.global	global_sym

	nop
	nop
local_sym:
	nop
	nop

	ldhi	$4,0x23
	ldhi	$4,local_sym
	ldhi	$4,local_sym+0x23
	ldhi	$4,local_sym-0x23
	ldhi	$4,global_sym
	ldhi	$4,global_sym+0x23
	ldhi	$4,global_sym-0x23
