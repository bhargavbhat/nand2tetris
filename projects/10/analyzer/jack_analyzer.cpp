#include<iostream>
#include<vector>
#include<string>

#include"compilation_engine.h"

void getFileList(const std::string& infile,
                    std::vector<std::string>& fileNames)
{
    //TODO: use the file/dir parsing logic from vm_translator
    //to return list of *.jack files

    (void) infile;

    fileNames.clear();
    fileNames.push_back("../ArrayTest/Main.jack");
}

void getOutputFile(const std::string& infile, std::string& outfile)
{
    // TODO: proper logic to generate output filename
    outfile = infile + ".xml";
}

int main()
{
    std::vector<std::string> files;
    getFileList("dummy", files);
    for(auto file : files)
    {
        std::string ofile;
        getOutputFile(file, ofile);
        CompilationEngine c(file, ofile);
        c.compileClass();
    }

    return 0;
}
