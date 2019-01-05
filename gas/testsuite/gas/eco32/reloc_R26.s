	.text

	.set	const_sym,0x24
	.global	global_sym

	nop
	nop
local_sym:
	nop
	nop

	j	0x24
	j	const_sym
	j	local_sym
	j	local_sym+0x24
	j	local_sym-0x24
	j	global_sym
	j	global_sym+0x24
	j	global_sym-0x24
