/*****************************************************************************
 *
 *	 tbl6509.c
 *	 6509 opcode functions and function pointer table
 *
 *	 Copyright (c) 2000 Peter Trauner, all rights reserved.
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
 *	 - Opcode information based on an Intel 386 '6510.asm' core
 *	   written by R.F. van Ee (1993)
 *	 - Cycle counts are guesswork :-)
 *
 *****************************************************************************/

#undef	OP
#define OP(nn) INLINE void m6509_##nn(void)

OP(00)
{
  m6509_ICount -= 7;		  /* 7 BRK */
  BRK;
}
OP(20)
{
  m6509_ICount -= 6;		  /* 6 JSR */
  JSR;
}
OP(40)
{
  m6509_ICount -= 6;		  /* 6 RTI */
  RTI;
}
OP(60)
{
  m6509_ICount -= 6;		  /* 6 RTS */
  RTS;
}
OP(80)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(a0)
{
  int tmp;  /* 2 LDY IMM */
  m6509_ICount -= 2;
  RD_IMM;
  LDY;
}
OP(c0)
{
  int tmp;  /* 2 CPY IMM */
  m6509_ICount -= 2;
  RD_IMM;
  CPY;
}
OP(e0)
{
  int tmp;  /* 2 CPX IMM */
  m6509_ICount -= 2;
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
  m6509_ICount -= 6;
  RD_IDX;
  ORA;
}
OP(21)
{
  int tmp;  /* 6 AND IDX */
  m6509_ICount -= 6;
  RD_IDX;
  AND;
}
OP(41)
{
  int tmp;  /* 6 EOR IDX */
  m6509_ICount -= 6;
  RD_IDX;
  EOR;
}
OP(61)
{
  int tmp;  /* 6 ADC IDX */
  m6509_ICount -= 6;
  RD_IDX;
  ADC;
}
OP(81)
{
  int tmp;  /* 6 STA IDX */
  m6509_ICount -= 6;
  STA;
  WR_IDX;
}
OP(a1)
{
  int tmp;  /* 6 LDA IDX */
  m6509_ICount -= 6;
  RD_IDX;
  LDA;
}
OP(c1)
{
  int tmp;  /* 6 CMP IDX */
  m6509_ICount -= 6;
  RD_IDX;
  CMP;
}
OP(e1)
{
  int tmp;  /* 6 SBC IDX */
  m6509_ICount -= 6;
  RD_IDX;
  SBC;
}

OP(11)
{
  int tmp;  /* 5 ORA IDY */
  m6509_ICount -= 5;
  RD_IDY;
  ORA;
}
OP(31)
{
  int tmp;  /* 5 AND IDY */
  m6509_ICount -= 5;
  RD_IDY;
  AND;
}
OP(51)
{
  int tmp;  /* 5 EOR IDY */
  m6509_ICount -= 5;
  RD_IDY;
  EOR;
}
OP(71)
{
  int tmp;  /* 5 ADC IDY */
  m6509_ICount -= 5;
  RD_IDY;
  ADC;
}
OP(91)
{
  int tmp;  /* 6 STA IDY */
  m6509_ICount -= 6;
  STA;
  WR_IDY_6509;
}
OP(b1)
{
  int tmp;  /* 5 LDA IDY */
  m6509_ICount -= 5;
  RD_IDY_6509;
  LDA;
}
OP(d1)
{
  int tmp;  /* 5 CMP IDY */
  m6509_ICount -= 5;
  RD_IDY;
  CMP;
}
OP(f1)
{
  int tmp;  /* 5 SBC IDY */
  m6509_ICount -= 5;
  RD_IDY;
  SBC;
}

OP(02)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(22)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(42)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(62)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(82)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(a2)
{
  int tmp;  /* 2 LDX IMM */
  m6509_ICount -= 2;
  RD_IMM;
  LDX;
}
OP(c2)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(e2)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}

OP(12)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(32)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(52)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(72)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(92)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(b2)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(d2)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}
OP(f2)
{
  m6502_ICount -= 2;		  /* 2 KIL */
  KIL;
}

