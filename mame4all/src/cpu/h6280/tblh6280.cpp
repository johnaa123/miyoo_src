/*****************************************************************************

    tblh6280.c

	Copyright (c) 1999 Bryan McPhail, mish@tendril.co.uk

	This source code is based (with permission!) on the 6502 emulator by
	Juergen Buchmueller.  It is released as part of the Mame emulator project.
	Let me know if you intend to use this code in any other project.

******************************************************************************/

#undef	OP
#define OP(nnn) INLINE void h6280_##nnn(void)

/*****************************************************************************
 *****************************************************************************
 *
 *	 Hu6280 opcodes
 *
 *****************************************************************************
 * op	  temp	   cycles		      rdmem	  opc   wrmem   ******************/
OP(000)
{
  h6280_ICount -= 8;		   // 8 BRK
  BRK;
}
OP(020)
{
  h6280_ICount -= 7;  // 7 JSR  ABS
  EA_ABS;
  JSR;
}
OP(040)
{
  h6280_ICount -= 7;		   // 7 RTI
  RTI;
}
OP(060)
{
  h6280_ICount -= 7;		   // 7 RTS
  RTS;
}
OP(080)
{
  int tmp;  				           // 4 BRA  REL
  BRA(1);
}
OP(0a0)
{
  int tmp;  // 2 LDY  IMM
  h6280_ICount -= 2;
  RD_IMM;
  LDY;
}
OP(0c0)
{
  int tmp;  // 2 CPY  IMM
  h6280_ICount -= 2;
  RD_IMM;
  CPY;
}
OP(0e0)
{
  int tmp;  // 2 CPX  IMM
  h6280_ICount -= 2;
  RD_IMM;
  CPX;
}

OP(010)
{
  int tmp;							   // 2/4 BPL  REL
  BPL;
}
OP(030)
{
  int tmp;							   // 2/4 BMI  REL
  BMI;
}
OP(050)
{
  int tmp;							   // 2/4 BVC  REL
  BVC;
}
OP(070)
{
  int tmp;							   // 2/4 BVS  REL
  BVS;
}
OP(090)
{
  int tmp;							   // 2/4 BCC  REL
  BCC;
}
OP(0b0)
{
  int tmp;							   // 2/4 BCS  REL
  BCS;
}
OP(0d0)
{
  int tmp;							   // 2/4 BNE  REL
  BNE;
}
OP(0f0)
{
  int tmp;							   // 2/4 BEQ  REL
  BEQ;
}

OP(001)
{
  int tmp;  // 7 ORA  IDX
  h6280_ICount -= 7;
  RD_IDX;
  ORA;
}
OP(021)
{
  int tmp;  // 7 AND  IDX
  h6280_ICount -= 7;
  RD_IDX;
  AND;
}
OP(041)
{
  int tmp;  // 7 EOR  IDX
  h6280_ICount -= 7;
  RD_IDX;
  EOR;
}
OP(061)
{
  int tmp;  // 7 ADC  IDX
  h6280_ICount -= 7;
  RD_IDX;
  ADC;
}
OP(081)
{
  int tmp;  // 7 STA  IDX
  h6280_ICount -= 7;
  STA;
  WR_IDX;
}
OP(0a1)
{
  int tmp;  // 7 LDA  IDX
  h6280_ICount -= 7;
  RD_IDX;
  LDA;
}
OP(0c1)
{
  int tmp;  // 7 CMP  IDX
  h6280_ICount -= 7;
  RD_IDX;
  CMP;
}
OP(0e1)
{
  int tmp;  // 7 SBC  IDX
  h6280_ICount -= 7;
  RD_IDX;
  SBC;
}

OP(011)
{
  int tmp;  // 7 ORA  IDY
  h6280_ICount -= 7;
  RD_IDY;
  ORA;
}
OP(031)
{
  int tmp;  // 7 AND  IDY
  h6280_ICount -= 7;
  RD_IDY;
  AND;
}
OP(051)
{
  int tmp;  // 7 EOR  IDY
  h6280_ICount -= 7;
  RD_IDY;
  EOR;
}
OP(071)
{
  int tmp;  // 7 ADC  AZP
  h6280_ICount -= 7;
  RD_IDY;
  ADC;
}
OP(091)
{
  int tmp;  // 7 STA  IDY
  h6280_ICount -= 7;
  STA;
  WR_IDY;
}
OP(0b1)
{
  int tmp;  // 7 LDA  IDY
  h6280_ICount -= 7;
  RD_IDY;
  LDA;
}
OP(0d1)
{
  int tmp;  // 7 CMP  IDY
  h6280_ICount -= 7;
  RD_IDY;
  CMP;
}
OP(0f1)
{
  int tmp;  // 7 SBC  IDY
  h6280_ICount -= 7;
  RD_IDY;
  SBC;
}

