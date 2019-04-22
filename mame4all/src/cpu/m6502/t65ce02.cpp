/*****************************************************************************
 *
 *	 t65ce02.c
 *	 65ce02 opcode functions and function pointer table
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
 *****************************************************************************/

#undef	OP
#ifdef M4510
  #define OP(nn) INLINE void m4510_##nn(void)
#else
  #define OP(nn) INLINE void m65ce02_##nn(void)
#endif

/*****************************************************************************
 *****************************************************************************
 *
 *	 overrides for 65C02 opcodes
 *
 *****************************************************************************
 * op	 temp	  cycles			 rdmem	 opc  wrmem   ********************/
OP(00)
{
  m65ce02_ICount-=7;		  /* 7 BRK */
  BRK;
}
OP(20)
{
  m65ce02_ICount-=6;		  /* 6 JSR */
  JSR;
}
OP(40)
{
  m65ce02_ICount-=6;		  /* 6 RTI */
  RTI;
}
OP(60)
{
  m65ce02_ICount-=6;		  /* 6 RTS */
  RTS;
}
OP(80)
{
  int tmp;							  /* 2 BRA */
  BRA(1);
}
OP(a0)
{
  int tmp;  /* 2 LDY IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  LDY;
}
OP(c0)
{
  int tmp;  /* 2 CPY IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  CPY;
}
OP(e0)
{
  int tmp;  /* 2 CPX IMM */
  m65ce02_ICount-=2;
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
  m65ce02_ICount-=6;
  RD_IDX;
  ORA;
}
OP(21)
{
  int tmp;  /* 6 AND IDX */
  m65ce02_ICount-=6;
  RD_IDX;
  AND;
}
OP(41)
{
  int tmp;  /* 6 EOR IDX */
  m65ce02_ICount-=6;
  RD_IDX;
  EOR;
}
OP(61)
{
  int tmp;  /* 6 ADC IDX */
  m65ce02_ICount-=6;
  RD_IDX;
  ADC;
}
OP(81)
{
  int tmp;  /* 6 STA IDX */
  m65ce02_ICount-=6;
  STA;
  WR_IDX;
}
OP(a1)
{
  int tmp;  /* 6 LDA IDX */
  m65ce02_ICount-=6;
  RD_IDX;
  LDA;
}
OP(c1)
{
  int tmp;  /* 6 CMP IDX */
  m65ce02_ICount-=6;
  RD_IDX;
  CMP;
}
OP(e1)
{
  int tmp;  /* 6 SBC IDX */
  m65ce02_ICount-=6;
  RD_IDX;
  SBC;
}

OP(11)
{
  int tmp;  /* 5 ORA IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  ORA;
}
OP(31)
{
  int tmp;  /* 5 AND IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  AND;
}
OP(51)
{
  int tmp;  /* 5 EOR IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  EOR;
}
OP(71)
{
  int tmp;  /* 5 ADC IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  ADC;
}
OP(91)
{
  int tmp;  /* 6 STA IDY */
  m65ce02_ICount-=6;
  STA;
  WR_IDY;
}
OP(b1)
{
  int tmp;  /* 5 LDA IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  LDA;
}
OP(d1)
{
  int tmp;  /* 5 CMP IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  CMP;
}
OP(f1)
{
  int tmp;  /* 5 SBC IDY */
  m65ce02_ICount-=5;
  RD_IDY;
  SBC;
}

OP(02)
{
  m65ce02_ICount-=2;		  /* ? CLE */
  CLE;
}
OP(22)
{
  m65ce02_ICount-=5;		  /* ? JSR IND */
  JSR_IND;
}
OP(42)
{
  m65ce02_ICount-=2;		  /* 2 NEG */
  NEG;
}
OP(62)
{
  int tmp;  /* ? RTS IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  RTS_IMM;
}
OP(82)
{
  int tmp;  /* 5 STA INSY */
  m65ce02_ICount-=5;
  RD_INSY;
  STA;
}
OP(a2)
{
  int tmp;  /* 2 LDX IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  LDX;
}
OP(c2)
{
  int tmp;  /* 2 CPZ IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  CPZ;
}
OP(e2)
{
  int tmp;  /* ? LDA INSY */
  m65ce02_ICount-=5;
  RD_INSY;
  LDA;
}

OP(12)
{
  int tmp;  /* 5 ORA IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  ORA;
}
OP(32)
{
  int tmp;  /* 5 AND IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  AND;
}
OP(52)
{
  int tmp;  /* 5 EOR IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  EOR;
}
OP(72)
{
  int tmp;  /* 5 ADC IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  ADC;
}
OP(92)
{
  int tmp;  /* 5 STA IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  STA;
}
OP(b2)
{
  int tmp;  /* 5 LDA IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  LDA;
}
OP(d2)
{
  int tmp;  /* 5 CMP IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  CMP;
}
OP(f2)
{
  int tmp;  /* 5 SBC IDZ */
  m65ce02_ICount-=5;
  RD_IDZ;
  SBC;
}

