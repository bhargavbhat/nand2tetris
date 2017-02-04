#include<iostream>
#include<string>
#include<fstream>
#include<vector>

#include"common.h"
#include"lookup.h"
#include"translate.h"
#include"test.h"

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
    std::vector<std::string> processedLines;

    while(infile.good())
    {
        // read input file line-by-line
        std::string line;
        std::getline(infile, line);
        line = trim(line);

        // if line has something interesting,
        // keep it for later processing
        if(!line.empty())
            processedLines.push_back(line);
    }


    for(auto ln : processedLines)
    {
        std::string machine_code;
        if(isAInstr(ln))
            machine_code = translateAInst(ln);
        else
            machine_code = translateCInst(ln);
        
        // write output to outfile
        if(!ln.empty())
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
