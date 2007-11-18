/*
	audio_win32.c: audio output for Windows 32bit

	copyright ?-2007 by the mpg123 project - free software under the terms of the LGPL 2.1
	see COPYING and AUTHORS files in distribution or http://mpg123.org
	initially written (as it seems) by Tony Million
	reworked by ravenexp
*/

#include "mpg123.h"

#include <windows.h>

/* Number of buffers in the playback ring */
#define NUM_BUFFERS 64 /* about 1 sec of 44100 sampled stream */

/* Buffer ring queue state */
struct queue_state
{
	WAVEHDR buffer_headers[NUM_BUFFERS];
	/* The next buffer to be filled and put in playback */
	int next_buffer;
	/* Buffer playback completion event */
	HANDLE play_done_event;
};

int audio_open(struct audio_info_struct *ai)
{
	struct queue_state* state;
	WAVEFORMATEX out_fmt;
	UINT dev_id;
	MMRESULT res;

	if(!ai) return -1;
	if(ai->rate == -1) return 0;

	/* Allocate queue state struct for this device */
	state = calloc(1, sizeof(struct queue_state));
	if(!state) return -1;

	state->play_done_event = CreateEvent(0,FALSE,FALSE,0);
	if(state->play_done_event == INVALID_HANDLE_VALUE)
	{
		free(state);
		return -1;
	}

	/* FIXME: real device enumeration by capabilities? */
	dev_id = WAVE_MAPPER;	/* probably does the same thing */
	ai->device = "WaveMapper";

	/* FIXME: support for smth besides MPG123_ENC_SIGNED_16? */
	out_fmt.wFormatTag = WAVE_FORMAT_PCM;
	out_fmt.wBitsPerSample = 16;
	out_fmt.nChannels = 2;
	out_fmt.nSamplesPerSec = ai->rate;
	out_fmt.nBlockAlign = out_fmt.nChannels*out_fmt.wBitsPerSample/8;
	out_fmt.nAvgBytesPerSec = out_fmt.nBlockAlign*out_fmt.nSamplesPerSec;
	out_fmt.cbSize = 0;

	res = waveOutOpen((HWAVEOUT*)&ai->fn, dev_id, &out_fmt,
	                  (DWORD)state->play_done_event, 0, CALLBACK_EVENT);

	if(res != MMSYSERR_NOERROR) free(state);

	switch(res){
		case MMSYSERR_NOERROR:
			break;
        	case MMSYSERR_ALLOCATED:
			error("Audio output device is already allocated.");
			return -1;
		case MMSYSERR_NODRIVER:
			error("No device driver is present.");
			return -1;
		case MMSYSERR_NOMEM:
			error("Unable to allocate or lock memory.");
			return -1;
		case WAVERR_BADFORMAT:
			error("Unsupported waveform-audio format.");
			return -1;
		default:
			error("Unable to open wave output device.");
			return -1;
	}

	/* Reset event from the "device open" message */
	ResetEvent(state->play_done_event);

	waveOutReset((HWAVEOUT)ai->fn);

	/* Playback starts when the full queue is prebuffered */
	waveOutPause((HWAVEOUT)ai->fn);
	ai->handle = state;

	return 0;
}

int audio_get_formats(struct audio_info_struct *ai)
{
	return AUDIO_FORMAT_SIGNED_16;
}

int audio_play_samples(struct audio_info_struct *ai,
                       unsigned char *buffer, int len)
{
	struct queue_state* state;

	MMRESULT res;
	WAVEHDR* hdr;
	
	if(!ai || !ai->handle) return -1;
	if(!buffer || len <= 0) return 0;

	state = (struct queue_state*)ai->handle;

	/* The last recently used buffer in the play ring */
	hdr = &state->buffer_headers[state->next_buffer];

	/* Check buffer header and wait if it's being played */
	while(hdr->dwFlags & WHDR_INQUEUE)
	{
		/* Looks like queue is full now, can start playing */
		waveOutRestart((HWAVEOUT)ai->fn);
		WaitForSingleObject(state->play_done_event, INFINITE);
		/* BUG: Sometimes event is signaled for some other reason. */
		if(!(hdr->dwFlags & WHDR_DONE))	debug("Audio output device signals something...");
	}

	/* Cleanup */
	if(hdr->dwFlags & WHDR_DONE) waveOutUnprepareHeader((HWAVEOUT)ai->fn, hdr, sizeof(WAVEHDR));

	hdr->dwFlags = 0;
	/* (Re)allocate buffer only if required.
	   hdr->dwUser = allocated length
	*/
	if(!hdr->lpData || hdr->dwUser < len)
	{
		hdr->lpData = realloc(hdr->lpData, len);
		if(!hdr->lpData){ error("Out of memory for playback buffers."); return -1; }

		hdr->dwUser = len;
	}
	hdr->dwBufferLength = len;
	memcpy(hdr->lpData, buffer, len);

	res = waveOutPrepareHeader((HWAVEOUT)ai->fn, hdr, sizeof(WAVEHDR));
	if(res != MMSYSERR_NOERROR){ error("Can't write to audio output device (prepare)."); return -1; }

	res = waveOutWrite((HWAVEOUT)ai->fn, hdr, sizeof(WAVEHDR));
	if(res != MMSYSERR_NOERROR){ error("Can't write to audio output device."); return -1; }

	/* Cycle to the next buffer in the ring queue */
	state->next_buffer = (state->next_buffer + 1) % NUM_BUFFERS;

	return len;
}

void audio_queueflush(struct audio_info_struct *ai)
{
	int i;
	struct queue_state* state;

	if(!ai || !ai->handle)
		return;

	state = (struct queue_state*)ai->handle;

	waveOutReset((HWAVEOUT)ai->fn);

	ResetEvent(state->play_done_event);

	for(i = 0; i < NUM_BUFFERS; i++)
	{
		if(state->buffer_headers[i].dwFlags & WHDR_DONE)
		waveOutUnprepareHeader((HWAVEOUT)ai->fn, &state->buffer_headers[i], sizeof(WAVEHDR));

		state->buffer_headers[i].dwFlags = 0;	
	}

	waveOutPause((HWAVEOUT)ai->fn);
}

int audio_close(struct audio_info_struct *ai)
{
	int i;
	struct queue_state* state;

	if(!ai || !ai->handle)
		return -1;

	state = (struct queue_state*)ai->handle;

	audio_queueflush(ai);
	waveOutClose((HWAVEOUT)ai->fn);
	CloseHandle(state->play_done_event);

	for(i = 0; i < NUM_BUFFERS; i++) free(state->buffer_headers[i].lpData);

	free(ai->handle);
	ai->handle = NULL;

	return 0;
}

