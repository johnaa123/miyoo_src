/*****************************************************************************
 *
 *	 tbl65c02.c
 *	 65c02 opcode functions and function pointer table
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
#define OP(nn) INLINE void m65c02_##nn(void)

/*****************************************************************************
 *****************************************************************************
 *
 *	 overrides for 65C02 opcodes
 *
 *****************************************************************************
 * op	 temp	  cycles			 rdmem	 opc  wrmem   ********************/
#define m65c02_00 m6502_00									/* 7 BRK */
#define m65c02_20 m6502_20									/* 6 JSR ABS */
#define m65c02_40 m6502_40									/* 6 RTI */
#define m65c02_60 m6502_60									/* 6 RTS */
OP(80)
{
  int tmp;							  /* 2 BRA */
  BRA(1);
}
#define m65c02_a0 m6502_a0									/* 2 LDY IMM */
#define m65c02_c0 m6502_c0									/* 2 CPY IMM */
#define m65c02_e0 m6502_e0									/* 2 CPX IMM */

#define m65c02_10 m6502_10									/* 2 BPL */
#define m65c02_30 m6502_30									/* 2 BMI */
#define m65c02_50 m6502_50									/* 2 BVC */
#define m65c02_70 m6502_70									/* 2 BVS */
#define m65c02_90 m6502_90									/* 2 BCC */
#define m65c02_b0 m6502_b0									/* 2 BCS */
#define m65c02_d0 m6502_d0									/* 2 BNE */
#define m65c02_f0 m6502_f0									/* 2 BEQ */

#define m65c02_01 m6502_01									/* 6 ORA IDX */
#define m65c02_21 m6502_21									/* 6 AND IDX */
#define m65c02_41 m6502_41									/* 6 EOR IDX */
#define m65c02_61 m6502_61									/* 6 ADC IDX */
#define m65c02_81 m6502_81									/* 6 STA IDX */
#define m65c02_a1 m6502_a1									/* 6 LDA IDX */
#define m65c02_c1 m6502_c1									/* 6 CMP IDX */
#define m65c02_e1 m6502_e1									/* 6 SBC IDX */

#define m65c02_11 m6502_11									/* 5 ORA IDY; */
#define m65c02_31 m6502_31									/* 5 AND IDY; */
#define m65c02_51 m6502_51									/* 5 EOR IDY; */
#define m65c02_71 m6502_71									/* 5 ADC IDY; */
#define m65c02_91 m6502_91									/* 6 STA IDY; */
#define m65c02_b1 m6502_b1									/* 5 LDA IDY; */
#define m65c02_d1 m6502_d1									/* 5 CMP IDY; */
#define m65c02_f1 m6502_f1									/* 5 SBC IDY; */

#define m65c02_02 m6502_02									/* 2 ILL */
#define m65c02_22 m6502_22									/* 2 ILL */
#define m65c02_42 m6502_42									/* 2 ILL */
#define m65c02_62 m6502_62									/* 2 ILL */
#define m65c02_82 m6502_82									/* 2 ILL */
#define m65c02_a2 m6502_a2									/* 2 LDX IMM */
#define m65c02_c2 m6502_c2									/* 2 ILL */
#define m65c02_e2 m6502_e2									/* 2 ILL */

#ifndef CORE_M65CE02
OP(12)
{
  int tmp;  /* 3 ORA ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  ORA;
}
OP(32)
{
  int tmp;  /* 3 AND ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  AND;
}
OP(52)
{
  int tmp;  /* 3 EOR ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  EOR;
}
OP(72)
{
  int tmp;  /* 3 ADC ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  ADC;
}
OP(92)
{
  int tmp;  /* 3 STA ZPI */
  m6502_ICount -= 4;
  STA;
  WR_ZPI;
}
OP(b2)
{
  int tmp;  /* 3 LDA ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  LDA;
}
OP(d2)
{
  int tmp;  /* 3 CMP ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  CMP;
}
OP(f2)
{
  int tmp;  /* 3 SBC ZPI */
  m6502_ICount -= 3;
  RD_ZPI;
  SBC;
}
#endif

