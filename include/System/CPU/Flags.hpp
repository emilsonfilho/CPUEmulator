#ifndef FLAGS_HPP
#define FLAGS_HPP

#include <string>
#include <sstream>

using namespace std;

struct Flags
{
    bool C = false; // Carry
    bool Ov = false; // Overflow
    bool Z = false; // Zero
    bool S = false; // 
    
    string showFlags() const;
};


#endif