#include "statusbar.h"
#include <mpd/client.h>

static struct mpd_connection * mp_connection = NULL;


static void mpd_init()
{
    mp_connection = mpd_connection_new("localhost", 0, 30000);
}


static void mpd_deinit()
{
    mpd_connection_free(mp_connection);
    mp_connection = NULL;
}


size_t mpd_update(char * output, size_t length)
{
    struct mpd_song       * p_song   = NULL;
    struct mpd_status     * p_status = NULL;

    const char * p_play_status = NULL;

    int time_elapsed = 0;
    int time_total   = 0;

    if (mp_connection == NULL)
    {
        mpd_init();
    }

    if (mp_connection != NULL)
    {
        if (mpd_connection_get_error(mp_connection) != MPD_ERROR_SUCCESS)
        {
            mpd_deinit();
        }
        else
        {
            mpd_command_list_begin(mp_connection, true);
            mpd_send_status(mp_connection);
            mpd_send_current_song(mp_connection);
            mpd_command_list_end(mp_connection);

            p_status = mpd_recv_status(mp_connection);

            if (p_status != NULL)
            {
                mpd_response_next(mp_connection);
                p_song = mpd_recv_song(mp_connection);

                if (p_song != NULL)
                {
                    time_elapsed = mpd_status_get_elapsed_time(p_status);
                    time_total   = mpd_status_get_total_time(p_status);

                    switch (mpd_status_get_state(p_status))
                    {
                        case MPD_STATE_PLAY:
                            p_play_status = "\u25b6";
                            break;

                        case MPD_STATE_PAUSE:
                            p_play_status = "\u23f8";
                            break;

                        case MPD_STATE_STOP:
                            p_play_status = "\u25a0";
                            break;

                        case MPD_STATE_UNKNOWN:
                            p_play_status = "?";
                            break;
                    }

                    snprintf(output, length, "%s %.2d:%.2d/%.2d:%.2d %s - %s",
                             p_play_status,
                             time_elapsed / 60, time_elapsed % 60,
                             time_total   / 60, time_total   % 60,
                             mpd_song_get_tag(p_song, MPD_TAG_ARTIST, 0),
                             mpd_song_get_tag(p_song, MPD_TAG_TITLE, 0));

                    mpd_song_free(p_song);
                }

                mpd_status_free(p_status);
            }

            mpd_response_finish(mp_connection);
        }
    }

    return strlen(output);
}
