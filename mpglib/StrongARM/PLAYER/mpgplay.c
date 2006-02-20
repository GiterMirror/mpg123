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
 *   3 10/25/00 Changed sound map to use dual index map.                    *
 *              SHOW_TIME_INFO may be defined to show running time.    GbG  *
 *   4 10/26/00 Reverted back to single index map for now.             GbG  *
 *   5 10/26/00 Added Big Endian Support. Tested on PowerStack with         *
 *              SD_CS 'Crystal CS4231' driver.                              *
 *              Added wait before exit to make sure we hear all the         *
 *              song. Do not want to miss that last half second.       GbG  *
 ****************************************************************************/

#define EDITION     5
 
_asm("_sysedit: equ %0", __obj_constant(EDITION));
 

/*
 * MPGPLAY
 * This program is not designed to be a full featured MP3 player
 * rather this program is designed to show how to use the MPGMP3
 * library sources as well sound map managment to the MAUI sound
 * system. 
 * 
 * MPGPLAY uses the "mpglib.il" MPG library based on MPG123 sources.
 * Three functions are all that is required to decompress MP3
 * information either from a file or a stream.
 *
 * InitMP3(&mp);
 * decodeMP3(&mp,bufin,inlen,bufout,OUT_BUF_SIZE,&outlen);
 * ExitMP3(&mp);
 *
 *
 * When decompressing MP3 images we create as many SMAP slots as 
 * required. We will start the sound maps playing and continue
 * decompressing the next block while the current blocks are
 * played.  This method provides very clean playback even on 
 * slower machines (133Mhz or greater should be fine). 
 *
 * On X86 and ARM we will run with INTEGER based code by default.
 * This will allow playing of MP3's even on 486SX based processors.
 * 
*/



#define _OPT_PROTOS

#include <types.h>
#include <stdlib.h>
#include <mpg123.h>
#include <mpglib.h>
#include <MAUI/maui_cdb.h>
#include <MAUI/maui_snd.h>
#include <MAUI/mfm_snd.h>
#include <modes.h>
#include <memory.h>
#include <math.h>
#include <alarm.h>
#include <string.h>
#include <signal.h>

#if defined(SHOW_TIME_INFO)
#include <time.h>
time_t  initial_time;
time_t  current_time;
u_int32 total_blks=0;
void  ts_time_decompose ( const unsigned long time_in_sec);
#endif

void sighand(int sig);

#define SMAP_DONE_SIG E_SIGUSR2

volatile int sigval;

u_char *sound_buffer=NULL;
u_int32 sound_buffer_size=0;

#define IN_BUF_SIZE (8192*4)
#define OUT_BUF_SIZE (8192*4) 
#define SND_BLK_SIZE (32768)

#if	defined(_BIG_END)
#define CODING_METHOD (SND_CM_MSBYTE1ST|SND_CM_PCM_SLINEAR)
#else /* Little Endian */
#define CODING_METHOD (SND_CM_LSBYTE1ST|SND_CM_PCM_SLINEAR)
#endif

#define SAMPLE_SIZE 16 	

int frequencies[9] = { 44100, 48000, 32000, 22050, 24000, 16000 , 11025 , 12000 , 8000 };
int bitrates[15] = { 0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320 };

SND_SMAP smap[100];


