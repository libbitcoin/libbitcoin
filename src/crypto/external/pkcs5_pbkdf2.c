/* OpenBSD: pkcs5_pbkdf2.c, v 1.9 2015/02/05 12:59:57 millert */
/**
 * Copyright (c) 2008 Damien Bergamini <damien.bergamini@free.fr>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "pkcs5_pbkdf2.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "hmac_sha512.h"
#include "zeroize.h"

int pkcs5_pbkdf2(const uint8_t* passphrase, size_t passphrase_length,
    const uint8_t* salt, size_t salt_length, uint8_t* key, size_t key_length,
    size_t iterations)
{
    uint8_t* asalt;
    size_t asalt_size;
    size_t count, index, iteration, length;
    uint8_t buffer[HMACSHA512_DIGEST_LENGTH];
    uint8_t digest1[HMACSHA512_DIGEST_LENGTH];
    uint8_t digest2[HMACSHA512_DIGEST_LENGTH];

    /* An iteration count of 0 is equivalent to a count of 1. */
    /* A key_length of 0 is a no-op. */
    /* A salt_length of 0 is perfectly valid. */

    if (salt_length > SIZE_MAX - 4)
        return -1;
    asalt_size = salt_length + 4;
    asalt = malloc(asalt_size);
    if (asalt == NULL)
        return -1;

    memcpy(asalt, salt, salt_length);
    for (count = 1; key_length > 0; count++)
    {
        asalt[salt_length + 0] = (count >> 24) & 0xff;
        asalt[salt_length + 1] = (count >> 16) & 0xff;
        asalt[salt_length + 2] = (count >> 8) & 0xff;
        asalt[salt_length + 3] = (count >> 0) & 0xff;
        HMACSHA512(asalt, asalt_size, passphrase, passphrase_length, digest1);
        memcpy(buffer, digest1, sizeof(buffer));

        for (iteration = 1; iteration < iterations; iteration++)
        {
            HMACSHA512(digest1, sizeof(digest1), passphrase, passphrase_length,
                digest2);
            memcpy(digest1, digest2, sizeof(digest1));
            for (index = 0; index < sizeof(buffer); index++)
                buffer[index] ^= digest1[index];
        }

        length = (key_length < sizeof(buffer) ? key_length : sizeof(buffer));
        memcpy(key, buffer, length);
        key += length;
        key_length -= length;
    };

    zeroize(digest1, sizeof(digest1));
    zeroize(digest2, sizeof(digest2));
    zeroize(buffer, sizeof(buffer));
    zeroize(asalt, asalt_size);
    free(asalt);

    return 0;
}
