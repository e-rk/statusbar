#include "statusbar.h"
#include <alloca.h>
#include <alsa/asoundlib.h>


size_t volume_update(char * output, size_t length)
{
    snd_mixer_t          * p_handle = NULL;
    snd_mixer_selem_id_t * p_sid    = NULL;
    snd_mixer_elem_t     * p_elem   = NULL;

    const char card[]       = "default";
    const char selem_name[] = "Master";

    long vol_min = 0;
    long vol_max = 0;

    long volume = 0;

    snd_mixer_open(&p_handle, 0);

    if (p_handle != NULL)
    {
        snd_mixer_attach(p_handle, card);
        snd_mixer_selem_register(p_handle, NULL, NULL);

        snd_mixer_selem_id_alloca(&p_sid);
        snd_mixer_selem_id_set_index(p_sid, 0);
        snd_mixer_selem_id_set_name(p_sid, selem_name);

        snd_mixer_load(p_handle);
        p_elem = snd_mixer_find_selem(p_handle, p_sid);

        if (p_elem != NULL)
        {
            snd_mixer_selem_get_playback_volume_range(p_elem, &vol_min, &vol_max);
            snd_mixer_selem_get_playback_volume(p_elem, 0, &volume);
            snprintf(output, length, "Vol:%ld%%", 100 * volume / vol_max);
        }

        snd_mixer_close(p_handle);
    }

    return strlen(output);
}