OP(03)
{
  m65ce02_ICount-=2;		  /* ? SEE */
  SEE;
}
OP(23)
{
  m65ce02_ICount-=6;		  /* ? JSR INDX */
  JSR_INDX;
}
OP(43)
{
  int tmp;  /* 2 ASR A */
  m65ce02_ICount-=2;
  RD_ACC;
  ASR_65CE02;
  WB_ACC;
}
OP(63)
{
  m65ce02_ICount-=6;		  /* ? BSR */
  BSR;
}
OP(83)
{
  BRA_WORD(1);  /* ? BRA REL WORD */
}
OP(a3)
{
  int tmp;  /* 2 LDZ IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  LDZ;
}
OP(c3)
{
  PAIR tmp;  /* ? DEW ABS */
  m65ce02_ICount-=9;
  RD_ZPG_WORD;
  DEW;
  WB_EA_WORD;
}
OP(e3)
{
  PAIR tmp;  /* ? INW ABS */
  m65ce02_ICount-=9;
  RD_ZPG_WORD;
  INW;
  WB_EA_WORD;
}

OP(13)
{
  BPL_WORD;	    /* ? BPL REL WORD */
}
OP(33)
{
  BMI_WORD;	    /* ? BMI REL WORD */
}
OP(53)
{
  BVC_WORD;	    /* ? BVC REL WORD */
}
OP(73)
{
  BVS_WORD;	    /* ? BVS REL WORD */
}
OP(93)
{
  BCC_WORD;	    /* ? BCC REL WORD */
}
OP(b3)
{
  BCS_WORD;	    /* ? BCS REL WORD */
}
OP(d3)
{
  BNE_WORD;	    /* ? BNE REL WORD */
}
OP(f3)
{
  BEQ_WORD;	    /* ? BEQ REL WORD */
}

OP(04)
{
  int tmp;  /* 3 TSB ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  TSB;
  WB_EA;
}
OP(24)
{
  int tmp;  /* 3 BIT ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  BIT;
}
OP(44)
{
  int tmp;  /* 5 ASR ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  ASR_65CE02;
  WB_EA;
}
OP(64)
{
  int tmp;  /* 3 STZ ZPG */
  m65ce02_ICount-=3;
  STZ_65CE02;
  WR_ZPG;
}
OP(84)
{
  int tmp;  /* 3 STY ZPG */
  m65ce02_ICount-=3;
  STY;
  WR_ZPG;
}
OP(a4)
{
  int tmp;  /* 3 LDY ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  LDY;
}
OP(c4)
{
  int tmp;  /* 3 CPY ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  CPY;
}
OP(e4)
{
  int tmp;  /* 3 CPX ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  CPX;
}

OP(14)
{
  int tmp;  /* 3 TRB ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  TRB;
  WB_EA;
}
OP(34)
{
  int tmp;  /* 4 BIT ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  BIT;
}
OP(54)
{
  int tmp;  /* 6 ASR ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  ASR_65CE02;
  WB_EA;
}
OP(74)
{
  int tmp;  /* 4 STZ ZPX */
  m65ce02_ICount-=4;
  STZ_65CE02;
  WR_ZPX;
}
OP(94)
{
  int tmp;  /* 4 STY ZPX */
  m65ce02_ICount-=4;
  STY;
  WR_ZPX;
}
OP(b4)
{
  int tmp;  /* 4 LDY ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  LDY;
}
OP(d4)
{
  int tmp;  /* 3 CPZ ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  CPZ;
}
OP(f4)
{
  PAIR tmp;  /* ? PHW imm16 */
  m65ce02_ICount-=6;
  RD_IMM_WORD;
  PUSH_WORD(tmp);
}

OP(05)
{
  int tmp;  /* 3 ORA ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  ORA;
}
OP(25)
{
  int tmp;  /* 3 AND ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  AND;
}
OP(45)
{
  int tmp;  /* 3 EOR ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  EOR;
}
OP(65)
{
  int tmp;  /* 3 ADC ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  ADC;
}
OP(85)
{
  int tmp;  /* 3 STA ZPG */
  m65ce02_ICount-=3;
  STA;
  WR_ZPG;
}
OP(a5)
{
  int tmp;  /* 3 LDA ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  LDA;
}
OP(c5)
{
  int tmp;  /* 3 CMP ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  CMP;
}
OP(e5)
{
  int tmp;  /* 3 SBC ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  SBC;
}

OP(15)
{
  int tmp;  /* 4 ORA ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  ORA;
}
OP(35)
{
  int tmp;  /* 4 AND ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  AND;
}
OP(55)
{
  int tmp;  /* 4 EOR ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  EOR;
}
OP(75)
{
  int tmp;  /* 4 ADC ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  ADC;
}
OP(95)
{
  int tmp;  /* 4 STA ZPX */
  m65ce02_ICount-=4;
  STA;
  WR_ZPX;
}
OP(b5)
{
  int tmp;  /* 4 LDA ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  LDA;
}
OP(d5)
{
  int tmp;  /* 4 CMP ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  CMP;
}
OP(f5)
{
  int tmp;  /* 4 SBC ZPX */
  m65ce02_ICount-=4;
  RD_ZPX;
  SBC;
}