OP(03)
{
  int tmp;  /* 7 SLO IDX */
  m6502_ICount -= 7;
  RD_IDX;
  SLO;
  WB_EA;
}
OP(23)
{
  int tmp;  /* 7 RLA IDX */
  m6502_ICount -= 7;
  RD_IDX;
  RLA;
  WB_EA;
}
OP(43)
{
  int tmp;  /* 7 SRE IDX */
  m6502_ICount -= 7;
  RD_IDX;
  SRE;
  WB_EA;
}
OP(63)
{
  int tmp;  /* 7 RRA IDX */
  m6502_ICount -= 7;
  RD_IDX;
  RRA;
  WB_EA;
}
OP(83)
{
  int tmp;  /* 6 SAX IDX */
  m6502_ICount -= 6;
  SAX;
  WR_IDX;
}
OP(a3)
{
  int tmp;  /* 6 LAX IDX */
  m6502_ICount -= 6;
  RD_IDX;
  LAX;
}
OP(c3)
{
  int tmp;  /* 7 DCP IDX */
  m6502_ICount -= 7;
  RD_IDX;
  DCP;
  WB_EA;
}
OP(e3)
{
  int tmp;  /* 7 ISB IDX */
  m6502_ICount -= 7;
  RD_IDX;
  ISB;
  WB_EA;
}

OP(13)
{
  int tmp;  /* 6 SLO IDY */
  m6502_ICount -= 6;
  RD_IDY;
  SLO;
  WB_EA;
}
OP(33)
{
  int tmp;  /* 6 RLA IDY */
  m6502_ICount -= 6;
  RD_IDY;
  RLA;
  WB_EA;
}
OP(53)
{
  int tmp;  /* 6 SRE IDY */
  m6502_ICount -= 6;
  RD_IDY;
  SRE;
  WB_EA;
}
OP(73)
{
  int tmp;  /* 6 RRA IDY */
  m6502_ICount -= 6;
  RD_IDY;
  RRA;
  WB_EA;
}
OP(93)
{
  int tmp;  /* 5 SAH IDY */
  m6502_ICount -= 5;
  EA_IDY;
  SAH;
  WB_EA;
}
OP(b3)
{
  int tmp;  /* 5 LAX IDY */
  m6502_ICount -= 5;
  RD_IDY;
  LAX;
}
OP(d3)
{
  int tmp;  /* 6 DCP IDY */
  m6502_ICount -= 6;
  RD_IDY;
  DCP;
  WB_EA;
}
OP(f3)
{
  int tmp;  /* 6 ISB IDY */
  m6502_ICount -= 6;
  RD_IDY;
  ISB;
  WB_EA;
}

OP(04)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(24)
{
  int tmp;  /* 3 BIT ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  BIT;
}
OP(44)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(64)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(84)
{
  int tmp;  /* 3 STY ZPG */
  m6509_ICount -= 3;
  STY;
  WR_ZPG;
}
OP(a4)
{
  int tmp;  /* 3 LDY ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  LDY;
}
OP(c4)
{
  int tmp;  /* 3 CPY ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  CPY;
}
OP(e4)
{
  int tmp;  /* 3 CPX ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  CPX;
}

OP(14)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(34)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(54)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(74)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(94)
{
  int tmp;  /* 4 STY ZPX */
  m6509_ICount -= 4;
  STY;
  WR_ZPX;
}
OP(b4)
{
  int tmp;  /* 4 LDY ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  LDY;
}
OP(d4)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(f4)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}

OP(05)
{
  int tmp;  /* 3 ORA ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  ORA;
}
OP(25)
{
  int tmp;  /* 3 AND ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  AND;
}
OP(45)
{
  int tmp;  /* 3 EOR ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  EOR;
}
OP(65)
{
  int tmp;  /* 3 ADC ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  ADC;
}
OP(85)
{
  int tmp;  /* 3 STA ZPG */
  m6509_ICount -= 3;
  STA;
  WR_ZPG;
}
OP(a5)
{
  int tmp;  /* 3 LDA ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  LDA;
}
OP(c5)
{
  int tmp;  /* 3 CMP ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  CMP;
}
OP(e5)
{
  int tmp;  /* 3 SBC ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  SBC;
}

OP(15)
{
  int tmp;  /* 4 ORA ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  ORA;
}
OP(35)
{
  int tmp;  /* 4 AND ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  AND;
}
OP(55)
{
  int tmp;  /* 4 EOR ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  EOR;
}
OP(75)
{
  int tmp;  /* 4 ADC ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  ADC;
}
OP(95)
{
  int tmp;  /* 4 STA ZPX */
  m6509_ICount -= 4;
  STA;
  WR_ZPX;
}
OP(b5)
{
  int tmp;  /* 4 LDA ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  LDA;
}
OP(d5)
{
  int tmp;  /* 4 CMP ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  CMP;
}
OP(f5)
{
  int tmp;  /* 4 SBC ZPX */
  m6509_ICount -= 4;
  RD_ZPX;
  SBC;
}

