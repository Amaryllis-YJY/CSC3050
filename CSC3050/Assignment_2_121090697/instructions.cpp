#include"instructions.h"

void Add(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=(int)Reg[Rs]+(int)Reg[Rt];}

void Addu(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=Reg[Rs]+Reg[Rt];}

void And(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=Reg[Rs]&Reg[Rt];}

void Div(uint* Reg,int Rs,int Rt)
{
    Reg[Reg_list["$lo"]]=(int)Reg[Rs]/(int)Reg[Rt];
    Reg[Reg_list["$hi"]]=(int)Reg[Rs]%(int)Reg[Rt];
}

void Divu(uint* Reg,int Rs,int Rt)
{
    Reg[Reg_list["$lo"]]=Reg[Rs]/Reg[Rt];
    Reg[Reg_list["$hi"]]=Reg[Rs]%Reg[Rt];
}

void Jalr(uint* Reg,int Rd,int Rs)
{
    Reg[Rd]=Reg[Reg_list["$pc"]];
    Reg[Reg_list["$pc"]]=Reg[Rs];
}

void Jr(uint* Reg,int Rs){Reg[Reg_list["$pc"]]=Reg[Rs];}

void Mfhi(uint* Reg,int Rd){Reg[Rd]=Reg[Reg_list["$hi"]];}

void Mflo(uint* Reg,int Rd){Reg[Rd]=Reg[Reg_list["$lo"]];}

void Mthi(uint* Reg,int Rs){Reg[Reg_list["$hi"]]=Reg[Rs];}

void Mtlo(uint* Reg,int Rs){Reg[Reg_list["$lo"]]=Reg[Rs];}

void Mult(uint* Reg,int Rs,int Rt)
{
    long long RS=(long long)Reg[Rs];
    long long RT=(long long)Reg[Rt];
    long long ans=RS*RT;
    Reg[Reg_list["$lo"]]=ans&0xffffffff;
    Reg[Reg_list["$hi"]]=ans>>32;
}

void Multu(uint* Reg,int Rs,int Rt)
{
    unsigned long long RS=(long long)Reg[Rs];
    unsigned long long RT=(long long)Reg[Rt];
    unsigned long long ans=RS*RT;
    Reg[Reg_list["$lo"]]=ans&0xffffffff;
    Reg[Reg_list["$hi"]]=ans>>32;
}

void Nor(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=~(Reg[Rs]|Reg[Rt]);}

void Or(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=Reg[Rs]|Reg[Rt];}

void Sll(uint* Reg,int Rd,int Rt,int Sa){Reg[Rd]=Reg[Rt]<<Sa;}

void Sllv(uint* Reg,int Rd,int Rt,int Rs){Reg[Rd]=Reg[Rt]<<Reg[Rs];}

void Slt(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=((int)Reg[Rs]<(int)Reg[Rt]);}

void Sltu(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=(Reg[Rs]<Reg[Rt]);}

void Sra(uint* Reg,int Rd,int Rt,int Sa)
{
    unsigned int sign=0x80000000&Reg[Rt];//get the sign bit of the binary
    Reg[Rd]=Reg[Rt]>>Sa;
    if (sign)
    {
        for(int i=0;i<32 and i<Sa;i++)
            Reg[Rd]|=sign>>i;
    }
}

void Srav(uint* Reg,int Rd,int Rt,int Rs)
{
    unsigned int sign=0x80000000&Reg[Rt];//get the sign bit of the binary
    Reg[Rd]=Reg[Rt]>>Reg[Rs];
    if (sign)
    {
        for(int i=0;i<32 and i<Reg[Rs];i++)
            Reg[Rd]|=sign>>i;
    }
}

void Srl(uint* Reg,int Rd,int Rt,int Sa){Reg[Rd]=Reg[Rt]>>Sa;}

void Srlv(uint* Reg,int Rd,int Rt,int Rs){Reg[Rd]=Reg[Rt]>>Reg[Rs];}

void Sub(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=(int)Reg[Rs]-(int)Reg[Rt];}

void Subu(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=Reg[Rs]-Reg[Rt];}

/*void Syscall(uint* Reg,std::ofstream file_out)
{
    switch(Reg[Reg_list["$v0"]])
    {
    case 1://print_int
        break;
    
    case 4://print_string
        break;

    case 5://read_int
        break;

    case 8://read_string
        break;

    case 9://sbrk
        break;
    
    case 10://exit
        break;
    
    case 11://print_char
        break;
    
    case 12://read_char
        break;
    
    case 13://open
        break;
    
    case 14://read
        break;
    
    case 15://write
        break;
    
    case 16://close
        break;
    
    case 17://exit2
        break;

    default:
        break;
    }
}*/

void Xor(uint* Reg,int Rd,int Rs,int Rt){Reg[Rd]=Reg[Rs]^Reg[Rt];}

void Addi(uint* Reg,int Rt,int Rs,int Imm){Reg[Rt]=(int)Reg[Rs]+(short)Imm;}

void Addiu(uint* Reg,int Rt,int Rs,int Imm){Reg[Rt]=Reg[Rs]+(unsigned short)Imm;}

void Andi(uint* Reg,int Rt,int Rs,int Imm){Reg[Rt]=Reg[Rs]&(Imm&0xffff);}