OP(06)
{
  int tmp;  /* 5 ASL ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  ASL;
  WB_EA;
}
OP(26)
{
  int tmp;  /* 5 ROL ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  ROL;
  WB_EA;
}
OP(46)
{
  int tmp;  /* 5 LSR ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  LSR;
  WB_EA;
}
OP(66)
{
  int tmp;  /* 5 ROR ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  ROR;
  WB_EA;
}
OP(86)
{
  int tmp;  /* 3 STX ZPG */
  m65ce02_ICount-=3;
  STX;
  WR_ZPG;
}
OP(a6)
{
  int tmp;  /* 3 LDX ZPG */
  m65ce02_ICount-=3;
  RD_ZPG;
  LDX;
}
OP(c6)
{
  int tmp;  /* 5 DEC ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  DEC;
  WB_EA;
}
OP(e6)
{
  int tmp;  /* 5 INC ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  INC;
  WB_EA;
}

OP(16)
{
  int tmp;  /* 6 ASL ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  ASL;
  WB_EA;
}
OP(36)
{
  int tmp;  /* 6 ROL ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  ROL;
  WB_EA;
}
OP(56)
{
  int tmp;  /* 6 LSR ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  LSR;
  WB_EA;
}
OP(76)
{
  int tmp;  /* 6 ROR ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  ROR;
  WB_EA;
}
OP(96)
{
  int tmp;  /* 4 STX ZPY */
  m65ce02_ICount-=4;
  STX;
  WR_ZPY;
}
OP(b6)
{
  int tmp;  /* 4 LDX ZPY */
  m65ce02_ICount-=4;
  RD_ZPY;
  LDX;
}
OP(d6)
{
  int tmp;  /* 6 DEC ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  DEC;
  WB_EA;
}
OP(f6)
{
  int tmp;  /* 6 INC ZPX */
  m65ce02_ICount-=6;
  RD_ZPX;
  INC;
  WB_EA;
}

OP(07)
{
  int tmp;  /* 5 RMB0 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(0);
  WB_EA;
}
OP(27)
{
  int tmp;  /* 5 RMB2 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(2);
  WB_EA;
}
OP(47)
{
  int tmp;  /* 5 RMB4 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(4);
  WB_EA;
}
OP(67)
{
  int tmp;  /* 5 RMB6 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(6);
  WB_EA;
}
OP(87)
{
  int tmp;  /* 5 SMB0 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(0);
  WB_EA;
}
OP(a7)
{
  int tmp;  /* 5 SMB2 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(2);
  WB_EA;
}
OP(c7)
{
  int tmp;  /* 5 SMB4 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(4);
  WB_EA;
}
OP(e7)
{
  int tmp;  /* 5 SMB6 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(6);
  WB_EA;
}

OP(17)
{
  int tmp;  /* 5 RMB1 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(1);
  WB_EA;
}
OP(37)
{
  int tmp;  /* 5 RMB3 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(3);
  WB_EA;
}
OP(57)
{
  int tmp;  /* 5 RMB5 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(5);
  WB_EA;
}
OP(77)
{
  int tmp;  /* 5 RMB7 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  RMB(7);
  WB_EA;
}
OP(97)
{
  int tmp;  /* 5 SMB1 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(1);
  WB_EA;
}
OP(b7)
{
  int tmp;  /* 5 SMB3 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(3);
  WB_EA;
}
OP(d7)
{
  int tmp;  /* 5 SMB5 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(5);
  WB_EA;
}
OP(f7)
{
  int tmp;  /* 5 SMB7 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  SMB(7);
  WB_EA;
}

OP(08)
{
  m65ce02_ICount-=2;		  /* 2 PHP */
  PHP;
}
OP(28)
{
  m65ce02_ICount-=2;		  /* 2 PLP */
  PLP;
}
OP(48)
{
  m65ce02_ICount-=2;		  /* 2 PHA */
  PHA;
}
OP(68)
{
  m65ce02_ICount-=2;		  /* 2 PLA */
  PLA;
}
OP(88)
{
  m65ce02_ICount-=2;		  /* 2 DEY */
  DEY;
}
OP(a8)
{
  m65ce02_ICount-=2;		  /* 2 TAY */
  TAY;
}
OP(c8)
{
  m65ce02_ICount-=2;		  /* 2 INY */
  INY;
}
OP(e8)
{
  m65ce02_ICount-=2;		  /* 2 INX */
  INX;
}

