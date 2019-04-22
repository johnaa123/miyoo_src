/*****************************************************************************
 *
 *	 tbl6502.c
 *	 6502 opcode functions and function pointer table
 *
 *	 Copyright (c) 1998,1999,2000 Juergen Buchmueller, all rights reserved.
 *
 *	 - This source code is released as freeware for non-commercial purposes.
 *	 - You are free to use and redistribute this code in modified or
 *	   unmodified form, provided you list me in the credits.
 *	 - If you modify this source code, you must add a notice to each modified
 *	   source file that it has been changed.  If you're a nice person, you
 *	   will clearly mark each change too.  :)
 *	 - If you wish to use this for commercial purposes, please contact me at
 *	   pullmoll@t-online.de
 *	 - The author of this copywritten work reserves the right to change the
 *	   terms of its usage and license at any time, including retroactively
 *	 - This entire notice must remain in the source code.
 *
 *****************************************************************************/

#undef	OP
#define OP(nn) INLINE void m6502_##nn(void)

/*****************************************************************************
 *****************************************************************************
 *
 *	 plain vanilla 6502 opcodes
 *
 *****************************************************************************
 * op	 temp	  cycles			 rdmem	 opc  wrmem   ********************/

OP(00)
{
  m6502_ICount -= 7;		  /* 7 BRK */
  BRK;
}
OP(20)
{
  m6502_ICount -= 6;		  /* 6 JSR */
  JSR;
}
OP(40)
{
  m6502_ICount -= 6;		  /* 6 RTI */
  RTI;
}
OP(60)
{
  m6502_ICount -= 6;		  /* 6 RTS */
  RTS;
}
OP(80)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(a0)
{
  int tmp;  /* 2 LDY IMM */
  m6502_ICount -= 2;
  RD_IMM;
  LDY;
}
OP(c0)
{
  int tmp;  /* 2 CPY IMM */
  m6502_ICount -= 2;
  RD_IMM;
  CPY;
}
OP(e0)
{
  int tmp;  /* 2 CPX IMM */
  m6502_ICount -= 2;
  RD_IMM;
  CPX;
}

OP(10)
{
  int tmp;							  /* 2 BPL REL */
  BPL;
}
OP(30)
{
  int tmp;							  /* 2 BMI REL */
  BMI;
}
OP(50)
{
  int tmp;							  /* 2 BVC REL */
  BVC;
}
OP(70)
{
  int tmp;							  /* 2 BVS REL */
  BVS;
}
OP(90)
{
  int tmp;							  /* 2 BCC REL */
  BCC;
}
OP(b0)
{
  int tmp;							  /* 2 BCS REL */
  BCS;
}
OP(d0)
{
  int tmp;							  /* 2 BNE REL */
  BNE;
}
OP(f0)
{
  int tmp;							  /* 2 BEQ REL */
  BEQ;
}

OP(01)
{
  int tmp;  /* 6 ORA IDX */
  m6502_ICount -= 6;
  RD_IDX;
  ORA;
}
OP(21)
{
  int tmp;  /* 6 AND IDX */
  m6502_ICount -= 6;
  RD_IDX;
  AND;
}
OP(41)
{
  int tmp;  /* 6 EOR IDX */
  m6502_ICount -= 6;
  RD_IDX;
  EOR;
}
OP(61)
{
  int tmp;  /* 6 ADC IDX */
  m6502_ICount -= 6;
  RD_IDX;
  ADC;
}
OP(81)
{
  int tmp;  /* 6 STA IDX */
  m6502_ICount -= 6;
  STA;
  WR_IDX;
}
OP(a1)
{
  int tmp;  /* 6 LDA IDX */
  m6502_ICount -= 6;
  RD_IDX;
  LDA;
}
OP(c1)
{
  int tmp;  /* 6 CMP IDX */
  m6502_ICount -= 6;
  RD_IDX;
  CMP;
}
OP(e1)
{
  int tmp;  /* 6 SBC IDX */
  m6502_ICount -= 6;
  RD_IDX;
  SBC;
}

