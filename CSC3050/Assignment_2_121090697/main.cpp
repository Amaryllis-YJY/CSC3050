#include "instructions.h"

bool return_tag=false;
std::string ckpts;
std::string asm_in;
std::string txt_in;
std::string in_file;
std::string out_file;
std::ifstream file_in;
std::ofstream file_out;
std::set<int> checkpoints;
std::vector<std::string> insts;
uint reg[35];
byte mem[MAX_MEMORY];

int opcode,rs,rt,rd,sa,funct,imm,tar;
int used_mem=0,return_value=0;
char inst_type;

void Syscall();

std::string unescape(std::string str)
{
    std::string ans;
    auto it=str.begin();
    while(it!=str.end())
    {
        char c=*it;
        it++;
        if(c=='\\'&&it!=str.end())
            switch(*it++)
            {
                case 'a':
                    c='\a';
                    break;
                case 'b':
                    c='\b';
                    break;
                case 'f':
                    c='\f';
                    break;
                case 'n':
                    c='\n';
                    break;
                case 'r':
                    c='\r';
                    break;
                case 't':
                    c='\t';
                    break;
                case 'v':
                    c='\v';
                    break;
                case '\\':
                    c='\\';
                    break;
                case '\'':
                    c='\'';
                    break;
                case '\?':
                    c='\?';
                    break;
                default:
                    continue;
            }
        ans +=c;
    }
    return ans;
}

void init()
{
    for(int i=0;i<35;i++)
    	reg[i]=0;
    memset(mem,0,sizeof(mem));

    std::stringstream ss;
    std::ifstream inst_in;
    inst_in.open(txt_in,std::ios::in);
    std::string inst;
    while(getline(inst_in,inst))
    	insts.push_back(inst);
    inst_in.close();
    	
    for(int i=0;i<insts.size();i++)
    {
        std::string &inst_temp=insts[i];
        for(int j=0;j<4;j++)
        {
            std::string temp=inst_temp.substr(j*8,8);
            mem[i*4+3-j]=bin_string(temp);
        }
    }

    std::ifstream in;
    in.open(asm_in,std::ios::in);
    if(!in)
    {
        std::cout<<"Asm file does not exist"<<std::endl;
        return;
    }

    int count=0;
    std::string str;
    while(getline(in,str))
        if(del_space(str).substr(0,5)==".data")
            break;
    
    while(getline(in,str))
    {
        if(del_space(str).substr(0,5)==".text")
            break;
        std::string data_str,data_type;

        data_str=del(str);
        ss.clear();
        ss.str("");
        ss.str(data_str);
        ss>>data_type;

        if(data_type==".ascii"||data_type==".asciiz")
        {
            std::string Name;
            getline(ss,Name);
            if(!Name.empty())
            {
                Name=del_space(Name);
                Name=del_space(Name,'\t');
                Name=del_space(Name,'\n');
                Name=del_space(Name,'\"');
            }
            Name=unescape(Name);
            if(data_type==".asciiz")
                Name+='\0';
            for(int i=0;i<Name.size();i++)
                mem[TEXT_SEGMENT+used_mem++]=Name[i];
        }
        else if(data_type==".word"||data_type==".byte"||data_type==".half")
        {
            std::vector<int> list;
            int num;
            char c;
            while(ss>>num)
            {
                list.push_back(num);
                ss>>c;
            }
            for(int i=0;i<list.size();i++)
            {
                int cur=TEXT_SEGMENT+used_mem;
                if(data_type==".word")
                {
                    mem[cur]=list[i]&0xff;
                    mem[cur+1]=(list[i]>>8)&0xff;
                    mem[cur+2]=(list[i]>>16)&0xff;
                    mem[cur+3]=(list[i]>>32)&0xff;
                    used_mem+=4;
                }
                else if(data_type==".byte")
                {
                    mem[cur]=list[i];
                    used_mem++;
                }
                else if(data_type==".half")
                {
                    mem[cur]=list[i]&0xff;
                    mem[cur+1]=(list[i]>>8)&0xff;
                    used_mem+=2;
                }
            }
        }
        if(used_mem&3)
            used_mem=(used_mem+4)& ~3;
    }
    in.close();

    in.open(ckpts);
    if(!in)
    {
        std::cout<<"Output Checkpoints file does not exist"<<std::endl;
        return;
    }
    int ckpt;
    while(in>>ckpt)
        checkpoints.insert(ckpt);
}

