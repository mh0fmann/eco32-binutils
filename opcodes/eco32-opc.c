#include "sysdep.h"
#include "opcode/eco32.h"

/* please keep it sorted this way */
const eco32_opc_info_t eco32_opc_info[ECO32_INSN_COUNT] =
{
  { OP_BAD,   ECO32_N,    "bad"   },

  /* RRR */
  { OP_ADD,   ECO32_RRR,  "add"   },
  { OP_SUB,   ECO32_RRR,  "sub"   },
  { OP_MUL,   ECO32_RRR,  "mul"   },
  { OP_MULU,  ECO32_RRR,  "mulu"  },
  { OP_DIV,   ECO32_RRR,  "div"   },
  { OP_DIVU,  ECO32_RRR,  "divu"  },
  { OP_REM,   ECO32_RRR,  "rem"   },
  { OP_REMU,  ECO32_RRR,  "remu"  },
  { OP_AND,   ECO32_RRR,  "and"   },
  { OP_OR,    ECO32_RRR,  "or"    },
  { OP_XOR,   ECO32_RRR,  "xor"   },
  { OP_XNOR,  ECO32_RRR,  "xnor"  },
  { OP_SLL,   ECO32_RRR,  "sll"   },
  { OP_SLR,   ECO32_RRR,  "slr"   },
  { OP_SAR,   ECO32_RRR,  "sar"   },

  /* RRS */
  { OP_ADDI,  ECO32_RRS,  "addi"  },
  { OP_SUBI,  ECO32_RRS,  "subi"  },
  { OP_MULI,  ECO32_RRS,  "muli"  },
  { OP_DIVI,  ECO32_RRS,  "divi"  },
  { OP_REMI,  ECO32_RRS,  "remi"  },

  /* RLS */
  { OP_LDW,   ECO32_RLS,  "ldw"   },
  { OP_LDH,   ECO32_RLS,  "ldh"   },
  { OP_LDHU,  ECO32_RLS,  "ldhu"  },
  { OP_LDB,   ECO32_RLS,  "ldb"   },
  { OP_LDBU,  ECO32_RLS,  "ldbu"  },
  { OP_STW,   ECO32_RLS,  "stw"   },
  { OP_STH,   ECO32_RLS,  "sth"   },
  { OP_STB,   ECO32_RLS,  "stb"   },
  { OP_LDLW,  ECO32_RLS,  "ldlw"  },
  { OP_STCW,  ECO32_RLS,  "stcw"  },

  /* RRH */ 
  { OP_MULUI, ECO32_RRH,  "mului" },
  { OP_DIVUI, ECO32_RRH,  "divui" },
  { OP_REMUI, ECO32_RRH,  "remui" },
  { OP_ANDI,  ECO32_RRH,  "andi"  },
  { OP_ORI,   ECO32_RRH,  "ori"   },
  { OP_XORI,  ECO32_RRH,  "xori"  },
  { OP_XNORI, ECO32_RRH,  "xnori" },
  { OP_SLLI,  ECO32_RRH,  "slli"  },
  { OP_SLRI,  ECO32_RRH,  "slri"  },
  { OP_SARI,  ECO32_RRH,  "sari"  },

  /* RHH */
  { OP_LDHI,  ECO32_RHH,  "ldhi"  },

  /* RRB */
  { OP_BEQ,   ECO32_RRB,  "beq"   },
  { OP_BNE,   ECO32_RRB,  "bne"   },
  { OP_BLE,   ECO32_RRB,  "ble"   },
  { OP_BLEU,  ECO32_RRB,  "bleu"  },
  { OP_BLT,   ECO32_RRB,  "blt"   },
  { OP_BLTU,  ECO32_RRB,  "bltu"  },
  { OP_BGE,   ECO32_RRB,  "bge"   },
  { OP_BGEU,  ECO32_RRB,  "bgeu"  },
  { OP_BGT,   ECO32_RRB,  "bgt"   },
  { OP_BGTU,  ECO32_RRB,  "bgtu"  },

  /* J */
  { OP_J,     ECO32_J,    "j"     },
  { OP_JAL,   ECO32_J,    "jal"   },

  /* R */
  { OP_JR,    ECO32_R,    "jr"    },
  { OP_JALR,  ECO32_R,    "jalr"  },

  /* RH */
  { OP_MVFS,  ECO32_RH,   "mvfs"  },
  { OP_MVTS,  ECO32_RH,   "mvts"  },

  /* N */
  { OP_CCTL,  ECO32_N,    "cctl"  },
  { OP_TRAP,  ECO32_N,    "trap"  },
  { OP_RFX,   ECO32_N,    "rfx"   },
  { OP_TBS,   ECO32_N,    "tbs"   },
  { OP_TBWR,  ECO32_N,    "tbwr"  },
  { OP_TBRI,  ECO32_N,    "tbri"  },
  { OP_TBWI,  ECO32_N,    "tbwi"  },
  /* keep NOP down here - it collides with OP_ADD in disassembly */
  { OP_NOP,   ECO32_N,    "nop"   },
  /* sentinel */
  { 0,        0,          NULL    }
};
