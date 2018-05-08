#include "common.hpp"
#include "util/timer.hpp"
#include "util/stringfunc.hpp"

using namespace std::chrono;


EXPORT Base::Timer::Timer(const string& name)
{
    this->name = name;
    startTime = high_resolution_clock::now();
}

EXPORT void Base::Timer::stop()
{
    endTime = high_resolution_clock::now();
    duration<double> timeSpan = duration_cast<duration<double>>(endTime - startTime);
    time = timeSpan.count() * 1000.0;
}

EXPORT void Base::Timer::print() const
{
    cout << name << " timer: " << toStringPrec(time, 3) << "ms" << endl;
}

EXPORT void Base::Timer::stopAndPrint()
{
    stop();
    print();
}