OP(06)
{
  int tmp;  /* 5 ASL ZPG */
  m6509_ICount -= 5;
  RD_ZPG;
  ASL;
  WB_EA;
}
OP(26)
{
  int tmp;  /* 5 ROL ZPG */
  m6509_ICount -= 5;
  RD_ZPG;
  ROL;
  WB_EA;
}
OP(46)
{
  int tmp;  /* 5 LSR ZPG */
  m6509_ICount -= 5;
  RD_ZPG;
  LSR;
  WB_EA;
}
OP(66)
{
  int tmp;  /* 5 ROR ZPG */
  m6509_ICount -= 5;
  RD_ZPG;
  ROR;
  WB_EA;
}
OP(86)
{
  int tmp;  /* 3 STX ZPG */
  m6509_ICount -= 3;
  STX;
  WR_ZPG;
}
OP(a6)
{
  int tmp;  /* 3 LDX ZPG */
  m6509_ICount -= 3;
  RD_ZPG;
  LDX;
}
OP(c6)
{
  int tmp;  /* 5 DEC ZPG */
  m6509_ICount -= 5;
  RD_ZPG;
  DEC;
  WB_EA;
}
OP(e6)
{
  int tmp;  /* 5 INC ZPG */
  m6509_ICount -= 5;
  RD_ZPG;
  INC;
  WB_EA;
}

OP(16)
{
  int tmp;  /* 6 ASL ZPX */
  m6509_ICount -= 6;
  RD_ZPX;
  ASL;
  WB_EA;
}
OP(36)
{
  int tmp;  /* 6 ROL ZPX */
  m6509_ICount -= 6;
  RD_ZPX;
  ROL;
  WB_EA;
}
OP(56)
{
  int tmp;  /* 6 LSR ZPX */
  m6509_ICount -= 6;
  RD_ZPX;
  LSR;
  WB_EA;
}
OP(76)
{
  int tmp;  /* 6 ROR ZPX */
  m6509_ICount -= 6;
  RD_ZPX;
  ROR;
  WB_EA;
}
OP(96)
{
  int tmp;  /* 4 STX ZPY */
  m6509_ICount -= 4;
  STX;
  WR_ZPY;
}
OP(b6)
{
  int tmp;  /* 4 LDX ZPY */
  m6509_ICount -= 4;
  RD_ZPY;
  LDX;
}
OP(d6)
{
  int tmp;  /* 6 DEC ZPX */
  m6509_ICount -= 6;
  RD_ZPX;
  DEC;
  WB_EA;
}
OP(f6)
{
  int tmp;  /* 6 INC ZPX */
  m6509_ICount -= 6;
  RD_ZPX;
  INC;
  WB_EA;
}

OP(07)
{
  int tmp;  /* 5 SLO ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SLO;
  WB_EA;
}
OP(27)
{
  int tmp;  /* 5 RLA ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RLA;
  WB_EA;
}
OP(47)
{
  int tmp;  /* 5 SRE ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SRE;
  WB_EA;
}
OP(67)
{
  int tmp;  /* 5 RRA ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RRA;
  WB_EA;
}
OP(87)
{
  int tmp;  /* 3 SAX ZPG */
  m6502_ICount -= 3;
  SAX;
  WR_ZPG;
}
OP(a7)
{
  int tmp;  /* 3 LAX ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  LAX;
}
OP(c7)
{
  int tmp;  /* 5 DCP ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  DCP;
  WB_EA;
}
OP(e7)
{
  int tmp;  /* 5 ISB ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  ISB;
  WB_EA;
}

OP(17)
{
  int tmp;  /* 4 SLO ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  SLO;
  WB_EA;
}
OP(37)
{
  int tmp;  /* 4 RLA ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  RLA;
  WB_EA;
}
OP(57)
{
  int tmp;  /* 4 SRE ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  SRE;
  WB_EA;
}
OP(77)
{
  int tmp;  /* 4 RRA ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  RRA;
  WB_EA;
}
OP(97)
{
  int tmp;  /* 4 SAX ZPY */
  m6502_ICount -= 4;
  SAX;
  WR_ZPY;
}
OP(b7)
{
  int tmp;  /* 4 LAX ZPY */
  m6502_ICount -= 4;
  RD_ZPY;
  LAX;
}
OP(d7)
{
  int tmp;  /* 6 DCP ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  DCP;
  WB_EA;
}
OP(f7)
{
  int tmp;  /* 6 ISB ZPX */
  m6502_ICount -= 6;
  RD_ZPX;
  ISB;
  WB_EA;
}

