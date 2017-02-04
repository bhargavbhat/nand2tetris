#include<iostream>
#include<string>
#include<sstream>
#include<algorithm> 

std::string trim(const std::string& str)
{
    std::stringstream out;

    for(auto i : str)
    {
        if((i == ' ')||(i == '\t'))
            continue;
        else if(i == '/')
            break;
        else
            out<<i;
    }
    return out.str();
}

int main()
{
    std::cout<<"***"<<(trim("   M=M+1       // incr M     "))<<"***"<<std::endl;
    
    return 0;
}
