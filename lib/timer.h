//---------------------------------------------------------------------------
#ifndef timerH
#define timerH
//---------------------------------------------------------------------------

#include <string>
#include <map>
#include <ctime>
#include "config.h"

enum  SHOWTIME_MODES {
    SHOWTIME_NONE = 0,
    SHOWTIME_FILE,
    SHOWTIME_SUMMARY,
    SHOWTIME_TOP5
};

class CLINTLIB TimerResultsIntf {
public:
    virtual ~TimerResultsIntf() { }

    virtual void AddResults(const std::string& str, std::clock_t clocks) = 0;
};

struct TimerResultsData {
    std::clock_t _clocks;
    long _numberOfResults;

    TimerResultsData()
        : _clocks(0)
        , _numberOfResults(0) {
    }

    double seconds() const {
        double ret = (double)((unsigned long)_clocks) / (double)CLOCKS_PER_SEC;
        return ret;
    }
};

class CLINTLIB TimerResults : public TimerResultsIntf {
public:
    TimerResults() {
    }

    void ShowResults(SHOWTIME_MODES mode) const;
    virtual void AddResults(const std::string& str, std::clock_t clocks);

private:
    std::map<std::string, struct TimerResultsData> _results;
};

class CLINTLIB Timer {
public:
    Timer(const std::string& str, unsigned int showtimeMode, TimerResultsIntf* timerResults = NULL);
    ~Timer();
    void Stop();

private:
    Timer& operator=(const Timer&); // disallow assignments

    const std::string _str;
    TimerResultsIntf* _timerResults;
    std::clock_t _start;
    const unsigned int _showtimeMode;
    bool _stopped;
};
//---------------------------------------------------------------------------
#endif // timerH