OP(002)
{
  int tmp;  // 3 SXY
  h6280_ICount -= 3;
  SXY;
}
OP(022)
{
  int tmp;  // 3 SAX
  h6280_ICount -= 3;
  SAX;
}
OP(042)
{
  int tmp;  // 3 SAY
  h6280_ICount -= 3;
  SAY;
}
OP(062)
{
  h6280_ICount -= 2;		   // 2 CLA
  CLA;
}
OP(082)
{
  h6280_ICount -= 2;		   // 2 CLX
  CLX;
}
OP(0a2)
{
  int tmp;  // 2 LDX  IMM
  h6280_ICount -= 2;
  RD_IMM;
  LDX;
}
OP(0c2)
{
  h6280_ICount -= 2;		   // 2 CLY
  CLY;
}
OP(0e2)
{
  ILL;		    // 2 ???
}

OP(012)
{
  int tmp;  // 7 ORA  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  ORA;
}
OP(032)
{
  int tmp;  // 7 AND  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  AND;
}
OP(052)
{
  int tmp;  // 7 EOR  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  EOR;
}
OP(072)
{
  int tmp;  // 7 ADC  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  ADC;
}
OP(092)
{
  int tmp;  // 7 STA  ZPI
  h6280_ICount -= 7;
  STA;
  WR_ZPI;
}
OP(0b2)
{
  int tmp;  // 7 LDA  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  LDA;
}
OP(0d2)
{
  int tmp;  // 7 CMP  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  CMP;
}
OP(0f2)
{
  int tmp;  // 7 SBC  ZPI
  h6280_ICount -= 7;
  RD_ZPI;
  SBC;
}

OP(003)
{
  int tmp;  // 4 ST0  IMM
  h6280_ICount -= 4;
  RD_IMM;
  ST0;
}
OP(023)
{
  int tmp;  // 4 ST2  IMM
  h6280_ICount -= 4;
  RD_IMM;
  ST2;
}
OP(043)
{
  int tmp;  // 4 TMA
  h6280_ICount -= 4;
  RD_IMM;
  TMA;
}
OP(063)
{
  ILL;		    // 2 ???
}
OP(083)
{
  int tmp,tmp2;  // 7 TST  IMM,ZPG
  h6280_ICount -= 7;
  RD_IMM2;
  RD_ZPG;
  TST;
}
OP(0a3)
{
  int tmp,tmp2;  // 7 TST  IMM,ZPX
  h6280_ICount -= 7;
  RD_IMM2;
  RD_ZPX;
  TST;
}
OP(0c3)
{
  int to,from,length;			       // 6*l+17 TDD  XFER
  TDD;
}
OP(0e3)
{
  int to,from,length,alternate;        // 6*l+17 TIA  XFER
  TIA;
}

OP(013)
{
  int tmp;  // 4 ST1
  h6280_ICount -= 4;
  RD_IMM;
  ST1;
}
OP(033)
{
  ILL;		    // 2 ???
}
OP(053)
{
  int tmp;  // 5 TAM  IMM
  h6280_ICount -= 5;
  RD_IMM;
  TAM;
}
OP(073)
{
  int to,from,length;    			   // 6*l+17 TII  XFER
  TII;
}
OP(093)
{
  int tmp,tmp2;  // 8 TST  IMM,ABS
  h6280_ICount -= 8;
  RD_IMM2;
  RD_ABS;
  TST;
}
OP(0b3)
{
  int tmp,tmp2;  // 8 TST  IMM,ABX
  h6280_ICount -= 8;
  RD_IMM2;
  RD_ABX;
  TST;
}
OP(0d3)
{
  int to,from,length;			       // 6*l+17 TIN  XFER
  TIN;
}
OP(0f3)
{
  int to,from,length,alternate;        // 6*l+17 TAI  XFER
  TAI;
}

