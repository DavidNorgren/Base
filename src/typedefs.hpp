#pragma once

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
using FileData = std::vector<char>;