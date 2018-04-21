#pragma once

#include <vector>


namespace Base
{
    /* A list of items, basically a wrapped standard C++ vector with added stuff. */
    template<typename T> class List
    {
      public:
        List() {}
      
        // Methods

        void add(T value)
        {
            vec.push_back(value);
        }

        void add(List<T> list)
        {
            vec.insert(vec.end(), list.vec.begin(), list.vec.end());
        }

        int find(T value)
        {
            auto i = std::find(vec.begin(), vec.end(), value);
            if (i == vec.end())
                return -1;
            return std::distance(vec.begin(), i);
        }

        void remove(int index)
        {
            vec.erase(vec.begin() + index);
        }

        void remove(T value)
        {
            auto i = std::find(vec.begin(), vec.end(), value);
            if (i == vec.end())
                return;
            vec.erase(i);
        }

        void insert(int index, T value)
        {
            vec.insert(vec.begin() + index, value);
        }

        void insert(int index, List<T> list)
        {
            vec.insert(vec.begin() + index, list.vec.begin(), list.vec.end());
        }

        uint size()
        {
            return vec.size();
        }

        void sort(bool descending = true)
        {
          // TODO
        }

        // Get/Set via [] operator
        inline T  operator [] (int i) const { return vec[i]; }
        inline T &operator [] (int i)       { return vec[i]; }

        // Support for range-based for loops
        typename std::vector<T>::iterator begin() { return vec.begin(); }
        typename std::vector<T>::const_iterator begin() const { return vec.begin(); }
        typename std::vector<T>::const_iterator cbegin() const { return vec.begin(); }
        typename std::vector<T>::iterator end() { return vec.end(); }
        typename std::vector<T>::const_iterator end() const { return vec.end(); }
        typename std::vector<T>::const_iterator cend() const { return vec.end(); }
    
      protected:
        std::vector<T> vec;
    };
}