OP(004)
{
  int tmp;  // 6 TSB  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  TSB;
  WB_EAZ;
}
OP(024)
{
  int tmp;  // 4 BIT  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  BIT;
}
OP(044)
{
  int tmp;							   // 8 BSR  REL
  BSR;
}
OP(064)
{
  int tmp;  // 4 STZ  ZPG
  h6280_ICount -= 4;
  STZ;
  WR_ZPG;
}
OP(084)
{
  int tmp;  // 4 STY  ZPG
  h6280_ICount -= 4;
  STY;
  WR_ZPG;
}
OP(0a4)
{
  int tmp;  // 4 LDY  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  LDY;
}
OP(0c4)
{
  int tmp;  // 4 CPY  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  CPY;
}
OP(0e4)
{
  int tmp;  // 4 CPX  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  CPX;
}

OP(014)
{
  int tmp;  // 6 TRB  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  TRB;
  WB_EAZ;
}
OP(034)
{
  int tmp;  // 4 BIT  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  BIT;
}
OP(054)
{
  h6280_ICount -= 2;					    // 2 CSL
}
OP(074)
{
  int tmp;  // 4 STZ  ZPX
  h6280_ICount -= 4;
  STZ;
  WR_ZPX;
}
OP(094)
{
  int tmp;  // 4 STY  ZPX
  h6280_ICount -= 4;
  STY;
  WR_ZPX;
}
OP(0b4)
{
  int tmp;  // 4 LDY  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  LDY;
}
OP(0d4)
{
  h6280_ICount -= 2;					    // 2 CSH
}
OP(0f4)
{
  h6280_ICount -= 2;		   // 2 SET
  SET;
}

OP(005)
{
  int tmp;  // 4 ORA  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  ORA;
}
OP(025)
{
  int tmp;  // 4 AND  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  AND;
}
OP(045)
{
  int tmp;  // 4 EOR  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  EOR;
}
OP(065)
{
  int tmp;  // 4 ADC  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  ADC;
}
OP(085)
{
  int tmp;  // 4 STA  ZPG
  h6280_ICount -= 4;
  STA;
  WR_ZPG;
}
OP(0a5)
{
  int tmp;  // 4 LDA  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  LDA;
}
OP(0c5)
{
  int tmp;  // 4 CMP  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  CMP;
}
OP(0e5)
{
  int tmp;  // 4 SBC  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  SBC;
}

OP(015)
{
  int tmp;  // 4 ORA  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  ORA;
}
OP(035)
{
  int tmp;  // 4 AND  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  AND;
}
OP(055)
{
  int tmp;  // 4 EOR  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  EOR;
}
OP(075)
{
  int tmp;  // 4 ADC  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  ADC;
}
OP(095)
{
  int tmp;  // 4 STA  ZPX
  h6280_ICount -= 4;
  STA;
  WR_ZPX;
}
OP(0b5)
{
  int tmp;  // 4 LDA  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  LDA;
}
OP(0d5)
{
  int tmp;  // 4 CMP  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  CMP;
}
OP(0f5)
{
  int tmp;  // 4 SBC  ZPX
  h6280_ICount -= 4;
  RD_ZPX;
  SBC;
}

OP(006)
{
  int tmp;  // 6 ASL  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  ASL;
  WB_EAZ;
}
OP(026)
{
  int tmp;  // 6 ROL  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  ROL;
  WB_EAZ;
}
OP(046)
{
  int tmp;  // 6 LSR  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  LSR;
  WB_EAZ;
}
OP(066)
{
  int tmp;  // 6 ROR  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  ROR;
  WB_EAZ;
}
OP(086)
{
  int tmp;  // 4 STX  ZPG
  h6280_ICount -= 4;
  STX;
  WR_ZPG;
}
OP(0a6)
{
  int tmp;  // 4 LDX  ZPG
  h6280_ICount -= 4;
  RD_ZPG;
  LDX;
}
OP(0c6)
{
  int tmp;  // 6 DEC  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  DEC;
  WB_EAZ;
}
OP(0e6)
{
  int tmp;  // 6 INC  ZPG
  h6280_ICount -= 6;
  RD_ZPG;
  INC;
  WB_EAZ;
}

OP(016)
{
  int tmp;  // 6 ASL  ZPX
  h6280_ICount -= 6;
  RD_ZPX;
  ASL;
  WB_EAZ
}
OP(036)
{
  int tmp;  // 6 ROL  ZPX
  h6280_ICount -= 6;
  RD_ZPX;
  ROL;
  WB_EAZ
}
OP(056)
{
  int tmp;  // 6 LSR  ZPX
  h6280_ICount -= 6;
  RD_ZPX;
  LSR;
  WB_EAZ
}
OP(076)
{
  int tmp;  // 6 ROR  ZPX
  h6280_ICount -= 6;
  RD_ZPX;
  ROR;
  WB_EAZ
}
OP(096)
{
  int tmp;  // 4 STX  ZPY
  h6280_ICount -= 4;
  STX;
  WR_ZPY;
}
OP(0b6)
{
  int tmp;  // 4 LDX  ZPY
  h6280_ICount -= 4;
  RD_ZPY;
  LDX;
}
OP(0d6)
{
  int tmp;  // 6 DEC  ZPX
  h6280_ICount -= 6;
  RD_ZPX;
  DEC;
  WB_EAZ;
}
OP(0f6)
{
  int tmp;  // 6 INC  ZPX
  h6280_ICount -= 6;
  RD_ZPX;
  INC;
  WB_EAZ;
}

