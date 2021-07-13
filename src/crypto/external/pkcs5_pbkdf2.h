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
#ifndef LIBBITCOIN_SYSTEM_PKCS5PBKDF2_H
#define LIBBITCOIN_SYSTEM_PKCS5PBKDF2_H

#include <stddef.h>
#include <stdint.h>
#include "pkcs5_pbkdf2.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Password-Based Key Derivation Function 2 (PKCS #5 v2.0). */
/* Code based on IEEE Std 802.11-2007, Annex H.4.2. */
/* returns 0 if successful. */
int pkcs5_pbkdf2(const uint8_t* passphrase, size_t passphrase_length,
    const uint8_t* salt, size_t salt_length, uint8_t* key, size_t key_length,
    size_t iterations);

#ifdef __cplusplus
}
#endif

#endif