OP(08)
{
  m6509_ICount -= 2;		  /* 2 PHP */
  PHP;
}
OP(28)
{
  m6509_ICount -= 2;		  /* 2 PLP */
  PLP;
}
OP(48)
{
  m6509_ICount -= 2;		  /* 2 PHA */
  PHA;
}
OP(68)
{
  m6509_ICount -= 2;		  /* 2 PLA */
  PLA;
}
OP(88)
{
  m6509_ICount -= 2;		  /* 2 DEY */
  DEY;
}
OP(a8)
{
  m6509_ICount -= 2;		  /* 2 TAY */
  TAY;
}
OP(c8)
{
  m6509_ICount -= 2;		  /* 2 INY */
  INY;
}
OP(e8)
{
  m6509_ICount -= 2;		  /* 2 INX */
  INX;
}

OP(18)
{
  m6509_ICount -= 2;		  /* 2 CLC */
  CLC;
}
OP(38)
{
  m6509_ICount -= 2;		  /* 2 SEC */
  SEC;
}
OP(58)
{
  m6509_ICount -= 2;		  /* 2 CLI */
  CLI;
}
OP(78)
{
  m6509_ICount -= 2;		  /* 2 SEI */
  SEI;
}
OP(98)
{
  m6509_ICount -= 2;		  /* 2 TYA */
  TYA;
}
OP(b8)
{
  m6509_ICount -= 2;		  /* 2 CLV */
  CLV;
}
OP(d8)
{
  m6509_ICount -= 2;		  /* 2 CLD */
  CLD;
}
OP(f8)
{
  m6509_ICount -= 2;		  /* 2 SED */
  SED;
}

OP(09)
{
  int tmp;  /* 2 ORA IMM */
  m6509_ICount -= 2;
  RD_IMM;
  ORA;
}
OP(29)
{
  int tmp;  /* 2 AND IMM */
  m6509_ICount -= 2;
  RD_IMM;
  AND;
}
OP(49)
{
  int tmp;  /* 2 EOR IMM */
  m6509_ICount -= 2;
  RD_IMM;
  EOR;
}
OP(69)
{
  int tmp;  /* 2 ADC IMM */
  m6509_ICount -= 2;
  RD_IMM;
  ADC;
}
OP(89)
{
  m6502_ICount -= 2;		  /* 2 DOP */
  DOP;
}
OP(a9)
{
  int tmp;  /* 2 LDA IMM */
  m6509_ICount -= 2;
  RD_IMM;
  LDA;
}
OP(c9)
{
  int tmp;  /* 2 CMP IMM */
  m6509_ICount -= 2;
  RD_IMM;
  CMP;
}
OP(e9)
{
  int tmp;  /* 2 SBC IMM */
  m6509_ICount -= 2;
  RD_IMM;
  SBC;
}

OP(19)
{
  int tmp;  /* 4 ORA ABY */
  m6509_ICount -= 4;
  RD_ABY;
  ORA;
}
OP(39)
{
  int tmp;  /* 4 AND ABY */
  m6509_ICount -= 4;
  RD_ABY;
  AND;
}
OP(59)
{
  int tmp;  /* 4 EOR ABY */
  m6509_ICount -= 4;
  RD_ABY;
  EOR;
}
OP(79)
{
  int tmp;  /* 4 ADC ABY */
  m6509_ICount -= 4;
  RD_ABY;
  ADC;
}
OP(99)
{
  int tmp;  /* 5 STA ABY */
  m6509_ICount -= 5;
  STA;
  WR_ABY;
}
OP(b9)
{
  int tmp;  /* 4 LDA ABY */
  m6509_ICount -= 4;
  RD_ABY;
  LDA;
}
OP(d9)
{
  int tmp;  /* 4 CMP ABY */
  m6509_ICount -= 4;
  RD_ABY;
  CMP;
}
OP(f9)
{
  int tmp;  /* 4 SBC ABY */
  m6509_ICount -= 4;
  RD_ABY;
  SBC;
}