OP(11)
{
  int tmp;  /* 5 ORA IDY */
  m6502_ICount -= 5;
  RD_IDY;
  ORA;
}
OP(31)
{
  int tmp;  /* 5 AND IDY */
  m6502_ICount -= 5;
  RD_IDY;
  AND;
}
OP(51)
{
  int tmp;  /* 5 EOR IDY */
  m6502_ICount -= 5;
  RD_IDY;
  EOR;
}
OP(71)
{
  int tmp;  /* 5 ADC IDY */
  m6502_ICount -= 5;
  RD_IDY;
  ADC;
}
OP(91)
{
  int tmp;  /* 6 STA IDY */
  m6502_ICount -= 6;
  STA;
  WR_IDY;
}
OP(b1)
{
  int tmp;  /* 5 LDA IDY */
  m6502_ICount -= 5;
  RD_IDY;
  LDA;
}
OP(d1)
{
  int tmp;  /* 5 CMP IDY */
  m6502_ICount -= 5;
  RD_IDY;
  CMP;
}
OP(f1)
{
  int tmp;  /* 5 SBC IDY */
  m6502_ICount -= 5;
  RD_IDY;
  SBC;
}

OP(02)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(22)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(42)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(62)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(82)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(a2)
{
  int tmp;  /* 2 LDX IMM */
  m6502_ICount -= 2;
  RD_IMM;
  LDX;
}
OP(c2)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(e2)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(12)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(32)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(52)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(72)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(92)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(b2)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(d2)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(f2)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(03)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(23)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(43)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(63)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(83)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(a3)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(c3)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(e3)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(13)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(33)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(53)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(73)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(93)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(b3)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(d3)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(f3)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(04)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(24)
{
  int tmp;  /* 3 BIT ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  BIT;
}
OP(44)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(64)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(84)
{
  int tmp;  /* 3 STY ZPG */
  m6502_ICount -= 3;
  STY;
  WR_ZPG;
}
OP(a4)
{
  int tmp;  /* 3 LDY ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  LDY;
}
OP(c4)
{
  int tmp;  /* 3 CPY ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  CPY;
}
OP(e4)
{
  int tmp;  /* 3 CPX ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  CPX;
}

OP(14)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(34)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(54)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(74)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(94)
{
  int tmp;  /* 4 STY ZPX */
  m6502_ICount -= 4;
  STY;
  WR_ZPX;
}
OP(b4)
{
  int tmp;  /* 4 LDY ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  LDY;
}
OP(d4)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(f4)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(05)
{
  int tmp;  /* 3 ORA ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  ORA;
}
OP(25)
{
  int tmp;  /* 3 AND ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  AND;
}
OP(45)
{
  int tmp;  /* 3 EOR ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  EOR;
}
OP(65)
{
  int tmp;  /* 3 ADC ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  ADC;
}
OP(85)
{
  int tmp;  /* 3 STA ZPG */
  m6502_ICount -= 3;
  STA;
  WR_ZPG;
}
OP(a5)
{
  int tmp;  /* 3 LDA ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  LDA;
}
OP(c5)
{
  int tmp;  /* 3 CMP ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  CMP;
}
OP(e5)
{
  int tmp;  /* 3 SBC ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  SBC;
}

OP(15)
{
  int tmp;  /* 4 ORA ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  ORA;
}
OP(35)
{
  int tmp;  /* 4 AND ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  AND;
}
OP(55)
{
  int tmp;  /* 4 EOR ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  EOR;
}
OP(75)
{
  int tmp;  /* 4 ADC ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  ADC;
}
OP(95)
{
  int tmp;  /* 4 STA ZPX */
  m6502_ICount -= 4;
  STA;
  WR_ZPX;
}
OP(b5)
{
  int tmp;  /* 4 LDA ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  LDA;
}
OP(d5)
{
  int tmp;  /* 4 CMP ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  CMP;
}
OP(f5)
{
  int tmp;  /* 4 SBC ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  SBC;
}

OP(06)
{
  int tmp;  /* 5 ASL ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  ASL;
  WB_EA;
}
OP(26)
{
  int tmp;  /* 5 ROL ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  ROL;
  WB_EA;
}
OP(46)
{
  int tmp;  /* 5 LSR ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  LSR;
  WB_EA;
}
OP(66)
{
  int tmp;  /* 5 ROR ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  ROR;
  WB_EA;
}
OP(86)
{
  int tmp;  /* 3 STX ZPG */
  m6502_ICount -= 3;
  STX;
  WR_ZPG;
}
OP(a6)
{
  int tmp;  /* 3 LDX ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  LDX;
}
OP(c6)
{
  int tmp;  /* 5 DEC ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  DEC;
  WB_EA;
}
OP(e6)
{
  int tmp;  /* 5 INC ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  INC;
  WB_EA;
}

OP(16)
{
  int tmp;  /* 6 ASL ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  ASL;
  WB_EA;
}
OP(36)
{
  int tmp;  /* 6 ROL ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  ROL;
  WB_EA;
}
OP(56)
{
  int tmp;  /* 6 LSR ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  LSR;
  WB_EA;
}
OP(76)
{
  int tmp;  /* 6 ROR ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  ROR;
  WB_EA;
}
OP(96)
{
  int tmp;  /* 4 STX ZPY */
  m6502_ICount -= 4;
  STX;
  WR_ZPY;
}
OP(b6)
{
  int tmp;  /* 4 LDX ZPY */
  m6502_ICount -= 4;
  RD_ZPY;
  LDX;
}
OP(d6)
{
  int tmp;  /* 6 DEC ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  DEC;
  WB_EA;
}
OP(f6)
{
  int tmp;  /* 6 INC ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  INC;
  WB_EA;
}

OP(07)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(27)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(47)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(67)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(87)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(a7)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(c7)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(e7)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(17)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(37)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(57)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(77)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(97)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(b7)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(d7)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(f7)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(08)
{
  m6502_ICount -= 2;		  /* 2 PHP */
  PHP;
}
OP(28)
{
  m6502_ICount -= 2;		  /* 2 PLP */
  PLP;
}
OP(48)
{
  m6502_ICount -= 2;		  /* 2 PHA */
  PHA;
}
OP(68)
{
  m6502_ICount -= 2;		  /* 2 PLA */
  PLA;
}
OP(88)
{
  m6502_ICount -= 2;		  /* 2 DEY */
  DEY;
}
OP(a8)
{
  m6502_ICount -= 2;		  /* 2 TAY */
  TAY;
}
OP(c8)
{
  m6502_ICount -= 2;		  /* 2 INY */
  INY;
}
OP(e8)
{
  m6502_ICount -= 2;		  /* 2 INX */
  INX;
}

