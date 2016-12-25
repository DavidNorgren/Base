#pragma once
#include <vector>


namespace Base
{

    template<typename T> inline int vectorFind(std::vector<T>& vec, T value)
    {
        auto i = std::find(vec.begin(), vec.end(), value);
        if (i == vec.end()) {
            return -1;
        }
        return std::distance(vec.begin(), i);
    }

    template<typename T> inline void vectorErase(std::vector<T>& vec, int index)
    {
        vec.erase(vec.begin() + index);
    }

    template<typename T> inline void vectorInsert(std::vector<T>& vec, int index, T value)
    {
        vec.insert(vec.begin() + index, value);
    }

}