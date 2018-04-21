#pragma once

#include <chrono>

namespace Base
{
    class Timer
    {
      public:
        Timer(string name);
        void stop();
        void print();
        void stopAndPrint();
        double getDuration();

      private:
        std::chrono::_V2::high_resolution_clock::time_point startTime, endTime;
        double time;
        string name;
    };
}