OP(0a)
{
  int tmp;  /* 2 ASL A */
  m6509_ICount -= 2;
  RD_ACC;
  ASL;
  WB_ACC;
}
OP(2a)
{
  int tmp;  /* 2 ROL A */
  m6509_ICount -= 2;
  RD_ACC;
  ROL;
  WB_ACC;
}
OP(4a)
{
  int tmp;  /* 2 LSR A */
  m6509_ICount -= 2;
  RD_ACC;
  LSR;
  WB_ACC;
}
OP(6a)
{
  int tmp;  /* 2 ROR A */
  m6509_ICount -= 2;
  RD_ACC;
  ROR;
  WB_ACC;
}
OP(8a)
{
  m6509_ICount -= 2;		  /* 2 TXA */
  TXA;
}
OP(aa)
{
  m6509_ICount -= 2;		  /* 2 TAX */
  TAX;
}
OP(ca)
{
  m6509_ICount -= 2;		  /* 2 DEX */
  DEX;
}
OP(ea)
{
  m6509_ICount -= 2;		  /* 2 NOP */
  NOP;
}

OP(1a)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}
OP(3a)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}
OP(5a)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}
OP(7a)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}
OP(9a)
{
  m6509_ICount -= 2;		  /* 2 TXS */
  TXS;
}
OP(ba)
{
  m6509_ICount -= 2;		  /* 2 TSX */
  TSX;
}
OP(da)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}
OP(fa)
{
  m6502_ICount -= 2;		  /* 2 NOP */
  NOP;
}

OP(0b)
{
  int tmp;  /* 2 ANC IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ANC;
}
OP(2b)
{
  int tmp;  /* 2 ANC IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ANC;
}
OP(4b)
{
  int tmp;  /* 2 ASR IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ASR;
  WB_ACC;
}
OP(6b)
{
  int tmp;  /* 2 ARR IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ARR;
  WB_ACC;
}
OP(8b)
{
  int tmp;  /* 2 AXA IMM */
  m6502_ICount -= 2;
  RD_IMM;
  AXA;
}
OP(ab)
{
  int tmp;  /* 2 OAL IMM */
  m6502_ICount -= 2;
  RD_IMM;
  OAL;
}
OP(cb)
{
  int tmp;  /* 2 ASX IMM */
  m6502_ICount -= 2;
  RD_IMM;
  ASX;
}
OP(eb)
{
  int tmp;  /* 2 SBC IMM */
  m6502_ICount -= 2;
  RD_IMM;
  SBC;
}

OP(1b)
{
  int tmp;  /* 4 SLO ABY */
  m6502_ICount -= 4;
  RD_ABY;
  SLO;
  WB_EA;
}
OP(3b)
{
  int tmp;  /* 4 RLA ABY */
  m6502_ICount -= 4;
  RD_ABY;
  RLA;
  WB_EA;
}
OP(5b)
{
  int tmp;  /* 4 SRE ABY */
  m6502_ICount -= 4;
  RD_ABY;
  SRE;
  WB_EA;
}
OP(7b)
{
  int tmp;  /* 4 RRA ABY */
  m6502_ICount -= 4;
  RD_ABY;
  RRA;
  WB_EA;
}
OP(9b)
{
  int tmp;  /* 5 SSH ABY */
  m6502_ICount -= 5;
  EA_ABY;
  SSH;
  WB_EA;
}
OP(bb)
{
  int tmp;  /* 4 AST ABY */
  m6502_ICount -= 4;
  RD_ABY;
  AST;
}
OP(db)
{
  int tmp;  /* 6 DCP ABY */
  m6502_ICount -= 6;
  RD_ABY;
  DCP;
  WB_EA;
}
OP(fb)
{
  int tmp;  /* 6 ISB ABY */
  m6502_ICount -= 6;
  RD_ABY;
  ISB;
  WB_EA;
}

OP(0c)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}
OP(2c)
{
  int tmp;  /* 4 BIT ABS */
  m6509_ICount -= 4;
  RD_ABS;
  BIT;
}
OP(4c)
{
  m6509_ICount -= 3;  /* 3 JMP ABS */
  EA_ABS;
  JMP;
}
OP(6c)
{
  int tmp;  /* 5 JMP IND */
  m6509_ICount -= 5;
  EA_IND;
  JMP;
}
OP(8c)
{
  int tmp;  /* 4 STY ABS */
  m6509_ICount -= 4;
  STY;
  WR_ABS;
}
OP(ac)
{
  int tmp;  /* 4 LDY ABS */
  m6509_ICount -= 4;
  RD_ABS;
  LDY;
}
OP(cc)
{
  int tmp;  /* 4 CPY ABS */
  m6509_ICount -= 4;
  RD_ABS;
  CPY;
}
OP(ec)
{
  int tmp;  /* 4 CPX ABS */
  m6509_ICount -= 4;
  RD_ABS;
  CPX;
}

