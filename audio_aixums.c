/* 
 *  audio routines for AIX Ultimedia services
 *  based on IBM sample code
 *  requires fileset UMS.objects
 *  (c) 2000 Christian Bauernfeind
 */

#include <sys/types.h>
#include <stdio.h>

#include "mpg123.h"

int audio_open(struct audio_info_struct *ai)
{
  int gain = 100;

  if(!ai)
    return -1;

  ai->ev = somGetGlobalEnvironment();
  ai->class = UMSAudioDeviceNewClass(UMSAudioDevice_MajorVersion,
                UMSAudioDevice_MinorVersion);

  if (!ai->class)
  {
    fprintf(stderr,"Can't create AudioDevice meta class.\n");
    exit(1);
  }

  ai->dev = UMSAudioDeviceMClass_make_by_alias(ai->class, ai->ev,
              "Audio", "PLAY", UMSAudioDevice_BlockingIO, &(ai->err),
              &(ai->errstr), &(ai->fmtal), &(ai->inp), &(ai->out));

  if (!ai->dev)
  {
    fprintf(stderr,"Can't create AudioDevice object.\n");
    exit(1);
  }

  if(ai->gain != -1)
  {
    gain = ai->gain;
    UMSAudioDevice_set_volume(ai->dev, ai->ev, gain);
  }

  if (ai->output & AUDIO_OUT_INTERNAL_SPEAKER)
    UMSAudioDevice_enable_output(ai->dev, ai->ev, "INTERNAL_SPEAKER", &gain, &gain);
  else
    UMSAudioDevice_disable_output(ai->dev, ai->ev, "INTERNAL_SPEAKER");
  if (ai->output & AUDIO_OUT_LINE_OUT)
    UMSAudioDevice_enable_output(ai->dev, ai->ev, "LINE_OUT", &gain, &gain);
  if (ai->output & AUDIO_OUT_HEADPHONES)
    UMSAudioDevice_enable_output(ai->dev, ai->ev, "HEADPHONE", &gain, &gain);

  if(audio_reset_parameters(ai) < 0) {
    return -1;
  }

  UMSAudioDevice_start(ai->dev, ai->ev);
  return 0;
}

int audio_reset_parameters(struct audio_info_struct *ai)
{
  audio_set_format(ai);
  audio_set_channels(ai);
  audio_set_rate(ai);
  UMSAudioDevice_initialize(ai->dev, ai->ev);
  return 0;
}

int audio_rate_best_match(struct audio_info_struct *ai)
{
  return 0;
}

int audio_set_rate(struct audio_info_struct *ai)
{
  int ret,dsp_rate;

  if(!ai || ai->rate < 0)
    return 0;
  dsp_rate = ai->rate;

  UMSAudioDevice_set_sample_rate(ai->dev, ai->ev, ai->rate, &dsp_rate);
  ai->rate = dsp_rate;
  return 0;
}

int audio_set_channels(struct audio_info_struct *ai)
{

  if(ai->channels < 0)
    return 0;

  return UMSAudioDevice_set_number_of_channels(ai->dev, ai->ev, ai->channels);

}

int audio_set_format(struct audio_info_struct *ai)
{
  int sample_size;
  char *format, *numfmt;

  if(ai->format == -1)
    return 0;

  switch(ai->format) {
    case AUDIO_FORMAT_SIGNED_16:
    default:
      format = "PCM";
      numfmt = "TWOS_COMPLEMENT";
      sample_size = 16;
      break;
    case AUDIO_FORMAT_UNSIGNED_16:
      format = "PCM";
      numfmt = "UNSIGNED";
      sample_size = 16;
      break;
    case AUDIO_FORMAT_SIGNED_8:
      format = "PCM";
      numfmt = "TWOS_COMPLEMENT";
      sample_size = 8;
      break;
    case AUDIO_FORMAT_UNSIGNED_8:
      format = "PCM";
      numfmt = "UNSIGNED";
      sample_size = 8;
      break;
    case AUDIO_FORMAT_ALAW_8:
      format = "A_LAW";
      numfmt = "TWOS_COMPLEMENT";
      sample_size = 8;
      break;
    case AUDIO_FORMAT_ULAW_8:
      format = "MU_LAW";
      numfmt = "TWOS_COMPLEMENT";
      sample_size = 8;
      break;

  }

  UMSAudioDevice_set_bits_per_sample(ai->dev, ai->ev, sample_size);
  UMSAudioDevice_set_audio_format_type(ai->dev, ai->ev, format);
  UMSAudioDevice_set_number_format(ai->dev, ai->ev, numfmt);
  UMSAudioDevice_set_byte_order(ai->dev, ai->ev, "MSB");
  return 0;
}

int audio_get_formats(struct audio_info_struct *ai)
{
  return AUDIO_FORMAT_SIGNED_16 | AUDIO_FORMAT_UNSIGNED_16 |
           AUDIO_FORMAT_SIGNED_8 | AUDIO_FORMAT_UNSIGNED_8 |
           AUDIO_FORMAT_ULAW_8 | AUDIO_FORMAT_ALAW_8;
}

int audio_play_samples(struct audio_info_struct *ai,unsigned char *buf,int len)
{
  int len_out;
  UMSAudioTypes_Buffer buffer;

  buffer._length = 0;
  buffer._buffer = buf;
  buffer._maximum = len;
  UMSAudioDevice_write(ai->dev, ai->ev, &buffer, len, &len_out);

  return len - len_out;
}

int audio_close(struct audio_info_struct *ai)
{
  UMSAudioDevice_play_remaining_data(ai->dev, ai->ev, TRUE);
  UMSAudioDevice_stop(ai->dev, ai->ev);
  UMSAudioDevice_close(ai->dev, ai->ev);
  _somFree(ai->dev);
  return 0;
}

int audio_pause(struct audio_info_struct *ai, int pause)
{
  if (pause)
    UMSAudioDevice_pause(ai->dev, ai->ev);
  else
    UMSAudioDevice_resume(ai->dev, ai->ev);

  return 0;
}

void audio_queueflush (struct audio_info_struct *ai)
{
  UMSAudioDevice_play_remaining_data(ai->dev, ai->ev, TRUE);
}
