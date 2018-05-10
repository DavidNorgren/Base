#pragma once

#include "util/mathfunc.hpp"


namespace Base
{
    template<typename T> class limit
    {
      public:
        void addMin(T value)
        {
            if (!isSet)
                limit = value;
            else
                limit = min(limit, value);
            isSet = true;
        }

        void addMax(T value)
        {
            if (!isSet)
                limit = value;
            else
                limit = max(limit, value);
            isSet = true;
        }

        T getLimit() const { return limit; }
        bool getIsSet() const { return isSet; }

      private:
        T limit;
        bool isSet = false;
    };
}