OP(18)
{
  m65ce02_ICount-=2;		  /* 2 CLC */
  CLC;
}
OP(38)
{
  m65ce02_ICount-=2;		  /* 2 SEC */
  SEC;
}
OP(58)
{
  m65ce02_ICount-=2;		  /* 2 CLI */
  CLI;
}
OP(78)
{
  m65ce02_ICount-=2;		  /* 2 SEI */
  SEI;
}
OP(98)
{
  m65ce02_ICount-=2;		  /* 2 TYA */
  TYA;
}
OP(b8)
{
  m65ce02_ICount-=2;		  /* 2 CLV */
  CLV;
}
OP(d8)
{
  m65ce02_ICount-=2;		  /* 2 CLD */
  CLD;
}
OP(f8)
{
  m65ce02_ICount-=2;		  /* 2 SED */
  SED;
}

OP(09)
{
  int tmp;  /* 2 ORA IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  ORA;
}
OP(29)
{
  int tmp;  /* 2 AND IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  AND;
}
OP(49)
{
  int tmp;  /* 2 EOR IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  EOR;
}
OP(69)
{
  int tmp;  /* 2 ADC IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  ADC;
}
OP(89)
{
  int tmp;  /* 2 BIT IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  BIT;
}
OP(a9)
{
  int tmp;  /* 2 LDA IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  LDA;
}
OP(c9)
{
  int tmp;  /* 2 CMP IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  CMP;
}
OP(e9)
{
  int tmp;  /* 2 SBC IMM */
  m65ce02_ICount-=2;
  RD_IMM;
  SBC;
}

OP(19)
{
  int tmp;  /* 4 ORA ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  ORA;
}
OP(39)
{
  int tmp;  /* 4 AND ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  AND;
}
OP(59)
{
  int tmp;  /* 4 EOR ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  EOR;
}
OP(79)
{
  int tmp;  /* 4 ADC ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  ADC;
}
OP(99)
{
  int tmp;  /* 5 STA ABY */
  m65ce02_ICount-=5;
  STA;
  WR_ABY;
}
OP(b9)
{
  int tmp;  /* 4 LDA ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  LDA;
}
OP(d9)
{
  int tmp;  /* 4 CMP ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  CMP;
}
OP(f9)
{
  int tmp;  /* 4 SBC ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  SBC;
}

OP(0a)
{
  int tmp;  /* 2 ASL A */
  m65ce02_ICount-=2;
  RD_ACC;
  ASL;
  WB_ACC;
}
OP(2a)
{
  int tmp;  /* 2 ROL A */
  m65ce02_ICount-=2;
  RD_ACC;
  ROL;
  WB_ACC;
}
OP(4a)
{
  int tmp;  /* 2 LSR A */
  m65ce02_ICount-=2;
  RD_ACC;
  LSR;
  WB_ACC;
}
OP(6a)
{
  int tmp;  /* 2 ROR A */
  m65ce02_ICount-=2;
  RD_ACC;
  ROR;
  WB_ACC;
}
OP(8a)
{
  m65ce02_ICount-=2;		  /* 2 TXA */
  TXA;
}
OP(aa)
{
  m65ce02_ICount-=2;		  /* 2 TAX */
  TAX;
}
OP(ca)
{
  m65ce02_ICount-=2;		  /* 2 DEX */
  DEX;
}
OP(ea)
{
  m65ce02_ICount-=2;		  /* 2 NOP */
  NOP;
}

OP(1a)
{
  m65ce02_ICount-=2;		  /* 2 INA */
  INA;
}
OP(3a)
{
  m65ce02_ICount-=2;		  /* 2 DEA */
  DEA;
}
OP(5a)
{
  m65ce02_ICount-=3;		  /* 3 PHY */
  PHY;
}
OP(7a)
{
  m65ce02_ICount-=4;		  /* 4 PLY */
  PLY;
}
OP(9a)
{
  m65ce02_ICount-=2;		  /* 2 TXS */
  TXS;
}
OP(ba)
{
  m65ce02_ICount-=2;		  /* 2 TSX */
  TSX;
}
OP(da)
{
  m65ce02_ICount-=3;		  /* 3 PHX */
  PHX;
}
OP(fa)
{
  m65ce02_ICount-=4;		  /* 4 PLX */
  PLX;
}

OP(0b)
{
  m65ce02_ICount-=2;		  /* 2 TSY */
  TSY;
}
OP(2b)
{
  m65ce02_ICount-=2;		  /* 2 TYS */
  TYS;
}
OP(4b)
{
  m65ce02_ICount-=2;		  /* 2 TAZ */
  TAZ;
}
OP(6b)
{
  m65ce02_ICount-=2;		  /* 2 TZA */
  TZA;
}
OP(8b)
{
  int tmp;  /* 5 STY ABX */
  m65ce02_ICount-=5;
  STY;
  WR_ABX;
}
OP(ab)
{
  int tmp;  /* 4 LDZ ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  LDZ;
}
OP(cb)
{
  PAIR tmp;  /* ? ASW ABS */
  m65ce02_ICount-=8;
  RD_ABS_WORD;
  ASW;
  WB_EA_WORD;
}
OP(eb)
{
  PAIR tmp;  /* ? roW ABS */
  m65ce02_ICount-=8;
  RD_ABS_WORD;
  ROW;
  WB_EA_WORD;
}

