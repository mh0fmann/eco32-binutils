	.text

	.set	const_sym,0x24
	.global	global_sym

	nop
	nop
local_sym:
	nop
	nop

	beq	$4,$5,0x24
	beq	$4,$5,const_sym
	beq	$4,$5,local_sym
	beq	$4,$5,local_sym+0x24
	beq	$4,$5,local_sym-0x24
	beq	$4,$5,global_sym
	beq	$4,$5,global_sym+0x24
	beq	$4,$5,global_sym-0x24
