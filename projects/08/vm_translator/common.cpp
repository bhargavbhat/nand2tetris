#include "common.h"

std::string getFileName(const std::string& fileName)
{
    std::string retVal = fileName;

    // Remove directory if present.
    auto last_slash_idx = retVal.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        retVal.erase(0, last_slash_idx + 1);
    }

    // Remove extension if present.
    auto period_idx = retVal.rfind('.');
    if (std::string::npos != period_idx)
    {
        retVal.erase(period_idx);
    }

    return retVal;
}

std::string removeFileExtn(const std::string& fileName)
{
    auto lastindex = fileName.find_last_of("."); 
    return fileName.substr(0, lastindex); 
}

