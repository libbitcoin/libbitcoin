/* libsodium: hmac_hmacsha256.c, v0.4.5 2014/04/16 */
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
#include "hmac_sha256.h"

#include <stdint.h>
#include <string.h>
#include "sha256.h"
#include "zeroize.h"

void HMACSHA256(const uint8_t* input, size_t length, const uint8_t* key,
    size_t key_length, uint8_t digest[HMACSHA256_DIGEST_LENGTH])
{
    HMACSHA256CTX context;
    HMACSHA256Init(&context, key, key_length);
    HMACSHA256Update(&context, input, length);
    HMACSHA256Final(&context, digest);
}

void HMACSHA256Final(HMACSHA256CTX* context, 
    uint8_t digest[HMACSHA256_DIGEST_LENGTH])
{
    uint8_t hash[HMACSHA256_DIGEST_LENGTH];

    SHA256Final(&context->ictx, hash);
    SHA256Update(&context->octx, hash, HMACSHA256_DIGEST_LENGTH);
    SHA256Final(&context->octx, digest);

    zeroize((void*)hash, sizeof hash);
}

void HMACSHA256Init(HMACSHA256CTX* context, const uint8_t* key, 
    size_t key_length)
{
    size_t i;
    uint8_t pad[SHA256_BLOCK_LENGTH];
    uint8_t key_hash[SHA256_DIGEST_LENGTH];

    if (key_length > SHA256_BLOCK_LENGTH)
    {
        SHA256Init(&context->ictx);
        SHA256Update(&context->ictx, key, key_length);
        SHA256Final(&context->ictx, key_hash);
        key = key_hash;
        key_length = SHA256_DIGEST_LENGTH;
    }

    SHA256Init(&context->ictx);
    fill(pad, SHA256_BLOCK_LENGTH, 0x36);

    for (i = 0; i < key_length; i++) 
        pad[i] ^= key[i];

    SHA256Update(&context->ictx, pad, SHA256_BLOCK_LENGTH);
    SHA256Init(&context->octx);
    fill(pad, SHA256_BLOCK_LENGTH, 0x5c);

    for (i = 0; i < key_length; i++) 
        pad[i] ^= key[i];

    SHA256Update(&context->octx, pad, SHA256_BLOCK_LENGTH);
    zeroize((void*)key_hash, sizeof key_hash);
}

void HMACSHA256Update(HMACSHA256CTX* context, const uint8_t* input,
    size_t length)
{
    SHA256Update(&context->ictx, input, length);
}