int main(int argc,char *argv[])
{
    std::cout<<"please enter the asm file, binary codes file, checkpoints file, input file and output file in order."<<std::endl;
	std::cin>>asm_in>>txt_in>>ckpts>>in_file>>out_file;
    /*if (argc!=6)
    {
        std::cout<<"please enter the asm file, binary codes file, checkpoints file, input file and output file in order."<<std::endl;
        return 0;
    }*/
    init();

    int index=0;
    int inst_cnt=0;
    reg[Reg_list["$gp"]]=START_MEM+TEXT_SEGMENT+0x8000;
    reg[Reg_list["$sp"]]=START_MEM+MAX_MEMORY;
    reg[Reg_list["$pc"]]=START_MEM;

    file_in.open(in_file);
    file_out.open(out_file);

    while(index>=0&&index<insts.size())
    {
        if(checkpoints.count(inst_cnt))
        {
            Dump_Mem(inst_cnt,mem);
            Dump_Reg(inst_cnt,reg);
        }
        if(return_tag) break;
        auto inst=insts[index];
        reg[Reg_list["$pc"]]+=4;

        opcode=bin_string(inst.substr(0,6));
        if(opcode==0b000000)
        {
            inst_type='r';
            rs=bin_string(inst.substr(6, 5));
            rt=bin_string(inst.substr(11, 5));
            rd=bin_string(inst.substr(16, 5));
            sa=bin_string(inst.substr(21, 5));
            funct=bin_string(inst.substr(26, 6));
            //std::cout<<funct<<std::endl;
            switch(funct)
            {
            case 0b100000:
                Add(reg,rd,rs,rt);
                break;
            case 0b100001:
                Addu(reg,rd,rs,rt);
                break;
            case 0b100100:
                And(reg,rd,rs,rt);
                break;
            case 0b011010:
                Div(reg,rs,rt);
                break;
            case 0b011011:
                Divu(reg,rs,rt);
                break;
            case 0b001001:
                Jalr(reg,rd,rs);
                break;
            case 0b001000:
                Jr(reg,rs);
                break;
            case 0b010000:
                Mfhi(reg,rd);
                break;
            case 0b010010:
                Mflo(reg,rd);
                break;
            case 0b010001:
                Mthi(reg,rs);
                break;
            case 0b010011:
                Mtlo(reg,rs);
                break;
            case 0b011000:
                Mult(reg,rs,rt);
                break;
            case 0b011001:
                Multu(reg,rs,rt);
                break;
            case 0b100111:
                Nor(reg,rd,rs,rt);
                break;
            case 0b100101:
                Or(reg,rd,rs,rt);
                break;
            case 0b000000:
                Sll(reg,rd,rt,sa);
                break;
            case 0b000100:
                Sllv(reg,rd,rt,rs);
                break;
            case 0b101010:
                Slt(reg,rd,rs,rt);
                break;
            case 0b101011:
                Sltu(reg,rd,rs,rt);
                break;
            case 0b000011:
                Sra(reg,rd,rt,sa);
                break;
            case 0b000111:
                Srav(reg,rd,rt,rs);
                break;
            case 0b000010:
                Srl(reg,rd,rt,sa);
                break;
            case 0b000110:
                Srlv(reg,rd,rt,rs);
                break;
            case 0b100010:
                Sub(reg,rd,rs,rt);
                break;
            case 0b100011:
                Subu(reg,rd,rs,rt);
                break;
            case 0b001100:
                Syscall();
                break;
            case 0b100110:
                Xor(reg,rd,rs,rt);
                break;
            default:
                break;
            }
        }
        else if(opcode==0b000010||opcode==0b000011)
        {
        	//std::cout<<opcode<<std::endl;
            inst_type='j';
            tar=bin_string(inst.substr(6,26));
            switch(opcode)
            {
            case 0b000010:
                J(reg,tar);
                break;
            case 0b000011:
                Jal(reg,tar);
                break;
            default:
                break;
            }
        }
        else
        {
        	//std::cout<<opcode<<std::endl;
            inst_type='i';
            rs=bin_string(inst.substr(6, 5));
            rt=bin_string(inst.substr(11, 5));
            imm=bin_string(inst.substr(16, 16));
            switch(opcode)
            {
            case 0b001000:
                Addi(reg,rt,rs,imm);
                break;
            case 0b001001:
                Addiu(reg,rt,rs,imm);
                break;
            case 0b001100:
                Andi(reg,rt,rs,imm);
                break;
            case 0b000100:
                Beq(reg,rs,rt,imm);
                break;
            case 0b000001:
            if(rt==0b00001)
                Bgez(reg,rs,imm);
            else if(rt==0b00000)
                Bltz(reg,rs,imm);
                break;
            case 0b000111:
                Bgtz(reg,rs,imm);
                break;
            case 0b000110:
                Blez(reg,rs,imm);
                break;
            case 0b000101:
                Bne(reg,rt,rs,imm);
                break;
            case 0b100000:
                Lb(mem,reg,rt,rs,imm);
                break;
            case 0b100100:
                Lbu(mem,reg,rt,rs,imm);
                break;
            case 0b100001:
                Lh(mem,reg,rt,rs,imm);
                break;
            case 0b100101:
                Lhu(mem,reg,rt,rs,imm);
                break;
            case 0b001111:
                Lui(reg,rt,imm);
                break;
            case 0b100011:
                Lw(mem,reg,rt,rs,imm);
                break;
            case 0b001101:
                Ori(reg,rt,rs,imm);
                break;
            case 0b101000:
                Sb(mem,reg,rt,rs,imm);
                break;
            case 0b001010:
                Slti(reg,rt,rs,imm);
                break;
            case 0b001011:
                Sltiu(reg,rt,rs,imm);
                break;
            case 0b101001:
                Sh(mem,reg,rt,rs,imm);
                break;
            case 0b101011:
                Sw(mem,reg,rt,rs,imm);
                break;
            case 0b001110:
                Xori(reg,rt,rs,imm);
                break;
            case 0b100010:
                Lwl(mem,reg,rt,rs,imm);
                break;
            case 0b100110:
                Lwr(mem,reg,rt,rs,imm);
                break;
            case 0b101010:
                Swl(mem,reg,rt,rs,imm);
                break;
            case 0b101110:
                Swr(mem,reg,rt,rs,imm);
                break;        
            default:
                break;
            }
        }
        index=(reg[Reg_list["$pc"]]-START_MEM)>>2;
        inst_cnt++;
    }

    if(checkpoints.count(inst_cnt))
    {
        Dump_Mem(inst_cnt,mem);
        Dump_Reg(inst_cnt,reg);
    }

    file_in.close();
    file_out.close();
    return return_value;
}

