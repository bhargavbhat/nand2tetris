#include"code_writer.h"
#include<sstream>

CodeWriter::CodeWriter(const std::string& outputFileName)
{
    _fOut.open(outputFileName);
    setFilename(outputFileName);
    _jmpNumber = 0;
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
    else if((line.compare("eq") == 0))
    {
        auto jmp = "E" + std::to_string(_jmpNumber++);
    
        _fOut<<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M"<<std::endl
            <<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M-D"<<std::endl
            <<"@"<<jmp<<std::endl
            <<"D;JEQ"<<std::endl
            <<"D=1"<<std::endl
            <<"("<<jmp<<")"<<std::endl
            <<"D=D-1"<<std::endl
            <<"@SP"<<std::endl
            <<"A=M"<<std::endl
            <<"M=D"<<std::endl
            <<"@SP"<<std::endl
            <<"M=M+1"<<std::endl;
    }
    else if((line.compare("lt") == 0) ||
            (line.compare("gt") == 0))
    {
        auto is_lt = (line.compare("lt") == 0);
        auto instr = (is_lt ? "JLT" : "JGT");
        auto condPass = (is_lt ? "JL.T" : "JG.T") + std::to_string(_jmpNumber);
        auto condFail = (is_lt ? "JL.F" : "JG.F") + std::to_string(_jmpNumber);
        ++_jmpNumber;

        _fOut <<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M"<<std::endl
            <<"@SP"<<std::endl
            <<"AM=M-1"<<std::endl
            <<"D=M-D"<<std::endl
            <<"@"<<condPass<<std::endl
            <<"D;"<<instr<<std::endl
            <<"D=0"<<std::endl
            <<"@"<<condFail<<std::endl
            <<"0;JMP"<<std::endl
            <<"("<<condPass<<")"<<std::endl
            <<"D=-1"<<std::endl
            <<"("<<condFail<<")"<<std::endl
            <<"@SP"<<std::endl
            <<"A=M"<<std::endl
            <<"M=D"<<std::endl
            <<"@SP"<<std::endl
            <<"M=M+1"<<std::endl;
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
        auto strIndex = std::to_string(index);
        auto baseAddr = getBaseAddr(segment);

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
        auto baseAddr = getBaseAddr(segment);
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

void CodeWriter::setFilename(const std::string& fileName)
{
    _fileName = getFileName(fileName);
}

void CodeWriter::writeInit(void)
{
    _fOut <<"// Init Code"<<std::endl
        // set SP=256
        <<"@256"<<std::endl
        <<"D=A"<<std::endl
        <<"@SP"<<std::endl
        <<"M=D"<<std::endl

        // TODO: Call Sys.Init

        // infinite loop after boot code
        <<"(BOOT)"<<std::endl
        <<"@BOOT"<<std::endl
        <<"0;JMP"<<std::endl
        <<std::endl;
}

void CodeWriter::writeLabel(const std::string& label)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif

    _fOut<<"("
        <<genLabel(label)
        <<")"<<std::endl;
}

void CodeWriter::writeGoto(const std::string& label)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    _fOut<<"@"
        <<genLabel(label)<<std::endl
        <<"0;JMP"<<std::endl;
}

void CodeWriter::writeIfGoto(const std::string& label)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    _fOut<<"@SP"<<std::endl
        <<"AM=M-1"<<std::endl
        <<"D=M"<<std::endl
        <<"@"<<_fileName<<"$"<<label<<std::endl
        <<"D;JNE"<<std::endl;
}

