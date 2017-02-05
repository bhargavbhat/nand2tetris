#include "code_writer.h"

CodeWriter::CodeWriter(const std::string& outputFileName)
{
    _fOut.open(outputFileName);
    _fileName = getFileName(outputFileName);
}

void CodeWriter::writeArithmetic(const std::string& line)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    if(line.compare("add") == 0)
    {
        _fOut<<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M"<<std::endl
            <<"M=0"<<std::endl
            <<"A=A-1"<<std::endl
            <<"M=M+D"<<std::endl;
    }
    else if(line.compare("sub") == 0)
    {
        _fOut<<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M"<<std::endl
            <<"M=0"<<std::endl
            <<"A=A-1"<<std::endl
            <<"M=M-D"<<std::endl;
    }
    else if(line.compare("neg") == 0)
    {
        _fOut<<"@SP"<<std::endl
            <<"A=M-1"<<std::endl
            <<"M=-M"<<std::endl;
    }
    else if(line.compare("and") == 0)
    {
        _fOut<<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M"<<std::endl
            <<"M=0"<<std::endl
            <<"A=A-1"<<std::endl
            <<"M=M&D"<<std::endl;
    }
    else if(line.compare("or") == 0)
    {
        _fOut<<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M"<<std::endl
            <<"M=0"<<std::endl
            <<"A=A-1"<<std::endl
            <<"M=M|D"<<std::endl;
    }
    else if(line.compare("not") == 0)
    {
        _fOut<<"@SP"<<std::endl
            <<"A=M-1"<<std::endl
            <<"M=!M"<<std::endl;
    }
    else if((line.compare("eq") == 0) ||
            (line.compare("lt") == 0) ||
            (line.compare("gt") == 0))
    {
    }
    else
    {
        _fOut<<"ERROR"<<std::endl;
    }
}

void CodeWriter::writePushPop(CommandType command, 
        const std::string& segment,
        uint16_t index)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    if(CommandType::C_PUSH == command)
    {
        std::string strIndex = std::to_string(index);
        std::string baseAddr = getBaseAddr(segment);

        if (segment.compare("constant") == 0)
        {
            _fOut<<"@"<<std::to_string(index)<<std::endl
                <<"D=A"<<std::endl
                <<"@SP"<<std::endl
                <<"A=M"<<std::endl
                <<"M=D"<<std::endl
                <<"@SP"<<std::endl
                <<"M=M+1"<<std::endl;
        }
        else if (segment.compare("static") == 0 || 
                    segment.compare("temp") == 0 || 
                    segment.compare("pointer") == 0)
        {
            std::string final_addr;
            if(segment.compare("temp") == 0)
            {
                // construct @R(5+input_index)
                final_addr = baseAddr + std::to_string(index+5);
            }
            else if(segment.compare("pointer") == 0)
            {
                // construct @R(3+input_index)
                final_addr = baseAddr + std::to_string(index+3);
            }
            else
            {
                // construct name as <filename><addr> eg: basicTest3
                final_addr = baseAddr + std::to_string(index);
            }

            _fOut<<"@"<<final_addr<<std::endl
                <<"D=M"<<std::endl
                <<"@SP"<<std::endl
                <<"A=M"<<std::endl
                <<"M=D"<<std::endl
                <<"@SP"<<std::endl
                <<"M=M+1"<<std::endl;
        }
        else
        {
            _fOut<<"@"<<baseAddr<<std::endl
                <<"D=M"<<std::endl
                <<"@"<<std::to_string(index)<<std::endl
                <<"A=D+A"<<std::endl
                <<"D=M"<<std::endl
                <<"@SP"<<std::endl
                <<"A=M"<<std::endl
                <<"M=D"<<std::endl
                <<"@SP"<<std::endl
                <<"M=M+1"<<std::endl;

        } 
    }
    else if(CommandType::C_POP == command)
    {
        std::string baseAddr = getBaseAddr(segment);
        if (segment.compare("static") == 0 ||
                segment.compare("temp") == 0 ||
                segment.compare("pointer") == 0)
        {
            std::string final_addr;
            if(segment.compare("temp") == 0)
            {
                // construct @R(5+input_index)
                final_addr = baseAddr + std::to_string(index+5);
            }
            else if(segment.compare("pointer") == 0)
            {
                // construct @R(3+input_index)
                final_addr = baseAddr + std::to_string(index+3);
            }
            else
            {
                // construct name as <filename><addr> eg: basicTest3
                final_addr = baseAddr + std::to_string(index);
            }

            _fOut<<"@SP"<<std::endl
                <<"AM=M-1"<<std::endl
                <<"D=M"<<std::endl
                <<"@"<<final_addr<<std::endl
                <<"M=D"<<std::endl;
        }
        else
        {
            // general purpose R15 used as temp for load/store
            _fOut<<"@"<<baseAddr<<std::endl
                <<"D=M"<<std::endl
                <<"@"<<std::to_string(index)<<std::endl
                <<"D=D+A"<<std::endl
                <<"@R15"<<std::endl
                <<"M=D"<<std::endl
                <<"@SP"<<std::endl
                <<"AM=M-1"<<std::endl
                <<"D=M"<<std::endl
                <<"@R15"<<std::endl
                <<"A=M"<<std::endl
                <<"M=D"<<std::endl;
        }
    }
    else
    {
        _fOut<<"ERROR"<<std::endl;
    }
}

void CodeWriter::close()
{
    _fOut.close();
}
void CodeWriter::setSourceLine(const std::string& source)
{
    _sourceLine = source;
}

std::string CodeWriter::getBaseAddr(const std::string& segment)
{
    if (segment.compare("local") == 0)
        return "LCL";
    else if(segment.compare("argument") == 0)
        return "ARG";
    else if(segment.compare("this") == 0)
        return "THIS";
    else if(segment.compare("that") == 0)
        return "THAT";
    else if((segment.compare("temp") == 0)
            || (segment.compare("pointer") == 0))
        return "R";
    else if(segment.compare("static") == 0)
        return _fileName;
    else
        return "ERROR";
}