void Syscall()
{
    switch(reg[Reg_list["$v0"]])
    {
    case 1://print_int
        file_out<<(int)reg[Reg_list["$a0"]];
        std::cout<<(int)reg[Reg_list["$a0"]]<<std::endl;
        file_out.flush();
        break;
    case 4://print_string
        file_out<<mem+reg[Reg_list["$a0"]]-START_MEM;
        std::cout<<mem+reg[Reg_list["$a0"]]-START_MEM<<std::endl;
        file_out.flush();
        break;
    case 5://read_int
    {
        int temp=0;
        file_in>>temp;
        std::string str0;
        getline(file_in,str0);
        reg[Reg_list["$v0"]]=temp;
        break;
    }
    case 8://read_string
    {
        std::string str1;
        getline(file_in,str1);
        for(int i=0;i<reg[Reg_list["$a1"]];i++)
            mem[reg[Reg_list["$a0"]]-START_MEM+i]=str1[i];
        break;
    }
    case 9://sbrk
        reg[Reg_list["$v0"]]=START_MEM+used_mem+TEXT_SEGMENT;
        used_mem+=reg[Reg_list["$a0"]];
        break;
    case 10://exit
        return_tag=true;
        break;    
    case 11://print_char
        file_out<<(char)reg[Reg_list["$a0"]];
        file_out.flush();
        break;    
    case 12://read_char
    {
        reg[Reg_list["$v0"]]=file_in.get();
        std::string str2;
        getline(file_in,str2);
        break;    
    }
    case 13://open
        reg[Reg_list["$a0"]]=open((const char*)mem+reg[Reg_list["$a0"]]-START_MEM,reg[Reg_list["$a1"]],reg[Reg_list["$a2"]]);
        break;    
    case 14://read
        reg[Reg_list["$a0"]]=read(reg[Reg_list["$a0"]],mem+reg[Reg_list["$a1"]]-START_MEM,reg[Reg_list["$a2"]]);
        break;    
    case 15://write
        reg[Reg_list["$a0"]]=write(reg[Reg_list["$a0"]],mem+reg[Reg_list["$a1"]]-START_MEM,reg[Reg_list["$a2"]]);
        break;    
    case 16://close
        close(reg[Reg_list["$a0"]]);
        break;    
    case 17://exit2
        return_tag=true;
        return_value=reg[Reg_list["$a0"]];
        break;
    default:
        break;
    }
}