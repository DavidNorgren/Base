#pragma once

#include <cstdlib>
//#include <cstdint>
#include <functional>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <array>

#define repeat(X) for (uint __repeat_index = 0; __repeat_index < X; ++__repeat_index)

using std::string;
using std::wstring;
using std::runtime_error;
using std::function;
using std::cout;
using std::cin;
using std::endl;
template<typename T, typename R> using Map = std::map<T, R>;

using uint = unsigned int;
using uchar = unsigned char;
    
/* A container with a pointer to raw byte data, along with its size. */
struct Data
{
    char* ptr;
    uint size;
};