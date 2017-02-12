#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<sys/stat.h>
#include<dirent.h>

#include"common.h"
#include"parser.h"
#include"code_writer.h"

void processInputFile(const std::string& infile, CodeWriter& c);
void getFileList(const std::string& infile,
                    std::vector<std::string>& fileNames,
                    std::string& outfile);

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout<<"Usage: vm_translator <input.vm>"<<std::endl;
        std::cout<<"       vm_translator <input_dir>"<<std::endl;
        return 0;
    }
   
    // fetch full paths of *.vm files to be translted
    std::vector<std::string> fileNames;
    std::string outfile;
    getFileList(argv[1], fileNames, outfile);

    // create a CodeWriter and write out the bootstrap code
    CodeWriter c(outfile);
    c.writeInit();

    // parse each input file and generate code for it
    for(auto file : fileNames)
        processInputFile(file, c);

    // close output file (also flushes the content)
    c.close();

    return 0;
}

void processInputFile(const std::string& infile, CodeWriter& c)
{
    Parser p(infile);
    c.setFilename(infile);
    
    while(p.hasMoreCommands())
    {
        p.advance();
        c.setSourceLine(p.getSourceLine());
        CommandType curr_command = p.commandType();
        switch(curr_command)
        {
            case CommandType::C_PUSH:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"PUSH"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                    std::cout<<"arg2 = "<<p.arg2()<<std::endl;
#endif
                    c.writePushPop(curr_command, p.arg1(), p.arg2());
                }
                break;
            case CommandType::C_POP:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"POP"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                    std::cout<<"arg2 = "<<p.arg2()<<std::endl;
#endif
                    c.writePushPop(curr_command, p.arg1(), p.arg2());
                }
                break;

            case CommandType::C_ARITHMETIC:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"ARITH"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
#endif
                    c.writeArithmetic(p.arg1());
                }
                break;
            case CommandType::C_LABEL:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"LABEL"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
#endif
                    c.writeLabel(p.arg1());
                }
                break;
            case CommandType::C_GOTO:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"GOTO"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
#endif
                    c.writeGoto(p.arg1());
                }
                break;
            case CommandType::C_IF_GOTO:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"IFGOTO"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
#endif
                    c.writeIfGoto(p.arg1());
                }
                break;
            case CommandType::C_FUNCTION:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"FUNC"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                    std::cout<<"arg2 = "<<p.arg2()<<std::endl;
#endif
                    c.writeFunction(p.arg1(), p.arg2());
                }
                break;

            case CommandType::C_CALL:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"CALL"<<std::endl;
                    std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                    std::cout<<"arg2 = "<<p.arg2()<<std::endl;
#endif
                    c.writeCall(p.arg1(), p.arg2());
                }
                break;

            case CommandType::C_RETURN:
                {
#ifdef DEBUG_MAIN
                    std::cout<<"RET"<<std::endl;
#endif
                    c.writeReturn();
                }
                break;

            default:
#ifdef DEBUG_MAIN
                std::cout<<"UNK"<<std::endl;
#endif
                break;
        }
    }
#ifdef DEBUG_MAIN
                std::cout<<"No More Source lines in "<<infile<<std::endl;
#endif
    // clean up
    p.close();
}


void getFileList(const std::string& infile, 
                    std::vector<std::string>& fileNames, 
                    std::string& outfile)
{
    struct stat s;

    if(stat(infile.c_str(),&s) == 0)
    {
        if(s.st_mode & S_IFDIR)
        {
#ifdef DEBUG_MAIN
            std::cout<<infile<<" is a directory with vm files: "<<std::endl;
#endif
            // input is a directory

            // generate output filename based on directory name
            // eg: /a/b/c -> c.asm
            auto trimmed = infile.substr(0, infile.find_last_not_of("//")+1);
            auto dir_name_end = trimmed.find_last_of("//") + 1;
            outfile = infile + "//" + trimmed.substr(dir_name_end) + ".asm";

            // get list of *.vm files in this dir
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
                        if(extn.compare("vm") == 0)
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
            outfile = removeFileExtn(infile) + ".asm";
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
    std::cout<<"Writing output to: "<<outfile<<std::endl;
#endif

}
