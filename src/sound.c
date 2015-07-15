/*
 * sound.c
 *
 *  Created on: 11-Jul-2015
 *      Author: angel
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <alsa/asoundlib.h>	//alsa sound

#include "wavfile.h"
#include "sound.h"



//global vars
FILE *sound_raw;		//write for sound file
snd_pcm_t *handle;		//pcm handle

int play (char *argv)
{
  FILE *fp;
  snd_pcm_t *handle;

  //if (argc < 2)
  //  {
  //    printf ("usage:play filename\n");
  //    return -1;
  //  }

  fp = fopen (argv, "r");

  if (fp == NULL)
    {
      printf ("Failed to open file\n");
      return -1;
    }


  if (snd_pcm_open (&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
      printf ("snd_pcm_open -- failed to open device\n");
      return -1;
    }

  if (snd_pcm_set_params (handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1,	// channels
		  WAVFILE_SAMPLES_PER_SECOND,	// sample rate
			  1,	// allow resampling
			  RECORD_TIME	// .5 seconds
      ) < 0)
    {
      printf ("snd_pcm_set_params -- failed to set parameters\n");
      return -1;
    }

  //buffer
  short *buffer = (short *) malloc (sizeof (short) * SAMPLE_SIZE);
  size_t read;

  do
    {
      read = fread (buffer, sizeof (short), SAMPLE_SIZE, fp);
      snd_pcm_writei (handle, buffer, read);	//put buffer into device
    }
  while (read > 0);		//hit the end of the file

  snd_pcm_close (handle);	//close audio device

  return 1;
}


void start_record(void *file_name2)
{

	short buffer[SAMPLE_SIZE];	//buffer for keep sound

	// Sigint handler code catch ctrl+c event
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = handle_sigint;
	sigemptyset (&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction (SIGINT, &sigIntHandler, NULL);

//	//sound_raw = fopen ("sound.raw", "w+");	//open a sound file
//	if (sound_raw == NULL)
//	{
//	  printf ("Failed to open sound.raw\n");
//	  return -1;
//	}

	//setup_device
	if (snd_pcm_open (&handle, "default", SND_PCM_STREAM_CAPTURE, 0) < 0)
	{
	  printf ("snd_pcm_open -- failed to open device\n");
	  //return -1;
	}

	//set device parameter
	if (snd_pcm_set_params (handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1,	// channels
			WAVFILE_SAMPLES_PER_SECOND,	// sample rate
			  1,	// allow resampling
			  RECORD_TIME	// .5 seconds
	  ) < 0)
	{
	  printf ("snd_pcm_set_params -- failed to set parameters\n");
	  //return -1;
	}

	struct file_name *rec_file_name = (struct file_name *)file_name2;
	FILE * f = wavfile_open(rec_file_name->name);


	while (buttons!='q')
	{
		//printf("Address(fun): %u",&buttons);
		snd_pcm_readi (handle, buffer, SAMPLE_SIZE);
		wavfile_write(f,buffer,SAMPLE_SIZE);
		//printf("%d",buttons);
		//fflush(stdout);
		//fwrite (f, sizeof (char), 80, sound_raw);
	}
	snd_pcm_close (handle);
	wavfile_close(f);
}

void handle_sigint (int s)
{				//signal responed

  //fclose (sound_raw);		//close file
  snd_pcm_close (handle);	//close device
  //exit (0);
}
