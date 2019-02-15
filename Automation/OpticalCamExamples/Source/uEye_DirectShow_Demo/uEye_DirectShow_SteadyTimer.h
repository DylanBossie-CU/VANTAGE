#if !defined(DS_STEADYTIMER_H_)
#define DS_STEADYTIMER_H_

#include <windows.h>

template<unsigned int N>
class SteadyTimer
{
public:
    SteadyTimer()
    {
        Restart();
    }

    ~SteadyTimer() {}

    LONG64 GetElapsedTime()
    {
        LARGE_INTEGER end;
        QueryPerformanceCounter(&end);

        LARGE_INTEGER elapsed;
        elapsed.QuadPart = end.QuadPart - m_start.QuadPart;

        elapsed.QuadPart *= N;
        elapsed.QuadPart /= m_frequency.QuadPart;

        return elapsed.QuadPart;
    }

    void Restart()
    {
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_start);
    }

private:
    LARGE_INTEGER m_start;
    LARGE_INTEGER m_frequency;

};

typedef SteadyTimer<1000000000> SteadyTimerNS;
typedef SteadyTimer<1000000> SteadyTimerUS;
typedef SteadyTimer<1000> SteadyTimerMS;
typedef SteadyTimer<1> SteadyTimerS;

#endif /* DS_STEADYTIMER_H_ */