int main(int argc, char **argv){

	char		*bufout;
	char		*bufin;
	path_id		path,devpath;
	error_code	error;
	u_int32		bufsize_out = OUT_BUF_SIZE;
	u_int32		bufsize_in = IN_BUF_SIZE;
	u_int32		count;
	int			inlen,outlen;
	char		*compbuf=NULL;
	int			compbuflen=0,compbufpos=0;
	int			status=MP3_OK;
	int			i;
	struct 		mpstr mp;
	int 		iflag=0;

	if (argc != 3)
	{
		printf("Play MP3 file\n");
		printf("Usage: mpgplay <filename><device>\n");
		exit(1);
	}
	/* set up signal handler */
	intercept (sighand);

	/* Initialize the MP3 decoder */
	InitMP3(&mp);
	
	if ((error = _os_open(argv[1], FAM_READ, &path)) != SUCCESS) {
		return error;
	}
    
    /* Prepare the Sound device */
    if ((error = _os_open(argv[2], FAM_WRITE, &devpath)) != SUCCESS) {
		return error;
    }

    if ((error = _os_srqmem (&bufsize_in, (void**)&bufin, 0)) != SUCCESS) {
		return error;
	}

    if ((error = _os_srqmem (&bufsize_out, (void**)&bufout, 0)) != SUCCESS) {
		return error;
	}


	sigval = SMAP_DONE_SIG; /* Sound is free */

	#if defined(SHOW_TIME_INFO)
	time (&initial_time);
	#endif

	while(status != MP3_ERR){
	
		count = IN_BUF_SIZE;

	    /* read the data */
    	if ((error = _os_read(path, bufin, &count)) != SUCCESS){
	      if (error == EOS_EOF) break; /* done */
    	  else return error;
		}

		inlen = count;

		/* Initialize buffer */

		outlen = compbufpos = compbuflen = 0;

		status = decodeMP3(&mp,bufin,inlen,bufout,OUT_BUF_SIZE,&outlen);

		if (!iflag){
			printf("Playing %s [%s - %dkbs]\n",
				argv[1],
					mp.fr.stereo?"Stereo":"Mono",
						frequencies[mp.fr.sampling_frequency]);


			#if defined(SHOW_FRAME)
				printf("FRAME: stereo %d jsbound %d single %d lsf %d\n",
			    mp.fr.stereo,
			    mp.fr.jsbound,
			    mp.fr.single,
			    mp.fr.lsf);
				printf("mpeg25 %d header_change %d lay %d error_protection %d\n",
			    mp.fr.mpeg25,
			    mp.fr.header_change,
			    mp.fr.lay,
			    mp.fr.error_protection);
				printf("bitrate_index %d sampling_frequency %d padding %d extension %d\n",
			    mp.fr.bitrate_index,
			    mp.fr.sampling_frequency,
			    mp.fr.padding,
			    mp.fr.extension);
				printf("mode %d mode_ext %d copyright %d original %d\n",
			    mp.fr.mode,
			    mp.fr.mode_ext,
			    mp.fr.copyright,
			    mp.fr.original);
				printf("emphasis %d framesize %d\n",
			    mp.fr.emphasis,
			    mp.fr.framesize);
			
			#endif



			iflag=1;

		}

		while(status == MP3_OK){


			/* Check buffer size */
			if ((compbufpos + outlen) > compbuflen || compbuf == NULL)
				{
				compbuflen = compbufpos+outlen;
				compbuf = (char *) realloc(compbuf,compbuflen); 
				}

			memcpy(compbuf+compbufpos,bufout,outlen);
			compbufpos+=outlen;

			status = decodeMP3(&mp,NULL,0,bufout,OUT_BUF_SIZE,&outlen);

		}


		if (compbufpos)
		{

			int blks =  compbufpos / SND_BLK_SIZE;
			int residual = compbufpos - (blks*SND_BLK_SIZE);


			#if defined(SHOW_TIME_INFO)
			{
				time (&current_time);
			    ts_time_decompose ( (unsigned long)difftime (current_time, initial_time));
				total_blks += (blks+1);
			}
			#endif


			/* Wait for buffer slot to empty */
		
			sigmask(1);
			if (sigval != SMAP_DONE_SIG){
  				tsleep(0);		
			} else 
				sigmask(-1);

			sigval = 0; 

		 	if (compbufpos > sound_buffer_size)
	 		{


				sound_buffer = (u_char *) realloc(sound_buffer,compbufpos); 

			}

				sound_buffer_size = compbufpos;

				memcpy(sound_buffer,compbuf,sound_buffer_size);

				/*
				 * Fill up the sound maps
				*/

				for(i=0;i<blks;i++)
				{

					smap[i].num_channels =  mp.fr.stereo;
					smap[i].sample_rate = frequencies[mp.fr.sampling_frequency]; 
					smap[i].coding_method = CODING_METHOD; 
					smap[i].sample_size = SAMPLE_SIZE; 
					smap[i].cur_offset = 0;
					smap[i].loop_start = 0;
					smap[i].loop_count = 1; 
					smap[i].loop_counter = 0; 
					smap[i].next = &smap[i+1];
					smap[i].trig_mask = 0;
					smap[i].trig_signal = 0;
					/* get the size of the data in this read */
					smap[i].buf_size = smap[i].loop_end = SND_BLK_SIZE; 
					smap[i].buf = &sound_buffer[i*SND_BLK_SIZE];


				}

				/* If we do not have residual then back up to last sound map 1st */

				if (residual==0)
					i--;

				/* Last block in chain */
				smap[i].num_channels =  mp.fr.stereo;
				smap[i].sample_rate = frequencies[mp.fr.sampling_frequency]; 
				smap[i].coding_method = CODING_METHOD; 
				smap[i].sample_size = SAMPLE_SIZE; 
				smap[i].cur_offset = 0;
				smap[i].loop_start = 0;
				smap[i].loop_count = 1; 
				smap[i].loop_counter = 0; 
				smap[i].next = NULL;
				smap[i].trig_mask = SND_TRIG_READY;
				smap[i].trig_signal = SMAP_DONE_SIG;
				/* get the size of the data in this read */
				smap[i].buf_size = smap[i].loop_end = residual?residual:SND_BLK_SIZE; 
				smap[i].buf = &sound_buffer[i*SND_BLK_SIZE];

				if ((error = _os_ss_snd_play(devpath, &smap[0], SND_NOBLOCK  )) != SUCCESS) {
					  return error;
				}

				compbufpos = 0;
		
		}

	}
	
	#if defined(SHOW_TIME_INFO)
	printf("\n");
	#endif

	
	/* Wait for buffer slot to empty */
		
	sigmask(1);
	if (sigval != SMAP_DONE_SIG){
			tsleep(0);		
	} else 
		sigmask(-1);


	/* Exit the MP3 decoder */
	ExitMP3(&mp);

	_os_srtmem(bufsize_in, (void *)bufin);
	_os_srtmem(bufsize_out, (void *)bufout);


}

#if defined(SHOW_TIME_INFO)
void  ts_time_decompose ( const unsigned long time_in_sec )
{
    unsigned long	hour = time_in_sec / 3600;
    unsigned int	min  = time_in_sec / 60 % 60;
    unsigned int	sec  = time_in_sec % 60;
	u_int32			size;
	char	 	  	buf[80];

    if ( hour == 0 )
        sprintf ( buf,"Playing Time: %2u:%02u - Block %d        \r", min, sec,total_blks);
    else 
        sprintf ( buf, "Playing Time: %2lu:%02u:%02u - Block %d        \r", hour, min, sec,total_blks);

	size = strlen(buf);
	_os_write(0, buf, &size);
	fflush(stdout);
}

#endif


void sighand(int sig)
{
	switch (sigval = sig) {
		case 0:
			fprintf (stderr, "SIG ZERO?\n");
			break;
	
		case 2:	/* fatal signals */
		case 3:
			exit(sig);
	
		case SMAP_DONE_SIG:
	  		/* printf("got my sig!!\n"); */
  		break;
		default:
		/* ignore signal */
		break;
	}

	 _os_rte();
}