void Beq(uint* Reg,int Rs,int Rt,int Imm)
{
    if(Reg[Rt]==Reg[Rs])
        Reg[Reg_list["$pc"]]+=(short)Imm*4;
}

void Bgez(uint* Reg,int Rs,int Imm)
{
    if((int)Reg[Rs]>=0)
        Reg[Reg_list["$pc"]]+=(short)Imm*4;
}

void Bgtz(uint* Reg,int Rs,int Imm)
{
    if((int)Reg[Rs]>0)
        Reg[Reg_list["$pc"]]+=(short)Imm*4;
}

void Blez(uint* Reg,int Rs,int Imm)
{
    if((int)Reg[Rs]<=0)
        Reg[Reg_list["$pc"]]+=(short)Imm*4;
}

void Bltz(uint* Reg,int Rs,int Imm)
{
    if((int)Reg[Rs]<0)
        Reg[Reg_list["$pc"]]+=(short)Imm*4;
}

void Bne(uint* Reg,int Rt,int Rs,int Imm)
{
    if(Reg[Rt]!=Reg[Rs])
        Reg[Reg_list["$pc"]]+=(short)Imm*4;
}

void Lb(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    Reg[Rt]=(char)Mem[Reg[Rs]+(short)Imm-START_MEM];
}

void Lbu(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    Reg[Rt]=Mem[Reg[Rs]+(short)Imm-START_MEM];
}

void Lh(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    byte hi=Mem[Reg[Rs]+(short)Imm-START_MEM+1];
    byte lo=Mem[Reg[Rs]+(short)Imm-START_MEM];
    Reg[Rt]=lo|(hi<<8);
    if(hi&0x80)
        Reg[Rt]|=0xffff<<16;
}

void Lhu(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    byte hi=Mem[Reg[Rs]+(short)Imm-START_MEM+1];
    byte lo=Mem[Reg[Rs]+(short)Imm-START_MEM];
    Reg[Rt]=lo|(hi<<8);
}

void Lui(uint* Reg,int Rt,int Imm){Reg[Rt]=Imm<<16;}

void Lw(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    Reg[Rt]=Mem[cur]|(Mem[cur+1]<<8)|(Mem[cur+2]<<16)|(Mem[cur+3]<<24);
}

void Ori(uint* Reg,int Rt,int Rs,int Imm){Reg[Rt]=Reg[Rs]|Imm;}

void Sb(byte* Mem,uint* Reg,int Rt,int Rs,int Imm){Mem[Reg[Rs]+(short)Imm-START_MEM] =Reg[Rt]&0xff;}

void Slti(uint* Reg,int Rt,int Rs,int Imm)
{
    if((int)Reg[Rs]<(short)Imm)
        Reg[Rt]=1;
    else
        Reg[Rt]=0;
}

void Sltiu(uint* Reg,int Rt,int Rs,int Imm)
{
    if(Reg[Rs]<(unsigned short)Imm)
        Reg[Rt]=1;
    else
        Reg[Rt]=0;
}

void Sh(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    Mem[cur]=Reg[Rt]&0xff;
    Mem[cur+1]=Reg[Rt]>>8;
}

void Sw(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    Mem[cur]=Reg[Rt]&0xff;
    Mem[cur+1]=(Reg[Rt]>>8)&0xff;
    Mem[cur+2]=(Reg[Rt]>>16)&0xff;
    Mem[cur+3]=(Reg[Rt]>>24)&0xff;
}

void Xori(uint* Reg,int Rt,int Rs,int Imm){Reg[Rt]=Reg[Rs]^(unsigned short)Imm;}

void Lwl(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    int lower=cur&(~3);
    int i=24;
    while(cur>=lower)
    {
        Reg[Rt]&=~(0xff<<i);
        Reg[Rt]|=Mem[cur]<<i;
        cur--;
        i-=8;
    }
}

void Lwr(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    int upper=(cur+4)&(~3);
    int i=0;
    while(cur<=upper)
    {
        Reg[Rt]&=~(0xff<<i);
        Reg[Rt]|=Mem[cur]<<i;
        cur++;
        i+=8;
    }
}

void Swl(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    int lower=cur&(~3);
    int i=24;
    while(cur>=lower)
    {
        Mem[i]=(Reg[Rt]>>i)&0xff;
        cur--;
        i-=8;
    }
}

void Swr(byte* Mem,uint* Reg,int Rt,int Rs,int Imm)
{
    int cur=Reg[Rs]+(short)Imm-START_MEM;
    int upper=(cur+4)&(~3);
    int i=0;
    while(cur<=upper)
    {
        Mem[i]=(Reg[Rt]>>i)&0xff;
        cur++;
        i+=8;
    }
}

void J(uint* Reg,int Tar)
{
    Reg[Reg_list["$pc"]]&=0xf0000000;
    Reg[Reg_list["$pc"]]|=Tar<<2;
}

void Jal(uint* Reg,int Tar)
{
    Reg[Reg_list["$ra"]]=Reg[Reg_list["$pc"]];
    Reg[Reg_list["$pc"]]&=0xf0000000;
    Reg[Reg_list["$pc"]]|=Tar<<2;
}

