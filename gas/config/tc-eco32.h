#define TC_ECO32 1
#define TARGET_BYTES_BIG_ENDIAN 1
#define WORKING_DOT_WORD

/* This macro is the BFD target name to use when creating the output
   file. This will normally depend upon the `OBJ_FMT' macro. */
#define TARGET_FORMAT "elf32-eco32"

/* This macro is the BFD architecture to pass to `bfd_set_arch_mach'. */
#define TARGET_ARCH bfd_arch_eco32

/* This macro is the BFD machine to pass to `bfd_set_arch_mach'. */
#define TARGET_MACH bfd_mach_eco32

/* These macros must be defined, but it will be a fatal assembler
   error if we ever hit them. */
#define md_estimate_size_before_relax(A, B) \
  (as_fatal (_("estimate size\n")),0)
#define md_convert_frag(B, S, F) \
  (as_fatal (_("convert_frag\n")), 0)

/* PC-relative operands are relative to the addr of the next insn */
#define md_pcrel_from(FIX) \
  ((FIX)->fx_where + (FIX)->fx_frag->fr_address + 4)

#define md_section_align(SEGMENT, SIZE) (SIZE)

/* '$' not allowed in labels because it's the register prefix */
#undef LOCAL_LABELS_DOLLAR
#define LOCAL_LABELS_DOLLAR 0

/* .-foo gets turned into PC relative relocs */
#define DIFF_EXPR_OK