#define m65c02_03 m6502_03									/* 2 ILL */
#define m65c02_23 m6502_23									/* 2 ILL */
#define m65c02_43 m6502_43									/* 2 ILL */
#define m65c02_63 m6502_63									/* 2 ILL */
#define m65c02_83 m6502_83									/* 2 ILL */
#define m65c02_a3 m6502_a3									/* 2 ILL */
#define m65c02_c3 m6502_c3									/* 2 ILL */
#define m65c02_e3 m6502_e3									/* 2 ILL */

#define m65c02_13 m6502_13									/* 2 ILL */
#define m65c02_33 m6502_33									/* 2 ILL */
#define m65c02_53 m6502_53									/* 2 ILL */
#define m65c02_73 m6502_73									/* 2 ILL */
#define m65c02_93 m6502_93									/* 2 ILL */
#define m65c02_b3 m6502_b3									/* 2 ILL */
#define m65c02_d3 m6502_d3									/* 2 ILL */
#define m65c02_f3 m6502_f3									/* 2 ILL */

OP(04)
{
  int tmp;  /* 3 TSB ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  TSB;
  WB_EA;
}
#define m65c02_24 m6502_24									/* 3 BIT ZPG */
#define m65c02_44 m6502_44									/* 2 ILL */
OP(64)
{
  int tmp;  /* 3 STZ ZPG */
  m6502_ICount -= 2;
  STZ;
  WR_ZPG;
}
#define m65c02_84 m6502_84									/* 3 STY ZPG */
#define m65c02_a4 m6502_a4									/* 3 LDY ZPG */
#define m65c02_c4 m6502_c4									/* 3 CPY ZPG */
#define m65c02_e4 m6502_e4									/* 3 CPX ZPG */

OP(14)
{
  int tmp;  /* 3 TRB ZPG */
  m6502_ICount -= 3;
  RD_ZPG;
  TRB;
  WB_EA;
}
OP(34)
{
  int tmp;  /* 4 BIT ZPX */
  m6502_ICount -= 4;
  RD_ZPX;
  BIT;
}
#define m65c02_54 m6502_54									/* 2 ILL */
OP(74)
{
  int tmp;  /* 4 STZ ZPX */
  m6502_ICount -= 4;
  STZ;
  WR_ZPX;
}
#define m65c02_94 m6502_94									/* 4 STY ZPX */
#define m65c02_b4 m6502_b4									/* 4 LDY ZPX */
#define m65c02_d4 m6502_d4									/* 2 ILL */
#define m65c02_f4 m6502_f4									/* 2 ILL */

#define m65c02_05 m6502_05									/* 3 ORA ZPG */
#define m65c02_25 m6502_25									/* 3 AND ZPG */
#define m65c02_45 m6502_45									/* 3 EOR ZPG */
#define m65c02_65 m6502_65									/* 3 ADC ZPG */
#define m65c02_85 m6502_85									/* 3 STA ZPG */
#define m65c02_a5 m6502_a5									/* 3 LDA ZPG */
#define m65c02_c5 m6502_c5									/* 3 CMP ZPG */
#define m65c02_e5 m6502_e5									/* 3 SBC ZPG */

#define m65c02_15 m6502_15									/* 4 ORA ZPX */
#define m65c02_35 m6502_35									/* 4 AND ZPX */
#define m65c02_55 m6502_55									/* 4 EOR ZPX */
#define m65c02_75 m6502_75									/* 4 ADC ZPX */
#define m65c02_95 m6502_95									/* 4 STA ZPX */
#define m65c02_b5 m6502_b5									/* 4 LDA ZPX */
#define m65c02_d5 m6502_d5									/* 4 CMP ZPX */
#define m65c02_f5 m6502_f5									/* 4 SBC ZPX */

#define m65c02_06 m6502_06									/* 5 ASL ZPG */
#define m65c02_26 m6502_26									/* 5 ROL ZPG */
#define m65c02_46 m6502_46									/* 5 LSR ZPG */
#define m65c02_66 m6502_66									/* 5 ROR ZPG */
#define m65c02_86 m6502_86									/* 3 STX ZPG */
#define m65c02_a6 m6502_a6									/* 3 LDX ZPG */
#define m65c02_c6 m6502_c6									/* 5 DEC ZPG */
#define m65c02_e6 m6502_e6									/* 5 INC ZPG */

#define m65c02_16 m6502_16									/* 6 ASL ZPX */
#define m65c02_36 m6502_36									/* 6 ROL ZPX */
#define m65c02_56 m6502_56									/* 6 LSR ZPX */
#define m65c02_76 m6502_76									/* 6 ROR ZPX */
#define m65c02_96 m6502_96									/* 4 STX ZPY */
#define m65c02_b6 m6502_b6									/* 4 LDX ZPY */
#define m65c02_d6 m6502_d6									/* 6 DEC ZPX */
#define m65c02_f6 m6502_f6									/* 6 INC ZPX */

OP(07)
{
  int tmp;  /* 5 RMB0 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(0);
  WB_EA;
}
OP(27)
{
  int tmp;  /* 5 RMB2 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(2);
  WB_EA;
}
OP(47)
{
  int tmp;  /* 5 RMB4 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(4);
  WB_EA;
}
OP(67)
{
  int tmp;  /* 5 RMB6 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(6);
  WB_EA;
}
OP(87)
{
  int tmp;  /* 5 SMB0 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(0);
  WB_EA;
}
OP(a7)
{
  int tmp;  /* 5 SMB2 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(2);
  WB_EA;
}
OP(c7)
{
  int tmp;  /* 5 SMB4 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(4);
  WB_EA;
}
OP(e7)
{
  int tmp;  /* 5 SMB6 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(6);
  WB_EA;
}

OP(17)
{
  int tmp;  /* 5 RMB1 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(1);
  WB_EA;
}
OP(37)
{
  int tmp;  /* 5 RMB3 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(3);
  WB_EA;
}
OP(57)
{
  int tmp;  /* 5 RMB5 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(5);
  WB_EA;
}
OP(77)
{
  int tmp;  /* 5 RMB7 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  RMB(7);
  WB_EA;
}
OP(97)
{
  int tmp;  /* 5 SMB1 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(1);
  WB_EA;
}
OP(b7)
{
  int tmp;  /* 5 SMB3 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(3);
  WB_EA;
}
OP(d7)
{
  int tmp;  /* 5 SMB5 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(5);
  WB_EA;
}
OP(f7)
{
  int tmp;  /* 5 SMB7 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  SMB(7);
  WB_EA;
}

#define m65c02_08 m6502_08									/* 3 PHP */
#define m65c02_28 m6502_28									/* 4 PLP */
#define m65c02_48 m6502_48									/* 3 PHA */
#define m65c02_68 m6502_68									/* 4 PLA */
#define m65c02_88 m6502_88									/* 2 DEY */
#define m65c02_a8 m6502_a8									/* 2 TAY */
#define m65c02_c8 m6502_c8									/* 2 INY */
#define m65c02_e8 m6502_e8									/* 2 INX */

#define m65c02_18 m6502_18									/* 2 CLC */
#define m65c02_38 m6502_38									/* 2 SEC */
#define m65c02_58 m6502_58									/* 2 CLI */
#define m65c02_78 m6502_78									/* 2 SEI */
#define m65c02_98 m6502_98									/* 2 TYA */
#define m65c02_b8 m6502_b8									/* 2 CLV */
#define m65c02_d8 m6502_d8									/* 2 CLD */
#define m65c02_f8 m6502_f8									/* 2 SED */

#define m65c02_09 m6502_09									/* 2 ORA IMM */
#define m65c02_29 m6502_29									/* 2 AND IMM */
#define m65c02_49 m6502_49									/* 2 EOR IMM */
#define m65c02_69 m6502_69									/* 2 ADC IMM */
OP(89)
{
  int tmp;  /* 2 BIT IMM */
  m6502_ICount -= 2;
  RD_IMM;
  BIT;
}
#define m65c02_a9 m6502_a9									/* 2 LDA IMM */
#define m65c02_c9 m6502_c9									/* 2 CMP IMM */
#define m65c02_e9 m6502_e9									/* 2 SBC IMM */

#define m65c02_19 m6502_19									/* 4 ORA ABY */
#define m65c02_39 m6502_39									/* 4 AND ABY */
#define m65c02_59 m6502_59									/* 4 EOR ABY */
#define m65c02_79 m6502_79									/* 4 ADC ABY */
#define m65c02_99 m6502_99									/* 5 STA ABY */
#define m65c02_b9 m6502_b9									/* 4 LDA ABY */
#define m65c02_d9 m6502_d9									/* 4 CMP ABY */
#define m65c02_f9 m6502_f9									/* 4 SBC ABY */

#define m65c02_0a m6502_0a									/* 2 ASL */
#define m65c02_2a m6502_2a									/* 2 ROL */
#define m65c02_4a m6502_4a									/* 2 LSR */
#define m65c02_6a m6502_6a									/* 2 ROR */
#define m65c02_8a m6502_8a									/* 2 TXA */
#define m65c02_aa m6502_aa									/* 2 TAX */
#define m65c02_ca m6502_ca									/* 2 DEX */
#define m65c02_ea m6502_ea									/* 2 NOP */

OP(1a)
{
  m6502_ICount -= 2;		  /* 2 INA */
  INA;
}
OP(3a)
{
  m6502_ICount -= 2;		  /* 2 DEA */
  DEA;
}
OP(5a)
{
  m6502_ICount -= 3;		  /* 3 PHY */
  PHY;
}
OP(7a)
{
  m6502_ICount -= 4;		  /* 4 PLY */
  PLY;
}
#define m65c02_9a m6502_9a									/* 2 TXS */
#define m65c02_ba m6502_ba									/* 2 TSX */
OP(da)
{
  m6502_ICount -= 3;		  /* 3 PHX */
  PHX;
}
OP(fa)
{
  m6502_ICount -= 4;		  /* 4 PLX */
  PLX;
}

#define m65c02_0b m6502_0b									/* 2 ILL */
#define m65c02_2b m6502_2b									/* 2 ILL */
#define m65c02_4b m6502_4b									/* 2 ILL */
#define m65c02_6b m6502_6b									/* 2 ILL */
#define m65c02_8b m6502_8b									/* 2 ILL */
#define m65c02_ab m6502_ab									/* 2 ILL */
#define m65c02_cb m6502_cb									/* 2 ILL */
#define m65c02_eb m6502_eb									/* 2 ILL */

#define m65c02_1b m6502_1b									/* 2 ILL */
#define m65c02_3b m6502_3b									/* 2 ILL */
#define m65c02_5b m6502_5b									/* 2 ILL */
#define m65c02_7b m6502_7b									/* 2 ILL */
#define m65c02_9b m6502_9b									/* 2 ILL */
#define m65c02_bb m6502_bb									/* 2 ILL */
#define m65c02_db m6502_db									/* 2 ILL */
#define m65c02_fb m6502_fb									/* 2 ILL */

OP(0c)
{
  int tmp;  /* 4 TSB ABS */
  m6502_ICount -= 2;
  RD_ABS;
  TSB;
  WB_EA;
}
#define m65c02_2c m6502_2c									/* 4 BIT ABS */
#define m65c02_4c m6502_4c									/* 3 JMP ABS */
OP(6c)
{
  int tmp;  /* 5 JMP IND */
  m6502_ICount -= 5;
  EA_IND;
  JMP;
}
#define m65c02_8c m6502_8c									/* 4 STY ABS */
#define m65c02_ac m6502_ac									/* 4 LDY ABS */
#define m65c02_cc m6502_cc									/* 4 CPY ABS */
#define m65c02_ec m6502_ec									/* 4 CPX ABS */

OP(1c)
{
  int tmp;  /* 4 TRB ABS */
  m6502_ICount -= 4;
  RD_ABS;
  TRB;
  WB_EA;
}
OP(3c)
{
  int tmp;  /* 4 BIT ABX */
  m6502_ICount -= 4;
  RD_ABX;
  BIT;
}
#define m65c02_5c m6502_5c									/* 2 ILL */
OP(7c)
{
  int tmp;  /* 6 JMP IAX */
  m6502_ICount -= 2;
  EA_IAX;
  JMP;
}
OP(9c)
{
  int tmp;  /* 4 STZ ABS */
  m6502_ICount -= 4;
  STZ;
  WR_ABS;
}
#define m65c02_bc m6502_bc									/* 4 LDY ABX */
#define m65c02_dc m6502_dc									/* 2 ILL */
#define m65c02_fc m6502_fc									/* 2 ILL */

#define m65c02_0d m6502_0d									/* 4 ORA ABS */
#define m65c02_2d m6502_2d									/* 4 AND ABS */
#define m65c02_4d m6502_4d									/* 4 EOR ABS */
#define m65c02_6d m6502_6d									/* 4 ADC ABS */
#define m65c02_8d m6502_8d									/* 4 STA ABS */
#define m65c02_ad m6502_ad									/* 4 LDA ABS */
#define m65c02_cd m6502_cd									/* 4 CMP ABS */
#define m65c02_ed m6502_ed									/* 4 SBC ABS */

#define m65c02_1d m6502_1d									/* 4 ORA ABX */
#define m65c02_3d m6502_3d									/* 4 AND ABX */
#define m65c02_5d m6502_5d									/* 4 EOR ABX */
#define m65c02_7d m6502_7d									/* 4 ADC ABX */
#define m65c02_9d m6502_9d									/* 5 STA ABX */
#define m65c02_bd m6502_bd									/* 4 LDA ABX */
#define m65c02_dd m6502_dd									/* 4 CMP ABX */
#define m65c02_fd m6502_fd									/* 4 SBC ABX */

#define m65c02_0e m6502_0e									/* 6 ASL ABS */
#define m65c02_2e m6502_2e									/* 6 ROL ABS */
#define m65c02_4e m6502_4e									/* 6 LSR ABS */
#define m65c02_6e m6502_6e									/* 6 ROR ABS */
#define m65c02_8e m6502_8e									/* 4 STX ABS */
#define m65c02_ae m6502_ae									/* 4 LDX ABS */
#define m65c02_ce m6502_ce									/* 6 DEC ABS */
#define m65c02_ee m6502_ee									/* 6 INC ABS */

#define m65c02_1e m6502_1e									/* 7 ASL ABX */
#define m65c02_3e m6502_3e									/* 7 ROL ABX */
#define m65c02_5e m6502_5e									/* 7 LSR ABX */
#define m65c02_7e m6502_7e									/* 7 ROR ABX */
OP(9e)
{
  int tmp;  /* 5 STZ ABX */
  m6502_ICount -= 5;
  STZ;
  WR_ABX;
}
#define m65c02_be m6502_be									/* 4 LDX ABY */
#define m65c02_de m6502_de									/* 7 DEC ABX */
#define m65c02_fe m6502_fe									/* 7 INC ABX */

OP(0f)
{
  int tmp;  /* 5 BBR0 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(0);
}
OP(2f)
{
  int tmp;  /* 5 BBR2 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(2);
}
OP(4f)
{
  int tmp;  /* 5 BBR4 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(4);
}
OP(6f)
{
  int tmp;  /* 5 BBR6 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(6);
}
OP(8f)
{
  int tmp;  /* 5 BBS0 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(0);
}
OP(af)
{
  int tmp;  /* 5 BBS2 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(2);
}
OP(cf)
{
  int tmp;  /* 5 BBS4 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(4);
}
OP(ef)
{
  int tmp;  /* 5 BBS6 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(6);
}

OP(1f)
{
  int tmp;  /* 5 BBR1 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(1);
}
OP(3f)
{
  int tmp;  /* 5 BBR3 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(3);
}
OP(5f)
{
  int tmp;  /* 5 BBR5 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(5);
}
OP(7f)
{
  int tmp;  /* 5 BBR7 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBR(7);
}
OP(9f)
{
  int tmp;  /* 5 BBS1 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(1);
}
OP(bf)
{
  int tmp;  /* 5 BBS3 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(3);
}
OP(df)
{
  int tmp;  /* 5 BBS5 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(5);
}
OP(ff)
{
  int tmp;  /* 5 BBS7 ZPG */
  m6502_ICount -= 5;
  RD_ZPG;
  BBS(7);
}

static void (*insn65c02[0x100])(void) =
{
  m65c02_00,m65c02_01,m65c02_02,m65c02_03,m65c02_04,m65c02_05,m65c02_06,m65c02_07,
  m65c02_08,m65c02_09,m65c02_0a,m65c02_0b,m65c02_0c,m65c02_0d,m65c02_0e,m65c02_0f,
  m65c02_10,m65c02_11,m65c02_12,m65c02_13,m65c02_14,m65c02_15,m65c02_16,m65c02_17,
  m65c02_18,m65c02_19,m65c02_1a,m65c02_1b,m65c02_1c,m65c02_1d,m65c02_1e,m65c02_1f,
  m65c02_20,m65c02_21,m65c02_22,m65c02_23,m65c02_24,m65c02_25,m65c02_26,m65c02_27,
  m65c02_28,m65c02_29,m65c02_2a,m65c02_2b,m65c02_2c,m65c02_2d,m65c02_2e,m65c02_2f,
  m65c02_30,m65c02_31,m65c02_32,m65c02_33,m65c02_34,m65c02_35,m65c02_36,m65c02_37,
  m65c02_38,m65c02_39,m65c02_3a,m65c02_3b,m65c02_3c,m65c02_3d,m65c02_3e,m65c02_3f,
  m65c02_40,m65c02_41,m65c02_42,m65c02_43,m65c02_44,m65c02_45,m65c02_46,m65c02_47,
  m65c02_48,m65c02_49,m65c02_4a,m65c02_4b,m65c02_4c,m65c02_4d,m65c02_4e,m65c02_4f,
  m65c02_50,m65c02_51,m65c02_52,m65c02_53,m65c02_54,m65c02_55,m65c02_56,m65c02_57,
  m65c02_58,m65c02_59,m65c02_5a,m65c02_5b,m65c02_5c,m65c02_5d,m65c02_5e,m65c02_5f,
  m65c02_60,m65c02_61,m65c02_62,m65c02_63,m65c02_64,m65c02_65,m65c02_66,m65c02_67,
  m65c02_68,m65c02_69,m65c02_6a,m65c02_6b,m65c02_6c,m65c02_6d,m65c02_6e,m65c02_6f,
  m65c02_70,m65c02_71,m65c02_72,m65c02_73,m65c02_74,m65c02_75,m65c02_76,m65c02_77,
  m65c02_78,m65c02_79,m65c02_7a,m65c02_7b,m65c02_7c,m65c02_7d,m65c02_7e,m65c02_7f,
  m65c02_80,m65c02_81,m65c02_82,m65c02_83,m65c02_84,m65c02_85,m65c02_86,m65c02_87,
  m65c02_88,m65c02_89,m65c02_8a,m65c02_8b,m65c02_8c,m65c02_8d,m65c02_8e,m65c02_8f,
  m65c02_90,m65c02_91,m65c02_92,m65c02_93,m65c02_94,m65c02_95,m65c02_96,m65c02_97,
  m65c02_98,m65c02_99,m65c02_9a,m65c02_9b,m65c02_9c,m65c02_9d,m65c02_9e,m65c02_9f,
  m65c02_a0,m65c02_a1,m65c02_a2,m65c02_a3,m65c02_a4,m65c02_a5,m65c02_a6,m65c02_a7,
  m65c02_a8,m65c02_a9,m65c02_aa,m65c02_ab,m65c02_ac,m65c02_ad,m65c02_ae,m65c02_af,
  m65c02_b0,m65c02_b1,m65c02_b2,m65c02_b3,m65c02_b4,m65c02_b5,m65c02_b6,m65c02_b7,
  m65c02_b8,m65c02_b9,m65c02_ba,m65c02_bb,m65c02_bc,m65c02_bd,m65c02_be,m65c02_bf,
  m65c02_c0,m65c02_c1,m65c02_c2,m65c02_c3,m65c02_c4,m65c02_c5,m65c02_c6,m65c02_c7,
  m65c02_c8,m65c02_c9,m65c02_ca,m65c02_cb,m65c02_cc,m65c02_cd,m65c02_ce,m65c02_cf,
  m65c02_d0,m65c02_d1,m65c02_d2,m65c02_d3,m65c02_d4,m65c02_d5,m65c02_d6,m65c02_d7,
  m65c02_d8,m65c02_d9,m65c02_da,m65c02_db,m65c02_dc,m65c02_dd,m65c02_de,m65c02_df,
  m65c02_e0,m65c02_e1,m65c02_e2,m65c02_e3,m65c02_e4,m65c02_e5,m65c02_e6,m65c02_e7,
  m65c02_e8,m65c02_e9,m65c02_ea,m65c02_eb,m65c02_ec,m65c02_ed,m65c02_ee,m65c02_ef,
  m65c02_f0,m65c02_f1,m65c02_f2,m65c02_f3,m65c02_f4,m65c02_f5,m65c02_f6,m65c02_f7,
  m65c02_f8,m65c02_f9,m65c02_fa,m65c02_fb,m65c02_fc,m65c02_fd,m65c02_fe,m65c02_ff
};


