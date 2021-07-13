/********************************************************************\
 *  FILE:     rmd160.c
 *  CONTENTS: A sample C-implementation of the RIPEMD-160 hash-function.
 *  TARGET:   any computer with an ANSI C compiler
 *  AUTHOR:   Antoon Bosselaers, Dept. Electrical Eng.-ESAT/COSIC
 *  DATE:     1 March 1996       VERSION:  1.0
  * ADAPTED:  by Libbitcoin Developers on 7 September 2016
\********************************************************************/
#include "ripemd160.h"

#include "zeroize.h"

/* collect four bytes into one word: */
#define BYTES_TO_DWORD(bytes) \
   (((uint32_t)*((bytes) + 3) << 24) | \
    ((uint32_t)*((bytes) + 2) << 16) | \
    ((uint32_t)*((bytes) + 1) <<  8) | \
    ((uint32_t)*((bytes) + 0) <<  0))

/* ROL(x, n) cyclically rotates x over n bits to the left */
/* x must be of an unsigned 32 bits type and 0 <= n < 32. */
#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* the five basic functions F(), G() and H() */
#define F(x, y, z) ((x) ^ (y) ^ (z)) 
#define G(x, y, z) (((x) & (y)) | (~(x) & (z))) 
#define H(x, y, z) (((x) | ~(y)) ^ (z))
#define I(x, y, z) (((x) & (z)) | ((y) & ~(z))) 
#define J(x, y, z) ((x) ^ ((y) | ~(z)))
  