OP(007)
{
  int tmp;  // 7 RMB0 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(0);
  WB_EAZ;
}
OP(027)
{
  int tmp;  // 7 RMB2 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(2);
  WB_EAZ;
}
OP(047)
{
  int tmp;  // 7 RMB4 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(4);
  WB_EAZ;
}
OP(067)
{
  int tmp;  // 7 RMB6 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(6);
  WB_EAZ;
}
OP(087)
{
  int tmp;  // 7 SMB0 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(0);
  WB_EAZ;
}
OP(0a7)
{
  int tmp;  // 7 SMB2 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(2);
  WB_EAZ;
}
OP(0c7)
{
  int tmp;  // 7 SMB4 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(4);
  WB_EAZ;
}
OP(0e7)
{
  int tmp;  // 7 SMB6 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(6);
  WB_EAZ;
}

OP(017)
{
  int tmp;  // 7 RMB1 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(1);
  WB_EAZ;
}
OP(037)
{
  int tmp;  // 7 RMB3 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(3);
  WB_EAZ;
}
OP(057)
{
  int tmp;  // 7 RMB5 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(5);
  WB_EAZ;
}
OP(077)
{
  int tmp;  // 7 RMB7 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  RMB(7);
  WB_EAZ;
}
OP(097)
{
  int tmp;  // 7 SMB1 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(1);
  WB_EAZ;
}
OP(0b7)
{
  int tmp;  // 7 SMB3 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(3);
  WB_EAZ;
}
OP(0d7)
{
  int tmp;  // 7 SMB5 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(5);
  WB_EAZ;
}
OP(0f7)
{
  int tmp;  // 7 SMB7 ZPG
  h6280_ICount -= 7;
  RD_ZPG;
  SMB(7);
  WB_EAZ;
}

OP(008)
{
  h6280_ICount -= 3;		   // 3 PHP
  PHP;
}
OP(028)
{
  h6280_ICount -= 4;		   // 4 PLP
  PLP;
}
OP(048)
{
  h6280_ICount -= 3;		   // 3 PHA
  PHA;
}
OP(068)
{
  h6280_ICount -= 4;		   // 4 PLA
  PLA;
}
OP(088)
{
  h6280_ICount -= 2;		   // 2 DEY
  DEY;
}
OP(0a8)
{
  h6280_ICount -= 2;		   // 2 TAY
  TAY;
}
OP(0c8)
{
  h6280_ICount -= 2;		   // 2 INY
  INY;
}
OP(0e8)
{
  h6280_ICount -= 2;		   // 2 INX
  INX;
}

OP(018)
{
  h6280_ICount -= 2;		   // 2 CLC
  CLC;
}
OP(038)
{
  h6280_ICount -= 2;		   // 2 SEC
  SEC;
}
OP(058)
{
  h6280_ICount -= 2;		   // 2 CLI
  CLI;
}
OP(078)
{
  h6280_ICount -= 2;		   // 2 SEI
  SEI;
}
OP(098)
{
  h6280_ICount -= 2;		   // 2 TYA
  TYA;
}
OP(0b8)
{
  h6280_ICount -= 2;		   // 2 CLV
  CLV;
}
OP(0d8)
{
  h6280_ICount -= 2;		   // 2 CLD
  CLD;
}
OP(0f8)
{
  h6280_ICount -= 2;		   // 2 SED
  SED;
}

