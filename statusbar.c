#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include <mpd/client.h>

#define ARRAY_SIZE(array) ( sizeof(array) / sizeof(array[0]) )

#define STATUS_LENGTH 128   /* Maximum status bar length.  */
#define SEPARATOR     " | " /* Status separator string.    */


typedef size_t (* handler_t)(char * output, size_t length);

static Display * dpy;


static void print_status(const char * status)
{
    XStoreName(dpy, DefaultRootWindow(dpy), status);
    XSync(dpy, false);
}


static size_t update_time(char * output, size_t length)
{
    struct tm * timeinfo;
    char      * status;
    time_t      rawtime;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    if (timeinfo != NULL)
    {
        return strftime(output, length, "%a %d %b %H:%M", timeinfo);
    }
    else
    {
        return 0;
    }
}


static size_t update_mpd(char * output, size_t length)
{
    struct mpd_song       * song       = NULL;
    struct mpd_connection * connection = NULL;
    struct mpd_status     * status     = NULL;

    const char * play_status = NULL;
    
    int time_elapsed = 0;
    int time_total   = 0;

    connection = mpd_connection_new("localhost", 0, 30000);

    if ((connection != NULL) && mpd_connection_get_error(connection) == 0)
    {
        mpd_command_list_begin(connection, true);
        mpd_send_status(connection);
        mpd_send_current_song(connection);
        mpd_command_list_end(connection);

        status = mpd_recv_status(connection);

        if (status != NULL)
        {
            mpd_response_next(connection);
            song = mpd_recv_song(connection);

            if (song != NULL)
            {

                time_elapsed = mpd_status_get_elapsed_time(status);
                time_total   = mpd_status_get_total_time(status);

                switch (mpd_status_get_state(status))
                {
                    case MPD_STATE_PLAY:
                        play_status = "Playing";
                        break;

                    case MPD_STATE_PAUSE:
                        play_status = "Paused";
                        break;

                    case MPD_STATE_STOP:
                        play_status = "Stopped";
                        break;
                }

                snprintf(output, length, "[%s] %.2d:%.2d/%.2d:%.2d %s - %s",
                         play_status,
                         time_elapsed / 60, time_elapsed % 60,
                         time_total   / 60, time_total   % 60,
                         mpd_song_get_tag(song, MPD_TAG_ARTIST, 0),
                         mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
            }
        }

        mpd_response_finish(connection);
        mpd_connection_free(connection);
    }

    return strlen(output);
}


static void update(handler_t handlers[], size_t status_count)
{
    static const size_t separator_length = strlen(SEPARATOR);

    char   status[STATUS_LENGTH];
    char * offset = status;

    size_t status_length = 0;
    size_t temp_length   = 0;

    memset(status, 0, sizeof(status));
    
    for (int i = 0; i < status_count; ++i)
    {
        temp_length = handlers[i](offset, STATUS_LENGTH - status_length);

        offset        += temp_length;
        status_length += temp_length;
        
        if (((STATUS_LENGTH - status_length) <= separator_length)
        ||  ((status_count - 1) == i))
        {
            /* Too little space to print anything or nothing to print at all. */
            break;
        }
        else if (temp_length > 0)
        {
            /* Print separator. */
            snprintf(offset, STATUS_LENGTH - status_length, "%s", SEPARATOR);
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
        update_mpd, update_time,
    };

    dpy = XOpenDisplay(NULL);

    if (dpy == NULL)
    {
        fprintf(stderr, "unable to open display.\n");
        return 1;
    }

    while (true)
    {
        update(handlers, ARRAY_SIZE(handlers));
        sleep(1);
    }

    XCloseDisplay(dpy);

    return 0;
}