OP(1b)
{
  m65ce02_ICount-=2;		  /* 2 INZ */
  INZ;
}
OP(3b)
{
  m65ce02_ICount-=2;		  /* 2 DEZ */
  DEZ;
}
OP(5b)
{
  m65ce02_ICount-=2;		  /* 2 TAB */
  TAB;
}
OP(7b)
{
  m65ce02_ICount-=2;		  /* 2 TBA */
  TBA;
}
OP(9b)
{
  int tmp;  /* 5 STX ABY */
  m65ce02_ICount-=5;
  STX;
  WR_ABY;
}
OP(bb)
{
  int tmp;  /* 4 LDZ ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  LDZ;
}
OP(db)
{
  m65ce02_ICount-=2;		  /* 2 PHZ */
  PHZ;
}
OP(fb)
{
  m65ce02_ICount-=2;		  /* 2 PLZ */
  PLZ;
}

OP(0c)
{
  int tmp;  /* 4 TSB ABS */
  m65ce02_ICount-=2;
  RD_ABS;
  TSB;
  WB_EA;
}
OP(2c)
{
  int tmp;  /* 4 BIT ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  BIT;
}
OP(4c)
{
  m65ce02_ICount-=3;  /* 3 JMP ABS */
  EA_ABS;
  JMP;
}
OP(6c)
{
  int tmp;  /* 5 JMP IND */
  m65ce02_ICount-=5;
  EA_IND;
  JMP;
}
OP(8c)
{
  int tmp;  /* 4 STY ABS */
  m65ce02_ICount-=4;
  STY;
  WR_ABS;
}
OP(ac)
{
  int tmp;  /* 4 LDY ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  LDY;
}
OP(cc)
{
  int tmp;  /* 4 CPY ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  CPY;
}
OP(ec)
{
  int tmp;  /* 4 CPX ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  CPX;
}

OP(1c)
{
  int tmp;  /* 4 TRB ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  TRB;
  WB_EA;
}
OP(3c)
{
  int tmp;  /* 4 BIT ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  BIT;
}
#ifdef M4510
OP(5c)
{
  m65ce02_ICount-=2;		  /* ? MAP */
  MAP;
}
#else
/* maybe memory management not in
  but I think it is in, and the additional address pins are
  not available */
/* preliminary databook says reserved */
/* nop with 3 byte argument */
OP(5c)
{
  m65ce02_ICount-=2;		  /* ? MAP */
  MAP;
}
#endif
OP(7c)
{
  int tmp;  /* 6 JMP IAX */
  m65ce02_ICount-=2;
  EA_IAX;
  JMP;
}
OP(9c)
{
  int tmp;  /* 4 STZ ABS */
  m65ce02_ICount-=4;
  STZ_65CE02;
  WR_ABS;
}
OP(bc)
{
  int tmp;  /* 4 LDY ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  LDY;
}
OP(dc)
{
  int tmp;  /* 3 CPZ ABS */
  m65ce02_ICount-=3;
  RD_ABS;
  CPZ;
}
OP(fc)
{
  PAIR tmp;  /* ? PHW ab */
  m65ce02_ICount-=6;
  RD_ABS_WORD;
  PUSH_WORD(tmp);
}

OP(0d)
{
  int tmp;  /* 4 ORA ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  ORA;
}
OP(2d)
{
  int tmp;  /* 4 AND ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  AND;
}
OP(4d)
{
  int tmp;  /* 4 EOR ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  EOR;
}
OP(6d)
{
  int tmp;  /* 4 ADC ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  ADC;
}
OP(8d)
{
  int tmp;  /* 4 STA ABS */
  m65ce02_ICount-=4;
  STA;
  WR_ABS;
}
OP(ad)
{
  int tmp;  /* 4 LDA ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  LDA;
}
OP(cd)
{
  int tmp;  /* 4 CMP ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  CMP;
}
OP(ed)
{
  int tmp;  /* 4 SBC ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  SBC;
}

OP(1d)
{
  int tmp;  /* 4 ORA ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  ORA;
}
OP(3d)
{
  int tmp;  /* 4 AND ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  AND;
}
OP(5d)
{
  int tmp;  /* 4 EOR ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  EOR;
}
OP(7d)
{
  int tmp;  /* 4 ADC ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  ADC;
}
OP(9d)
{
  int tmp;  /* 5 STA ABX */
  m65ce02_ICount-=5;
  STA;
  WR_ABX;
}
OP(bd)
{
  int tmp;  /* 4 LDA ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  LDA;
}
OP(dd)
{
  int tmp;  /* 4 CMP ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  CMP;
}
OP(fd)
{
  int tmp;  /* 4 SBC ABX */
  m65ce02_ICount-=4;
  RD_ABX;
  SBC;
}

