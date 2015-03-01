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
#include <bitcoin/bitcoin/math/external/pkcs5_pbkdf2.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <bitcoin/bitcoin/math/external/hmac_sha512.h>
#include <bitcoin/bitcoin/math/external/zeroize.h>

int pkcs5_pbkdf2(const char* passphrase, size_t passphrase_length,
    const uint8_t* salt, size_t salt_length, uint8_t* key, size_t key_length,
    uint32_t rounds)
{
    size_t length;
    uint8_t* asalt;
    size_t asalt_size;
    uint32_t count, round, buffer_index;
    uint8_t buffer[HMACSHA512_DIGEST_LENGTH];
    uint8_t digest1[HMACSHA512_DIGEST_LENGTH];
    uint8_t digest2[HMACSHA512_DIGEST_LENGTH];
    size_t buffer_size = sizeof(buffer);

    if (rounds == 0 || key_length == 0)
        return -1;

    if (salt_length == 0 || salt_length > SIZE_MAX - 4)
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
        memcpy(buffer, digest1, buffer_size);

        for (round = 1; round < rounds; round++)
        {
            HMACSHA512(digest1, sizeof(digest1), passphrase, passphrase_length,
                digest2);
            memcpy(digest1, digest2, sizeof(digest1));
            for (buffer_index = 0; buffer_index < buffer_size; buffer_index++)
                buffer[buffer_index] ^= digest1[buffer_index];
        }

        length = (key_length < buffer_size ? key_length : buffer_size);
        memcpy(key, buffer, length);
        key += length;
        key_length -= length;
    };

    zeroize(digest1, sizeof(digest1));
    zeroize(digest2, sizeof(digest2));
    zeroize(buffer, buffer_size);
    zeroize(asalt, asalt_size);
    free(asalt);

    return 0;
}