/* the ten basic operations FF() through III() */
#define FF(a, b, c, d, e, x, s) \
{ \
    (a) += F((b), (c), (d)) + (x); \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define GG(a, b, c, d, e, x, s) \
{ \
    (a) += G((b), (c), (d)) + (x) + 0x5a827999UL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define HH(a, b, c, d, e, x, s) \
{ \
    (a) += H((b), (c), (d)) + (x) + 0x6ed9eba1UL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define II(a, b, c, d, e, x, s) \
{ \
    (a) += I((b), (c), (d)) + (x) + 0x8f1bbcdcUL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define JJ(a, b, c, d, e, x, s) \
{ \
    (a) += J((b), (c), (d)) + (x) + 0xa953fd4eUL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define FFF(a, b, c, d, e, x, s) \
{ \
    (a) += F((b), (c), (d)) + (x); \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define GGG(a, b, c, d, e, x, s) \
{ \
    (a) += G((b), (c), (d)) + (x) + 0x7a6d76e9UL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define HHH(a, b, c, d, e, x, s) \
{ \
    (a) += H((b), (c), (d)) + (x) + 0x6d703ef3UL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define III(a, b, c, d, e, x, s) \
{ \
    (a) += I((b), (c), (d)) + (x) + 0x5c4dd124UL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}
#define JJJ(a, b, c, d, e, x, s) \
{ \
    (a) += J((b), (c), (d)) + (x) + 0x50a28be6UL; \
    (a) = ROL((a), (s)) + (e); \
    (c) = ROL((c), 10); \
}

void RMDcompress(RMD160CTX* context);
void RMDfinish(RMD160CTX* context, const uint8_t* message, size_t length);

void RMD160(const uint8_t* message, size_t length,
    uint8_t digest[RMD160_DIGEST_LENGTH])
{
    RMD160CTX context;
    RMDInit(&context);
    RMDUpdate(&context, message, length);
    RMDFinal(&context, digest);
}

void RMDInit(RMD160CTX* context)
{
    context->state[0] = 0x67452301UL;
    context->state[1] = 0xefcdab89UL;
    context->state[2] = 0x98badcfeUL;
    context->state[3] = 0x10325476UL;
    context->state[4] = 0xc3d2e1f0UL;
}

void RMDUpdate(RMD160CTX* context, const uint8_t* message, size_t length)
{
    size_t i;
    size_t byte;

    for (byte = length; byte > 63; byte -= 64)
    {
        for (i = 0; i < 16; i++)
        {
            context->chunk[i] = BYTES_TO_DWORD(message);
            message += 4;
        }

        RMDcompress(context);
    }

    RMDfinish(context, message, length);
}

void RMDFinal(RMD160CTX* context, uint8_t digest[RMD160_DIGEST_LENGTH])
{
    size_t i;

    for (i = 0; i < RMD160_DIGEST_LENGTH; i += 4)
    {
        digest[i + 0] = (context->state[i >> 2] >> 0);
        digest[i + 1] = (context->state[i >> 2] >> 8);
        digest[i + 2] = (context->state[i >> 2] >> 16);
        digest[i + 3] = (context->state[i >> 2] >> 24);
    }
}

/* Local */

void RMDcompress(RMD160CTX* context)
{
    uint32_t* state = context->state;
    uint32_t* chunk = context->chunk;

    uint32_t aa = state[0];
    uint32_t bb = state[1];
    uint32_t cc = state[2];
    uint32_t dd = state[3];
    uint32_t ee = state[4];

    uint32_t aaa = aa;
    uint32_t bbb = bb;
    uint32_t ccc = cc;
    uint32_t ddd = dd;
    uint32_t eee = ee;

    /* round 1 */
    FF(aa, bb, cc, dd, ee, chunk[ 0], 11);
    FF(ee, aa, bb, cc, dd, chunk[ 1], 14);
    FF(dd, ee, aa, bb, cc, chunk[ 2], 15);
    FF(cc, dd, ee, aa, bb, chunk[ 3], 12);
    FF(bb, cc, dd, ee, aa, chunk[ 4],  5);
    FF(aa, bb, cc, dd, ee, chunk[ 5],  8);
    FF(ee, aa, bb, cc, dd, chunk[ 6],  7);
    FF(dd, ee, aa, bb, cc, chunk[ 7],  9);
    FF(cc, dd, ee, aa, bb, chunk[ 8], 11);
    FF(bb, cc, dd, ee, aa, chunk[ 9], 13);
    FF(aa, bb, cc, dd, ee, chunk[10], 14);
    FF(ee, aa, bb, cc, dd, chunk[11], 15);
    FF(dd, ee, aa, bb, cc, chunk[12],  6);
    FF(cc, dd, ee, aa, bb, chunk[13],  7);
    FF(bb, cc, dd, ee, aa, chunk[14],  9);
    FF(aa, bb, cc, dd, ee, chunk[15],  8);
                             
    /* round 2 */
    GG(ee, aa, bb, cc, dd, chunk[ 7],  7);
    GG(dd, ee, aa, bb, cc, chunk[ 4],  6);
    GG(cc, dd, ee, aa, bb, chunk[13],  8);
    GG(bb, cc, dd, ee, aa, chunk[ 1], 13);
    GG(aa, bb, cc, dd, ee, chunk[10], 11);
    GG(ee, aa, bb, cc, dd, chunk[ 6],  9);
    GG(dd, ee, aa, bb, cc, chunk[15],  7);
    GG(cc, dd, ee, aa, bb, chunk[ 3], 15);
    GG(bb, cc, dd, ee, aa, chunk[12],  7);
    GG(aa, bb, cc, dd, ee, chunk[ 0], 12);
    GG(ee, aa, bb, cc, dd, chunk[ 9], 15);
    GG(dd, ee, aa, bb, cc, chunk[ 5],  9);
    GG(cc, dd, ee, aa, bb, chunk[ 2], 11);
    GG(bb, cc, dd, ee, aa, chunk[14],  7);
    GG(aa, bb, cc, dd, ee, chunk[11], 13);
    GG(ee, aa, bb, cc, dd, chunk[ 8], 12);

    /* round 3 */
    HH(dd, ee, aa, bb, cc, chunk[ 3], 11);
    HH(cc, dd, ee, aa, bb, chunk[10], 13);
    HH(bb, cc, dd, ee, aa, chunk[14],  6);
    HH(aa, bb, cc, dd, ee, chunk[ 4],  7);
    HH(ee, aa, bb, cc, dd, chunk[ 9], 14);
    HH(dd, ee, aa, bb, cc, chunk[15],  9);
    HH(cc, dd, ee, aa, bb, chunk[ 8], 13);
    HH(bb, cc, dd, ee, aa, chunk[ 1], 15);
    HH(aa, bb, cc, dd, ee, chunk[ 2], 14);
    HH(ee, aa, bb, cc, dd, chunk[ 7],  8);
    HH(dd, ee, aa, bb, cc, chunk[ 0], 13);
    HH(cc, dd, ee, aa, bb, chunk[ 6],  6);
    HH(bb, cc, dd, ee, aa, chunk[13],  5);
    HH(aa, bb, cc, dd, ee, chunk[11], 12);
    HH(ee, aa, bb, cc, dd, chunk[ 5],  7);
    HH(dd, ee, aa, bb, cc, chunk[12],  5);

    /* round 4 */
    II(cc, dd, ee, aa, bb, chunk[ 1], 11);
    II(bb, cc, dd, ee, aa, chunk[ 9], 12);
    II(aa, bb, cc, dd, ee, chunk[11], 14);
    II(ee, aa, bb, cc, dd, chunk[10], 15);
    II(dd, ee, aa, bb, cc, chunk[ 0], 14);
    II(cc, dd, ee, aa, bb, chunk[ 8], 15);
    II(bb, cc, dd, ee, aa, chunk[12],  9);
    II(aa, bb, cc, dd, ee, chunk[ 4],  8);
    II(ee, aa, bb, cc, dd, chunk[13],  9);
    II(dd, ee, aa, bb, cc, chunk[ 3], 14);
    II(cc, dd, ee, aa, bb, chunk[ 7],  5);
    II(bb, cc, dd, ee, aa, chunk[15],  6);
    II(aa, bb, cc, dd, ee, chunk[14],  8);
    II(ee, aa, bb, cc, dd, chunk[ 5],  6);
    II(dd, ee, aa, bb, cc, chunk[ 6],  5);
    II(cc, dd, ee, aa, bb, chunk[ 2], 12);

    /* round 5 */
    JJ(bb, cc, dd, ee, aa, chunk[ 4],  9);
    JJ(aa, bb, cc, dd, ee, chunk[ 0], 15);
    JJ(ee, aa, bb, cc, dd, chunk[ 5],  5);
    JJ(dd, ee, aa, bb, cc, chunk[ 9], 11);
    JJ(cc, dd, ee, aa, bb, chunk[ 7],  6);
    JJ(bb, cc, dd, ee, aa, chunk[12],  8);
    JJ(aa, bb, cc, dd, ee, chunk[ 2], 13);
    JJ(ee, aa, bb, cc, dd, chunk[10], 12);
    JJ(dd, ee, aa, bb, cc, chunk[14],  5);
    JJ(cc, dd, ee, aa, bb, chunk[ 1], 12);
    JJ(bb, cc, dd, ee, aa, chunk[ 3], 13);
    JJ(aa, bb, cc, dd, ee, chunk[ 8], 14);
    JJ(ee, aa, bb, cc, dd, chunk[11], 11);
    JJ(dd, ee, aa, bb, cc, chunk[ 6],  8);
    JJ(cc, dd, ee, aa, bb, chunk[15],  5);
    JJ(bb, cc, dd, ee, aa, chunk[13],  6);

    /* parallel round 1 */
    JJJ(aaa, bbb, ccc, ddd, eee, chunk[ 5],  8);
    JJJ(eee, aaa, bbb, ccc, ddd, chunk[14],  9);
    JJJ(ddd, eee, aaa, bbb, ccc, chunk[ 7],  9);
    JJJ(ccc, ddd, eee, aaa, bbb, chunk[ 0], 11);
    JJJ(bbb, ccc, ddd, eee, aaa, chunk[ 9], 13);
    JJJ(aaa, bbb, ccc, ddd, eee, chunk[ 2], 15);
    JJJ(eee, aaa, bbb, ccc, ddd, chunk[11], 15);
    JJJ(ddd, eee, aaa, bbb, ccc, chunk[ 4],  5);
    JJJ(ccc, ddd, eee, aaa, bbb, chunk[13],  7);
    JJJ(bbb, ccc, ddd, eee, aaa, chunk[ 6],  7);
    JJJ(aaa, bbb, ccc, ddd, eee, chunk[15],  8);
    JJJ(eee, aaa, bbb, ccc, ddd, chunk[ 8], 11);
    JJJ(ddd, eee, aaa, bbb, ccc, chunk[ 1], 14);
    JJJ(ccc, ddd, eee, aaa, bbb, chunk[10], 14);
    JJJ(bbb, ccc, ddd, eee, aaa, chunk[ 3], 12);
    JJJ(aaa, bbb, ccc, ddd, eee, chunk[12],  6);

    /* parallel round 2 */
    III(eee, aaa, bbb, ccc, ddd, chunk[ 6],  9); 
    III(ddd, eee, aaa, bbb, ccc, chunk[11], 13);
    III(ccc, ddd, eee, aaa, bbb, chunk[ 3], 15);
    III(bbb, ccc, ddd, eee, aaa, chunk[ 7],  7);
    III(aaa, bbb, ccc, ddd, eee, chunk[ 0], 12);
    III(eee, aaa, bbb, ccc, ddd, chunk[13],  8);
    III(ddd, eee, aaa, bbb, ccc, chunk[ 5],  9);
    III(ccc, ddd, eee, aaa, bbb, chunk[10], 11);
    III(bbb, ccc, ddd, eee, aaa, chunk[14],  7);
    III(aaa, bbb, ccc, ddd, eee, chunk[15],  7);
    III(eee, aaa, bbb, ccc, ddd, chunk[ 8], 12);
    III(ddd, eee, aaa, bbb, ccc, chunk[12],  7);
    III(ccc, ddd, eee, aaa, bbb, chunk[ 4],  6);
    III(bbb, ccc, ddd, eee, aaa, chunk[ 9], 15);
    III(aaa, bbb, ccc, ddd, eee, chunk[ 1], 13);
    III(eee, aaa, bbb, ccc, ddd, chunk[ 2], 11);

    /* parallel round 3 */
    HHH(ddd, eee, aaa, bbb, ccc, chunk[15],  9);
    HHH(ccc, ddd, eee, aaa, bbb, chunk[ 5],  7);
    HHH(bbb, ccc, ddd, eee, aaa, chunk[ 1], 15);
    HHH(aaa, bbb, ccc, ddd, eee, chunk[ 3], 11);
    HHH(eee, aaa, bbb, ccc, ddd, chunk[ 7],  8);
    HHH(ddd, eee, aaa, bbb, ccc, chunk[14],  6);
    HHH(ccc, ddd, eee, aaa, bbb, chunk[ 6],  6);
    HHH(bbb, ccc, ddd, eee, aaa, chunk[ 9], 14);
    HHH(aaa, bbb, ccc, ddd, eee, chunk[11], 12);
    HHH(eee, aaa, bbb, ccc, ddd, chunk[ 8], 13);
    HHH(ddd, eee, aaa, bbb, ccc, chunk[12],  5);
    HHH(ccc, ddd, eee, aaa, bbb, chunk[ 2], 14);
    HHH(bbb, ccc, ddd, eee, aaa, chunk[10], 13);
    HHH(aaa, bbb, ccc, ddd, eee, chunk[ 0], 13);
    HHH(eee, aaa, bbb, ccc, ddd, chunk[ 4],  7);
    HHH(ddd, eee, aaa, bbb, ccc, chunk[13],  5);

    /* parallel round 4 */   
    GGG(ccc, ddd, eee, aaa, bbb, chunk[ 8], 15);
    GGG(bbb, ccc, ddd, eee, aaa, chunk[ 6],  5);
    GGG(aaa, bbb, ccc, ddd, eee, chunk[ 4],  8);
    GGG(eee, aaa, bbb, ccc, ddd, chunk[ 1], 11);
    GGG(ddd, eee, aaa, bbb, ccc, chunk[ 3], 14);
    GGG(ccc, ddd, eee, aaa, bbb, chunk[11], 14);
    GGG(bbb, ccc, ddd, eee, aaa, chunk[15],  6);
    GGG(aaa, bbb, ccc, ddd, eee, chunk[ 0], 14);
    GGG(eee, aaa, bbb, ccc, ddd, chunk[ 5],  6);
    GGG(ddd, eee, aaa, bbb, ccc, chunk[12],  9);
    GGG(ccc, ddd, eee, aaa, bbb, chunk[ 2], 12);
    GGG(bbb, ccc, ddd, eee, aaa, chunk[13],  9);
    GGG(aaa, bbb, ccc, ddd, eee, chunk[ 9], 12);
    GGG(eee, aaa, bbb, ccc, ddd, chunk[ 7],  5);
    GGG(ddd, eee, aaa, bbb, ccc, chunk[10], 15);
    GGG(ccc, ddd, eee, aaa, bbb, chunk[14],  8);

    /* parallel round 5 */
    FFF(bbb, ccc, ddd, eee, aaa, chunk[12] ,  8);
    FFF(aaa, bbb, ccc, ddd, eee, chunk[15] ,  5);
    FFF(eee, aaa, bbb, ccc, ddd, chunk[10] , 12);
    FFF(ddd, eee, aaa, bbb, ccc, chunk[ 4] ,  9);
    FFF(ccc, ddd, eee, aaa, bbb, chunk[ 1] , 12);
    FFF(bbb, ccc, ddd, eee, aaa, chunk[ 5] ,  5);
    FFF(aaa, bbb, ccc, ddd, eee, chunk[ 8] , 14);
    FFF(eee, aaa, bbb, ccc, ddd, chunk[ 7] ,  6);
    FFF(ddd, eee, aaa, bbb, ccc, chunk[ 6] ,  8);
    FFF(ccc, ddd, eee, aaa, bbb, chunk[ 2] , 13);
    FFF(bbb, ccc, ddd, eee, aaa, chunk[13] ,  6);
    FFF(aaa, bbb, ccc, ddd, eee, chunk[14] ,  5);
    FFF(eee, aaa, bbb, ccc, ddd, chunk[ 0] , 15);
    FFF(ddd, eee, aaa, bbb, ccc, chunk[ 3] , 13);
    FFF(ccc, ddd, eee, aaa, bbb, chunk[ 9] , 11);
    FFF(bbb, ccc, ddd, eee, aaa, chunk[11] , 11);

    /* combine results */
    ddd += cc + state[1];
    state[1] = state[2] + dd + eee;
    state[2] = state[3] + ee + aaa;
    state[3] = state[4] + aa + bbb;
    state[4] = state[0] + bb + ccc;
    state[0] = ddd;
}

void RMDfinish(RMD160CTX* context, const uint8_t* message, size_t length)
{
    uint32_t i;
    uint32_t* chunk = context->chunk;
    const uint32_t lo_length = (uint32_t)length;
    const uint32_t hi_length = (uint32_t)(((uint64_t)length) >> 32);

    zeroize(chunk, RMD160_CHUNK_LENGTH * sizeof(uint32_t));

    for (i = 0; i < (lo_length & 63); i++)
    {
        chunk[i >> 2] ^= (uint32_t)*message++ << (8 * (i & 3));
    }

    chunk[(lo_length >> 2) & 15] ^= (uint32_t)1 << (8 * (lo_length & 3) + 7);

    if ((lo_length & 63) > 55)
    {
        RMDcompress(context);
        zeroize(chunk, RMD160_CHUNK_LENGTH * sizeof(uint32_t));
    }

    chunk[14] = lo_length << 3;
    chunk[15] = (lo_length >> 29) | (hi_length << 3);
    RMDcompress(context);
}