OP(1c)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}
OP(3c)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}
OP(5c)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}
OP(7c)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}
OP(9c)
{
  int tmp;  /* 5 SYH ABX */
  m6502_ICount -= 5;
  EA_ABX;
  SYH;
  WB_EA;
}
OP(bc)
{
  int tmp;  /* 4 LDY ABX */
  m6509_ICount -= 4;
  RD_ABX;
  LDY;
}
OP(dc)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}
OP(fc)
{
  m6502_ICount -= 2;		  /* 2 TOP */
  TOP;
}

OP(0d)
{
  int tmp;  /* 4 ORA ABS */
  m6509_ICount -= 4;
  RD_ABS;
  ORA;
}
OP(2d)
{
  int tmp;  /* 4 AND ABS */
  m6509_ICount -= 4;
  RD_ABS;
  AND;
}
OP(4d)
{
  int tmp;  /* 4 EOR ABS */
  m6509_ICount -= 4;
  RD_ABS;
  EOR;
}
OP(6d)
{
  int tmp;  /* 4 ADC ABS */
  m6509_ICount -= 4;
  RD_ABS;
  ADC;
}
OP(8d)
{
  int tmp;  /* 4 STA ABS */
  m6509_ICount -= 4;
  STA;
  WR_ABS;
}
OP(ad)
{
  int tmp;  /* 4 LDA ABS */
  m6509_ICount -= 4;
  RD_ABS;
  LDA;
}
OP(cd)
{
  int tmp;  /* 4 CMP ABS */
  m6509_ICount -= 4;
  RD_ABS;
  CMP;
}
OP(ed)
{
  int tmp;  /* 4 SBC ABS */
  m6509_ICount -= 4;
  RD_ABS;
  SBC;
}

OP(1d)
{
  int tmp;  /* 4 ORA ABX */
  m6509_ICount -= 4;
  RD_ABX;
  ORA;
}
OP(3d)
{
  int tmp;  /* 4 AND ABX */
  m6509_ICount -= 4;
  RD_ABX;
  AND;
}
OP(5d)
{
  int tmp;  /* 4 EOR ABX */
  m6509_ICount -= 4;
  RD_ABX;
  EOR;
}
OP(7d)
{
  int tmp;  /* 4 ADC ABX */
  m6509_ICount -= 4;
  RD_ABX;
  ADC;
}
OP(9d)
{
  int tmp;  /* 5 STA ABX */
  m6509_ICount -= 5;
  STA;
  WR_ABX;
}
OP(bd)
{
  int tmp;  /* 4 LDA ABX */
  m6509_ICount -= 4;
  RD_ABX;
  LDA;
}
OP(dd)
{
  int tmp;  /* 4 CMP ABX */
  m6509_ICount -= 4;
  RD_ABX;
  CMP;
}
OP(fd)
{
  int tmp;  /* 4 SBC ABX */
  m6509_ICount -= 4;
  RD_ABX;
  SBC;
}

OP(0e)
{
  int tmp;  /* 6 ASL ABS */
  m6509_ICount -= 6;
  RD_ABS;
  ASL;
  WB_EA;
}
OP(2e)
{
  int tmp;  /* 6 ROL ABS */
  m6509_ICount -= 6;
  RD_ABS;
  ROL;
  WB_EA;
}
OP(4e)
{
  int tmp;  /* 6 LSR ABS */
  m6509_ICount -= 6;
  RD_ABS;
  LSR;
  WB_EA;
}
OP(6e)
{
  int tmp;  /* 6 ROR ABS */
  m6509_ICount -= 6;
  RD_ABS;
  ROR;
  WB_EA;
}
OP(8e)
{
  int tmp;  /* 5 STX ABS */
  m6509_ICount -= 5;
  STX;
  WR_ABS;
}
OP(ae)
{
  int tmp;  /* 4 LDX ABS */
  m6509_ICount -= 4;
  RD_ABS;
  LDX;
}
OP(ce)
{
  int tmp;  /* 6 DEC ABS */
  m6509_ICount -= 6;
  RD_ABS;
  DEC;
  WB_EA;
}
OP(ee)
{
  int tmp;  /* 6 INC ABS */
  m6509_ICount -= 6;
  RD_ABS;
  INC;
  WB_EA;
}

