/* libsodium: crypto_auth_hmacsha512.h, v0.4.5 2014/04/16 */
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
#ifndef LIBBITCOIN_SYSTEM_HMACSHA512_H
#define LIBBITCOIN_SYSTEM_HMACSHA512_H

#include <stdint.h>
#include <stddef.h>
#include "sha512.h"

#define HMACSHA512_DIGEST_LENGTH 64U

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct HMACSHA512CTX
{
    SHA512CTX ictx;
    SHA512CTX octx;
} HMACSHA512CTX;

void HMACSHA512(const uint8_t* input, size_t length, const uint8_t* key,
    size_t key_length, uint8_t digest[HMACSHA512_DIGEST_LENGTH]);

void HMACSHA512Final(HMACSHA512CTX* context,
    uint8_t digest[HMACSHA512_DIGEST_LENGTH]);

void HMACSHA512Init(HMACSHA512CTX* context, const uint8_t* key,
    size_t key_length);

void HMACSHA512Update(HMACSHA512CTX* context, const uint8_t* input,
    size_t length);

#ifdef __cplusplus
}
#endif

#endif