OP(18)
{
  m6502_ICount -= 2;		  /* 2 CLC */
  CLC;
}
OP(38)
{
  m6502_ICount -= 2;		  /* 2 SEC */
  SEC;
}
OP(58)
{
  m6502_ICount -= 2;		  /* 2 CLI */
  CLI;
}
OP(78)
{
  m6502_ICount -= 2;		  /* 2 SEI */
  SEI;
}
OP(98)
{
  m6502_ICount -= 2;		  /* 2 TYA */
  TYA;
}
OP(b8)
{
  m6502_ICount -= 2;		  /* 2 CLV */
  CLV;
}
OP(d8)
{
  m6502_ICount -= 2;		  /* 2 CLD */
  CLD;
}
OP(f8)
{
  m6502_ICount -= 2;		  /* 2 SED */
  SED;
}

OP(09)
{
  int tmp;  /* 2 ORA IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ORA;
}
OP(29)
{
  int tmp;  /* 2 AND IMM */
  m6502_ICount -= 2;
  RD_IMM;
  AND;
}
OP(49)
{
  int tmp;  /* 2 EOR IMM */
  m6502_ICount -= 2;
  RD_IMM;
  EOR;
}
OP(69)
{
  int tmp;  /* 2 ADC IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ADC;
}
OP(89)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(a9)
{
  int tmp;  /* 2 LDA IMM */
  m6502_ICount -= 2;
  RD_IMM;
  LDA;
}
OP(c9)
{
  int tmp;  /* 2 CMP IMM */
  m6502_ICount -= 2;
  RD_IMM;
  CMP;
}
OP(e9)
{
  int tmp;  /* 2 SBC IMM */
  m6502_ICount -= 2;
  RD_IMM;
  SBC;
}

