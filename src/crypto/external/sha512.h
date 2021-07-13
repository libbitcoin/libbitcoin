/* libsodium: crypto_hash_sha512.h, v0.4.5 2014/04/16 */
/**
 * Copyright 2005,2007,2009 Colin Percival. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef LIBBITCOIN_SYSTEM_SHA512_H
#define LIBBITCOIN_SYSTEM_SHA512_H

#include <stdint.h>
#include <stddef.h>

#define SHA512_STATE_LENGTH 8U
#define SHA512_COUNT_LENGTH 2U
#define SHA512_BLOCK_LENGTH 128U
#define SHA512_DIGEST_LENGTH 64U

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct SHA512CTX 
{
    uint64_t state[SHA512_STATE_LENGTH];
    uint64_t count[SHA512_COUNT_LENGTH];
    uint8_t buf[SHA512_BLOCK_LENGTH];
} SHA512CTX;

void SHA512_(const uint8_t* input, size_t length,
    uint8_t digest[SHA512_DIGEST_LENGTH]);

void SHA512Init(SHA512CTX* context);
void SHA512Update(SHA512CTX* context, const uint8_t* input, size_t length);
void SHA512Final(SHA512CTX* context, uint8_t digest[SHA512_DIGEST_LENGTH]);

#ifdef __cplusplus
}
#endif

#endif
