#include "LabelTable.h"

std::string& del_space(std::string &s)
{
	int l=0;
  	int r=s.length()-1;
    while(l<=r&&s[l]==' ')
    	l++;
    while(l<=r&&s[r]==' ')
    	r--;

 	s.erase(r+1,s.length());
 	s.erase(0,l);
 	return s;
}

std::string del(std::string s)
{
	if(s.find('#')!=std::string::npos)
		s=s.substr(0,s.find('#'));
	if(s.find(':')!=std::string::npos)
		s=s.substr(s.find(':')+1,s.length());
	return del_space(s);
}

std::string convert(int num,int digit)
{
	std::string bin="";
	while(num!=0)
	{
		int temp=num%2;
		if(temp==0)
			bin="0"+bin;
		else
			bin="1"+bin;
		num=num/2;
		digit--;		
	}
	while(digit>0)
	{
		bin="0"+bin;
		digit--;
	}
	return bin;
}

int convert_num(std::string str)
{
	int num;
	std::stringstream ss;
	ss.str(str);
	ss>>num;
	while(num>=65536)
		num-=65536;
	while(num<0)
		num+=65536;
	return num;
}

std::map<std::string,Inst*> Inst_list
{
		{"add",new Inst{'r',0,0x20,{"rd","rs","rt"}}},
		{"addu",new Inst{'r',0,0x21,{"rd","rs","rt"}}},
		{"and",new Inst{'r',0,0x24,{"rd","rs","rt"}}},
		{"div",new Inst{'r',0,0x1A,{"rs","rt",""}}},
		{"divu",new Inst{'r',0,0x1B,{"rs","rt",""}}},
		{"jalr",new Inst{'r',0,0x09,{"rd","rs",""}}},
		{"jr",new Inst{'r',0,0x08,{"rs","",""}}},
		{"mfhi",new Inst{'r',0,0x10,{"rd","",""}}},
		{"mflo",new Inst{'r',0,0x12,{"rd","",""}}},
		{"mthi",new Inst{'r',0,0x11,{"rs","",""}}},
		{"mtlo",new Inst{'r',0,0x13,{"rs","",""}}},
		{"mult",new Inst{'r',0,0x18,{"rs","rt",""}}},
		{"multu",new Inst{'r',0,0x19,{"rs","rt",""}}},
		{"nor",new Inst{'r',0,0x27,{"rd","rs","rt"}}},
		{"or",new Inst{'r',0,0x25,{"rd","rs","rt"}}},
		{"sll",new Inst{'r',0,0x00,{"rd","rt","sa"}}},
		{"sllv",new Inst{'r',0,0x04,{"rd","rt","rs"}}},
		{"slt",new Inst{'r',0,0x2A,{"rd","rs","rt"}}},
		{"sltu",new Inst{'r',0,0x2B,{"rd","rs","rt"}}},
		{"sra",new Inst{'r',0,0x03,{"rd","rs","sa"}}},
		{"srav",new Inst{'r',0,0x07,{"rd","rt","rs"}}},
		{"srl",new Inst{'r',0,0x02,{"rd","rt","sa"}}},
		{"srlv",new Inst{'r',0,0x06,{"rd","rt","rs"}}},
		{"sub",new Inst{'r',0,0x22,{"rd","rs","rt"}}},
		{"subu",new Inst{'r',0,0x23,{"rd","rs","rt"}}},
		{"syscall",new Inst{'r',0,0x0C,{"","",""}}},
		{"xor",new Inst{'r',0,0x26,{"rd","rs","rt"}}},
		
		{"addi",new Inst{'i',0x08,0,{"rt","rs","imm"}}},
		{"addiu",new Inst{'i',0x09,0,{"rt","rs","imm"}}},
		{"andi",new Inst{'i',0x0C,0,{"rt","rs","imm"}}},
		{"beq",new Inst{'i',0x04,0,{"rs","rt","label"}}},
		{"bgez",new Inst{'i',0x01,0,{"rs","label",""}}},
		{"bgtz",new Inst{'i',0x07,0,{"rs","label",""}}},
		{"blez",new Inst{'i',0x06,0,{"rs","label",""}}},
		{"bltz",new Inst{'i',0x01,0,{"rs","label",""}}},
		{"bne",new Inst{'i',0x05,0,{"rs","rt","label"}}},
		{"lb",new Inst{'i',0x20,0,{"rt","imm_rs",""}}},
		{"lbu",new Inst{'i',0x24,0,{"rt","imm_rs",""}}},
		{"lh",new Inst{'i',0x21,0,{"rt","imm_rs",""}}},
		{"lhu",new Inst{'i',0x25,0,{"rt","imm_rs",""}}},
		{"lui",new Inst{'i',0x0F,0,{"rt","imm",""}}},
		{"lw",new Inst{'i',0x23,0,{"rt","imm_rs",""}}},
		{"ori",new Inst{'i',0x0D,0,{"rt","rs","imm"}}},
		{"sb",new Inst{'i',0x28,0,{"rt","imm_rs",""}}},
		{"slti",new Inst{'i',0x0A,0,{"rt","rs","imm"}}},
		{"sltiu",new Inst{'i',0x0B,0,{"rt","rs","imm"}}},
		{"sh",new Inst{'i',0x29,0,{"rt","imm_rs",""}}},
		{"sw",new Inst{'i',0x2B,0,{"rt","imm_rs",""}}},
		{"xori",new Inst{'i',0x0E,0,{"rt","rs","imm"}}},
		{"lwl",new Inst{'i',0x22,0,{"rt","imm_rs",""}}},
		{"lwr",new Inst{'i',0x26,0,{"rt","imm_rs",""}}},
		{"swl",new Inst{'i',0x2A,0,{"rt","imm_rs",""}}},
		{"swr",new Inst{'i',0x2E,0,{"rt","imm_rs",""}}},
		
		{"j",new Inst{'j',0x02,0,{"label","",""}}},
		{"jal",new Inst{'j',0x03,0,{"label","",""}}}
};

std::map<std::string,int> Reg_list
{
		{"$zero",0},{"$at",1},
		{"$v0",2},{"$v1",3},
		{"$a0",4},{"$a1",5},{"$a2",6},{"$a3",7},
		{"$t0",8},{"$t1",9},{"$t2",10},{"$t3",11},{"$t4",12},{"$t5",13},{"$t6",14},{"$t7",15},
		{"$s0",16},{"$s1",17},{"$s2",18},{"$s3",19},{"$s4",20},{"$s5",21},{"$s6",22},{"$s7",23},
		{"$t8",24},{"$t9",25},
		{"$k0",26},{"$k1",27},
		{"$gp",28},{"$sp",29},{"$fp",30},{"$ra",31}
};