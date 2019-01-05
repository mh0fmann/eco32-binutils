#include "as.h"
#include "safe-ctype.h"
#include "opcode/eco32.h"

#include <stdlib.h>

extern const eco32_opc_info_t eco32_opc_info[ECO32_INSN_COUNT];

const char comment_chars[] = "#";
const char line_separator_chars[] = ";";
const char line_comment_chars[] = "#";

static struct hash_control *opcode_hash_control;

/* allow synthesized instructions from the beginning */
static int synth_instr = 1;

const char FLT_CHARS[] = "rRsSfFdDxXpP";
const char EXP_CHARS[] = "eE";

/* debugging */
static int debug_new_fixups = 0;
static int debug_app_fixups = 0;
static int debug_gen_relocs = 0;


static void
eco32_syn(int syn)
{
  synth_instr = syn;
}


const pseudo_typeS md_pseudo_table[] =
{
  { "align",  s_align_bytes,  0 },
  { "byte",   cons,           1 },
  { "hword",  cons,           2 },
  { "word",   cons,           4 },
  { "syn",    eco32_syn,      1 },
  { "nosyn",  eco32_syn,      0 },
  { NULL ,    NULL,           0 }
};


symbolS *
md_undefined_symbol (char *name ATTRIBUTE_UNUSED)
{
  return NULL;
}


void
md_operand (expressionS *op __attribute__((unused)))
{
  /* empty for now */
}


/* This function is called once, at assembler startup time. It sets
   up the hash table with all the opcodes in it, and also initializes
   some aliases for compatibility with other assemblers. */

void
md_begin (void)
{
  const eco32_opc_info_t *opcode;
  opcode_hash_control = hash_new ();

  /* Insert names into hash table. */
  for (opcode = eco32_opc_info; opcode->name; opcode++)
  hash_insert (opcode_hash_control, opcode->name, (char *) opcode);

  bfd_set_arch_mach (stdoutput, TARGET_ARCH, 0);
}


/* Parse an expression and then restore the input line pointer. */

static char *
parse_exp_save_ilp (char *s, expressionS *op)
{
  char *save = input_line_pointer;
  input_line_pointer = s;
  /*
   * THIS IS NOT THE CORRECT WAY TO DO THIS.
   * Check schould be elsewhere (in big switch over opc)
   * and check for other strange ways of writing!
   */
  if (*s == ',')
  {
    as_bad(_("expected an immediate value but got %s"), s);
    exit(-1);
  }

  expression (op);
  s = input_line_pointer;
  input_line_pointer = save;
  return s;
}


static int
parse_register_operand(char **op)
{
  char *op_end = *op;
  int regno = 0;
  if (*op_end != '$')
  {
    as_bad(_("expecting register ($), got: >%s< "), op_end);
    return -1;
  }
  op_end++;  /* skip $ */
  regno = REGNO(op_end[0]);
  op_end++;  /* skip number */
  if (*op_end != 0 && *op_end != ',' && *op_end != '$')
  {
    regno = (regno * 10) + REGNO(op_end[0]);
    op_end++;  /* skip number */
  }
  if (regno < 0 || regno > 31)
  {
    as_bad(_("illegal register number %d\n"), regno);
    return -1;
  }
  *op += (regno > 9 ? 3 : 2);
  return regno;
}


static void gen_fix_exp(fragS *frag,
                        int where,
                        int size,
                        expressionS *exp,
                        int pcrel,
                        bfd_reloc_code_real_type r_type)
{
  if (debug_new_fixups)
  {
    char buffer[500];
    sprintf(buffer,
            "new fix: frag=%p, where=0x%x, size=0x%x, exp=%p\n"
            "         pcrel=%d, r_type=%s (%d)\n",
            frag, where, size, exp, pcrel,
            bfd_get_reloc_code_name(r_type), r_type);
    fprintf(stderr, "%s", buffer);
  }
  fix_new_exp(frag, where, size, exp, pcrel, r_type);
}


/* This is the guts of the machine-dependent assembler. STR points to
   a machine dependent instruction. This function is supposed to emit
   the frags/bytes it assembles to. */

