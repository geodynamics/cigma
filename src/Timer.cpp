#include "Timer.h"

std::ostream &operator<<(std::ostream &os, const Timer &T)
{
    os << T.current << " "
       << T.num_per_sec << " "
       << T.elapsed_mins << " "
       << T.remaining_mins << " "
       << T.total_mins << " "
       << T.progress << "%"
       << "          "
          "          "
          "\r"
       << std::flush;
}
