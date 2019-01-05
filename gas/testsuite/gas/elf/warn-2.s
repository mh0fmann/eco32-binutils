;# { dg-do assemble }
;# { dg-options "--gdwarf2 --defsym nop_type=0" }
;# { dg-options "--gdwarf2 --defsym nop_type=1" { target ia64-*-* } }
;# { dg-options "--gdwarf2 --defsym nop_type=2" { target or1k*-*-* } }
;# { dg-options "--gdwarf2 --defsym nop_type=3" { target i370-*-* } }

	.offset 40
	
 .ifeq nop_type - 1
	nop 0
 .else
 .ifeq nop_type - 2
	l.nop 0
 .else
 .ifeq nop_type - 3
	nopr 1
 .else
	nop
 .endif
 .endif
 .endif

;# { dg-warning "Warning: dwarf line number information for .* ignored" "" { xfail eco32-*-* i370-*-* mcore-*-* mn10200-*-* moxie-*-* v850*-*-* } 0 }
