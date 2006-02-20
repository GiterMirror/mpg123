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

struct buf {
	unsigned char *pnt;
	long size;
	long pos;
	struct buf *next;
	struct buf *prev;
};

struct framebuf {
	struct buf *buf;
	long pos;
	struct frame *next;
	struct frame *prev;
};

struct mpstr {
	struct buf *head,*tail;
	int bsize;
	int framesize;
	int fsizeold;
	struct frame fr;
	unsigned char bsspace[2][MAXFRAMESIZE+512]; /* MAXFRAMESIZE */
	real hybrid_block[2][2][SBLIMIT*SSLIMIT];
	int hybrid_blc[2];
	unsigned long header;
	int bsnum;
	real synth_buffs[2][2][0x110];
	int  synth_bo;
};

#define BOOL int

#define MP3_ERR -1
#define MP3_OK  0
#define MP3_NEED_MORE 1


BOOL InitMP3(struct mpstr *mp);
int decodeMP3(struct mpstr *mp,char *inmemory,int inmemsize,
				  char *outmemory,int outmemsize,int *done);
void ExitMP3(struct mpstr *mp);
