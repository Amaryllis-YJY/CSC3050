#ifndef LABELTABLE_H
#define LABELTABLE_H

#include<bits/stdc++.h>
/*#include<map>
#include<vector>
#include<string>
#include<sstream>*/

#define MAX_MEMORY 0x600000
#define START_MEM 0x400000
#define TEXT_SEGMENT 0x100000

typedef unsigned int uint;
typedef unsigned char byte;

uint bin_string(std::string s);

std::string& del_space(std::string &s,char c=' ');

std::string del(std::string s);

std::string convert(int num,int digit);

int convert_num(std::string str);

struct Inst
{
	char inst_type;//"i", "j", or "r"
	uint opcode;
	uint funct;
	std::string arg[3];
};
//arguments types: rd, rs, rt, sa, imm, imm_rs, label;"" means empty;

extern std::map<std::string,int> Reg_list;

extern std::map<std::string,Inst*> Inst_list;

struct LabelTable
{
	std::map<std::string, int> label_list;
};

#endif