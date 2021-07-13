/**
 *  sha1.c
 *
 *  Description:
 *      This file implements the Secure Hashing Algorithm 1 as
 *      defined in FIPS PUB 180-1 published April 17, 1995.
 *
 *      The SHA-1, produces a 160-bit message digest for a given
 *      data stream.  It should take about 2**n steps to find a
 *      message with the same digest as a given message and
 *      2**(n/2) to find any two messages with the same digest,
 *      when n is the digest size in bits.  Therefore, this
 *      algorithm can serve as a means of providing a
 *      "fingerprint" for a message.
 *
 *  Portability Issues:
 *      SHA-1 is defined in terms of 32-bit "words".  This code
 *      uses <stdint.h> (included via "sha1.h" to define 32 and 8
 *      bit unsigned integer types.  If your C compiler does not
 *      support 32 bit unsigned integers, this code is not
 *      appropriate.
 *
 *  Caveats:
 *      SHA-1 is designed to work with messages less than 2^64 bits
 *      long.  Although SHA-1 allows a message digest to be generated
 *      for messages of any number of bits less than 2^64, this
 *      implementation only works with messages with a length that is
 *      a multiple of the size of an 8-bit character.
 *
 * Adapted:
 *      by Libbitcoin Developers on 7 September 2016
 */
#include "sha1.h"

#include <stdint.h>
#include <stddef.h>

#define SHIFT(bits, word) \
    (((word) << (bits)) | ((word) >> (32 - (bits))))

void SHA1PadMessage(SHA1CTX* context);
void SHA1ProcessMessageBlock(SHA1CTX* context);

void SHA1_(const uint8_t* message, size_t length,
    uint8_t digest[SHA1_DIGEST_LENGTH])
{
    SHA1CTX context;
    SHA1Init(&context);
    SHA1Update(&context, message, length);
    SHA1Final(&context, digest);
}

void SHA1Init(SHA1CTX* context)
{
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->length = 0;
    context->index = 0;
}

void SHA1Update(SHA1CTX* context, const uint8_t* message, size_t length)
{
    /* Guard against overflow in while loop (returns digest of empty message). */
    if (length > SIZE_MAX / 8)
        return;

    while (length--)
    {
        context->block[context->index++] = (*message & 0xFF);
        context->length += 8;

        if (context->index == 64)
        {
            SHA1ProcessMessageBlock(context);
        }

        message++;
    }
}

void SHA1Final(SHA1CTX* context, uint8_t digest[SHA1_DIGEST_LENGTH])
{
    size_t i;

    SHA1PadMessage(context);

    for (i = 0; i < SHA1_BLOCK_LENGTH; ++i)
    {
        context->block[i] = 0;
    }

    for (i = 0; i < SHA1_DIGEST_LENGTH; ++i)
    {
        digest[i] = context->state[i >> 2] >> 8 * (3 - (i & 0x03));
    }
}

/* Local */

void SHA1ProcessMessageBlock(SHA1CTX* context)
{
    const uint32_t K[] =
    {
        0x5A827999,
        0x6ED9EBA1,
        0x8F1BBCDC,
        0xCA62C1D6
    };

    size_t t; 
    uint32_t temp;
    uint32_t W[80];
    uint32_t A, B, C, D, E;

    for (t = 0; t < 16; t++)
    {
        W[t]  = context->block[t * 4 + 0] << 24;
        W[t] |= context->block[t * 4 + 1] << 16;
        W[t] |= context->block[t * 4 + 2] << 8;
        W[t] |= context->block[t * 4 + 3] << 0;
    }

    for (t = 16; t < 80; t++)
    {
       W[t] = SHIFT(1, W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

    A = context->state[0];
    B = context->state[1];
    C = context->state[2];
    D = context->state[3];
    E = context->state[4];

    for (t = 0; t < 20; t++)
    {
        temp = SHIFT(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = SHIFT(30, B);

        B = A;
        A = temp;
    }

    for (t = 20; t < 40; t++)
    {
        temp = SHIFT(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHIFT(30, B);
        B = A;
        A = temp;
    }

    for (t = 40; t < 60; t++)
    {
        temp = SHIFT(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHIFT(30, B);
        B = A;
        A = temp;
    }

    for (t = 60; t < 80; t++)
    {
        temp = SHIFT(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHIFT(30, B);
        B = A;
        A = temp;
    }

    context->state[0] += A;
    context->state[1] += B;
    context->state[2] += C;
    context->state[3] += D;
    context->state[4] += E;
    context->index = 0;
}

void SHA1PadMessage(SHA1CTX* context)
{
    uint32_t lo_length;
    uint32_t hi_length;

    if (context->index > 55)
    {
        context->block [context->index++] = 0x80;

        while (context->index < 64)
        {
            context->block[context->index++] = 0;
        }

        SHA1ProcessMessageBlock(context);

        while (context->index < 56)
        {
            context->block[context->index++] = 0;
        }
    }
    else
    {
        context->block[context->index++] = 0x80;

        while (context->index < 56)
        {
            context->block[context->index++] = 0;
        }
    }

    lo_length = (uint32_t)(context->length);
    hi_length = (uint32_t)(((uint64_t)context->length) >> 32);

    context->block[56] = hi_length >> 24;
    context->block[57] = hi_length >> 16;
    context->block[58] = hi_length >> 8;
    context->block[59] = hi_length >> 0;

    context->block[60] = lo_length >> 24;
    context->block[61] = lo_length >> 16;
    context->block[62] = lo_length >> 8;
    context->block[63] = lo_length >> 0;

    SHA1ProcessMessageBlock(context);
}
