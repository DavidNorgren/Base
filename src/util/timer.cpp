#include "common.hpp"
#include "util/timer.hpp"
#include "util/stringfunc.hpp"

using namespace std::chrono;

Base::Timer::Timer(string name)
{
    this->name = name;
    startTime = high_resolution_clock::now();
}

void Base::Timer::stop()
{
    endTime = high_resolution_clock::now();
}

void Base::Timer::stopAndPrint()
{
    stop();
    duration<double> timeSpan = duration_cast<duration<double>>(endTime - startTime);
    time = timeSpan.count();
    cout << name << " timer: " << std::fixed << toStringPrec(time, 3) << "ms" << endl;
}

double Base::Timer::getDuration()
{
    return time;
}