OP(0e)
{
  int tmp;  /* 6 ASL ABS */
  m65ce02_ICount-=6;
  RD_ABS;
  ASL;
  WB_EA;
}
OP(2e)
{
  int tmp;  /* 6 ROL ABS */
  m65ce02_ICount-=6;
  RD_ABS;
  ROL;
  WB_EA;
}
OP(4e)
{
  int tmp;  /* 6 LSR ABS */
  m65ce02_ICount-=6;
  RD_ABS;
  LSR;
  WB_EA;
}
OP(6e)
{
  int tmp;  /* 6 ROR ABS */
  m65ce02_ICount-=6;
  RD_ABS;
  ROR;
  WB_EA;
}
OP(8e)
{
  int tmp;  /* 5 STX ABS */
  m65ce02_ICount-=5;
  STX;
  WR_ABS;
}
OP(ae)
{
  int tmp;  /* 4 LDX ABS */
  m65ce02_ICount-=4;
  RD_ABS;
  LDX;
}
OP(ce)
{
  int tmp;  /* 6 DEC ABS */
  m65ce02_ICount-=6;
  RD_ABS;
  DEC;
  WB_EA;
}
OP(ee)
{
  int tmp;  /* 6 INC ABS */
  m65ce02_ICount-=6;
  RD_ABS;
  INC;
  WB_EA;
}

OP(1e)
{
  int tmp;  /* 7 ASL ABX */
  m65ce02_ICount-=7;
  RD_ABX;
  ASL;
  WB_EA;
}
OP(3e)
{
  int tmp;  /* 7 ROL ABX */
  m65ce02_ICount-=7;
  RD_ABX;
  ROL;
  WB_EA;
}
OP(5e)
{
  int tmp;  /* 7 LSR ABX */
  m65ce02_ICount-=7;
  RD_ABX;
  LSR;
  WB_EA;
}
OP(7e)
{
  int tmp;  /* 7 ROR ABX */
  m65ce02_ICount-=7;
  RD_ABX;
  ROR;
  WB_EA;
}
OP(9e)
{
  int tmp;  /* 5 STZ ABX */
  m65ce02_ICount-=5;
  STZ_65CE02;
  WR_ABX;
}
OP(be)
{
  int tmp;  /* 4 LDX ABY */
  m65ce02_ICount-=4;
  RD_ABY;
  LDX;
}
OP(de)
{
  int tmp;  /* 7 DEC ABX */
  m65ce02_ICount-=7;
  RD_ABX;
  DEC;
  WB_EA;
}
OP(fe)
{
  int tmp;  /* 7 INC ABX */
  m65ce02_ICount-=7;
  RD_ABX;
  INC;
  WB_EA;
}

OP(0f)
{
  int tmp;  /* 5 BBR0 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(0);
}
OP(2f)
{
  int tmp;  /* 5 BBR2 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(2);
}
OP(4f)
{
  int tmp;  /* 5 BBR4 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(4);
}
OP(6f)
{
  int tmp;  /* 5 BBR6 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(6);
}
OP(8f)
{
  int tmp;  /* 5 BBS0 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(0);
}
OP(af)
{
  int tmp;  /* 5 BBS2 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(2);
}
OP(cf)
{
  int tmp;  /* 5 BBS4 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(4);
}
OP(ef)
{
  int tmp;  /* 5 BBS6 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(6);
}

OP(1f)
{
  int tmp;  /* 5 BBR1 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(1);
}
OP(3f)
{
  int tmp;  /* 5 BBR3 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(3);
}
OP(5f)
{
  int tmp;  /* 5 BBR5 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(5);
}
OP(7f)
{
  int tmp;  /* 5 BBR7 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBR(7);
}
OP(9f)
{
  int tmp;  /* 5 BBS1 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(1);
}
OP(bf)
{
  int tmp;  /* 5 BBS3 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(3);
}
OP(df)
{
  int tmp;  /* 5 BBS5 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(5);
}
OP(ff)
{
  int tmp;  /* 5 BBS7 ZPG */
  m65ce02_ICount-=5;
  RD_ZPG;
  BBS(7);
}

