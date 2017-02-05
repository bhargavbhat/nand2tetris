#ifndef CODE_WRITER_H
#define CODE_WRITER_H
#include<string>
#include"common.h"
class CodeWriter
{
    private:

    public:
        CodeWriter(const std::string& outputFileName);
        void writeArithmetic(const std::string);
        void writePushPop(CommandType command, 
                            std::string segment,
                            uint16_t index);
        void close();
};

#endif //CODE_WRITER_H
