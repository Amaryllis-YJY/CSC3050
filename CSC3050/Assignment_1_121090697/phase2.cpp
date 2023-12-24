#include "LabelTable.h"
#include<vector>
#include<iostream>
#include<fstream>


int cnt=0;

std::string R_type(std::string inst_name,std::vector<std::string> list,LabelTable table)
{
    std::string opcode="000000",
                rs="00000",
                rt="00000",
                rd="00000",
                sa="00000",
                function="000000";

	function=convert(Inst_list[inst_name]->funct,6);
	for(int i=0;i<list.size();i++)
	{
		std::string temp=Inst_list[inst_name]->arg[i];
		if(temp=="rs")
			rs=convert(Reg_list[list[i]],5);
		else if(temp=="rt")
			rt=convert(Reg_list[list[i]],5);
		else if(temp=="rd")
			rd=convert(Reg_list[list[i]],5);
		else if(temp=="sa")
			sa=convert(Reg_list[list[i]],5);
	}
	std::string com=opcode+rs+rt+rd+sa+function;
	return com;
}

std::string I_type(std::string inst_name,std::vector<std::string> list,LabelTable table)
{
	std::string opcode="000000",
                rs="00000",
                rt="00000",
                imm="0000000000000000";
	if(inst_name=="bgez")
		rt="00001";
	opcode=convert(Inst_list[inst_name]->opcode,6);
	for(int i=0;i<list.size();i++)
	{
		std::string temp=Inst_list[inst_name]->arg[i];
		if(temp=="rs")
			rs=convert(Reg_list[list[i]],5);
		else if(temp=="rt")
			rt=convert(Reg_list[list[i]],5);
		else if(temp=="imm")
			imm=convert(convert_num(list[i]),16);
		else if(temp=="imm_rs")
		{
			int l=0,r=0;	//the position of left and right brackets
			l=list[i].find('(');
			r=list[i].find(')');
			int num=convert_num(list[i].substr(0,l));
			imm=convert(num,16);
			std::string reg=list[i].substr(l+1,r-l-1);
			rs=convert(Reg_list[reg],5);
		}
		else if(temp=="label")
		{
			int num=table.label_list[list[i]];
			int dis=num-cnt-1;	//if dis<0 the convert period behaves not well
			if(dis<0)
				dis+=65536;
			imm=convert(dis,16);
			//cout<<list[i]<<endl;
			//cout<<num<<' '<<cnt<<' '<<num-cnt-1<<endl;
		}
	}
	std::string com=opcode+rs+rt+imm;
	return com;
}

std::string J_type(std::string inst_name,std::vector<std::string> list,LabelTable table)
{
	std::string opcode="000000",
                imm="00000000000000000000000000";
	opcode=convert(Inst_list[inst_name]->opcode,6);
	imm=convert((0x100000+table.label_list[list[0]]),26);
	std::string com=opcode+imm;
	//cout<<list[0]<<" j"<<endl;
	return com;
}


void pass2(std::string input_file,std::string output_file,LabelTable table)
{
	std::ifstream in_file;
	std::ofstream out_file;
	in_file.open(input_file,std::ios::in);
	out_file.open(output_file,std::ios::out);
	
	std::string str;
	while(getline(in_file,str))
		if(del_space(str).substr(0,5)==".text")
			break;
	
	std::stringstream ss;
	while(getline(in_file,str))
	{
		str=del(str);
		if(str.length())
		{
			std::vector<std::string> arg_list;
			ss.clear();
			ss.str("");
			std::string inst,arg;
			ss.str(str);
			
			ss>>inst;
			while(ss>>arg)
				if(arg!=",")
				{
					if(arg.back()==',')
						arg=arg.substr(0,arg.length()-1);
					arg_list.push_back(arg);
				}

			if(Inst_list[inst]->inst_type=='i')
				out_file<<I_type(inst,arg_list,table)<<std::endl;
			else if(Inst_list[inst]->inst_type=='j')
				out_file<<J_type(inst,arg_list,table)<<std::endl;
			else if(Inst_list[inst]->inst_type=='r')
				out_file<<R_type(inst,arg_list,table)<<std::endl;
			else
				out_file<<""<<std::endl;
			cnt++;				
		}
	}
}