void
md_assemble (char *str)
{
  char *op_start;
  char *op_end;
  int nlen;

  eco32_opc_info_t *opcode;
  unsigned int iword;
  char *where;
  int regno;
  int immediate;
  char pend;
  expressionS arg;

  /* Drop leading whitespace. */
  while (*str == ' ')
    str++;

  /* Find the op code end. */
  op_start = str;
  nlen = 0;
  for (op_end = op_start;
       *op_end && !is_end_of_line[*op_end & 0xff] && *op_end != ' ';
       op_end++)
    nlen++;

  pend = *op_end;
  *op_end = 0;

  if (nlen == 0)
     as_bad (_("can't find opcode"));

  opcode = (eco32_opc_info_t *) hash_find (opcode_hash_control, op_start);
  *op_end = pend;

  if (opcode == NULL)
  {
    as_bad (_("unknown opcode %s"), op_start);
    return;
  }

  while (ISSPACE (*op_end))
    op_end++;

  /* now branch according to insn format */
  switch (opcode->itype)
  {
    case ECO32_N:
      /* no operands */
      {
        iword = 0x0;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != 0)
        {
          /* accept an optional immediate operand */
          op_end = parse_exp_save_ilp (op_end, &arg);
          switch (arg.X_op)
          {
            case O_constant:
              immediate = arg.X_add_number;
              iword |= immediate & LO_HALF;
              break;
          }
        }
        where = frag_more (ECO32_INSN_BYTE);
        iword |= opcode->opcode << OPC_OFFSET;
        md_number_to_chars (where, iword, ECO32_INSN_BYTE);
      }
      break;
    case ECO32_J:
      /* no registers and a sign-extended 26 bit PC-relative word offset */
      {
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }
        op_end = parse_exp_save_ilp (op_end, &arg);
        where = frag_more (ECO32_INSN_BYTE);
        iword = 0x0;
        iword |= opcode->opcode << OPC_OFFSET;
        gen_fix_exp (frag_now,                      /* which frag */
                     where - frag_now->fr_literal,  /* where in frag */
                     ECO32_INSN_BYTE,               /* length */
                     &arg,                          /* expression */
                     TRUE,                          /* PC-relative? */
                     BFD_RELOC_ECO32_R26            /* reloc type */
                    );
        md_number_to_chars (where, iword, ECO32_INSN_BYTE);
      }
      break;
    case ECO32_R:
      /* one register operand */
      {
        where = frag_more(ECO32_INSN_BYTE);
        iword = 0x0;
        iword |= opcode->opcode << OPC_OFFSET;
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }

        regno = parse_register_operand(&op_end);
        if (regno == -1)
          return;
        iword |= regno << R_reg;
        md_number_to_chars (where, iword, ECO32_INSN_BYTE);
      }
      break;
    case ECO32_RH:
      /* one register and the lower 16 bits of a word */
      {
        int dest;

        dest = parse_register_operand(&op_end);
        if (dest == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }
        op_end = parse_exp_save_ilp (op_end, &arg);
        switch (arg.X_op)
        {
          case O_constant:
            immediate = arg.X_add_number;
            iword = 0x0;
            iword |= opcode->opcode << OPC_OFFSET;
            iword |= dest << RHH_dest_reg;
            iword |= immediate & LO_HALF;
            where = frag_more(ECO32_INSN_BYTE);
            md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            break;
          case O_symbol:
            where = frag_more (ECO32_INSN_BYTE);
            iword = 0x0;
            iword |= opcode->opcode << OPC_OFFSET;
            iword |= dest<<RH_dest_reg;
            gen_fix_exp (frag_now,                      /* which frag */
                         where - frag_now->fr_literal,  /* where in frag */
                         ECO32_INSN_BYTE,               /* length */
                         &arg,                          /* expression */
                         FALSE,                         /* PC-relative? */
                         BFD_RELOC_ECO32_L16            /* reloc type */
                        );
            md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            break;
        }
      }
      break;
    case ECO32_RHH:
      /* one register and the upper 16 bits of a word */
      {
        int dest;

        dest = parse_register_operand(&op_end);
        if (dest == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }
        op_end = parse_exp_save_ilp (op_end, &arg);
        switch (arg.X_op)
        {
          case O_constant:
            immediate = arg.X_add_number;
            iword = 0x0;
            iword |= opcode->opcode << OPC_OFFSET;
            iword |= dest<<RHH_dest_reg;
            iword |= (immediate >> 16) & LO_HALF;
            where = frag_more(ECO32_INSN_BYTE);
            md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            break;
          case O_symbol:
            where = frag_more (ECO32_INSN_BYTE);
            iword = 0x0;
            iword |= opcode->opcode << OPC_OFFSET;
            iword |= dest<<RHH_dest_reg;
            gen_fix_exp (frag_now,                      /* which frag */
                         where - frag_now->fr_literal,  /* where in frag */
                         ECO32_INSN_BYTE,               /* length */
                         &arg,                          /* expression */
                         FALSE,                         /* PC-relative? */
                         BFD_RELOC_ECO32_H16            /* reloc type */
                        );
            md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            break;
        }
      }
      break;
    case ECO32_RLS:
    case ECO32_RRS:
      /* two registers and a sign-extended halfword */
      {
        int dest = 0;
        int src = 0;

        dest = parse_register_operand(&op_end);
        if (dest == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;

        src = parse_register_operand(&op_end);
        if (src == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }
        op_end = parse_exp_save_ilp (op_end, &arg);
        switch (arg.X_op)
        {
          case O_constant:
            immediate = arg.X_add_number;
            /* check if ldhi is needed */
            if (!synth_instr)
            {
              where = frag_more(ECO32_INSN_BYTE);
              iword = 0x0;
              iword |= opcode->opcode << OPC_OFFSET;
              iword |= dest << RRS_dest_reg;
              iword |= src << RRS_src_reg;
              iword |= immediate & LO_HALF;
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            }
            else
            {
              if ((immediate & 0xFFFF8000) == 0x00000000 ||
                  (immediate & 0xFFFF8000) == 0xFFFF8000)
              {
                where = frag_more(ECO32_INSN_BYTE);
                iword = 0x0;
                iword |= opcode->opcode << OPC_OFFSET;
                iword |= dest << RRS_dest_reg;
                iword |= src << RRS_src_reg;
                iword |= immediate & LO_HALF;
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              }
              else
              {
                /* code: ldhi $1,imm; or $1,$1,imm; add $1,$1,src; op dst,$1,0 */
                /* ldhi $1,imm */
                iword = 0x0;
                iword |= OP_LDHI << OPC_OFFSET;
                iword |= ECO32_AUX << 16;
                iword |= (immediate & HI_HALF) >> 16;
                where = frag_more(ECO32_INSN_BYTE);
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                /* or $1,$1,imm */
                iword = 0x0;
                iword |= OP_ORI << OPC_OFFSET;
                iword |= ECO32_AUX << RRS_dest_reg;
                iword |= ECO32_AUX << RRS_src_reg;
                iword |= immediate & LO_HALF;
                where = frag_more(ECO32_INSN_BYTE);
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                switch (opcode->itype)
                {
                  case ECO32_RRS:
                    /* opc-1 dest,$src,$1 */
                    iword = 0x0;
                    iword |= (opcode->opcode - 1) << OPC_OFFSET;
                    iword |= dest << RRR_dest_reg;
                    iword |= src << RRR_src1_reg;
                    iword |= ECO32_AUX << RRR_src2_reg;
                    where = frag_more(ECO32_INSN_BYTE);
                    md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                    break;
                  case ECO32_RLS:
                    /* add $1,$1,src */
                    iword = 0x0;
                    iword |= OP_ADD << OPC_OFFSET;
                    iword |= ECO32_AUX << RRR_dest_reg;
                    iword |= ECO32_AUX << RRR_src1_reg;
                    iword |= src << RRR_src2_reg;
                    where = frag_more(ECO32_INSN_BYTE);
                    md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                    /* op dst,$1,0 */
                    iword = 0x0;
                    iword |= opcode->opcode << OPC_OFFSET;
                    iword |= ECO32_AUX << RRS_src_reg;
                    iword |= dest << RRS_dest_reg;
                    where = frag_more(ECO32_INSN_BYTE);
                    md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                    break;
                }
              }
            }
            break;
          case O_symbol:
            if (!synth_instr)
            {
              where = frag_more (ECO32_INSN_BYTE);
              iword = 0x0;
              iword |= opcode->opcode << OPC_OFFSET;
              iword |= dest << RRS_dest_reg;
              iword |= src << RRS_src_reg;
              gen_fix_exp (frag_now,                      /* which frag */
                           where - frag_now->fr_literal,  /* where in frag */
                           ECO32_INSN_BYTE,               /* length */
                           &arg,                          /* expression */
                           FALSE,                         /* PC-relative? */
                           BFD_RELOC_ECO32_L16            /* reloc type */
                          );
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            }
            else
            {
              /* code: ldhi $1,con; or $1,$1,con; add $1,$1,src; op dst,$1,0 */
              where = frag_more (ECO32_INSN_BYTE);
              /* ldhi $1,imm */
              iword = 0x0;
              iword |= OP_LDHI << OPC_OFFSET;
              iword |= ECO32_AUX << 16;
              gen_fix_exp (frag_now,                      /* which frag */
                           where - frag_now->fr_literal,  /* where in frag */
                           ECO32_INSN_BYTE,               /* length */
                           &arg,                          /* expression */
                           FALSE,                         /* PC-relative? */
                           BFD_RELOC_ECO32_H16            /* reloc type */
                          );
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              where = frag_more (ECO32_INSN_BYTE);
              /* or $1,$1,imm */
              iword = 0x0;
              iword |= OP_ORI << OPC_OFFSET;
              iword |= ECO32_AUX << RRS_dest_reg;
              iword |= ECO32_AUX << RRS_src_reg;
              gen_fix_exp (frag_now,                      /* which frag */
                           where - frag_now->fr_literal,  /* where in frag */
                           ECO32_INSN_BYTE,               /* length */
                           &arg,                          /* expression */
                           FALSE,                         /* PC-relative? */
                           BFD_RELOC_ECO32_L16            /* reloc type */
                          );
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              switch (opcode->itype)
              {
                case ECO32_RRS:
                  /* opc-1 dest,$src,$1 */
                  iword = 0x0;
                  iword |= (opcode->opcode - 1) << OPC_OFFSET;
                  iword |= dest << RRR_dest_reg;
                  iword |= src << RRR_src1_reg;
                  iword |= ECO32_AUX << RRR_src2_reg;
                  where = frag_more(ECO32_INSN_BYTE);
                  md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                  break;
                case ECO32_RLS:
                  /* add $1,$1,src */
                  iword = 0x0;
                  iword |= OP_ADD << OPC_OFFSET;
                  iword |= ECO32_AUX << RRR_dest_reg;
                  iword |= ECO32_AUX << RRR_src1_reg;
                  iword |= src << RRR_src2_reg;
                  where = frag_more(ECO32_INSN_BYTE);
                  md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                  /* op dst,$1,0 */
                  iword = 0x0;
                  iword |= opcode->opcode << OPC_OFFSET;
                  iword |= ECO32_AUX << RRS_src_reg;
                  iword |= dest << RRS_dest_reg;
                  where = frag_more(ECO32_INSN_BYTE);
                  md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                  break;
              }
            }
            break;
        }
      }
      break;
    case ECO32_RRH:
      /* two registers and a zero-extended halfword */
      {
        int dest = 0;
        int src = 0;

        dest = parse_register_operand(&op_end);
        if (dest == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;

        src = parse_register_operand(&op_end);
        if (src == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }
        op_end = parse_exp_save_ilp (op_end, &arg);

        switch (arg.X_op)
        {
          case O_constant:
            immediate = arg.X_add_number;
            if (!synth_instr)
            {
              where = frag_more(ECO32_INSN_BYTE);
              iword = 0x0;
              iword |= opcode->opcode << OPC_OFFSET;
              iword |= dest << RRS_dest_reg;
              iword |= src << RRS_src_reg;
              iword |= immediate & LO_HALF;
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            }
            else
            {
              /* check if ldhi is needed */
              if ((immediate & HI_HALF) == 0)
              {
                where = frag_more(ECO32_INSN_BYTE);
                iword = 0x0;
                iword |= opcode->opcode << OPC_OFFSET;
                iword |= dest << RRS_dest_reg;
                iword |= src << RRS_src_reg;
                iword |= immediate & LO_HALF;
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              }
              else
              {
                /* code: ldhi $1,imm; ori $1,$1,imm; opc-1 dst,src,$1 */
                /* ldhi $1,imm */
                iword = 0x0;
                iword |= OP_LDHI << OPC_OFFSET;
                iword |= ECO32_AUX << 16;
                iword |= (immediate & HI_HALF) >> 16;
                where = frag_more(ECO32_INSN_BYTE);
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                /* ori $1,$1,imm */
                iword = 0x0;
                iword |= OP_ORI << OPC_OFFSET;
                iword |= ECO32_AUX << RRS_dest_reg;
                iword |= ECO32_AUX << RRS_src_reg;
                iword |= immediate & LO_HALF;
                where = frag_more(ECO32_INSN_BYTE);
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
                /* opc-1 dest,$src,$1 */
                iword = 0x0;
                iword |= (opcode->opcode - 1) << OPC_OFFSET;
                iword |= dest << RRR_dest_reg;
                iword |= src << RRR_src1_reg;
                iword |= ECO32_AUX << RRR_src2_reg;
                where = frag_more(ECO32_INSN_BYTE);
                md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              }
            }
            break;
          case O_symbol:
            if (!synth_instr)
            {
              where = frag_more (ECO32_INSN_BYTE);
              iword = 0x0;
              iword |= opcode->opcode << OPC_OFFSET;
              iword |= dest << RRS_dest_reg;
              iword |= src << RRS_src_reg;
              gen_fix_exp (frag_now,                      /* which frag */
                           where - frag_now->fr_literal,  /* where in frag */
                           ECO32_INSN_BYTE,               /* length */
                           &arg,                          /* expression */
                           FALSE,                         /* PC-relative? */
                           BFD_RELOC_ECO32_L16            /* relocation type */
                          );
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            }
            else
            {
              where = frag_more (ECO32_INSN_BYTE);
              /* ldhi $1,imm */
              iword = 0x0;
              iword |= OP_LDHI << OPC_OFFSET;
              iword |= ECO32_AUX << 16;
              gen_fix_exp (frag_now,                      /* which frag */
                           where - frag_now->fr_literal,  /* where in frag */
                           ECO32_INSN_BYTE,               /* length */
                           &arg,                          /* expression */
                           FALSE,                         /* PC-relative? */
                           BFD_RELOC_ECO32_H16            /* reloc type */
                          );
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              where = frag_more (ECO32_INSN_BYTE);
              /* or $1,$1,imm */
              iword = 0x0;
              iword |= OP_ORI << OPC_OFFSET;
              iword |= ECO32_AUX << RRS_dest_reg;
              iword |= ECO32_AUX << RRS_src_reg;
              gen_fix_exp (frag_now,                      /* which frag */
                           where - frag_now->fr_literal,  /* where in frag */
                           ECO32_INSN_BYTE,               /* length */
                           &arg,                          /* expression */
                           FALSE,                         /* PC-relative? */
                           BFD_RELOC_ECO32_L16            /* reloc type */
                          );
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
              /* opc-1 dest,$src,$1 */
              iword = 0x0;
              iword |= (opcode->opcode - 1) << OPC_OFFSET;
              iword |= dest << RRR_dest_reg;
              iword |= src << RRR_src1_reg;
              iword |= ECO32_AUX << RRR_src2_reg;
              where = frag_more(ECO32_INSN_BYTE);
              md_number_to_chars (where, iword, ECO32_INSN_BYTE);
            }
            break;
        }
      }
      break;
    case ECO32_RRB:
      /* two registers and a sign-extended 16 bit PC-relative word offset */
      {
        int src1;
        int src2;

        src1 = parse_register_operand(&op_end);
        if (src1 == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;

        src2 = parse_register_operand(&op_end);
        if (src2 == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end == 0)
        {
          as_bad(_("expecting an operand in %s"), str);
          return;
        }

        op_end = parse_exp_save_ilp (op_end, &arg);

        where = frag_more (ECO32_INSN_BYTE);
        iword = 0x0;
        iword |= opcode->opcode << OPC_OFFSET;
        iword |= src1 << RRB_src1;
        iword |= src2 << RRB_src2;
        gen_fix_exp (frag_now,                      /* which frag */
                     where - frag_now->fr_literal,  /* where in frag */
                     ECO32_INSN_BYTE,               /* length */
                     &arg,                          /* expression */
                     TRUE,                          /* PC-relative? */
                     BFD_RELOC_ECO32_R16            /* relocation type */
                    );
        md_number_to_chars (where, iword, ECO32_INSN_BYTE);
      }
      break;
    case ECO32_RRR:
      /* three registers */
      {
        where = frag_more(ECO32_INSN_BYTE);
        iword = 0x0;
        iword |= opcode->opcode << OPC_OFFSET;

        regno = parse_register_operand(&op_end);
        if (regno == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("RRR OP2 expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        iword |= regno << RRR_dest_reg;
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;

        regno = parse_register_operand(&op_end);
        if (regno == -1)
          return;
        while (ISSPACE (*op_end))
          op_end++;
        if (*op_end != ',')
        {
          as_bad(_("RRR OP3 expecting ',' delimited operand, "
                   "got >%s< : %s"), op_end, str);
          return;
        }
        iword |= regno << RRR_src1_reg;
        op_end++;  /* skip comma */
        while (ISSPACE (*op_end))
          op_end++;

        regno = parse_register_operand(&op_end);
        if (regno == -1)
          return;
        iword |= regno << RRR_src2_reg;
        md_number_to_chars (where, iword, ECO32_INSN_BYTE);
      }
      break;
    default:
      /* this should never happen */
      {
        as_bad (_("unrecognizable instruction %s\n"), str);
      }
      break;
  }
}


/* Turn a string in input_line_pointer into a floating point constant
   of type type, and store the appropriate bytes in *LITP. The number
   of LITTLENUMS emitted is stored in *SIZEP. An error message is
   returned, or NULL on OK. */

char *
md_atof (int type, char *litP, int *sizeP)
{
  int prec;
  LITTLENUM_TYPE words[4];
  char *t;
  int i;

  switch (type)
  {
    case 'f':
      prec = 2;
      break;

    case 'd':
      prec = 4;
      break;

    default:
      *sizeP = 0;
      return _("bad call to md_atof");
  }

  t = atof_ieee (input_line_pointer, type, words);
  if (t)
    input_line_pointer = t;

  *sizeP = prec * 2;

  for (i = prec - 1; i >= 0; i--)
  {
    md_number_to_chars (litP, (valueT) words[i], 2);
    litP += 2;
  }

  return NULL;
}


const char *md_shortopts = "";

struct option md_longopts[] =
{
  { NULL, no_argument, NULL, 0 }
};

size_t md_longopts_size = sizeof (md_longopts);


/* We have no target specific options yet, so these
   next two functions are empty. */

int
md_parse_option (int c ATTRIBUTE_UNUSED, char *arg ATTRIBUTE_UNUSED)
{
  return 0;
}


void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
}


/* Apply a fixup to the object file. */

void
md_apply_fix (fixS *fixP, valueT *val, segT seg ATTRIBUTE_UNUSED)
{
  char *bp;
  unsigned int iword;

  if (debug_app_fixups)
  {
    char buffer[500];
    symbolS *addsym;
    symbolS *subsym;
    addsym = fixP->fx_addsy;
    subsym = fixP->fx_subsy;
    sprintf(buffer,
            "app fix: frag=%p, where=0x%lx, addsym=%s, subsym=%s\n"
            "         offset=0x%lx\n"
            "         pcrel=%u, done=%u, noovr=%u, signed=%u, size=%d\n"
            "         r_type=%s (%d), file=%s, line=%u\n"
            "         val=0x%lx\n",
            fixP->fx_frag, fixP->fx_where,
            addsym ? S_GET_NAME(addsym) : "(null)",
            subsym ? S_GET_NAME(subsym) : "(null)",
            fixP->fx_offset,
            fixP->fx_pcrel, fixP->fx_done, fixP->fx_no_overflow,
            fixP->fx_signed, fixP->fx_size,
            bfd_get_reloc_code_name(fixP->fx_r_type), fixP->fx_r_type,
            fixP->fx_file, fixP->fx_line, *val);
    fprintf(stderr, "%s", buffer);
  }

  /* We don't currently support subtracting a symbol. */
  if (fixP->fx_subsy != (symbolS *) NULL)
    as_bad_where (fixP->fx_file,
                  fixP->fx_line,
                  _("subtracting a symbol is not supported"));

  if (fixP->fx_addsy == NULL && !fixP->fx_pcrel)
  {
    /* Apply fixups for PC-relative relocations (i.e. jumps) to symbols
       that have already been resolved (typically local symbols). */
    bp = fixP->fx_frag->fr_literal + fixP->fx_where;
    iword = (((unsigned int) (unsigned char) bp[0]) << 24) |
            (((unsigned int) (unsigned char) bp[1]) << 16) |
            (((unsigned int) (unsigned char) bp[2]) <<  8) |
            (((unsigned int) (unsigned char) bp[3]) <<  0);
    switch (fixP->fx_r_type)
    {
      case BFD_RELOC_ECO32_R16:
        /* *val == target - . - 4 */
        iword |= (*val >> 2) & 0x0000ffff;
        md_number_to_chars (bp, iword, ECO32_INSN_BYTE);
        break;
      case BFD_RELOC_ECO32_R26:
        /* *val == target - . - 4 */
        iword |= (*val >> 2) & 0x03ffffff;
        md_number_to_chars (bp, iword, ECO32_INSN_BYTE);
        break;
      case BFD_RELOC_32:
      case BFD_RELOC_16:
      case BFD_RELOC_8:
        md_number_to_chars (bp, *val, fixP->fx_size);
        break;
      default:
        as_fatal(_("unknown relocation type %s (%d)"),
                 bfd_get_reloc_code_name(fixP->fx_r_type),
                 fixP->fx_r_type);
    }
    fixP->fx_done = 1;
    if (debug_app_fixups)
    {
      fprintf(stderr, "         <fixup done>\n");
    }
  }
}


/* Put number into target byte order. */

void
md_number_to_chars (char *ptr, valueT use, int nbytes)
{
  number_to_chars_bigendian (ptr, use, nbytes);
}


/* Generate a machine-dependent relocation. */

arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixP)
{
  bfd_reloc_code_real_type code;
  arelent *relP;

  if (debug_gen_relocs)
  {
    char buffer[500];
    symbolS *addsym;
    symbolS *subsym;
    addsym = fixP->fx_addsy;
    subsym = fixP->fx_subsy;
    sprintf(buffer,
            "gen rel: frag=%p, where=%ld, addsym=%s, subsym=%s, offset=%ld\n"
            "         pcrel=%u, done=%u, noovr=%u, signed=%u, size=%d\n"
            "         r_type=%s (%d), file=%s, line=%u\n",
            fixP->fx_frag, fixP->fx_where,
            addsym ? S_GET_NAME(addsym) : "(null)",
            subsym ? S_GET_NAME(subsym) : "(null)",
            fixP->fx_offset,
            fixP->fx_pcrel, fixP->fx_done, fixP->fx_no_overflow,
            fixP->fx_signed, fixP->fx_size,
            bfd_get_reloc_code_name(fixP->fx_r_type), fixP->fx_r_type,
            fixP->fx_file, fixP->fx_line);
    fprintf(stderr, "%s", buffer);
  }

  code = fixP->fx_r_type;
  switch (code)
  {
    case BFD_RELOC_32:
    case BFD_RELOC_16:
    case BFD_RELOC_8:
    case BFD_RELOC_ECO32_H16:
    case BFD_RELOC_ECO32_L16:
      /* all parameters ok */
      break;
    case BFD_RELOC_ECO32_R16:
    case BFD_RELOC_ECO32_R26:
      /* PC-relative relocs encode distance from next insn */
      /* it is easier to correct this here instead of at link time */
      fixP->fx_offset -= ECO32_INSN_BYTE;
      break;
    default:
      as_bad_where (fixP->fx_file, fixP->fx_line,
                    _("unknown fixup type"));
      return NULL;
  }

  relP = xmalloc (sizeof (arelent));
  gas_assert (relP != 0);
  relP->sym_ptr_ptr = xmalloc (sizeof (asymbol *));
  *relP->sym_ptr_ptr = symbol_get_bfdsym (fixP->fx_addsy);
  relP->address = fixP->fx_frag->fr_address + fixP->fx_where;

  relP->addend = fixP->fx_offset;

  relP->howto = bfd_reloc_type_lookup (stdoutput, code);
  if (relP->howto == NULL)
  {
    const char *name;

    name = S_GET_NAME (fixP->fx_addsy);
    if (name == NULL)
      name = _("<unknown>");
    as_fatal (_("Cannot generate relocation type for symbol %s, code %s"),
              name, bfd_get_reloc_code_name (code));
  }

  return relP;
}
