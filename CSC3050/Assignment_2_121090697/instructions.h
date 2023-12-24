#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include"dump.h"
#include"LabelTable.h"

void Add(uint* Reg,int Rd,int Rs,int Rt);
void Addu(uint* Reg,int Rd,int Rs,int Rt);
void And(uint* Reg,int Rd,int Rs,int Rt);
void Div(uint* Reg,int Rs,int Rt);
void Divu(uint* Reg,int Rs,int Rt);
void Jalr(uint* Reg,int Rd,int Rs);
void Jr(uint* Reg,int Rs);
void Mfhi(uint* Reg,int Rd);
void Mflo(uint* Reg,int Rd);
void Mthi(uint* Reg,int Rs);
void Mtlo(uint* Reg,int Rs);
void Mult(uint* Reg,int Rs,int Rt);
void Multu(uint* Reg,int Rs,int Rt);
void Nor(uint* Reg,int Rd,int Rs,int Rt);
void Or(uint* Reg,int Rd,int Rs,int Rt);
void Sll(uint* Reg,int Rd,int Rt,int Sa);
void Sllv(uint* Reg,int Rd,int Rt,int Rs);
void Slt(uint* Reg,int Rd,int Rs,int Rt);
void Sltu(uint* Reg,int Rd,int Rs,int Rt);
void Sra(uint* Reg,int Rd,int Rt,int Sa);
void Srav(uint* Reg,int Rd,int Rt,int Rs);
void Srl(uint* Reg,int Rd,int Rt,int Sa);
void Srlv(uint* Reg,int Rd,int Rt,int Rs);
void Sub(uint* Reg,int Rd,int Rs,int Rt);
void Subu(uint* Reg,int Rd,int Rs,int Rt);
//void syscall();
void Xor(uint* Reg,int Rd,int Rs,int Rt);

void Addi(uint* Reg,int Rt,int Rs,int Imm);
void Addiu(uint* Reg,int Rt,int Rs,int Imm);
void Andi(uint* Reg,int Rt,int Rs,int Imm);
void Beq(uint* Reg,int Rs,int Rt,int Imm);
void Bgez(uint* Reg,int Rs,int Imm);
void Bgtz(uint* Reg,int Rs,int Imm);
void Blez(uint* Reg,int Rs,int Imm);
void Bltz(uint* Reg,int Rs,int Imm);
void Bne(uint* Reg,int Rt,int Rs,int Imm);
void Lb(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Lbu(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Lh(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Lhu(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Lui(uint* Reg,int Rt,int Imm);
void Lw(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Ori(uint* Reg,int Rt,int Rs,int Imm);
void Sb(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Slti(uint* Reg,int Rt,int Rs,int Imm);
void Sltiu(uint* Reg,int Rt,int Rs,int Imm);
void Sh(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Sw(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Xori(uint* Reg,int Rt,int Rs,int Imm);
void Lwl(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Lwr(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Swl(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void Swr(byte* Mem,uint* Reg,int Rt,int Rs,int Imm);
void J(uint* Reg,int Tar);
void Jal(uint* Reg,int Tar);

#endif
