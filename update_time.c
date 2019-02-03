#include "statusbar.h"
#include <time.h>

size_t time_update(char * output, size_t length)
{
    struct tm * p_timeinfo;
    time_t      rawtime;

    time(&rawtime);
    p_timeinfo = localtime(&rawtime);

    if (p_timeinfo != NULL)
    {
        return strftime(output, length, "%a %d %b %H:%M", p_timeinfo);
    }

    return 0;
}
