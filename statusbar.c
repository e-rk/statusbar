#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include "statusbar.h"


static const int    m_max_status_length = 128;    /* Maximum status bar length.  */
static const char   m_separator[]       = " | ";  /* Status separator string.    */
static Display    * mp_dpy              = NULL;


static void print_status(const char * status)
{
    XStoreName(mp_dpy, DefaultRootWindow(mp_dpy), status);
    XSync(mp_dpy, false);
}


static void update(handler_t handlers[], size_t status_count)
{
    static const size_t separator_length = ARRAY_SIZE(m_separator) - 1;

    char   status[m_max_status_length];
    char * offset = status;

    size_t status_length = 0;
    size_t temp_length   = 0;

    memset(status, 0, sizeof(status));

    for (int i = 0; i < status_count; ++i)
    {
        temp_length = handlers[i](offset, m_max_status_length - status_length);

        offset        += temp_length;
        status_length += temp_length;

        if (((m_max_status_length - status_length) <= separator_length)
        ||  ((status_count - 1) == i))
        {
            /* Too little space to print anything or nothing to print at all. */
            break;
        }
        else if (temp_length > 0)
        {
            /* Print separator. */
            snprintf(offset, m_max_status_length - status_length, "%s", m_separator);
            offset        += separator_length;
            status_length += separator_length;
        }
    }

    print_status(status);
}


int main(int argc, char *argv[])
{
    handler_t handlers[] =
    {
        battery_update,
        volume_update,
        mpd_update,
        time_update,
    };

    mp_dpy = XOpenDisplay(NULL);

    if (mp_dpy == NULL)
    {
        fprintf(stderr, "unable to open display.\n");
        return 1;
    }

    while (true)
    {
        update(handlers, ARRAY_SIZE(handlers));
        sleep(1);
    }

    /* Should never reach this line. */
    XCloseDisplay(mp_dpy);

    return 0;
}
