#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include"common.h"
#include"pass.h"
#include"test.h"

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout<<"Usage: "<< argv[0] << " input.asm output.hack\n"<<std::endl;
        test_routines();
        return 0;
    }

    std::ifstream infile(argv[1]);
    std::vector<std::string> processedLines;

    if(infile.is_open())
    {
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
        infile.close();

        // PASS 1a : build symbol table & delete unwanted labels
        pass_symbols(processedLines);

        // Pass 1b : Resolve forward references
        pass_fwdref(processedLines);

        // Pass 2 : Convert to machine code
        auto machine_code = pass_translate(processedLines);

        // Output to file
        std::ofstream outfile(argv[2]);
        if(outfile.is_open())
        {
            outfile<<machine_code;
            outfile.close();
        }
        else
        {
            std::cout<<"Can't open output file: "<<argv[2]<<std::endl;
        }
    }
    else
    {
        std::cout<<"Can't open input file: "<<argv[1]<<std::endl;
    }
    
    return 0;
}
