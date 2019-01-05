	.text

	.set	const_sym,0x24
	.global	global_sym

	nop
	nop
local_sym:
	nop
	nop

	.word	0x24
	.word	const_sym
	.word	local_sym
	.word	local_sym+0x24
	.word	local_sym-0x24
	.word	global_sym
	.word	global_sym+0x24
	.word	global_sym-0x24