OP(009)
{
  int tmp;  // 2 ORA  IMM
  h6280_ICount -= 2;
  RD_IMM;
  ORA;
}
OP(029)
{
  int tmp;  // 2 AND  IMM
  h6280_ICount -= 2;
  RD_IMM;
  AND;
}
OP(049)
{
  int tmp;  // 2 EOR  IMM
  h6280_ICount -= 2;
  RD_IMM;
  EOR;
}
OP(069)
{
  int tmp;  // 2 ADC  IMM
  h6280_ICount -= 2;
  RD_IMM;
  ADC;
}
OP(089)
{
  int tmp;  // 2 BIT  IMM
  h6280_ICount -= 2;
  RD_IMM;
  BIT;
}
OP(0a9)
{
  int tmp;  // 2 LDA  IMM
  h6280_ICount -= 2;
  RD_IMM;
  LDA;
}
OP(0c9)
{
  int tmp;  // 2 CMP  IMM
  h6280_ICount -= 2;
  RD_IMM;
  CMP;
}
OP(0e9)
{
  int tmp;  // 2 SBC  IMM
  h6280_ICount -= 2;
  RD_IMM;
  SBC;
}

OP(019)
{
  int tmp;  // 5 ORA  ABY
  h6280_ICount -= 5;
  RD_ABY;
  ORA;
}
OP(039)
{
  int tmp;  // 5 AND  ABY
  h6280_ICount -= 5;
  RD_ABY;
  AND;
}
OP(059)
{
  int tmp;  // 5 EOR  ABY
  h6280_ICount -= 5;
  RD_ABY;
  EOR;
}
OP(079)
{
  int tmp;  // 5 ADC  ABY
  h6280_ICount -= 5;
  RD_ABY;
  ADC;
}
OP(099)
{
  int tmp;  // 5 STA  ABY
  h6280_ICount -= 5;
  STA;
  WR_ABY;
}
OP(0b9)
{
  int tmp;  // 5 LDA  ABY
  h6280_ICount -= 5;
  RD_ABY;
  LDA;
}
OP(0d9)
{
  int tmp;  // 5 CMP  ABY
  h6280_ICount -= 5;
  RD_ABY;
  CMP;
}
OP(0f9)
{
  int tmp;  // 5 SBC  ABY
  h6280_ICount -= 5;
  RD_ABY;
  SBC;
}

OP(00a)
{
  int tmp;  // 2 ASL  A
  h6280_ICount -= 2;
  RD_ACC;
  ASL;
  WB_ACC;
}
OP(02a)
{
  int tmp;  // 2 ROL  A
  h6280_ICount -= 2;
  RD_ACC;
  ROL;
  WB_ACC;
}
OP(04a)
{
  int tmp;  // 2 LSR  A
  h6280_ICount -= 2;
  RD_ACC;
  LSR;
  WB_ACC;
}
OP(06a)
{
  int tmp;  // 2 ROR  A
  h6280_ICount -= 2;
  RD_ACC;
  ROR;
  WB_ACC;
}
OP(08a)
{
  h6280_ICount -= 2;		   // 2 TXA
  TXA;
}
OP(0aa)
{
  h6280_ICount -= 2;		   // 2 TAX
  TAX;
}
OP(0ca)
{
  h6280_ICount -= 2;		   // 2 DEX
  DEX;
}
OP(0ea)
{
  h6280_ICount -= 2;		   // 2 NOP
  NOP;
}

OP(01a)
{
  h6280_ICount -= 2;		   // 2 INC  A
  INA;
}
OP(03a)
{
  h6280_ICount -= 2;		   // 2 DEC  A
  DEA;
}
OP(05a)
{
  h6280_ICount -= 3;		   // 3 PHY
  PHY;
}
OP(07a)
{
  h6280_ICount -= 4;		   // 4 PLY
  PLY;
}
OP(09a)
{
  h6280_ICount -= 2;		   // 2 TXS
  TXS;
}
OP(0ba)
{
  h6280_ICount -= 2;		   // 2 TSX
  TSX;
}
OP(0da)
{
  h6280_ICount -= 3;		   // 3 PHX
  PHX;
}
OP(0fa)
{
  h6280_ICount -= 4;		   // 4 PLX
  PLX;
}

OP(00b)
{
  ILL;		    // 2 ???
}
OP(02b)
{
  ILL;		    // 2 ???
}
OP(04b)
{
  ILL;		    // 2 ???
}
OP(06b)
{
  ILL;		    // 2 ???
}
OP(08b)
{
  ILL;		    // 2 ???
}
OP(0ab)
{
  ILL;		    // 2 ???
}
OP(0cb)
{
  ILL;		    // 2 ???
}
OP(0eb)
{
  ILL;		    // 2 ???
}

OP(01b)
{
  ILL;		    // 2 ???
}
OP(03b)
{
  ILL;		    // 2 ???
}
OP(05b)
{
  ILL;		    // 2 ???
}
OP(07b)
{
  ILL;		    // 2 ???
}
OP(09b)
{
  ILL;		    // 2 ???
}
OP(0bb)
{
  ILL;		    // 2 ???
}
OP(0db)
{
  ILL;		    // 2 ???
}
OP(0fb)
{
  ILL;		    // 2 ???
}