OP(19)
{
  int tmp;  /* 4 ORA ABY */
  m6502_ICount -= 4;
  RD_ABY;
  ORA;
}
OP(39)
{
  int tmp;  /* 4 AND ABY */
  m6502_ICount -= 4;
  RD_ABY;
  AND;
}
OP(59)
{
  int tmp;  /* 4 EOR ABY */
  m6502_ICount -= 4;
  RD_ABY;
  EOR;
}
OP(79)
{
  int tmp;  /* 4 ADC ABY */
  m6502_ICount -= 4;
  RD_ABY;
  ADC;
}
OP(99)
{
  int tmp;  /* 5 STA ABY */
  m6502_ICount -= 5;
  STA;
  WR_ABY;
}
OP(b9)
{
  int tmp;  /* 4 LDA ABY */
  m6502_ICount -= 4;
  RD_ABY;
  LDA;
}
OP(d9)
{
  int tmp;  /* 4 CMP ABY */
  m6502_ICount -= 4;
  RD_ABY;
  CMP;
}
OP(f9)
{
  int tmp;  /* 4 SBC ABY */
  m6502_ICount -= 4;
  RD_ABY;
  SBC;
}

OP(0a)
{
  int tmp;  /* 2 ASL A */
  m6502_ICount -= 2;
  RD_ACC;
  ASL;
  WB_ACC;
}
OP(2a)
{
  int tmp;  /* 2 ROL A */
  m6502_ICount -= 2;
  RD_ACC;
  ROL;
  WB_ACC;
}
OP(4a)
{
  int tmp;  /* 2 LSR A */
  m6502_ICount -= 2;
  RD_ACC;
  LSR;
  WB_ACC;
}
OP(6a)
{
  int tmp;  /* 2 ROR A */
  m6502_ICount -= 2;
  RD_ACC;
  ROR;
  WB_ACC;
}
OP(8a)
{
  m6502_ICount -= 2;		  /* 2 TXA */
  TXA;
}
OP(aa)
{
  m6502_ICount -= 2;		  /* 2 TAX */
  TAX;
}
OP(ca)
{
  m6502_ICount -= 2;		  /* 2 DEX */
  DEX;
}
OP(ea)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}

