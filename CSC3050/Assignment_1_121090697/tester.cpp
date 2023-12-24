#include<iostream> 
#include<fstream>
#include "LabelTable.h"
using namespace std;


int compare_files(FILE* fp1, FILE* fp2)
{
	char char1 = fgetc(fp1);
	char char2 = fgetc(fp2);

	while(char1 != EOF && char2 != EOF){
		if(char1 != char2){
			return -1;
		}
        char1 = fgetc(fp1);
        char2 = fgetc(fp2);
	}
	return 0;
}
int main(void)
{
	cout<<"Please enter an input file, an output file, and expected output file"<<endl;
	cout<<"No spaces included in file name!"<<endl;
	string in_file,out_file,exp_out;
	cin>>in_file>>out_file>>exp_out;
    LabelTable table;
    table = pass1(in_file);
    pass2(in_file, out_file, table);
    
    const char* out1=out_file.c_str();
    const char* out2=exp_out.c_str();

    FILE* fp1;
    FILE* fp2;
    fp1 = fopen(out1, "r");
    fp2 = fopen(out2, "r");

    if(fp1 == NULL || fp2 == NULL){
    	cout<<"Error: Files are not open correctly"<<endl;
    }

    int res = compare_files(fp1, fp2);

    if(res == 0){
    	cout<<"ALL PASSED! CONGRATS :) "<<endl;
    }else{
    	cout<<"YOU DID SOMETHING WRONG :( "<<endl;
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}