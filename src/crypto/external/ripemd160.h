/********************************************************************\
 *  FILE:     rmd160.c
 *  CONTENTS: A sample C-implementation of the RIPEMD-160 hash-function.
 *  TARGET:   any computer with an ANSI C compiler
 *  AUTHOR:   Antoon Bosselaers, Dept. Electrical Eng.-ESAT/COSIC
 *  DATE:     1 March 1996       VERSION:  1.0
  * ADAPTED:  by Libbitcoin Developers on 7 September 2016
\********************************************************************/
#ifndef LIBBITCOIN_SYSTEM_RIPEMD160_H
#define LIBBITCOIN_SYSTEM_RIPEMD160_H

#include <stdint.h>
#include <stddef.h>

#define RMD160_STATE_LENGTH 5U
#define RMD160_CHUNK_LENGTH 16U
#define RMD160_BLOCK_LENGTH 64U
#define RMD160_DIGEST_LENGTH 20U

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RMD160CTX
{
    uint32_t state[RMD160_STATE_LENGTH];
    uint32_t chunk[RMD160_CHUNK_LENGTH];
} RMD160CTX;

void RMD160(const uint8_t* message, size_t length,
    uint8_t digest[RMD160_DIGEST_LENGTH]);

void RMDInit(RMD160CTX* context);
void RMDUpdate(RMD160CTX* context, const uint8_t* message, size_t length);
void RMDFinal(RMD160CTX* context, uint8_t digest[RMD160_DIGEST_LENGTH]);

#ifdef __cplusplus
}
#endif

#endif
