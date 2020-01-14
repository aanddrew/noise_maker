#include <alsa/asoundlib.h>
#include "sound_engine.h"

int init_sound_engine() {
    stream = SND_PCM_STREAM_PLAYBACK;
    rate = 44100;
    periods = 2;
    periodsize = 8192;

    //init vars and allocate hwparams
    pcm_name = strdup("hw:1");
    snd_pcm_hw_params_alloca(&hwparams);

    int err;
    //open the pcm device
    fprintf(stderr, "Opening PCM device: %s\n", pcm_name);
    if ((err = snd_pcm_open(&pcm_handle, pcm_name, stream, 0)) < 0) {
        fprintf(stderr, "Error opening PCM device %s: %s\n", pcm_name, snd_strerror(err));
        return 0;
    }

    if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
        fprintf(stderr, "Can not configure this PCM device.\n");
        return 0;
    }

    //requesting these params from sound card
    if ((err = snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "Error setting access: %s\n", snd_strerror(err));
        return 0;
    }
    if ((err = snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf(stderr, "Error setting format: %s\n", snd_strerror(err));
        return 0;
    }
    exact_rate = rate;
    if ((err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0)) < 0) {
        fprintf(stderr, "Error setting rate: %s\n", snd_strerror(err));
        return 0;
    }
    if (rate != exact_rate) {
        fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n ==> Using %d Hz instead.\n", rate, exact_rate);
    }
    if ((err = snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 2)) < 0) {
        fprintf(stderr, "Error setting channels: %s\n", snd_strerror(err));
        return(-1);
    }
    if ((err = snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0)) < 0) {
        fprintf(stderr, "Error setting periods: %s\n", snd_strerror(err));
        return(-1);
    }

    #define num_bytes_in_frame 4
    //this expects buffer size in frames, not bytes
    frames_per_buffer = (periodsize * periods) / num_bytes_in_frame;
    if ((err = snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, frames_per_buffer)) < 0) {
        fprintf(stderr, "Error setting buffersize: %s\n", snd_strerror(err));
        return(-1);
    }

    /* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    if ((err = snd_pcm_hw_params(pcm_handle, hwparams)) < 0) {
      fprintf(stderr, "Error setting HW params: %s\n", snd_strerror(err));
      return(-1);
    }

    return 1;
}
