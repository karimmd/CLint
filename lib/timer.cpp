#include "timer.h"
#include <algorithm>
#include <iostream>
#include <vector>
/*
    TODO:
    - rename "file" to "single"
    - synchronise map access in multithreaded mode or disable timing
    - add unit tests
        - for --showtime (needs input file)
        - for Timer* classes
*/

namespace {
    typedef std::pair<std::string, struct TimerResultsData> dataElementType;
    bool more_second_sec(const dataElementType& lhs, const dataElementType& rhs)
    {
        return lhs.second.seconds() > rhs.second.seconds();
    }
}

void TimerResults::ShowResults(SHOWTIME_MODES mode) const
{
    if (mode == SHOWTIME_NONE)
        return;

    std::cout << std::endl;
    TimerResultsData overallData;

    std::vector<dataElementType> data(_results.begin(), _results.end());
    std::sort(data.begin(), data.end(), more_second_sec);

    size_t ordinal = 1; // maybe it would be nice to have an ordinal in output later!
    for (std::vector<dataElementType>::const_iterator iter=data.begin() ; iter!=data.end(); ++iter) {
        const double sec = iter->second.seconds();
        const double secAverage = sec / (double)(iter->second._numberOfResults);
        overallData._clocks += iter->second._clocks;
        if ((mode != SHOWTIME_TOP5) || (ordinal<=5)) {
            std::cout << iter->first << ": " << sec << "s (avg. " << secAverage << "s - " << iter->second._numberOfResults  << " result(s))" << std::endl;
        }
        ++ordinal;
    }

    const double secOverall = overallData.seconds();
    std::cout << "Overall time: " << secOverall << "s" << std::endl;
}

void TimerResults::AddResults(const std::string& str, std::clock_t clocks)
{
    _results[str]._clocks += clocks;
    _results[str]._numberOfResults++;
}

Timer::Timer(const std::string& str, unsigned int showtimeMode, TimerResultsIntf* timerResults)
    : _str(str)
    , _timerResults(timerResults)
    , _start(0)
    , _showtimeMode(showtimeMode)
    , _stopped(false)
{
    if (showtimeMode != SHOWTIME_NONE)
        _start = std::clock();
}

Timer::~Timer()
{
    Stop();
}

void Timer::Stop()
{
    if ((_showtimeMode != SHOWTIME_NONE) && !_stopped) {
        const std::clock_t end = std::clock();
        const std::clock_t diff = end - _start;

        if (_showtimeMode == SHOWTIME_FILE) {
            double sec = (double)diff / CLOCKS_PER_SEC;
            std::cout << _str << ": " << sec << "s" << std::endl;
        } else {
            if (_timerResults)
                _timerResults->AddResults(_str, diff);
        }
    }

    _stopped = true;
}
