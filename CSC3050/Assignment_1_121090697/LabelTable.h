#ifndef LABELTABLE_H
#define LABELTABLE_H

#include<map>
#include<vector>
#include<string>
#include<sstream>


typedef unsigned int uint;

std::string& del_space(std::string &s);

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

LabelTable pass1(std::string filename);
void pass2(std::string input_file,std::string output_file,LabelTable table);

#endif