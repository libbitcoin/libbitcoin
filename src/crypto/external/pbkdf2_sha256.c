/**
 * Copyright 2005,2007,2009 Colin Percival
 * All rights reserved.
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
#include "pbkdf2_sha256.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "hmac_sha256.h"
#include "zeroize.h"

static inline void be32enc(void* pp, uint32_t x)
{
    uint8_t* p = (uint8_t*)pp;

    p[3] = x & 0xff;
    p[2] = (x >> 8) & 0xff;
    p[1] = (x >> 16) & 0xff;
    p[0] = (x >> 24) & 0xff;
}

/**
 * pbkdf2_sha256(passphrase, passphrase_length, salt, salt_length, c, buf, dk_length):
 * Compute pbkdf2(passwd, salt, c, dkLen) using hmac_sha256 as the PRF, and
 * write the output to buf.  The value dkLen must be at most 32 * (2^32 - 1).
 */
void pbkdf2_sha256(const uint8_t* passphrase, size_t passphrase_length,
    const uint8_t* salt, size_t salt_length, uint64_t c,
    uint8_t* buf, size_t dk_length)
{
    uint8_t ivec[4];
    uint8_t U[32], T[32];
    size_t i, j, k, clen;
    HMACSHA256CTX PShctx, hctx;

    HMACSHA256Init(&PShctx, passphrase, passphrase_length);
    HMACSHA256Update(&PShctx, salt, salt_length);

    for (i = 0; i * 32 < dk_length; i++) {
        be32enc(ivec, (uint32_t)(i + 1));
        memcpy(&hctx, &PShctx, sizeof(HMACSHA256CTX));
        HMACSHA256Update(&hctx, ivec, 4);
        HMACSHA256Final(&hctx, U);

        memcpy(T, U, 32);
        /* LCOV_EXCL_START */
        for (j = 2; j <= c; j++) {
            HMACSHA256Init(&hctx, passphrase, passphrase_length);
            HMACSHA256Update(&hctx, U, 32);
            HMACSHA256Final(&hctx, U);

            for (k = 0; k < 32; k++) {
                T[k] ^= U[k];
            }
        }
        /* LCOV_EXCL_STOP */

        clen = dk_length - i * 32;
        if (clen > 32) {
            clen = 32;
        }
        memcpy(&buf[i * 32], T, clen);
    }
    zeroize(&PShctx, sizeof PShctx);
}
