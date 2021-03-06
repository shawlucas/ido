#include <string.h>

#include "libu.h"
#include "ucode.h"

// state shared between op_name and op_fmt
struct UiniShared {
    Uopcode op;
    int fmt_index;
};

/* inner function
 * 00480964 uini
 */
static void op_name(struct UiniShared *s, Uopcode op, char name[4]) {
    memcpy(utab[op].opcname, name, 4);
    s->fmt_index = 0;
    s->op = op;
}

/* inner function
 * 00480964 uini
 */
static void op_fmt(struct UiniShared *s, uoperand operand) {
    utab[s->op].format[s->fmt_index++] = operand;
}

/* 
 * 00434720 processargs
 */
void uini(void) {
    struct UiniShared s;
    int i;

    // The volatile ops are partially undefined, format and name not set
    op_name(&s, Uabs,  "ABS "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uadd,  "ADD "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uadj,  "ADJ "); op_fmt(&s, Sdtype); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ualia, "ALIA"); op_fmt(&s, Ssomenumber); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Uand,  "AND "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uaos,  "AOS "); op_fmt(&s, Send);
    op_name(&s, Ubgn,  "BGN "); op_fmt(&s, Sblockno); op_fmt(&s, Slength); op_fmt(&s, Soffset); op_fmt(&s, Send);
    op_name(&s, Ubgnb, "BGNB"); op_fmt(&s, Sblockno); op_fmt(&s, Send);
    op_name(&s, Ubsub, "BSUB"); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uchkt, "CHKT"); op_fmt(&s, Send);
    op_name(&s, Uchkh, "CHKH"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Uchkl, "CHKL"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Uchkn, "CHKN"); op_fmt(&s, Send);
    op_name(&s, Ucia,  "CIA "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Soffset); op_fmt(&s, Ssomenumber); op_fmt(&s, Sconstval); op_fmt(&s, Send);
    op_name(&s, Uclab, "CLAB"); op_fmt(&s, Slabel0); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uclbd, "CLBD"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Ucubd, "CUBD"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Uctrl, "CTRL"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ucomm, "COMM"); op_fmt(&s, Scomment); op_fmt(&s, Send);
    op_name(&s, Ucup,  "CUP "); op_fmt(&s, Sdtype); op_fmt(&s, Slexlev); op_fmt(&s, Sblockno); op_fmt(&s, Spop); op_fmt(&s, Spush); op_fmt(&s, Sexternal); op_fmt(&s, Send);
    op_name(&s, Ucvt,  "CVT "); op_fmt(&s, Sdtype); op_fmt(&s, Sdtype2); op_fmt(&s, Send);
    op_name(&s, Ucvtl, "CVTL"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Udec,  "DEC "); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Udef,  "DEF "); op_fmt(&s, Smtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Udif,  "DIF "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Udiv,  "DIV "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Udup,  "DUP "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uend,  "END "); op_fmt(&s, Sblockno); op_fmt(&s, Send);
    op_name(&s, Uendb, "ENDB"); op_fmt(&s, Sblockno); op_fmt(&s, Send);
    op_name(&s, Uent,  "ENT "); op_fmt(&s, Sdtype); op_fmt(&s, Slexlev); op_fmt(&s, Sblockno); op_fmt(&s, Spop); op_fmt(&s, Spush); op_fmt(&s, Sexternal); op_fmt(&s, Send);
    op_name(&s, Uaent, "AENT"); op_fmt(&s, Sdtype); op_fmt(&s, Slexlev); op_fmt(&s, Sblockno); op_fmt(&s, Spop); op_fmt(&s, Spush); op_fmt(&s, Sexternal); op_fmt(&s, Send);
    op_name(&s, Uequ,  "EQU "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ufjp,  "FJP "); op_fmt(&s, Slabel1); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Ugeq,  "GEQ "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ugrt,  "GRT "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ufill, "FILL"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uidx,  "IDX "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uiequ, "IEQU"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uigeq, "IGEQ"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uigrt, "IGRT"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uileq, "ILEQ"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uiles, "ILES"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uilod, "ILOD"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Soffset); op_fmt(&s, Send);
    op_name(&s, Uineq, "INEQ"); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uinc,  "INC "); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Uinit, "INIT"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Soffset2); op_fmt(&s, Slength); op_fmt(&s, SarrayOffset); op_fmt(&s, Slexlev); op_fmt(&s, Sinitval); op_fmt(&s, Send);
    op_name(&s, Uinn,  "INN "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Uint,  "INT "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uior,  "IOR "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uistr, "ISTR"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Soffset); op_fmt(&s, Send);
    op_name(&s, Uixa,  "IXA "); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Ulab,  "LAB "); op_fmt(&s, Slabel0); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uldef, "LDEF"); op_fmt(&s, Slabel0); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ulbd,  "LBD "); op_fmt(&s, Send);
    op_name(&s, Ulca,  "LCA "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Ssomenumber); op_fmt(&s, Sconstval); op_fmt(&s, Send);
    op_name(&s, Uilda, "ILDA"); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Soffset2); op_fmt(&s, Send);
    op_name(&s, Ulda,  "LDA "); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Soffset2); op_fmt(&s, Send);
    op_name(&s, Uldc,  "LDC "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Sconstval); op_fmt(&s, Send);
    op_name(&s, Uldsp, "LDSP"); op_fmt(&s, Send);
    op_name(&s, Uldap, "LDAP"); op_fmt(&s, Send);
    op_name(&s, Uleq,  "LEQ "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ules,  "LES "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ulex,  "LEX "); op_fmt(&s, Slexlev); op_fmt(&s, Sblockno); op_fmt(&s, Send);
    op_name(&s, Uloc,  "LOC "); op_fmt(&s, Slexlev); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Ulod,  "LOD "); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Umin,  "MIN "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Umax,  "MAX "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Umod,  "MOD "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Urem,  "REM "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Umov,  "MOV "); op_fmt(&s, Sdtype); op_fmt(&s, Slexlev); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Umpy,  "MPY "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Umst,  "MST "); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Umtag, "MTAG"); op_fmt(&s, Ssomenumber); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Umus,  "MUS "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uneg,  "NEG "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uneq,  "NEQ "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Unop,  "NOP "); op_fmt(&s, Send);
    op_name(&s, Unot,  "NOT "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uodd,  "ODD "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uxpar, "XPAR"); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uoptn, "OPTN"); op_fmt(&s, Ssomenumber); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Upar,  "PAR "); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Updef, "PDEF"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Upop,  "POP "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uregs, "REGS"); op_fmt(&s, Slexlev); op_fmt(&s, Ssomenumber); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uret,  "RET "); op_fmt(&s, Send);
    op_name(&s, Urlda, "RLDA"); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Urldc, "RLDC"); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Ssomenumber); op_fmt(&s, Sconstval); op_fmt(&s, Send);
    op_name(&s, Urlod, "RLOD"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Urnd,  "RND "); op_fmt(&s, Sdtype); op_fmt(&s, Sdtype2); op_fmt(&s, Send);
    op_name(&s, Urpar, "RPAR"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Urstr, "RSTR"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Usdef, "SDEF"); op_fmt(&s, Sblockno); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uasym, "ASYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Uhsym, "HSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Usgs,  "SGS "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Usqr,  "SQR "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Usqrt, "SQRT"); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ustep, "STEP"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Ustp,  "STP "); op_fmt(&s, Sblockno); op_fmt(&s, Send);
    op_name(&s, Ustr,  "STR "); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ustsp, "STSP"); op_fmt(&s, Send);
    op_name(&s, Usub,  "SUB "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uswp,  "SWP "); op_fmt(&s, Sdtype); op_fmt(&s, Sdtype2); op_fmt(&s, Send);
    op_name(&s, Utjp,  "TJP "); op_fmt(&s, Slabel1); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Utyp,  "TYP "); op_fmt(&s, Sdtype); op_fmt(&s, Sdtype2); op_fmt(&s, Send);
    op_name(&s, Uujp,  "UJP "); op_fmt(&s, Slabel1); op_fmt(&s, Send);
    op_name(&s, Uuni,  "UNI "); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uxjp,  "XJP "); op_fmt(&s, Sdtype); op_fmt(&s, Slabel1); op_fmt(&s, Slabel2); op_fmt(&s, Slbound); op_fmt(&s, Shbound); op_fmt(&s, Send);
    op_name(&s, Uxor,  "XOR "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ucsym, "CSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uesym, "ESYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ufsym, "FSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ugsym, "GSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ulsym, "LSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Ushl,  "SHL "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ushr,  "SHR "); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Ulnot, "LNOT"); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uicuf, "ICUF"); op_fmt(&s, Sdtype); op_fmt(&s, Spop); op_fmt(&s, Spush); op_fmt(&s, Sexternal); op_fmt(&s, Send);
    op_name(&s, Uisld, "ISLD"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Uisst, "ISST"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Upmov, "PMOV"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Umpmv, "MPMV"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Uunal, "UNAL"); op_fmt(&s, Slexlev); op_fmt(&s, Ssomenumber); op_fmt(&s, Sdtypenum); op_fmt(&s, Send);
    op_name(&s, Uvreg, "VREG"); op_fmt(&s, Sdtype); op_fmt(&s, Smtype); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slength); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Uijp,  "IJP "); op_fmt(&s, Send);
    op_name(&s, Ucg1,  "CG1 "); op_fmt(&s, Send);
    op_name(&s, Ucg2,  "CG2 "); op_fmt(&s, Send);
    op_name(&s, Utpeq, "TPEQ"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Utpne, "TPNE"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Utplt, "TPLT"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Utpgt, "TPGT"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Utpge, "TPGE"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Utple, "TPLE"); op_fmt(&s, Sdtype); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Uubd,  "UBD "); op_fmt(&s, Send);
    op_name(&s, Ulbgn, "LBGN"); op_fmt(&s, Ssomenumber); op_fmt(&s, Send);
    op_name(&s, Ulbdy, "LBDY"); op_fmt(&s, Slexlev); op_fmt(&s, Send);
    op_name(&s, Ultrm, "LTRM"); op_fmt(&s, Send);
    op_name(&s, Ulend, "LEND"); op_fmt(&s, Send);
    op_name(&s, Ussym, "SSYM"); op_fmt(&s, Sdtype); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Sconstval); op_fmt(&s, Send);
    op_name(&s, Usign, "SIGN"); op_fmt(&s, Sdtype); op_fmt(&s, Send);
    op_name(&s, Uildi, "ILDI"); op_fmt(&s, Sdtype); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Soffset); op_fmt(&s, Send);
    op_name(&s, Uisti, "ISTI"); op_fmt(&s, Sdtype); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Soffset); op_fmt(&s, Send);
    op_name(&s, Uirld, "IRLD"); op_fmt(&s, Sdtype); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uirst, "IRST"); op_fmt(&s, Sdtype); op_fmt(&s, Sblockno); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uldrc, "LDRC"); op_fmt(&s, Sdtype); op_fmt(&s, Slength); op_fmt(&s, Sblockno); op_fmt(&s, Send);
    op_name(&s, Urcuf, "RCUF"); op_fmt(&s, Sdtype); op_fmt(&s, Sblockno); op_fmt(&s, Spop); op_fmt(&s, Spush); op_fmt(&s, Sexternal); op_fmt(&s, Send);
    op_name(&s, Umsym, "MSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uksym, "KSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Slexlev); op_fmt(&s, Slength); op_fmt(&s, Send);
    op_name(&s, Uosym, "OSYM"); op_fmt(&s, Sblockno); op_fmt(&s, Soffset); op_fmt(&s, Send);

    // default initialization for the rest of the fields
    for (i = 0; i < sizeof(utab) / sizeof(utab[0]); i++) {
        utab[i].hasattr = false;
        utab[i].hasconst = false;
        utab[i].instlength = 2;
        utab[i].stack_pop = 0;
        utab[i].stack_push = 0;
    }

    utab[Ulod].hasattr  = true;
    utab[Ustr].hasattr  = true;
    utab[Uilod].hasattr = true;
    utab[Uistr].hasattr = true;
    utab[Uildi].hasattr = true;
    utab[Uisti].hasattr = true;
    utab[Uirld].hasattr = true;
    utab[Uirst].hasattr = true;
    utab[Umov].hasattr  = true;
    utab[Uabs].hasattr  = true;
    utab[Uneg].hasattr  = true;
    utab[Usqr].hasattr  = true;
    utab[Uchkn].hasattr = true;
    utab[Uchkt].hasattr = true;
    utab[Ulnot].hasattr = true;
    utab[Unot].hasattr  = true;
    utab[Uodd].hasattr  = true;
    utab[Usqrt].hasattr = true;
    utab[Ucvtl].hasattr = true;
    utab[Udec].hasattr  = true;
    utab[Uinc].hasattr  = true;
    utab[Uchkh].hasattr = true;
    utab[Uchkl].hasattr = true;
    utab[Usgs].hasattr  = true;
    utab[Ucvt].hasattr  = true;
    utab[Urnd].hasattr  = true;
    utab[Uadj].hasattr  = true;
    utab[Uadd].hasattr  = true;
    utab[Udiv].hasattr  = true;
    utab[Umod].hasattr  = true;
    utab[Umpy].hasattr  = true;
    utab[Usub].hasattr  = true;
    utab[Urem].hasattr  = true;
    utab[Uand].hasattr  = true;
    utab[Uequ].hasattr  = true;
    utab[Ugeq].hasattr  = true;
    utab[Ugrt].hasattr  = true;
    utab[Uior].hasattr  = true;
    utab[Uleq].hasattr  = true;
    utab[Ules].hasattr  = true;
    utab[Umax].hasattr  = true;
    utab[Umin].hasattr  = true;
    utab[Uneq].hasattr  = true;
    utab[Ushl].hasattr  = true;
    utab[Ushr].hasattr  = true;
    utab[Uxor].hasattr  = true;
    utab[Uixa].hasattr  = true;
    utab[Uint].hasattr  = true;
    utab[Udif].hasattr  = true;
    utab[Umus].hasattr  = true;
    utab[Uuni].hasattr  = true;
    utab[Uinn].hasattr  = true;
    utab[Uiequ].hasattr = true;
    utab[Uigrt].hasattr = true;
    utab[Uigeq].hasattr = true;
    utab[Uileq].hasattr = true;
    utab[Uiles].hasattr = true;
    utab[Uineq].hasattr = true;
    utab[Usign].hasattr = true;
    utab[Uisld].hasattr = true;
    utab[Uisst].hasattr = true;

    utab[Ucia].hasconst  = true;
    utab[Ucomm].hasconst = true;
    utab[Uinit].hasconst = true;
    utab[Ulca].hasconst  = true;
    utab[Uldc].hasconst  = true;
    utab[Urldc].hasconst = true;
    utab[Ussym].hasconst = true;

    utab[Uadj].instlength  = 4;
    utab[Ubgn].instlength  = 4;
    utab[Ubgnb].instlength = 2; // why set instlength to 2 again?
    utab[Uchkh].instlength = 2;
    utab[Uchkl].instlength = 2;
    utab[Ucia].instlength  = 4;
    utab[Uclab].instlength = 4;
    utab[Uclbd].instlength = 2;
    utab[Ucubd].instlength = 2;
    utab[Uctrl].instlength = 4;
    utab[Ucomm].instlength = 4;
    utab[Ucup].instlength  = 4;
    utab[Ucvt].instlength  = 4;
    utab[Ucvtl].instlength = 2;
    utab[Udec].instlength  = 2;
    utab[Uinc].instlength  = 2;
    utab[Udef].instlength  = 4;
    utab[Udif].instlength  = 4;
    utab[Uint].instlength  = 4;
    utab[Uuni].instlength  = 4;
    utab[Uend].instlength  = 2;
    utab[Uendb].instlength = 2;
    utab[Uent].instlength  = 4;
    utab[Uaent].instlength = 4;
    utab[Uiequ].instlength = 4;
    utab[Uigeq].instlength = 4;
    utab[Uigrt].instlength = 4;
    utab[Uileq].instlength = 4;
    utab[Uiles].instlength = 4;
    utab[Uineq].instlength = 4;
    utab[Ufjp].instlength  = 2;
    utab[Utjp].instlength  = 2;
    utab[Uujp].instlength  = 2;
    utab[Ufill].instlength = 4;
    utab[Uildv].instlength = 4;
    utab[Uilod].instlength = 4;
    utab[Uinit].instlength = 6;
    utab[Uinn].instlength  = 4;
    utab[Uistv].instlength = 4;
    utab[Uistr].instlength = 4;
    utab[Uixa].instlength  = 2;
    utab[Ulab].instlength  = 4;
    utab[Uldef].instlength = 4;
    utab[Ulca].instlength  = 4;
    utab[Uilda].instlength = 6;
    utab[Ulbgn].instlength = 2;
    utab[Ulda].instlength  = 6;
    utab[Uldc].instlength  = 4;
    utab[Ulex].instlength  = 2;
    utab[Uloc].instlength  = 2;
    utab[Ulod].instlength  = 4;
    utab[Umov].instlength  = 4;
    utab[Umus].instlength  = 4;
    utab[Uoptn].instlength = 4;
    utab[Upar].instlength  = 4;
    utab[Updef].instlength = 4;
    utab[Uregs].instlength = 4;
    utab[Urlda].instlength = 4;
    utab[Urldc].instlength = 4;
    utab[Urlod].instlength = 4;
    utab[Urnd].instlength  = 4;
    utab[Urpar].instlength = 4;
    utab[Urstr].instlength = 4;
    utab[Usdef].instlength = 4;
    utab[Uasym].instlength = 4;
    utab[Uhsym].instlength = 4;
    utab[Usgs].instlength  = 4;
    utab[Ustep].instlength = 2;
    utab[Ustp].instlength  = 2;
    utab[Ustr].instlength  = 4;
    utab[Uswp].instlength  = 4;
    utab[Uxjp].instlength  = 8;
    utab[Ucsym].instlength = 4;
    utab[Uesym].instlength = 4;
    utab[Ufsym].instlength = 4;
    utab[Ugsym].instlength = 4;
    utab[Ulsym].instlength = 4;
    utab[Uicuf].instlength = 4;
    utab[Uisld].instlength = 4;
    utab[Uisst].instlength = 4;
    utab[Upmov].instlength = 4;
    utab[Umpmv].instlength = 4;
    utab[Uunal].instlength = 2;
    utab[Uvreg].instlength = 4;
    utab[Utpeq].instlength = 2;
    utab[Utpne].instlength = 2;
    utab[Utplt].instlength = 2;
    utab[Utpgt].instlength = 2;
    utab[Utpge].instlength = 2;
    utab[Utple].instlength = 2;
    utab[Utyp].instlength  = 4;
    utab[Ussym].instlength = 4;
    utab[Urcuf].instlength = 4;
    utab[Uldrc].instlength = 4;
    utab[Umsym].instlength = 4;
    utab[Uksym].instlength = 4;
    utab[Uosym].instlength = 4;
    utab[Uildi].instlength = 4;
    utab[Uisti].instlength = 4;
    utab[Uirld].instlength = 4;
    utab[Uirst].instlength = 4;

    utab[Ustr].stack_pop  = 1;
    utab[Uchkn].stack_pop = 1;
    utab[Uchkt].stack_pop = 1;
    utab[Uabs].stack_pop  = 1;
    utab[Ubsub].stack_pop = 2;
    utab[Uneg].stack_pop  = 1;
    utab[Usqr].stack_pop  = 1;
    utab[Ulnot].stack_pop = 1;
    utab[Unot].stack_pop  = 1;
    utab[Uodd].stack_pop  = 1;
    utab[Usqrt].stack_pop = 1;
    utab[Ucvtl].stack_pop = 1;
    utab[Udec].stack_pop  = 1;
    utab[Uinc].stack_pop  = 1;
    utab[Uchkh].stack_pop = 1;
    utab[Uchkl].stack_pop = 1;
    utab[Usgs].stack_pop  = 1;
    utab[Ucvt].stack_pop  = 1;
    utab[Utyp].stack_pop  = 1;
    utab[Urnd].stack_pop  = 1;
    utab[Uadj].stack_pop  = 1;
    utab[Uilda].stack_pop = 1;
    utab[Ulbd].stack_pop  = 1;
    utab[Uubd].stack_pop  = 1;
    utab[Uadd].stack_pop  = 2;
    utab[Udiv].stack_pop  = 2;
    utab[Umod].stack_pop  = 2;
    utab[Umpy].stack_pop  = 2;
    utab[Usub].stack_pop  = 2;
    utab[Urem].stack_pop  = 2;
    utab[Uand].stack_pop  = 2;
    utab[Uequ].stack_pop  = 2;
    utab[Ugeq].stack_pop  = 2;
    utab[Ugrt].stack_pop  = 2;
    utab[Uior].stack_pop  = 2;
    utab[Uleq].stack_pop  = 2;
    utab[Ules].stack_pop  = 2;
    utab[Umax].stack_pop  = 2;
    utab[Umin].stack_pop  = 2;
    utab[Uneq].stack_pop  = 2;
    utab[Ushl].stack_pop  = 2;
    utab[Ushr].stack_pop  = 2;
    utab[Uxor].stack_pop  = 2;
    utab[Uixa].stack_pop  = 2;
    utab[Uidx].stack_pop  = 3;
    utab[Uint].stack_pop  = 2;
    utab[Udif].stack_pop  = 2;
    utab[Umus].stack_pop  = 2;
    utab[Uuni].stack_pop  = 2;
    utab[Uinn].stack_pop  = 2;
    utab[Uilod].stack_pop = 1;
    utab[Uistr].stack_pop = 2;
    utab[Uildi].stack_pop = 2;
    utab[Uisti].stack_pop = 3;
    utab[Uirld].stack_pop = 1;
    utab[Uirst].stack_pop = 2;
    utab[Uisld].stack_pop = 1;
    utab[Uisst].stack_pop = 2;
    utab[Umov].stack_pop  = 2;
    utab[Ufill].stack_pop = 2;
    utab[Uiequ].stack_pop = 2;
    utab[Uigeq].stack_pop = 2;
    utab[Uigrt].stack_pop = 2;
    utab[Uileq].stack_pop = 2;
    utab[Uiles].stack_pop = 2;
    utab[Uineq].stack_pop = 2;
    utab[Ufjp].stack_pop  = 1;
    utab[Utjp].stack_pop  = 1;
    utab[Uijp].stack_pop  = 1;
    utab[Uxjp].stack_pop  = 1;
    utab[Uicuf].stack_pop = 1;
    utab[Upar].stack_pop  = 1;
    utab[Uxpar].stack_pop = 1;
    utab[Upmov].stack_pop = 1;
    utab[Umpmv].stack_pop = 1;
    utab[Utpeq].stack_pop = 2;
    utab[Utpge].stack_pop = 2;
    utab[Utpgt].stack_pop = 2;
    utab[Utple].stack_pop = 2;
    utab[Utplt].stack_pop = 2;
    utab[Utpne].stack_pop = 2;
    utab[Upop].stack_pop  = 1;
    utab[Uswp].stack_pop  = 1;
    utab[Uaos].stack_pop  = 1;
    utab[Ustsp].stack_pop = 1;
    utab[Usign].stack_pop = 2;

    utab[Ulod].stack_push  = 1;
    utab[Uldc].stack_push  = 1;
    utab[Uldrc].stack_push = 1;
    utab[Ulca].stack_push  = 1;
    utab[Ulda].stack_push  = 1;
    utab[Uchkn].stack_push = 1;
    utab[Uabs].stack_push  = 1;
    utab[Ubsub].stack_push = 1;
    utab[Uneg].stack_push  = 1;
    utab[Usqr].stack_push  = 1;
    utab[Ulnot].stack_push = 1;
    utab[Unot].stack_push  = 1;
    utab[Uodd].stack_push  = 1;
    utab[Usqrt].stack_push = 1;
    utab[Ucvtl].stack_push = 1;
    utab[Udec].stack_push  = 1;
    utab[Uinc].stack_push  = 1;
    utab[Uchkh].stack_push = 1;
    utab[Uchkl].stack_push = 1;
    utab[Usgs].stack_push  = 1;
    utab[Ucvt].stack_push  = 1;
    utab[Utyp].stack_push  = 1;
    utab[Urnd].stack_push  = 1;
    utab[Uadj].stack_push  = 1;
    utab[Uilda].stack_push = 1;
    utab[Ulbd].stack_push  = 1;
    utab[Uubd].stack_push  = 1;
    utab[Uadd].stack_push  = 1;
    utab[Udiv].stack_push  = 1;
    utab[Umod].stack_push  = 1;
    utab[Umpy].stack_push  = 1;
    utab[Usub].stack_push  = 1;
    utab[Urem].stack_push  = 1;
    utab[Uand].stack_push  = 1;
    utab[Uequ].stack_push  = 1;
    utab[Ugeq].stack_push  = 1;
    utab[Ugrt].stack_push  = 1;
    utab[Uior].stack_push  = 1;
    utab[Uleq].stack_push  = 1;
    utab[Ules].stack_push  = 1;
    utab[Umax].stack_push  = 1;
    utab[Umin].stack_push  = 1;
    utab[Uneq].stack_push  = 1;
    utab[Ushl].stack_push  = 1;
    utab[Ushr].stack_push  = 1;
    utab[Uxor].stack_push  = 1;
    utab[Uixa].stack_push  = 1;
    utab[Uidx].stack_push  = 1;
    utab[Uint].stack_push  = 1;
    utab[Udif].stack_push  = 1;
    utab[Umus].stack_push  = 1;
    utab[Uuni].stack_push  = 1;
    utab[Uinn].stack_push  = 1;
    utab[Uilod].stack_push = 1;
    utab[Uildi].stack_push = 1;
    utab[Uirld].stack_push = 1;
    utab[Uisld].stack_push = 1;
    utab[Uiequ].stack_push = 1;
    utab[Uigeq].stack_push = 1;
    utab[Uigrt].stack_push = 1;
    utab[Uileq].stack_push = 1;
    utab[Uiles].stack_push = 1;
    utab[Uineq].stack_push = 1;
    utab[Udup].stack_push  = 1;
    utab[Uswp].stack_push  = 1;
    utab[Uldsp].stack_push = 1;
    utab[Uldap].stack_push = 1;
    utab[Usign].stack_push = 1;
}

/*
 * unused
 */
void getutabrec(struct utabrec *rec, Uopcode op) {
    *rec = utab[op];
}
