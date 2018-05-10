#pragma once


namespace Base
{
    /* A list of items, basically a wrapped standard C++ vector with added stuff. */
    template<typename T> class List
    {
      public:
        List() {}

        List(uint size)
        {
            vec.reserve(size);
        }
      
        List(std::vector<T> vec)
        {
            this->vec = vec;
        }

        // Methods

        void add(T value)
        {
            vec.push_back(value);
        }

        void addList(const List<T>& list)
        {
            vec.insert(vec.end(), list.vec.begin(), list.vec.end());
        }

        void addList(const std::vector<T>& vec)
        {
            this->vec.insert(this->vec.end(), vec.begin(), vec.end());
        }

        int find(const T& value) const
        {
            auto i = std::find(vec.begin(), vec.end(), value);
            if (i == vec.end())
                return -1;
            return std::distance(vec.begin(), i);
        }

        void removeAt(int index)
        {
            vec.erase(vec.begin() + index);
        }

        void remove(const T& value)
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

        uint size() const
        {
            return vec.size();
        }

        bool isEmpty() const
        {
            return size() == 0;
        }

        void clear()
        {
            vec.clear();
        }

        void reserve(uint size)
        {
            vec.reserve(size);
        }

        void resize(uint size)
        {
            vec.resize(size);
        }

        void sort(bool descending = true)
        {
            // TODO
        }

        inline List& operator = (const std::vector<T>& other)
        {
            vec = other;
            return *this;
        }

        // Get/Set via [] operator
        inline T  operator [] (int i) const { return vec[i]; }
        inline T& operator [] (int i)       { return vec[i]; }

        // Support for range-based for loops
        typename std::vector<T>::iterator begin()               { return vec.begin(); }
        typename std::vector<T>::const_iterator begin() const   { return vec.begin(); }
        typename std::vector<T>::iterator end()                 { return vec.end(); }
        typename std::vector<T>::const_iterator end() const     { return vec.end(); }
    
      protected:
        std::vector<T> vec;
    };
}