void CodeWriter::writeCall(const std::string& funcName,
                            uint16_t numArgs)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    auto ret = "RET." + funcName + "." + std::to_string(_jmpNumber);
    ++_jmpNumber;

    // Push ret address to stack
    _fOut<<"@"<<ret<<std::endl
     <<"D=A"<<std::endl
     <<"@SP"<<std::endl
     <<"A=M"<< std::endl
     <<"M=D"<<std::endl
     <<"@SP"<< std::endl
     <<"M=M+1"<< std::endl

     // Save caller's segments
     <<saveSegment("LCL")
     <<saveSegment("ARG")
     <<saveSegment("THIS")
     <<saveSegment("THAT")
     
     // Setup ARG for callee
     // ARG = SP - numArgs - 5
     <<"@SP"<<std::endl
     <<"D=M"<<std::endl
     <<"@"<<numArgs<<std::endl
     <<"D=D-A"<<std::endl
     <<"@5"<<std::endl
     <<"D=D-A"<<std::endl
     <<"@ARG"<<std::endl
     <<"M=D"<<std::endl

     // Setup LCL for callee
     <<"@SP"<<std::endl
     <<"D=M"<<std::endl
     <<"@LCL"<<std::endl
     <<"M=D"<<std::endl

    //jump to function code
    <<"@"<<funcName<<std::endl
    <<"0;JMP"<<std::endl

    //add return address label
    <<"("<<ret<<")"<< std::endl; 
}

void CodeWriter::writeFunction(const std::string& funcName,
                            uint16_t numLocals)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    // store function name for label generation
    _currFunction = funcName;
    _fOut<<"("<<funcName<<")"<<std::endl;
    for (int i = 0; i < numLocals; ++i)
    {
        _fOut<<"@0"<<std::endl
        <<"D=A"<<std::endl
        <<"@SP"<<std::endl
        <<"A=M"<<std::endl
        <<"M=D"<<std::endl
        <<"@SP"<<std::endl
        <<"M=M+1"<<std::endl;
    }
}

void CodeWriter::writeReturn(void)
{
#ifdef DEBUG_CODE_WRITER
    _fOut<<"// "<<_sourceLine<<std::endl;
#endif
    // Save result
    _fOut<<"@SP"<<std::endl
    <<"A=M-1"<<std::endl
    <<"D=M"<<std::endl
    <<"@R13"<<std::endl
    <<"M=D"<<std::endl

    <<"@LCL"<<std::endl
    <<"D=M"<<std::endl
    <<"@R14"<<std::endl
    <<"M=D"<<std::endl

    // Get return value
    <<"@R14"<<std::endl
    <<"D=M"<<std::endl
    <<"@5"<<std::endl
    <<"A=D-A"<<std::endl
    <<"D=M"<<std::endl
    <<"@R15"<<std::endl
    <<"M=D"<<std::endl

    // Write return value to ARG[0]
    <<"@R13"<<std::endl
    <<"D=M"<<std::endl
    <<"@ARG"<<std::endl
    <<"A=M"<<std::endl
    <<"M=D"<<std::endl

    // Set SP to return value + 1
    <<"@ARG"<<std::endl
    <<"D=M+1"<<std::endl
    <<"@SP"<<std::endl
    <<"M=D"<<std::endl

    // Restore Caller's segments
    <<restoreSegment("THAT",1)
    <<restoreSegment("THIS",2)
    <<restoreSegment("ARG",3)
    <<restoreSegment("LCL",4)

    // Jump to return address
    <<"@R15"<<std::endl
    <<"A=M"<<std::endl
    <<"0;JMP"<<std::endl;
}

std::string CodeWriter::saveSegment(const std::string& segment)
{
    std::stringstream ss;
    ss<<"@"<<segment<<std::endl
        <<"D=M"<<std::endl
        <<"@SP"<<std::endl
        <<"A=M"<<std::endl
        <<"M=D"<<std::endl
        <<"@SP"<<std::endl
        <<"M=M+1"<<std::endl;
    return ss.str();
}

std::string CodeWriter::restoreSegment(const std::string& segment,
                                        uint16_t offset)
{
    std::stringstream ss;
    ss<<"@R14"<<std::endl
        <<"D=M"<<std::endl
        <<"@"<<offset<<std::endl
        <<"A=D-A"<<std::endl
        <<"D=M"<<std::endl
        <<"@"<<segment<<std::endl
        <<"M=D"<<std::endl;
    return ss.str();
}

std::string CodeWriter::genLabel(const std::string& input)
{
    return (_currFunction.empty() ? _fileName : _currFunction) + "$" + input;
}
