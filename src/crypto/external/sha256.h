/* libsodium: crypto_hash_sha256.h, v0.4.5 2014/04/16 */
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
#ifndef LIBBITCOIN_SYSTEM_SHA256_H
#define LIBBITCOIN_SYSTEM_SHA256_H

#include <stdint.h>
#include <stddef.h>

#define SHA256_STATE_LENGTH 8U
#define SHA256_COUNT_LENGTH 2U
#define SHA256_BLOCK_LENGTH 64U
#define SHA256_DIGEST_LENGTH 32U

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct SHA256CTX
{
    uint32_t state[SHA256_STATE_LENGTH];
    uint32_t count[SHA256_COUNT_LENGTH];
    uint8_t buf[SHA256_BLOCK_LENGTH];
} SHA256CTX;

void SHA256_(const uint8_t* input, size_t length,
    uint8_t digest[SHA256_DIGEST_LENGTH]);

void SHA256Init(SHA256CTX* context);
void SHA256Update(SHA256CTX* context, const uint8_t* input, size_t length);
void SHA256Final(SHA256CTX* context, uint8_t digest[SHA256_DIGEST_LENGTH]);

#ifdef __cplusplus
}
#endif

#endif
