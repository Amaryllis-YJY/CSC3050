#include "dump.h"

void Dump_Reg(int inst_cnt,uint* Reg)
{
    std::string str="register_"+std::to_string(inst_cnt)+".bin";
    std::ofstream os(str,std::ios::binary);
    os.write((const char *)Reg,(35<<2));
    os.close();
}

void Dump_Mem(int inst_cnt,byte* Mem)
{
    std::string str="memory_"+std::to_string(inst_cnt)+".bin";
    std::ofstream os(str,std::ios::binary);
    os.write((const char *)Mem,MAX_MEMORY);
    os.close();
}