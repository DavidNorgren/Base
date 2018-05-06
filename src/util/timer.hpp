#pragma once

#include <chrono> // high_resolution_clock


namespace Base
{
    class Timer
    {
      public:
        Timer(const string& name);

        /* Timer functionality */
        void stop();
        void print() const;
        void stopAndPrint();

        /* Get the time elapsed in milliseconds. */
        double getDuration() const { return time; }

      private:
        std::chrono::_V2::high_resolution_clock::time_point startTime, endTime;
        double time;
        string name;
    };
}