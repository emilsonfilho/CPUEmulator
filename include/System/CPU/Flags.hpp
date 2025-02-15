#ifndef FLAGS_HPP
#define FLAGS_HPP

#include <string>
#include <sstream>
#include <cstdint>

using namespace std;

struct Flags
{
    bool C = false; // Carry
    bool Ov = false; // Overflow
    bool Z = false; // Zero
    bool S = false; // 
    
    string showFlags() const;

    void setZero(uint32_t result);

    void setSign(uint32_t result);
};


#endif