OP(1e)
{
  int tmp;  /* 7 ASL ABX */
  m6509_ICount -= 7;
  RD_ABX;
  ASL;
  WB_EA;
}
OP(3e)
{
  int tmp;  /* 7 ROL ABX */
  m6509_ICount -= 7;
  RD_ABX;
  ROL;
  WB_EA;
}
OP(5e)
{
  int tmp;  /* 7 LSR ABX */
  m6509_ICount -= 7;
  RD_ABX;
  LSR;
  WB_EA;
}
OP(7e)
{
  int tmp;  /* 7 ROR ABX */
  m6509_ICount -= 7;
  RD_ABX;
  ROR;
  WB_EA;
}
OP(9e)
{
  int tmp;  /* 2 SXH ABY */
  m6502_ICount -= 2;
  EA_ABY;
  SXH;
  WB_EA;
}
OP(be)
{
  int tmp;  /* 4 LDX ABY */
  m6509_ICount -= 4;
  RD_ABY;
  LDX;
}
OP(de)
{
  int tmp;  /* 7 DEC ABX */
  m6509_ICount -= 7;
  RD_ABX;
  DEC;
  WB_EA;
}
OP(fe)
{
  int tmp;  /* 7 INC ABX */
  m6509_ICount -= 7;
  RD_ABX;
  INC;
  WB_EA;
}

OP(0f)
{
  int tmp;  /* 4 SLO ABS */
  m6502_ICount -= 6;
  RD_ABS;
  SLO;
  WB_EA;
}
OP(2f)
{
  int tmp;  /* 4 RLA ABS */
  m6502_ICount -= 6;
  RD_ABS;
  RLA;
  WB_EA;
}
OP(4f)
{
  int tmp;  /* 4 SRE ABS */
  m6502_ICount -= 6;
  RD_ABS;
  SRE;
  WB_EA;
}
OP(6f)
{
  int tmp;  /* 4 RRA ABS */
  m6502_ICount -= 6;
  RD_ABS;
  RRA;
  WB_EA;
}
OP(8f)
{
  int tmp;  /* 4 SAX ABS */
  m6502_ICount -= 4;
  SAX;
  WR_ABS;
}
OP(af)
{
  int tmp;  /* 4 LAX ABS */
  m6502_ICount -= 5;
  RD_ABS;
  LAX;
}
OP(cf)
{
  int tmp;  /* 6 DCP ABS */
  m6502_ICount -= 6;
  RD_ABS;
  DCP;
  WB_EA;
}
OP(ef)
{
  int tmp;  /* 6 ISB ABS */
  m6502_ICount -= 6;
  RD_ABS;
  ISB;
  WB_EA;
}

OP(1f)
{
  int tmp;  /* 4 SLO ABX */
  m6502_ICount -= 4;
  RD_ABX;
  SLO;
  WB_EA;
}
OP(3f)
{
  int tmp;  /* 4 RLA ABX */
  m6502_ICount -= 4;
  RD_ABX;
  RLA;
  WB_EA;
}
OP(5f)
{
  int tmp;  /* 4 SRE ABX */
  m6502_ICount -= 4;
  RD_ABX;
  SRE;
  WB_EA;
}
OP(7f)
{
  int tmp;  /* 4 RRA ABX */
  m6502_ICount -= 4;
  RD_ABX;
  RRA;
  WB_EA;
}
OP(9f)
{
  int tmp;  /* 5 SAH ABY */
  m6502_ICount -= 6;
  EA_ABY;
  SAH;
  WB_EA;
}
OP(bf)
{
  int tmp;  /* 4 LAX ABY */
  m6502_ICount -= 6;
  RD_ABY;
  LAX;
}
OP(df)
{
  int tmp;  /* 7 DCP ABX */
  m6502_ICount -= 7;
  RD_ABX;
  DCP;
  WB_EA;
}
OP(ff)
{
  int tmp;  /* 7 ISB ABX */
  m6502_ICount -= 7;
  RD_ABX;
  ISB;
  WB_EA;
}

