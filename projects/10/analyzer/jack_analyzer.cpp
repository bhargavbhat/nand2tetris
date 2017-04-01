#include<iostream>
#include<vector>
#include<string>
#include<sys/stat.h>
#include<dirent.h>

#include"compilation_engine.h"
#include"common.h"


void getOutputFile(const std::string& infile, std::string& outfile);
void getFileList(const std::string& infile, 
                    std::vector<std::string>& fileNames);

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout<<"Usage: JackAnalyzer <input.jack>"<<std::endl;
        std::cout<<"       JackAnalyzer <input_dir>"<<std::endl;
        return 0;
    }

    std::vector<std::string> fileNames;
    getFileList(argv[1], fileNames);
    for(auto file : fileNames)
    {
        std::string ofile;
        getOutputFile(file, ofile);
        CompilationEngine c(file, ofile);
        c.compileClass();
    }

    return 0;
}
void getOutputFile(const std::string& infile, std::string& outfile)
{
    // TODO: proper logic to generate output filename
    outfile = removeFileExtn(infile) + ".xml";
#ifdef DEBUG_MAIN
    std::cout<<"Writing output to: "<<outfile<<std::endl;
#endif
}

void getFileList(const std::string& infile, 
                    std::vector<std::string>& fileNames)
{
    struct stat s;

    if(stat(infile.c_str(),&s) == 0)
    {
        if(s.st_mode & S_IFDIR)
        {
#ifdef DEBUG_MAIN
            std::cout<<infile<<" is a directory with jack files: "<<std::endl;
#endif
            // input is a directory
            // get list of *.jack files in this dir
            DIR *d = NULL;
            dirent *dir = NULL;
            d = opendir(infile.c_str());
            if(d)
            {
                while((dir = readdir(d)) != NULL)
                {
                    if(dir->d_type == DT_REG)
                    {
                        std::string name = dir->d_name;
                        std::string extn = name.substr(name.find_last_of(".") + 1);
                        if(extn.compare("jack") == 0)
                        {
#ifdef DEBUG_MAIN
                            std::cout<<name<<std::endl;
#endif
                            fileNames.push_back(std::string(infile) + "//" + name);
                        }
                    }
                }
                closedir(d);
            }
        }
        else if(s.st_mode & S_IFREG)
        {
#ifdef DEBUG_MAIN
            std::cout<<infile<<" is a file"<<std::endl;
#endif
            // input is a single file
            fileNames.push_back(infile);
        }
        else
        {
            std::cout<<"stat() error!"<<std::endl;
            exit(0);
        }
    }
    else
    {
        std::cout<<"stat() error!"<<std::endl;
        exit(0);
    }

#ifdef DEBUG_MAIN
    std::cout<<"There are "<<fileNames.size()<<" input file(s)"<<std::endl;
    for(auto file : fileNames)
        std::cout<<file<<std::endl;
#endif
}