OP(00c)
{
  int tmp;  // 7 TSB  ABS
  h6280_ICount -= 7;
  RD_ABS;
  TSB;
  WB_EA;
}
OP(02c)
{
  int tmp;  // 5 BIT  ABS
  h6280_ICount -= 5;
  RD_ABS;
  BIT;
}
OP(04c)
{
  h6280_ICount -= 4;  // 4 JMP  ABS
  EA_ABS;
  JMP;
}
OP(06c)
{
  int tmp;  // 7 JMP  IND
  h6280_ICount -= 7;
  EA_IND;
  JMP;
}
OP(08c)
{
  int tmp;  // 5 STY  ABS
  h6280_ICount -= 5;
  STY;
  WR_ABS;
}
OP(0ac)
{
  int tmp;  // 5 LDY  ABS
  h6280_ICount -= 5;
  RD_ABS;
  LDY;
}
OP(0cc)
{
  int tmp;  // 5 CPY  ABS
  h6280_ICount -= 5;
  RD_ABS;
  CPY;
}
OP(0ec)
{
  int tmp;  // 5 CPX  ABS
  h6280_ICount -= 5;
  RD_ABS;
  CPX;
}

OP(01c)
{
  int tmp;  // 7 TRB  ABS
  h6280_ICount -= 7;
  RD_ABS;
  TRB;
  WB_EA;
}
OP(03c)
{
  int tmp;  // 5 BIT  ABX
  h6280_ICount -= 5;
  RD_ABX;
  BIT;
}
OP(05c)
{
  ILL;		    // 2 ???
}
OP(07c)
{
  int tmp;  // 7 JMP  IAX
  h6280_ICount -= 7;
  EA_IAX;
  JMP;
}
OP(09c)
{
  int tmp;  // 5 STZ  ABS
  h6280_ICount -= 5;
  STZ;
  WR_ABS;
}
OP(0bc)
{
  int tmp;  // 5 LDY  ABX
  h6280_ICount -= 5;
  RD_ABX;
  LDY;
}
OP(0dc)
{
  ILL;		    // 2 ???
}
OP(0fc)
{
  ILL;		    // 2 ???
}

OP(00d)
{
  int tmp;  // 5 ORA  ABS
  h6280_ICount -= 5;
  RD_ABS;
  ORA;
}
OP(02d)
{
  int tmp;  // 4 AND  ABS
  h6280_ICount -= 5;
  RD_ABS;
  AND;
}
OP(04d)
{
  int tmp;  // 4 EOR  ABS
  h6280_ICount -= 5;
  RD_ABS;
  EOR;
}
OP(06d)
{
  int tmp;  // 4 ADC  ABS
  h6280_ICount -= 5;
  RD_ABS;
  ADC;
}
OP(08d)
{
  int tmp;  // 4 STA  ABS
  h6280_ICount -= 5;
  STA;
  WR_ABS;
}
OP(0ad)
{
  int tmp;  // 4 LDA  ABS
  h6280_ICount -= 5;
  RD_ABS;
  LDA;
}
OP(0cd)
{
  int tmp;  // 4 CMP  ABS
  h6280_ICount -= 5;
  RD_ABS;
  CMP;
}
OP(0ed)
{
  int tmp;  // 4 SBC  ABS
  h6280_ICount -= 5;
  RD_ABS;
  SBC;
}

OP(01d)
{
  int tmp;  // 5 ORA  ABX
  h6280_ICount -= 5;
  RD_ABX;
  ORA;
}
OP(03d)
{
  int tmp;  // 4 AND  ABX
  h6280_ICount -= 5;
  RD_ABX;
  AND;
}
OP(05d)
{
  int tmp;  // 4 EOR  ABX
  h6280_ICount -= 5;
  RD_ABX;
  EOR;
}
OP(07d)
{
  int tmp;  // 4 ADC  ABX
  h6280_ICount -= 5;
  RD_ABX;
  ADC;
}
OP(09d)
{
  int tmp;  // 5 STA  ABX
  h6280_ICount -= 5;
  STA;
  WR_ABX;
}
OP(0bd)
{
  int tmp;  // 5 LDA  ABX
  h6280_ICount -= 5;
  RD_ABX;
  LDA;
}
OP(0dd)
{
  int tmp;  // 4 CMP  ABX
  h6280_ICount -= 5;
  RD_ABX;
  CMP;
}
OP(0fd)
{
  int tmp;  // 4 SBC  ABX
  h6280_ICount -= 5;
  RD_ABX;
  SBC;
}

