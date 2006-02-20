/****************************************************************************
 *              Copyright 2000 by Microware Systems Corporation             *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This is a derivative work of the MP3 library from the mpg123 player,     *
 * specifically the 0.59r version. The home page for the player is          *
 * http://mpg123.org/ This software is under the GPL license. The GPL.TXT   *
 * file included with this software explains the license. Michael Hipp the  *
 * author of the mpg123 player may still be doing licenses other than GPL.  *
 *                                                                          *
 ****************************************************************************
 *			                                                                *  
 * Edition History:														    *
 *																		    *
 * #   Date     Comments                                                By  *
 * --- -------- -----------------------------------------------------  ---  *
 *   1 10/23/00 First working release                                  GbG  *
 ****************************************************************************/


#include <ctype.h>
#include <stdlib.h>
#include <signal.h>

#if defined(_OS9000)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "mpg123.h"

struct parameter param = { 1 , 1 , 0 , 0 };

int tabsel_123[2][16] = {
   {0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320},
   {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160}
};

long freqs[9] = { 44100, 48000, 32000,
                  22050, 24000, 16000 ,
                  11025 , 12000 , 8000 };

int bitindex;
unsigned char *wordpointer;

/*
 * the code a header and write the information
 * into the frame structure
 */
int decode_header(struct frame *fr,unsigned long newhead)
{
    if( newhead & (1<<20) ) {
      fr->lsf = (newhead & (1<<19)) ? 0x0 : 0x1;
      fr->mpeg25 = 0;
    }
    else {
      fr->lsf = 1;
      fr->mpeg25 = 1;
    }
    
    fr->lay = 4-((newhead>>17)&3);
    if( ((newhead>>10)&0x3) == 0x3) {
      /* fprintf(stderr,"Stream error\n"); */
      return(0);
    }

    if(fr->mpeg25) {
      fr->sampling_frequency = 6 + ((newhead>>10)&0x3);
    }
    else
	    fr->sampling_frequency = ((newhead>>10)&0x3) + (fr->lsf*3);
    fr->error_protection = ((newhead>>16)&0x1)^0x1;

    if(fr->mpeg25) /* allow Bitrate change for 2.5 ... */
      fr->bitrate_index = ((newhead>>12)&0xf);

    fr->bitrate_index = ((newhead>>12)&0xf);
    fr->padding   = ((newhead>>9)&0x1);
    fr->extension = ((newhead>>8)&0x1);
    fr->mode      = ((newhead>>6)&0x3);
    fr->mode_ext  = ((newhead>>4)&0x3);
    fr->copyright = ((newhead>>3)&0x1);
    fr->original  = ((newhead>>2)&0x1);
    fr->emphasis  = newhead & 0x3;

    fr->stereo    = (fr->mode == MPG_MD_MONO) ? 1 : 2;

    if(!fr->bitrate_index)
    {
      /* fprintf(stderr,"Free format not supported.\n"); */
      return (0);
    }

    if (fr->lay != 3) return 0;
	 else
    {
          fr->framesize  = (long) tabsel_123[fr->lsf][fr->bitrate_index] * 144000;
          fr->framesize /= freqs[fr->sampling_frequency]<<(fr->lsf);
          fr->framesize = fr->framesize + fr->padding - 4;
    }

    return 1;
}

unsigned int getbits(int number_of_bits)
{
  unsigned long rval;

  if(!number_of_bits)
    return 0;

  {
    rval = wordpointer[0];
    rval <<= 8;
    rval |= wordpointer[1];
    rval <<= 8;
    rval |= wordpointer[2];
    rval <<= bitindex;
    rval &= 0xffffff;

    bitindex += number_of_bits;

    rval >>= (24-number_of_bits);

    wordpointer += (bitindex>>3);
    bitindex &= 7;
  }
  return rval;
}

unsigned int getbits_fast(int number_of_bits)
{
  unsigned long rval;

  {
    rval = wordpointer[0];
    rval <<= 8;	
    rval |= wordpointer[1];
    rval <<= bitindex;
    rval &= 0xffff;
    bitindex += number_of_bits;

    rval >>= (16-number_of_bits);

    wordpointer += (bitindex>>3);
    bitindex &= 7;
  }
  return rval;
}

unsigned int get1bit(void)
{
  unsigned char rval;
  rval = *wordpointer << bitindex;

  bitindex++;
  wordpointer += (bitindex>>3);
  bitindex &= 7;
  return rval>>7;
}