static void (*insn6509[0x100])(void) =
{
  m6509_00,m6509_01,m6509_02,m6509_03,m6509_04,m6509_05,m6509_06,m6509_07,
  m6509_08,m6509_09,m6509_0a,m6509_0b,m6509_0c,m6509_0d,m6509_0e,m6509_0f,
  m6509_10,m6509_11,m6509_12,m6509_13,m6509_14,m6509_15,m6509_16,m6509_17,
  m6509_18,m6509_19,m6509_1a,m6509_1b,m6509_1c,m6509_1d,m6509_1e,m6509_1f,
  m6509_20,m6509_21,m6509_22,m6509_23,m6509_24,m6509_25,m6509_26,m6509_27,
  m6509_28,m6509_29,m6509_2a,m6509_2b,m6509_2c,m6509_2d,m6509_2e,m6509_2f,
  m6509_30,m6509_31,m6509_32,m6509_33,m6509_34,m6509_35,m6509_36,m6509_37,
  m6509_38,m6509_39,m6509_3a,m6509_3b,m6509_3c,m6509_3d,m6509_3e,m6509_3f,
  m6509_40,m6509_41,m6509_42,m6509_43,m6509_44,m6509_45,m6509_46,m6509_47,
  m6509_48,m6509_49,m6509_4a,m6509_4b,m6509_4c,m6509_4d,m6509_4e,m6509_4f,
  m6509_50,m6509_51,m6509_52,m6509_53,m6509_54,m6509_55,m6509_56,m6509_57,
  m6509_58,m6509_59,m6509_5a,m6509_5b,m6509_5c,m6509_5d,m6509_5e,m6509_5f,
  m6509_60,m6509_61,m6509_62,m6509_63,m6509_64,m6509_65,m6509_66,m6509_67,
  m6509_68,m6509_69,m6509_6a,m6509_6b,m6509_6c,m6509_6d,m6509_6e,m6509_6f,
  m6509_70,m6509_71,m6509_72,m6509_73,m6509_74,m6509_75,m6509_76,m6509_77,
  m6509_78,m6509_79,m6509_7a,m6509_7b,m6509_7c,m6509_7d,m6509_7e,m6509_7f,
  m6509_80,m6509_81,m6509_82,m6509_83,m6509_84,m6509_85,m6509_86,m6509_87,
  m6509_88,m6509_89,m6509_8a,m6509_8b,m6509_8c,m6509_8d,m6509_8e,m6509_8f,
  m6509_90,m6509_91,m6509_92,m6509_93,m6509_94,m6509_95,m6509_96,m6509_97,
  m6509_98,m6509_99,m6509_9a,m6509_9b,m6509_9c,m6509_9d,m6509_9e,m6509_9f,
  m6509_a0,m6509_a1,m6509_a2,m6509_a3,m6509_a4,m6509_a5,m6509_a6,m6509_a7,
  m6509_a8,m6509_a9,m6509_aa,m6509_ab,m6509_ac,m6509_ad,m6509_ae,m6509_af,
  m6509_b0,m6509_b1,m6509_b2,m6509_b3,m6509_b4,m6509_b5,m6509_b6,m6509_b7,
  m6509_b8,m6509_b9,m6509_ba,m6509_bb,m6509_bc,m6509_bd,m6509_be,m6509_bf,
  m6509_c0,m6509_c1,m6509_c2,m6509_c3,m6509_c4,m6509_c5,m6509_c6,m6509_c7,
  m6509_c8,m6509_c9,m6509_ca,m6509_cb,m6509_cc,m6509_cd,m6509_ce,m6509_cf,
  m6509_d0,m6509_d1,m6509_d2,m6509_d3,m6509_d4,m6509_d5,m6509_d6,m6509_d7,
  m6509_d8,m6509_d9,m6509_da,m6509_db,m6509_dc,m6509_dd,m6509_de,m6509_df,
  m6509_e0,m6509_e1,m6509_e2,m6509_e3,m6509_e4,m6509_e5,m6509_e6,m6509_e7,
  m6509_e8,m6509_e9,m6509_ea,m6509_eb,m6509_ec,m6509_ed,m6509_ee,m6509_ef,
  m6509_f0,m6509_f1,m6509_f2,m6509_f3,m6509_f4,m6509_f5,m6509_f6,m6509_f7,
  m6509_f8,m6509_f9,m6509_fa,m6509_fb,m6509_fc,m6509_fd,m6509_fe,m6509_ff
};