OP(00e)
{
  int tmp;  // 6 ASL  ABS
  h6280_ICount -= 7;
  RD_ABS;
  ASL;
  WB_EA;
}
OP(02e)
{
  int tmp;  // 6 ROL  ABS
  h6280_ICount -= 7;
  RD_ABS;
  ROL;
  WB_EA;
}
OP(04e)
{
  int tmp;  // 6 LSR  ABS
  h6280_ICount -= 7;
  RD_ABS;
  LSR;
  WB_EA;
}
OP(06e)
{
  int tmp;  // 6 ROR  ABS
  h6280_ICount -= 7;
  RD_ABS;
  ROR;
  WB_EA;
}
OP(08e)
{
  int tmp;  // 4 STX  ABS
  h6280_ICount -= 5;
  STX;
  WR_ABS;
}
OP(0ae)
{
  int tmp;  // 5 LDX  ABS
  h6280_ICount -= 5;
  RD_ABS;
  LDX;
}
OP(0ce)
{
  int tmp;  // 6 DEC  ABS
  h6280_ICount -= 7;
  RD_ABS;
  DEC;
  WB_EA;
}
OP(0ee)
{
  int tmp;  // 6 INC  ABS
  h6280_ICount -= 7;
  RD_ABS;
  INC;
  WB_EA;
}

OP(01e)
{
  int tmp;  // 7 ASL  ABX
  h6280_ICount -= 7;
  RD_ABX;
  ASL;
  WB_EA;
}
OP(03e)
{
  int tmp;  // 7 ROL  ABX
  h6280_ICount -= 7;
  RD_ABX;
  ROL;
  WB_EA;
}
OP(05e)
{
  int tmp;  // 7 LSR  ABX
  h6280_ICount -= 7;
  RD_ABX;
  LSR;
  WB_EA;
}
OP(07e)
{
  int tmp;  // 7 ROR  ABX
  h6280_ICount -= 7;
  RD_ABX;
  ROR;
  WB_EA;
}
OP(09e)
{
  int tmp;  // 5 STZ  ABX
  h6280_ICount -= 5;
  STZ;
  WR_ABX;
}
OP(0be)
{
  int tmp;  // 4 LDX  ABY
  h6280_ICount -= 5;
  RD_ABY;
  LDX;
}
OP(0de)
{
  int tmp;  // 7 DEC  ABX
  h6280_ICount -= 7;
  RD_ABX;
  DEC;
  WB_EA;
}
OP(0fe)
{
  int tmp;  // 7 INC  ABX
  h6280_ICount -= 7;
  RD_ABX;
  INC;
  WB_EA;
}

OP(00f)
{
  int tmp;  // 6/8 BBR0 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(0);
}
OP(02f)
{
  int tmp;  // 6/8 BBR2 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(2);
}
OP(04f)
{
  int tmp;  // 6/8 BBR4 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(4);
}
OP(06f)
{
  int tmp;  // 6/8 BBR6 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(6);
}
OP(08f)
{
  int tmp;  // 6/8 BBS0 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(0);
}
OP(0af)
{
  int tmp;  // 6/8 BBS2 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(2);
}
OP(0cf)
{
  int tmp;  // 6/8 BBS4 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(4);
}
OP(0ef)
{
  int tmp;  // 6/8 BBS6 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(6);
}

OP(01f)
{
  int tmp;  // 6/8 BBR1 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(1);
}
OP(03f)
{
  int tmp;  // 6/8 BBR3 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(3);
}
OP(05f)
{
  int tmp;  // 6/8 BBR5 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(5);
}
OP(07f)
{
  int tmp;  // 6/8 BBR7 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBR(7);
}
OP(09f)
{
  int tmp;  // 6/8 BBS1 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(1);
}
OP(0bf)
{
  int tmp;  // 6/8 BBS3 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(3);
}
OP(0df)
{
  int tmp;  // 6/8 BBS5 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(5);
}
OP(0ff)
{
  int tmp;  // 6/8 BBS7 ZPG,REL
  h6280_ICount -= 4;
  RD_ZPG;
  BBS(7);
}

