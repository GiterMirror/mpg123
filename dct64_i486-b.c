/* Discrete Cosine Tansform (DCT) for subband synthesis.
 *
 * This code is optimized for 80486. It should be compiled with gcc
 * 2.7.2 or higher.
 *
 * Note: This code does not give the necessary accuracy. Moreover, no
 * overflow test are done.
 *
 * (c) 1998 Fabrice Bellard.  
 * 
 * Rewrite to asm with algorithm and memory optimization 
 *
 * (c)  2000 Petr Salinger
 *
 * GPL clean
 *
 */

#include "mpg123.h"

#define COS_0_0 16403
#define COS_0_1 16563
#define COS_0_2 16890
#define COS_0_3 17401
#define COS_0_4 18124
#define COS_0_5 19101
#define COS_0_6 20398
#define COS_0_7 22112
#define COS_0_8 24396
#define COS_0_9 27503
#define COS_0_10 31869
#define COS_0_11 38320
#define COS_0_12 48633
#define COS_0_13 67429
#define COS_0_14 111660
#define COS_0_15 333906
#define COS_1_0 16463
#define COS_1_1 17121
#define COS_1_2 18577
#define COS_1_3 21195
#define COS_1_4 25826
#define COS_1_5 34756
#define COS_1_6 56441
#define COS_1_7 167154
#define COS_2_0 16704
#define COS_2_1 19704
#define COS_2_2 29490
#define COS_2_3 83981
#define COS_3_0 17733
#define COS_3_1 42813
#define COS_4_0 23170         

#define SETOUT(out,n,expr) out[FIR_BUFFER_SIZE*(n)]=(expr)
#define MUL(a,b) (((a)*(b)) >> 15)
#define MULL(a,b) (((long long)(a)*(long long)(b)) >> 15)
#define TOINT(a) ((int)((a)*32768.0))

void dct64_1_486_a(real *samples, int *bx);

void dct64_1_486_b(int *out0, int *out1, int *b1)
{
  b1[0x02] += b1[0x03];
  b1[0x06] += b1[0x07];
  b1[0x04] += b1[0x06];
  b1[0x06] += b1[0x05];
  b1[0x05] += b1[0x07];

  b1[0x0A] += b1[0x0B];
  b1[0x0E] += b1[0x0F];
  b1[0x0C] += b1[0x0E];
  b1[0x0E] += b1[0x0D];
  b1[0x0D] += b1[0x0F];

  b1[0x12] += b1[0x13];
  b1[0x16] += b1[0x17];
  b1[0x14] += b1[0x16];
  b1[0x16] += b1[0x15];
  b1[0x15] += b1[0x17];

  b1[0x1A] += b1[0x1B];
  b1[0x1E] += b1[0x1F];
  b1[0x1C] += b1[0x1E];
  b1[0x1E] += b1[0x1D];
  b1[0x1D] += b1[0x1F];

 SETOUT(out0,16,b1[0x00]);
 SETOUT(out0,12,b1[0x04]);
 SETOUT(out0, 8,b1[0x02]);
 SETOUT(out0, 4,b1[0x06]);
 SETOUT(out0, 0,b1[0x01]);
 SETOUT(out1, 0,b1[0x01]);
 SETOUT(out1, 4,b1[0x05]);
 SETOUT(out1, 8,b1[0x03]);
 SETOUT(out1,12,b1[0x07]);

 SETOUT(out0,14, b1[0x08] + b1[0x0C]);
 SETOUT(out0,10, b1[0x0C] + b1[0x0A]);
 SETOUT(out0, 6, b1[0x0A] + b1[0x0E]);
 SETOUT(out0, 2, b1[0x0E] + b1[0x09]);
 SETOUT(out1, 2, b1[0x09] + b1[0x0D]);
 SETOUT(out1, 6, b1[0x0D] + b1[0x0B]);
 SETOUT(out1,10, b1[0x0B] + b1[0x0F]);
 SETOUT(out1,14, b1[0x0F]);

 b1[0x18] += b1[0x1C];
 SETOUT(out0,15,b1[0x10] + b1[0x18]);
 SETOUT(out0,13,b1[0x18] + b1[0x14]);
 b1[0x1C] += b1[0x1a];
 SETOUT(out0,11,b1[0x14] + b1[0x1C]);
 SETOUT(out0, 9,b1[0x1C] + b1[0x12]);
 b1[0x1A] += b1[0x1E];
 SETOUT(out0, 7,b1[0x12] + b1[0x1A]);
 SETOUT(out0, 5,b1[0x1A] + b1[0x16]);
 b1[0x1E] += b1[0x19];
 SETOUT(out0, 3,b1[0x16] + b1[0x1E]);
 SETOUT(out0, 1,b1[0x1E] + b1[0x11]);
 b1[0x19] += b1[0x1D];
 SETOUT(out1, 1,b1[0x11] + b1[0x19]);
 SETOUT(out1, 3,b1[0x19] + b1[0x15]);
 b1[0x1D] += b1[0x1B];
 SETOUT(out1, 5,b1[0x15] + b1[0x1D]);
 SETOUT(out1, 7,b1[0x1D] + b1[0x13]);
 b1[0x1B] += b1[0x1F];
 SETOUT(out1, 9,b1[0x13] + b1[0x1B]);
 SETOUT(out1,11,b1[0x1B] + b1[0x17]);
 SETOUT(out1,13,b1[0x17] + b1[0x1F]);
 SETOUT(out1,15,b1[0x1F]);
}

/*
 * main DCT function
 */
void dct64_486(int *a,int *b,real *samples)
{
  int bufs[33]; /* 32 + 1 extra for 8B store from x87 */

  dct64_1_486_a(samples,bufs);
  dct64_1_486_b(a,b,bufs);
}

