#include "LabelTable.h"
#include<iostream>
#include<fstream>

LabelTable pass1(std::string filename)
{
	LabelTable table;
	std::ifstream file;
	file.open(filename,std::ios::in);
	if(!file)
	{
		std::cout<<"Failed to open "<<filename<<"."<<std::endl;
		return table;
	}
	
	std::string str;
	while(getline(file,str))
		if(del_space(str).substr(0,5)==".text")
			break;

	int cnt=0;

	while(getline(file,str))
	{
		if(str.back()==':')
			table.label_list[str.substr(0,str.length()-1)]=cnt;
		if(del(str).length())
			cnt++;
	}
	file.close();
	return table;
}
