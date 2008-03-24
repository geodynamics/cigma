#ifndef __TIMER_H__
#define __TIMER_H__

#include <iostream>
#include <cassert>
#include <ctime>

class Timer
{
public:
    int total;
    int current;
    time_t t_0, t_n;
    double elapsed_mins;
    double mins_per_num;
    double num_per_sec;
    double remaining_mins;
    double total_mins;
    double progress;

public:
    
    Timer()
    {
        t_0 = 0;
        total = 0;
        current = 0;
    }
    
    ~Timer()
    {
    }
    
public:

    void print_header(std::ostream &os, const char *firstcol)
    {
        assert(firstcol != 0);
        os << firstcol << " rate mins eta total progress\n";
    }

public:

    void start(int total)
    {
        time(&t_0);
        this->total = total;
    }

    void update(int current)
    {
        time(&t_n);
        this->current = current;
        elapsed_mins = (t_n - t_0) / 60.0;
        mins_per_num = elapsed_mins / current;
        num_per_sec = (1.0/60.0) / mins_per_num;
        remaining_mins = (total - current) * mins_per_num;
        total_mins = total * mins_per_num;
        progress = 100 * elapsed_mins / total_mins;
    }
};


std::ostream &operator<<(std::ostream &os, const Timer &T);


#endif
