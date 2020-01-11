#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H 

#include <alsa/asoundlib.h>

snd_pcm_t* pcm_handle;
snd_pcm_stream_t stream;
snd_pcm_hw_params_t* hwparams;

//variables for parameters 
unsigned int rate;
unsigned int exact_rate;
int periods;
snd_pcm_uframes_t periodsize;
int frames_per_buffer;

char* pcm_name;
 
int init_sound_engine();

#endif
