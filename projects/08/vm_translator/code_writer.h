#ifndef CODE_WRITER_H
#define CODE_WRITER_H
#include<string>
#include<fstream>
#include"common.h"

class CodeWriter
{
    public:
        CodeWriter(const std::string& outputFileName);
        void writeArithmetic(const std::string& line);
        void writePushPop(CommandType command, 
                            const std::string& segment,
                            uint16_t index);
        void close();
        void setSourceLine(const std::string& source);

        void setFilename(const std::string& fileName);
        void writeInit(void);
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIfGoto(const std::string& label);
        void writeCall(const std::string& funcName, 
                        uint16_t numArgs);
        void writeFunction(const std::string& funcName,
                        uint16_t numLocals);
        void writeReturn(void);

    private:
        std::string getBaseAddr(const std::string& segment);
        std::string saveSegment(const std::string& segment);
        std::string restoreSegment(const std::string& segment, 
                                    uint16_t offset);
        std::string genLabel(const std::string& input);
        std::ofstream _fOut;
        std::string _sourceLine;
        std::string _fileName;
        std::string _currFunction;
        unsigned int _jmpNumber;

};

#endif //CODE_WRITER_H