#ifdef M4510
static void (*insn4510[0x100])(void) =
{
  m4510_00,m4510_01,m4510_02,m4510_03,m4510_04,m4510_05,m4510_06,m4510_07,
  m4510_08,m4510_09,m4510_0a,m4510_0b,m4510_0c,m4510_0d,m4510_0e,m4510_0f,
  m4510_10,m4510_11,m4510_12,m4510_13,m4510_14,m4510_15,m4510_16,m4510_17,
  m4510_18,m4510_19,m4510_1a,m4510_1b,m4510_1c,m4510_1d,m4510_1e,m4510_1f,
  m4510_20,m4510_21,m4510_22,m4510_23,m4510_24,m4510_25,m4510_26,m4510_27,
  m4510_28,m4510_29,m4510_2a,m4510_2b,m4510_2c,m4510_2d,m4510_2e,m4510_2f,
  m4510_30,m4510_31,m4510_32,m4510_33,m4510_34,m4510_35,m4510_36,m4510_37,
  m4510_38,m4510_39,m4510_3a,m4510_3b,m4510_3c,m4510_3d,m4510_3e,m4510_3f,
  m4510_40,m4510_41,m4510_42,m4510_43,m4510_44,m4510_45,m4510_46,m4510_47,
  m4510_48,m4510_49,m4510_4a,m4510_4b,m4510_4c,m4510_4d,m4510_4e,m4510_4f,
  m4510_50,m4510_51,m4510_52,m4510_53,m4510_54,m4510_55,m4510_56,m4510_57,
  m4510_58,m4510_59,m4510_5a,m4510_5b,m4510_5c,m4510_5d,m4510_5e,m4510_5f,
  m4510_60,m4510_61,m4510_62,m4510_63,m4510_64,m4510_65,m4510_66,m4510_67,
  m4510_68,m4510_69,m4510_6a,m4510_6b,m4510_6c,m4510_6d,m4510_6e,m4510_6f,
  m4510_70,m4510_71,m4510_72,m4510_73,m4510_74,m4510_75,m4510_76,m4510_77,
  m4510_78,m4510_79,m4510_7a,m4510_7b,m4510_7c,m4510_7d,m4510_7e,m4510_7f,
  m4510_80,m4510_81,m4510_82,m4510_83,m4510_84,m4510_85,m4510_86,m4510_87,
  m4510_88,m4510_89,m4510_8a,m4510_8b,m4510_8c,m4510_8d,m4510_8e,m4510_8f,
  m4510_90,m4510_91,m4510_92,m4510_93,m4510_94,m4510_95,m4510_96,m4510_97,
  m4510_98,m4510_99,m4510_9a,m4510_9b,m4510_9c,m4510_9d,m4510_9e,m4510_9f,
  m4510_a0,m4510_a1,m4510_a2,m4510_a3,m4510_a4,m4510_a5,m4510_a6,m4510_a7,
  m4510_a8,m4510_a9,m4510_aa,m4510_ab,m4510_ac,m4510_ad,m4510_ae,m4510_af,
  m4510_b0,m4510_b1,m4510_b2,m4510_b3,m4510_b4,m4510_b5,m4510_b6,m4510_b7,
  m4510_b8,m4510_b9,m4510_ba,m4510_bb,m4510_bc,m4510_bd,m4510_be,m4510_bf,
  m4510_c0,m4510_c1,m4510_c2,m4510_c3,m4510_c4,m4510_c5,m4510_c6,m4510_c7,
  m4510_c8,m4510_c9,m4510_ca,m4510_cb,m4510_cc,m4510_cd,m4510_ce,m4510_cf,
  m4510_d0,m4510_d1,m4510_d2,m4510_d3,m4510_d4,m4510_d5,m4510_d6,m4510_d7,
  m4510_d8,m4510_d9,m4510_da,m4510_db,m4510_dc,m4510_dd,m4510_de,m4510_df,
  m4510_e0,m4510_e1,m4510_e2,m4510_e3,m4510_e4,m4510_e5,m4510_e6,m4510_e7,
  m4510_e8,m4510_e9,m4510_ea,m4510_eb,m4510_ec,m4510_ed,m4510_ee,m4510_ef,
  m4510_f0,m4510_f1,m4510_f2,m4510_f3,m4510_f4,m4510_f5,m4510_f6,m4510_f7,
  m4510_f8,m4510_f9,m4510_fa,m4510_fb,m4510_fc,m4510_fd,m4510_fe,m4510_ff
};
#else
static void (*insn65ce02[0x100])(void) =
{
  m65ce02_00,m65ce02_01,m65ce02_02,m65ce02_03,m65ce02_04,m65ce02_05,m65ce02_06,m65ce02_07,
  m65ce02_08,m65ce02_09,m65ce02_0a,m65ce02_0b,m65ce02_0c,m65ce02_0d,m65ce02_0e,m65ce02_0f,
  m65ce02_10,m65ce02_11,m65ce02_12,m65ce02_13,m65ce02_14,m65ce02_15,m65ce02_16,m65ce02_17,
  m65ce02_18,m65ce02_19,m65ce02_1a,m65ce02_1b,m65ce02_1c,m65ce02_1d,m65ce02_1e,m65ce02_1f,
  m65ce02_20,m65ce02_21,m65ce02_22,m65ce02_23,m65ce02_24,m65ce02_25,m65ce02_26,m65ce02_27,
  m65ce02_28,m65ce02_29,m65ce02_2a,m65ce02_2b,m65ce02_2c,m65ce02_2d,m65ce02_2e,m65ce02_2f,
  m65ce02_30,m65ce02_31,m65ce02_32,m65ce02_33,m65ce02_34,m65ce02_35,m65ce02_36,m65ce02_37,
  m65ce02_38,m65ce02_39,m65ce02_3a,m65ce02_3b,m65ce02_3c,m65ce02_3d,m65ce02_3e,m65ce02_3f,
  m65ce02_40,m65ce02_41,m65ce02_42,m65ce02_43,m65ce02_44,m65ce02_45,m65ce02_46,m65ce02_47,
  m65ce02_48,m65ce02_49,m65ce02_4a,m65ce02_4b,m65ce02_4c,m65ce02_4d,m65ce02_4e,m65ce02_4f,
  m65ce02_50,m65ce02_51,m65ce02_52,m65ce02_53,m65ce02_54,m65ce02_55,m65ce02_56,m65ce02_57,
  m65ce02_58,m65ce02_59,m65ce02_5a,m65ce02_5b,m65ce02_5c,m65ce02_5d,m65ce02_5e,m65ce02_5f,
  m65ce02_60,m65ce02_61,m65ce02_62,m65ce02_63,m65ce02_64,m65ce02_65,m65ce02_66,m65ce02_67,
  m65ce02_68,m65ce02_69,m65ce02_6a,m65ce02_6b,m65ce02_6c,m65ce02_6d,m65ce02_6e,m65ce02_6f,
  m65ce02_70,m65ce02_71,m65ce02_72,m65ce02_73,m65ce02_74,m65ce02_75,m65ce02_76,m65ce02_77,
  m65ce02_78,m65ce02_79,m65ce02_7a,m65ce02_7b,m65ce02_7c,m65ce02_7d,m65ce02_7e,m65ce02_7f,
  m65ce02_80,m65ce02_81,m65ce02_82,m65ce02_83,m65ce02_84,m65ce02_85,m65ce02_86,m65ce02_87,
  m65ce02_88,m65ce02_89,m65ce02_8a,m65ce02_8b,m65ce02_8c,m65ce02_8d,m65ce02_8e,m65ce02_8f,
  m65ce02_90,m65ce02_91,m65ce02_92,m65ce02_93,m65ce02_94,m65ce02_95,m65ce02_96,m65ce02_97,
  m65ce02_98,m65ce02_99,m65ce02_9a,m65ce02_9b,m65ce02_9c,m65ce02_9d,m65ce02_9e,m65ce02_9f,
  m65ce02_a0,m65ce02_a1,m65ce02_a2,m65ce02_a3,m65ce02_a4,m65ce02_a5,m65ce02_a6,m65ce02_a7,
  m65ce02_a8,m65ce02_a9,m65ce02_aa,m65ce02_ab,m65ce02_ac,m65ce02_ad,m65ce02_ae,m65ce02_af,
  m65ce02_b0,m65ce02_b1,m65ce02_b2,m65ce02_b3,m65ce02_b4,m65ce02_b5,m65ce02_b6,m65ce02_b7,
  m65ce02_b8,m65ce02_b9,m65ce02_ba,m65ce02_bb,m65ce02_bc,m65ce02_bd,m65ce02_be,m65ce02_bf,
  m65ce02_c0,m65ce02_c1,m65ce02_c2,m65ce02_c3,m65ce02_c4,m65ce02_c5,m65ce02_c6,m65ce02_c7,
  m65ce02_c8,m65ce02_c9,m65ce02_ca,m65ce02_cb,m65ce02_cc,m65ce02_cd,m65ce02_ce,m65ce02_cf,
  m65ce02_d0,m65ce02_d1,m65ce02_d2,m65ce02_d3,m65ce02_d4,m65ce02_d5,m65ce02_d6,m65ce02_d7,
  m65ce02_d8,m65ce02_d9,m65ce02_da,m65ce02_db,m65ce02_dc,m65ce02_dd,m65ce02_de,m65ce02_df,
  m65ce02_e0,m65ce02_e1,m65ce02_e2,m65ce02_e3,m65ce02_e4,m65ce02_e5,m65ce02_e6,m65ce02_e7,
  m65ce02_e8,m65ce02_e9,m65ce02_ea,m65ce02_eb,m65ce02_ec,m65ce02_ed,m65ce02_ee,m65ce02_ef,
  m65ce02_f0,m65ce02_f1,m65ce02_f2,m65ce02_f3,m65ce02_f4,m65ce02_f5,m65ce02_f6,m65ce02_f7,
  m65ce02_f8,m65ce02_f9,m65ce02_fa,m65ce02_fb,m65ce02_fc,m65ce02_fd,m65ce02_fe,m65ce02_ff
};
#endif