OP(1a)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(3a)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(5a)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(7a)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(9a)
{
  m6502_ICount -= 2;		  /* 2 TXS */
  TXS;
}
OP(ba)
{
  m6502_ICount -= 2;		  /* 2 TSX */
  TSX;
}
OP(da)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(fa)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(0b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(2b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(4b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(6b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(8b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(ab)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(cb)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(eb)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(1b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(3b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(5b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(7b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(9b)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(bb)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(db)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(fb)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(0c)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(2c)
{
  int tmp;  /* 4 BIT ABS */
  m6502_ICount -= 4;
  RD_ABS;
  BIT;
}
OP(4c)
{
  m6502_ICount -= 3;  /* 3 JMP ABS */
  EA_ABS;
  JMP;
}
OP(6c)
{
  int tmp;  /* 5 JMP IND */
  m6502_ICount -= 5;
  EA_IND;
  JMP;
}
OP(8c)
{
  int tmp;  /* 4 STY ABS */
  m6502_ICount -= 4;
  STY;
  WR_ABS;
}
OP(ac)
{
  int tmp;  /* 4 LDY ABS */
  m6502_ICount -= 4;
  RD_ABS;
  LDY;
}
OP(cc)
{
  int tmp;  /* 4 CPY ABS */
  m6502_ICount -= 4;
  RD_ABS;
  CPY;
}
OP(ec)
{
  int tmp;  /* 4 CPX ABS */
  m6502_ICount -= 4;
  RD_ABS;
  CPX;
}

OP(1c)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(3c)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(5c)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(7c)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(9c)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(bc)
{
  int tmp;  /* 4 LDY ABX */
  m6502_ICount -= 4;
  RD_ABX;
  LDY;
}
OP(dc)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(fc)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(0d)
{
  int tmp;  /* 4 ORA ABS */
  m6502_ICount -= 4;
  RD_ABS;
  ORA;
}
OP(2d)
{
  int tmp;  /* 4 AND ABS */
  m6502_ICount -= 4;
  RD_ABS;
  AND;
}
OP(4d)
{
  int tmp;  /* 4 EOR ABS */
  m6502_ICount -= 4;
  RD_ABS;
  EOR;
}
OP(6d)
{
  int tmp;  /* 4 ADC ABS */
  m6502_ICount -= 4;
  RD_ABS;
  ADC;
}
OP(8d)
{
  int tmp;  /* 4 STA ABS */
  m6502_ICount -= 4;
  STA;
  WR_ABS;
}
OP(ad)
{
  int tmp;  /* 4 LDA ABS */
  m6502_ICount -= 4;
  RD_ABS;
  LDA;
}
OP(cd)
{
  int tmp;  /* 4 CMP ABS */
  m6502_ICount -= 4;
  RD_ABS;
  CMP;
}
OP(ed)
{
  int tmp;  /* 4 SBC ABS */
  m6502_ICount -= 4;
  RD_ABS;
  SBC;
}

OP(1d)
{
  int tmp;  /* 4 ORA ABX */
  m6502_ICount -= 4;
  RD_ABX;
  ORA;
}
OP(3d)
{
  int tmp;  /* 4 AND ABX */
  m6502_ICount -= 4;
  RD_ABX;
  AND;
}
OP(5d)
{
  int tmp;  /* 4 EOR ABX */
  m6502_ICount -= 4;
  RD_ABX;
  EOR;
}
OP(7d)
{
  int tmp;  /* 4 ADC ABX */
  m6502_ICount -= 4;
  RD_ABX;
  ADC;
}
OP(9d)
{
  int tmp;  /* 5 STA ABX */
  m6502_ICount -= 5;
  STA;
  WR_ABX;
}
OP(bd)
{
  int tmp;  /* 4 LDA ABX */
  m6502_ICount -= 4;
  RD_ABX;
  LDA;
}
OP(dd)
{
  int tmp;  /* 4 CMP ABX */
  m6502_ICount -= 4;
  RD_ABX;
  CMP;
}
OP(fd)
{
  int tmp;  /* 4 SBC ABX */
  m6502_ICount -= 4;
  RD_ABX;
  SBC;
}

OP(0e)
{
  int tmp;  /* 6 ASL ABS */
  m6502_ICount -= 6;
  RD_ABS;
  ASL;
  WB_EA;
}
OP(2e)
{
  int tmp;  /* 6 ROL ABS */
  m6502_ICount -= 6;
  RD_ABS;
  ROL;
  WB_EA;
}
OP(4e)
{
  int tmp;  /* 6 LSR ABS */
  m6502_ICount -= 6;
  RD_ABS;
  LSR;
  WB_EA;
}
OP(6e)
{
  int tmp;  /* 6 ROR ABS */
  m6502_ICount -= 6;
  RD_ABS;
  ROR;
  WB_EA;
}
OP(8e)
{
  int tmp;  /* 5 STX ABS */
  m6502_ICount -= 5;
  STX;
  WR_ABS;
}
OP(ae)
{
  int tmp;  /* 4 LDX ABS */
  m6502_ICount -= 4;
  RD_ABS;
  LDX;
}
OP(ce)
{
  int tmp;  /* 6 DEC ABS */
  m6502_ICount -= 6;
  RD_ABS;
  DEC;
  WB_EA;
}
OP(ee)
{
  int tmp;  /* 6 INC ABS */
  m6502_ICount -= 6;
  RD_ABS;
  INC;
  WB_EA;
}

OP(1e)
{
  int tmp;  /* 7 ASL ABX */
  m6502_ICount -= 7;
  RD_ABX;
  ASL;
  WB_EA;
}
OP(3e)
{
  int tmp;  /* 7 ROL ABX */
  m6502_ICount -= 7;
  RD_ABX;
  ROL;
  WB_EA;
}
OP(5e)
{
  int tmp;  /* 7 LSR ABX */
  m6502_ICount -= 7;
  RD_ABX;
  LSR;
  WB_EA;
}
OP(7e)
{
  int tmp;  /* 7 ROR ABX */
  m6502_ICount -= 7;
  RD_ABX;
  ROR;
  WB_EA;
}
OP(9e)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(be)
{
  int tmp;  /* 4 LDX ABY */
  m6502_ICount -= 4;
  RD_ABY;
  LDX;
}
OP(de)
{
  int tmp;  /* 7 DEC ABX */
  m6502_ICount -= 7;
  RD_ABX;
  DEC;
  WB_EA;
}
OP(fe)
{
  int tmp;  /* 7 INC ABX */
  m6502_ICount -= 7;
  RD_ABX;
  INC;
  WB_EA;
}

OP(0f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(2f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(4f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(6f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(8f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(af)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(cf)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(ef)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

OP(1f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(3f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(5f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(7f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(9f)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(bf)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(df)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}
OP(ff)
{
  m6502_ICount -= 2;		  /* 2 ILL */
  ILL;
}

/* and here's the array of function pointers */

static void (*insn6502[0x100])(void) =
{
  m6502_00,m6502_01,m6502_02,m6502_03,m6502_04,m6502_05,m6502_06,m6502_07,
  m6502_08,m6502_09,m6502_0a,m6502_0b,m6502_0c,m6502_0d,m6502_0e,m6502_0f,
  m6502_10,m6502_11,m6502_12,m6502_13,m6502_14,m6502_15,m6502_16,m6502_17,
  m6502_18,m6502_19,m6502_1a,m6502_1b,m6502_1c,m6502_1d,m6502_1e,m6502_1f,
  m6502_20,m6502_21,m6502_22,m6502_23,m6502_24,m6502_25,m6502_26,m6502_27,
  m6502_28,m6502_29,m6502_2a,m6502_2b,m6502_2c,m6502_2d,m6502_2e,m6502_2f,
  m6502_30,m6502_31,m6502_32,m6502_33,m6502_34,m6502_35,m6502_36,m6502_37,
  m6502_38,m6502_39,m6502_3a,m6502_3b,m6502_3c,m6502_3d,m6502_3e,m6502_3f,
  m6502_40,m6502_41,m6502_42,m6502_43,m6502_44,m6502_45,m6502_46,m6502_47,
  m6502_48,m6502_49,m6502_4a,m6502_4b,m6502_4c,m6502_4d,m6502_4e,m6502_4f,
  m6502_50,m6502_51,m6502_52,m6502_53,m6502_54,m6502_55,m6502_56,m6502_57,
  m6502_58,m6502_59,m6502_5a,m6502_5b,m6502_5c,m6502_5d,m6502_5e,m6502_5f,
  m6502_60,m6502_61,m6502_62,m6502_63,m6502_64,m6502_65,m6502_66,m6502_67,
  m6502_68,m6502_69,m6502_6a,m6502_6b,m6502_6c,m6502_6d,m6502_6e,m6502_6f,
  m6502_70,m6502_71,m6502_72,m6502_73,m6502_74,m6502_75,m6502_76,m6502_77,
  m6502_78,m6502_79,m6502_7a,m6502_7b,m6502_7c,m6502_7d,m6502_7e,m6502_7f,
  m6502_80,m6502_81,m6502_82,m6502_83,m6502_84,m6502_85,m6502_86,m6502_87,
  m6502_88,m6502_89,m6502_8a,m6502_8b,m6502_8c,m6502_8d,m6502_8e,m6502_8f,
  m6502_90,m6502_91,m6502_92,m6502_93,m6502_94,m6502_95,m6502_96,m6502_97,
  m6502_98,m6502_99,m6502_9a,m6502_9b,m6502_9c,m6502_9d,m6502_9e,m6502_9f,
  m6502_a0,m6502_a1,m6502_a2,m6502_a3,m6502_a4,m6502_a5,m6502_a6,m6502_a7,
  m6502_a8,m6502_a9,m6502_aa,m6502_ab,m6502_ac,m6502_ad,m6502_ae,m6502_af,
  m6502_b0,m6502_b1,m6502_b2,m6502_b3,m6502_b4,m6502_b5,m6502_b6,m6502_b7,
  m6502_b8,m6502_b9,m6502_ba,m6502_bb,m6502_bc,m6502_bd,m6502_be,m6502_bf,
  m6502_c0,m6502_c1,m6502_c2,m6502_c3,m6502_c4,m6502_c5,m6502_c6,m6502_c7,
  m6502_c8,m6502_c9,m6502_ca,m6502_cb,m6502_cc,m6502_cd,m6502_ce,m6502_cf,
  m6502_d0,m6502_d1,m6502_d2,m6502_d3,m6502_d4,m6502_d5,m6502_d6,m6502_d7,
  m6502_d8,m6502_d9,m6502_da,m6502_db,m6502_dc,m6502_dd,m6502_de,m6502_df,
  m6502_e0,m6502_e1,m6502_e2,m6502_e3,m6502_e4,m6502_e5,m6502_e6,m6502_e7,
  m6502_e8,m6502_e9,m6502_ea,m6502_eb,m6502_ec,m6502_ed,m6502_ee,m6502_ef,
  m6502_f0,m6502_f1,m6502_f2,m6502_f3,m6502_f4,m6502_f5,m6502_f6,m6502_f7,
  m6502_f8,m6502_f9,m6502_fa,m6502_fb,m6502_fc,m6502_fd,m6502_fe,m6502_ff
};

