/**
 *  sha1.h
 *
 *  Description:
 *      This is the header file for code which implements the Secure
 *      Hashing Algorithm 1 as defined in FIPS PUB 180-1 published
 *      April 17, 1995.
 *
 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the names
 *      used in the publication.
 *
 *      Please read the file sha1.c for more information.
 *
 * Adapted:
 *      by Libbitcoin Developers on 7 September 2016
 */
#ifndef LIBBITCOIN_SYSTEM_SHA1_H
#define LIBBITCOIN_SYSTEM_SHA1_H

#include <stdint.h>
#include <stddef.h>

#define SHA1_STATE_LENGTH 5U
#define SHA1_BLOCK_LENGTH 64U
#define SHA1_DIGEST_LENGTH 20U

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct SHA1CTX
{
    uint32_t state[SHA1_STATE_LENGTH];
    uint8_t block[SHA1_BLOCK_LENGTH];
    size_t length;
    size_t index;
} SHA1CTX;

void SHA1_(const uint8_t* message, size_t length,
    uint8_t digest[SHA1_DIGEST_LENGTH]);

void SHA1Init(SHA1CTX* context);
void SHA1Update(SHA1CTX* context, const uint8_t* message, size_t length);
void SHA1Final(SHA1CTX* context, uint8_t digest[SHA1_DIGEST_LENGTH]);

#ifdef __cplusplus
}
#endif

#endif