static void (*insnh6280[0x100])(void) =
{
  h6280_000,h6280_001,h6280_002,h6280_003,h6280_004,h6280_005,h6280_006,h6280_007,
  h6280_008,h6280_009,h6280_00a,h6280_00b,h6280_00c,h6280_00d,h6280_00e,h6280_00f,
  h6280_010,h6280_011,h6280_012,h6280_013,h6280_014,h6280_015,h6280_016,h6280_017,
  h6280_018,h6280_019,h6280_01a,h6280_01b,h6280_01c,h6280_01d,h6280_01e,h6280_01f,
  h6280_020,h6280_021,h6280_022,h6280_023,h6280_024,h6280_025,h6280_026,h6280_027,
  h6280_028,h6280_029,h6280_02a,h6280_02b,h6280_02c,h6280_02d,h6280_02e,h6280_02f,
  h6280_030,h6280_031,h6280_032,h6280_033,h6280_034,h6280_035,h6280_036,h6280_037,
  h6280_038,h6280_039,h6280_03a,h6280_03b,h6280_03c,h6280_03d,h6280_03e,h6280_03f,
  h6280_040,h6280_041,h6280_042,h6280_043,h6280_044,h6280_045,h6280_046,h6280_047,
  h6280_048,h6280_049,h6280_04a,h6280_04b,h6280_04c,h6280_04d,h6280_04e,h6280_04f,
  h6280_050,h6280_051,h6280_052,h6280_053,h6280_054,h6280_055,h6280_056,h6280_057,
  h6280_058,h6280_059,h6280_05a,h6280_05b,h6280_05c,h6280_05d,h6280_05e,h6280_05f,
  h6280_060,h6280_061,h6280_062,h6280_063,h6280_064,h6280_065,h6280_066,h6280_067,
  h6280_068,h6280_069,h6280_06a,h6280_06b,h6280_06c,h6280_06d,h6280_06e,h6280_06f,
  h6280_070,h6280_071,h6280_072,h6280_073,h6280_074,h6280_075,h6280_076,h6280_077,
  h6280_078,h6280_079,h6280_07a,h6280_07b,h6280_07c,h6280_07d,h6280_07e,h6280_07f,
  h6280_080,h6280_081,h6280_082,h6280_083,h6280_084,h6280_085,h6280_086,h6280_087,
  h6280_088,h6280_089,h6280_08a,h6280_08b,h6280_08c,h6280_08d,h6280_08e,h6280_08f,
  h6280_090,h6280_091,h6280_092,h6280_093,h6280_094,h6280_095,h6280_096,h6280_097,
  h6280_098,h6280_099,h6280_09a,h6280_09b,h6280_09c,h6280_09d,h6280_09e,h6280_09f,
  h6280_0a0,h6280_0a1,h6280_0a2,h6280_0a3,h6280_0a4,h6280_0a5,h6280_0a6,h6280_0a7,
  h6280_0a8,h6280_0a9,h6280_0aa,h6280_0ab,h6280_0ac,h6280_0ad,h6280_0ae,h6280_0af,
  h6280_0b0,h6280_0b1,h6280_0b2,h6280_0b3,h6280_0b4,h6280_0b5,h6280_0b6,h6280_0b7,
  h6280_0b8,h6280_0b9,h6280_0ba,h6280_0bb,h6280_0bc,h6280_0bd,h6280_0be,h6280_0bf,
  h6280_0c0,h6280_0c1,h6280_0c2,h6280_0c3,h6280_0c4,h6280_0c5,h6280_0c6,h6280_0c7,
  h6280_0c8,h6280_0c9,h6280_0ca,h6280_0cb,h6280_0cc,h6280_0cd,h6280_0ce,h6280_0cf,
  h6280_0d0,h6280_0d1,h6280_0d2,h6280_0d3,h6280_0d4,h6280_0d5,h6280_0d6,h6280_0d7,
  h6280_0d8,h6280_0d9,h6280_0da,h6280_0db,h6280_0dc,h6280_0dd,h6280_0de,h6280_0df,
  h6280_0e0,h6280_0e1,h6280_0e2,h6280_0e3,h6280_0e4,h6280_0e5,h6280_0e6,h6280_0e7,
  h6280_0e8,h6280_0e9,h6280_0ea,h6280_0eb,h6280_0ec,h6280_0ed,h6280_0ee,h6280_0ef,
  h6280_0f0,h6280_0f1,h6280_0f2,h6280_0f3,h6280_0f4,h6280_0f5,h6280_0f6,h6280_0f7,
  h6280_0f8,h6280_0f9,h6280_0fa,h6280_0fb,h6280_0fc,h6280_0fd,h6280_0fe,h6280_0ff
};
