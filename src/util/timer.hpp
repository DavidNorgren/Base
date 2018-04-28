#pragma once

#include <chrono>


namespace Base
{
    class Timer
    {
      public:
        Timer(const string& name);
        void stop();
        void print() const;
        void stopAndPrint();
        double getDuration() const;

      private:
        std::chrono::_V2::high_resolution_clock::time_point startTime, endTime;
        double time;
        string name;
    };
}