#include<iostream>
#include<string>
#include<sstream>
#include<algorithm> 
#include<cstdint>
#include<map>
#include<bitset>
#include<fstream>

//#define DEBUG
//#define TEST

// Helper to remove unwanted chars
std::string trim(const std::string& str)
{
    std::stringstream out;

    // if len< 2 ignore, valid shortest instr: @1 etc
    // if first two chars are "//", ignore, it is a comment
    if((str.length() >= 2) && (str[0] != '/' && str[1] != '/'))
    {
        for(auto i : str)
        {
            if((i == ' ')||(i == '\t'))
                continue;
            else if((i == '/') || (i == '\r') || (i == '\n'))
                break;
            else
                out<<i;
        }
    }
    return out.str();
}

bool isAInstr(const std::string& instr)
{
    return (!instr.empty() && instr[0] == '@') ? true : false;
}

std::string translateAInst(const std::string& instr)
{
    //format: @num 
    //strip '@' and atoi on number
    const std::string op_code = "0";
    uint16_t value = 0;

    if(!instr.empty())
        value = std::stoul(instr.substr(1, instr.length()));

    // there is not std::bin, so use bitset
    // to convert value into binary representation
    // and discard MSB
    std::bitset<16> resp = value;
    return op_code + resp.to_string().substr(1, 15);
}

std::string lookup(bool isCMP, const std::string& in)
{
    std::string resp;

    // lookup map for cmp bits
    const std::map<std::string, std::string> CMAP = {
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

    // common lookup for dst and jmp bits
    std::map<std::string, std::string> DJMAP = {
        {"null", "000"},

        // dst bits
        {"M","001"},
        {"D","010"},
        {"MD","011"},
        {"A","100"},
        {"AM","101"},
        {"AD","110"},
        {"AMD","111"},

        // jmp bits
        {"JGT ","001"},
        {"JEQ ","010"},
        {"JGE ","011"},
        {"JLT ","100"},
        {"JNE ","101"},
        {"JLE ","110"},
        {"JMP ","111"},
    };

    // determine which map to use for lookup
    // operation based on isCMP
    auto& lookupMap = isCMP ? CMAP : DJMAP;
    
    // retrieve the bit pattern corresponding
    // to the input parameter
    auto it = lookupMap.find(in);
    if( it != lookupMap.end())
        resp = it->second;
   
#ifdef DEBUG
    std::cout<<__FUNCTION__<<"in:**"
            <<in<<"**\t\t**"
            <<resp<<"**"<<std::endl;
#endif
    // return bit pattern
    return resp;
}

std::string translateCInst(const std::string& instr)
{
    //format: dst=cmp;jmp
    //split into components: dst, cmp & jmp
    //translate each and concat to get result
    const std::string op_code = "111";
   
    // find the dst and cmp sub-expressions
    // by looking for = and ; respectively
    // dst is always to left of =
    // cmp is always between = and ;
    auto dst_marker = instr.find("=", 0);
    auto cmp_marker = instr.find(";", dst_marker);

    // extract each component
    auto dst = (dst_marker == std::string::npos)
                ? "null" : instr.substr(0, dst_marker);
    auto cmp = (cmp_marker == std::string::npos) 
                ? instr.substr(dst_marker+1, instr.length()) 
                : instr.substr(dst_marker+1, cmp_marker-dst_marker-1);
    auto jmp = (cmp_marker == std::string::npos)
                ? "null" : instr.substr(cmp_marker+1, instr.length());

    // lookup bit pattern for each component
    auto cmp_code = lookup(true, cmp);
    auto dst_code = lookup(false, dst);
    auto jmp_code = lookup(false, jmp);

    // concat to get final bit pattern
    auto final_code = op_code + cmp_code + dst_code + jmp_code;

#ifdef DEBUG   
    std::cout<<"i:"<<instr<<"\t"
             <<"l:"<<instr.length()<<"\t"
             <<"d:"<<dst_marker<<"\t"
             <<"c:"<<cmp_marker<<std::endl;

    std::cout<<"D:"<<dst<<"\t\t"
             <<"C:"<<cmp<<"\t\t"
             <<"J:"<<jmp<<"\t\t"
             <<std::endl;

    std::cout<<"R:"<<final_code<<std::endl;
#endif

    return final_code;
}

void test_routines()
{
    auto LINE_OF_CODE = "  M=M+1       // incr M     ";
    auto A_INSTR="@42";
    auto ASSIGN="M=M+1";
    auto C_INSTR1="MD=M+1;null";
    auto C_INSTR2="MD=D+1";
    auto C_INSTR3="MD=A-1;JGE";
    std::cout<<"***"<<LINE_OF_CODE<<"***"<<(trim(LINE_OF_CODE))<<"***"<<std::endl;
    std::cout<<A_INSTR<<" "<<isAInstr(A_INSTR)<<std::endl;
    std::cout<<ASSIGN<<" "<<isAInstr(ASSIGN)<<std::endl;
    if(isAInstr(A_INSTR))
        std::cout<<A_INSTR<<" trans: "<<translateAInst(A_INSTR)<<std::endl;
    std::cout<<C_INSTR1<<" trans: " << translateCInst(C_INSTR1)<<std::endl;
    std::cout<<"-----------------"<<std::endl;
    std::cout<<C_INSTR2<<" trans: " << translateCInst(C_INSTR2)<<std::endl;
    std::cout<<"-----------------"<<std::endl;
    std::cout<<C_INSTR3<<" trans: " << translateCInst(C_INSTR3)<<std::endl;
}

int main(int argc, char** argv)
{
#ifdef TEST
    (void) argc;
    (void) argv;
    test_routines();
#else
    
    if(argc != 3)
    {
        std::cout<<"Usage: "<< argv[0] << " input.asm output.hack\n"<<std::endl;
        return 0;
    }

    std::ifstream infile(argv[1]);
    std::ofstream outfile(argv[2]);

    while(infile.good())
    {
        // read input file line-by-line
        std::string line;
        std::string machine_code;
        std::getline(infile, line);
        line = trim(line);
    
        // if line has something interesting,
        // process it and get the output binary code
        if(!line.empty())
        {
            if(isAInstr(line))
                machine_code = translateAInst(line);
            else
                machine_code = translateCInst(line);
        }

        // write output to outfile
        if(!line.empty())
        {
            std::cout<<machine_code<<std::endl;
            outfile<<machine_code<<std::endl;
        }
    }

    //cleanup
    infile.close();
    outfile.close();
#endif

    return 0;
}
