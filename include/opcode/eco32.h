/**************************************************************/
/* some definition macros to extract stuff from assembler lines */

#define ECO32_INSN_BYTE 4
#define ECO32_MAX_INSN_SIZE (ECO32_INSN_BYTE * 8)

#define ECO32_AUX 1

#define HEX 16

#define LO_HALF 0x0000FFFF
#define HI_HALF 0xFFFF0000
#define OPC_OFFSET 26

#define RRR_dest_reg 11
#define RRR_src1_reg 21
#define RRR_src2_reg 16

#define RRS_dest_reg 16
#define RRS_src_reg 21

#define RHH_dest_reg 16
#define RH_dest_reg 16

#define R_reg 21

#define RRB_src1 21
#define RRB_src2 16

#define REGNO(r) ((r)-'0')

/**************************************************************/

#define ECO32_INSN_COUNT 67

#define OP_BAD		0xec

/* taken from old assembler and sorted new */
#define OP_NOP		0x00

/* RRR */
#define OP_ADD		0x00
#define OP_SUB		0x02
#define OP_MUL		0x04
#define OP_MULU		0x06
#define OP_DIV		0x08
#define OP_DIVU		0x0A
#define OP_REM		0x0C
#define OP_REMU		0x0E
#define OP_AND		0x10
#define OP_OR		0x12
#define OP_XOR		0x14
#define OP_XNOR		0x16
#define OP_SLL		0x18
#define OP_SLR		0x1A
#define OP_SAR		0x1C

/* RRS */
#define OP_ADDI		0x01
#define OP_SUBI		0x03
#define OP_MULI		0x05
#define OP_DIVI		0x09
#define OP_REMI		0x0D

/* RLS */
#define OP_LDW		0x30
#define OP_LDH		0x31
#define OP_LDHU		0x32
#define OP_LDB		0x33
#define OP_LDBU		0x34
#define OP_STW		0x35
#define OP_STH		0x36
#define OP_STB		0x37
#define OP_LDLW		0x3E
#define OP_STCW		0x3F

/* RRH */
#define OP_MULUI	0x07
#define OP_DIVUI	0x0B
#define OP_REMUI	0x0F
#define OP_ANDI		0x11
#define OP_ORI		0x13
#define OP_XORI		0x15
#define OP_XNORI	0x17
#define OP_SLLI		0x19
#define OP_SLRI		0x1B
#define OP_SARI		0x1D

/* RHH */
#define OP_LDHI		0x1F

/* RRB */
#define OP_BEQ		0x20
#define OP_BNE		0x21
#define OP_BLE		0x22
#define OP_BLEU		0x23
#define OP_BLT		0x24
#define OP_BLTU		0x25
#define OP_BGE		0x26
#define OP_BGEU		0x27
#define OP_BGT		0x28
#define OP_BGTU		0x29

/* J */
#define OP_J		0x2A
#define OP_JAL		0x2C

/* R */
#define OP_JR		0x2B
#define OP_JALR		0x2D

/* RH */
#define OP_MVFS		0x38
#define OP_MVTS		0x39

/* N */
#define OP_CCTL		0x1E
#define OP_TRAP		0x2E
#define OP_RFX		0x2F
#define OP_TBS		0x3A
#define OP_TBWR		0x3B
#define OP_TBRI		0x3C
#define OP_TBWI		0x3D

/**************************************************************/

#define ECO32_N 	0x00	/*no operands*/
#define ECO32_R 	0x01	/*one register*/
#define ECO32_RH 	0x02	/*one register and the lower 16 bits of a word*/
#define ECO32_RHH 	0x03	/*one register and the upper 16 bits of a word*/
#define ECO32_RRH 	0x04	/*two registers and a zero-extended halfword*/
#define ECO32_RRS 	0x05	/*two registers and a sign-extended halfword*/
#define ECO32_RLS 	0x15	/*two registers and a sign-extended halfword*/
#define ECO32_RRR 	0x06	/*three registers*/

#define ECO32_RRB 	0x07	/*two registers and a sign-extended 16 bit offset*/
#define ECO32_J 	0x08	/*no registers and a sign-extended 26 bit offset*/

/**************************************************************/

typedef struct eco32_opc_info_t
{
  unsigned int opcode;
  unsigned itype;
  const char *name;
} eco32_opc_info_t;
