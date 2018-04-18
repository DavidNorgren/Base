#pragma once

#include <cstdlib>
#include <functional>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>

#define repeat(X) for (uint __repeat_index = 0; __repeat_index < X; ++__repeat_index)

using std::string;
using std::wstring;
using std::runtime_error;
using std::function;
using std::cout;
using std::endl;
using std::map;
template<typename T> using list = std::vector<T>;

using uint = unsigned int;
using uchar = unsigned char;
