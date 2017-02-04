#include<iostream>
#include<string>
#include<sstream>
#include<algorithm> 
#include<cstdint>
#include<map>

// Helper to remove unwanted chars
std::string trim(const std::string& str)
{
    std::stringstream out;
    for(auto i : str)
    {
        if((i == ' ')||(i == '\t'))
            continue;
        else if(i == '/')
            break;
        else
            out<<i;
    }
    return out.str();
}

bool isAInstr(const std::string& instr)
{
    if(instr.empty())
        return false;
    else if(instr[0] == '@')
        return true;
    else
        return false;
}

uint16_t translateAInst(const std::string& instr)
{
    //format: @num 
    //strip '@' and atoi on number
    if(instr.empty())
        return 0;
    else
        return std::stoul(instr.substr(1, instr.length()));
}

std::string translateCmp(const std::string& cmp)
{
    std::string resp;
    const std::map<std::string, std::string> CMP_MAP = {
        {"0",    "0101010"},
        {"1",    "0111111"},
        {"-1",   "0111010"},
        {"D",    "0001100"},
        {"A",    "0110000"},
        {"M",    "1110000"},
        {"!D",   "0001101"},
        {"!A",   "0110001"},
        {"!M",   "1110001"},
        {"-D",   "0001111"},
        {"-A",   "0110011"},
        {"-M",   "1110011"},
        {"D+1",  "0011111"},
        {"A+1",  "0110111"},
        {"M+1",  "1110111"},
        {"D-1",  "0001110"},
        {"A-1",  "0110010"},
        {"M-1",  "1110010"},
        {"D+A",  "0000010"},
        {"D+M",  "1000010"},
        {"D-A",  "0010011"},
        {"D-M",  "1010011"},
        {"A-D",  "0000111"},
        {"M-D",  "1000111"},
        {"D&A",  "0000000"},
        {"D&M",  "1000000"},
        {"D|A",  "0010101"},
        {"D|M",  "1010101"},
    };
    
    // retrieve the bit pattern corresponding
    // to the input computation
    auto it = CMP_MAP.find(cmp);
    if( it != CMP_MAP.end())
        resp = it->second;

    return resp;
}

std::string translateDst(const std::string& dst)
{
    std::string resp;
    const std::map<std::string, std::string> DST_MAP = {
        {"null","000"},
        {"M","001"},
        {"D","010"},
        {"MD","011"},
        {"A","100"},
        {"AM","101"},
        {"AD","110"},
        {"AMD","111"},
    };

    // retrieve the bit pattern corresponding
    // to the input computation
    auto it = DST_MAP.find(dst);
    if( it != DST_MAP.end())
        resp = it->second;

    return resp;
}

std::string translateJmp(const std::string& jmp)
{
    std::string resp;
    const std::map<std::string, std::string> JMP_MAP = {
    {"null","000"},
    {"JGT ","001"},
    {"JEQ ","010"},
    {"JGE ","011"},
    {"JLT ","100"},
    {"JNE ","101"},
    {"JLE ","110"},
    {"JMP ","111"},
    };

    // retrieve the bit pattern corresponding
    // to the input jump parameter
    auto it = JMP_MAP.find(jmp);
    if( it != JMP_MAP.end())
        resp = it->second;
    
    return resp;
}
uint16_t translateCInst(const std::string& instr)
{
    //format: dst=cmp;jmp
    //split into components: dst, cmp & jmp
    //translate each
    //concatenate result
    auto dst_marker = instr.find("=", 0);
    auto cmp_marker = instr.find(";", dst_marker);

#if 0
    std::cout<<"l:"<<instr.length()<<" d:"<<dst_marker<<" c:"<<cmp_marker<<std::endl;
#endif

    auto dst = (dst_marker == std::string::npos)
                ? "null" : instr.substr(0, dst_marker);
    auto cmp = (cmp_marker == std::string::npos) 
                ? instr.substr(dst_marker+1, instr.length()) 
                : instr.substr(dst_marker+1, cmp_marker-dst_marker-1);
    auto jmp = (cmp_marker == std::string::npos)
                ? "null" : instr.substr(cmp_marker+1, instr.length());

#if 0
    std::cout<<"|"<<dst
             <<"| = |"
             <<cmp
             <<"| ; |" 
             <<jmp
             <<"|"<<std::endl;
#endif

    const std::string op_code = "111";
    std::string cmp_code = translateCmp(cmp);
    std::string dst_code = translateDst(dst);
    std::string jmp_code = translateJmp(jmp);
    std::string final_code = op_code + cmp_code + dst_code + jmp_code;

#if 0
    std::cout<<"breakup: "<<"|" << op_code << "|"
             <<cmp_code <<"|"
             <<dst_code <<"|"
             <<jmp_code <<"|"
             <<std::endl;

    std::cout<<"binary: "<<final_code<<std::endl;
#endif

    return std::stoul(final_code);
}

void test_routines()
{
    auto LINE_OF_CODE = "  M=M+1       // incr M     ";
    auto A_INSTR="@42";
    auto ASSIGN="M=M+1";
    auto C_INSTR1="MD=M+1;null";
    auto C_INSTR2="MD=M+1";
    std::cout<<"***"<<LINE_OF_CODE<<"***"<<(trim(LINE_OF_CODE))<<"***"<<std::endl;
    std::cout<<A_INSTR<<" "<<isAInstr(A_INSTR)<<std::endl;
    std::cout<<ASSIGN<<" "<<isAInstr(ASSIGN)<<std::endl;
    if(isAInstr(A_INSTR))
        std::cout<<A_INSTR<<" trans: "<<translateAInst(A_INSTR)<<std::endl;
    std::cout<<C_INSTR1<<" trans: " << translateCInst(C_INSTR1)<<std::endl;
    std::cout<<"-----------------"<<std::endl;
    std::cout<<C_INSTR2<<" trans: " << translateCInst(C_INSTR2)<<std::endl;
}

int main()
{
    test_routines();
    return 0;
}
