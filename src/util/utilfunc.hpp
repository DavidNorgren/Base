#pragma once


namespace Base
{
    template<typename T> inline int vectorFind(list<T>& vec, T value)
    {
        auto i = std::find(vec.begin(), vec.end(), value);
        if (i == vec.end())
            return -1;
        return std::distance(vec.begin(), i);
    }

    template<typename T> inline void vectorErase(list<T>& vec, int index)
    {
        vec.erase(vec.begin() + index);
    }

    template<typename T> inline void vectorErase(list<T>& vec, T value)
    {
        auto i = std::find(vec.begin(), vec.end(), value);
        if (i == vec.end())
            return;
        vec.erase(i);
    }

    template<typename T> inline void vectorInsert(list<T>& vec, int index, T value)
    {
        vec.insert(vec.begin() + index, value);
    }
}