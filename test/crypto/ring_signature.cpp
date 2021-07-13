/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../test.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(ring_signature_tests)

const hash_digest valid_digest
{
    0xff, 0xff, 0x03, 0x00, 0xfc, 0xff, 0x01, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xe7, 0xff
};
const ec_secret valid_challenge = base16_literal("1f28ee25be3489eb7ae28a294f2b9d99b68be18f184c03ecfea6e7f257b524b3");
const secret_list valid_secrets
{
    base16_literal("0000000000000000000000000000000000000000000000000000000000000001"),
    base16_literal("0000000000000000000000000000000000000000000000000000000000000001"),
    base16_literal("0000000000000000000000000000000000000000000000000000000000000001")
};
const key_rings valid_public_rings
{
    {
        base16_literal("03ffffffffff0100fcffffff3f00000000000000000080ffffffffffffff7f0000"),
        base16_literal("0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")
    },
    {
        base16_literal("0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("020000000000f8ffffff0300000000c0ffffffff3f0000000000000000feffffff"),
        base16_literal("030000000000000000000000fe01f000000000000000000000ffffffffffffffff"),
        base16_literal("02ff3f0000000000ffc3ffff7fc0ffffff01000000300000000000e0ff1f000000"),
        base16_literal("03000000c0ff1f00000000000000f0ffffe1ffffffffffffffffffff0700f8ffff")
    },
    {
        base16_literal("0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("02ff0f00000000000080ffffffffff0f0000000f0080ffffffffffff7f0000fcff"),
        base16_literal("031c00fc0f000000e03f0000000000000000e0ff3fe003000000c0ffff3f0000f8"),
        base16_literal("02ffff0f0000e0ffff030000f0fffffdffffffff1f00f0ff030000000000000000")
    }
};
const secret_list valid_salts
{
    base16_literal("4e7fee78574b38fc965938741ce9619cc640c2d45c47982479680910fdebabc6"),
    base16_literal("8a96b2847f42449e15784c5caa2edd32a18e3980e6641b83bc669194ac09e5ea"),
    base16_literal("0000000000000000000000000000000000000000000000000000000000000001")
};
const ring_signature::proof_list valid_proofs
{
    {
        base16_literal("0000000000000000000000000000000000000000000000000000000000000001"),
        base16_literal("73599254449748740c52233f36af21aed47d410aa1205096b0ca81cc5719711d")
    },
    {
        base16_literal("d015f9207618449baab9031a7841028f95275b0835b9198585b8a27010a34892"),
        base16_literal("950cdb0c59e7f06599558e679b6b8c44c2daa0b152fba1147144f7b412f1942e"),
        base16_literal("83bac40b333bea8bb7b2fe9cf09a6ea2ac5585b1d34c909e8f42593351a4e421"),
        base16_literal("1d156c30721968de8df96c3bec4c1af72f0ae9343677956f8905ea52fa4f6bfd"),
        base16_literal("61c2c9d15190312e29ed9d9c44965496558798a44c35028d6aedba6e750697c9")
    },
    {
        base16_literal("6e62e566a710eaf1c297eb2e31e04c3ec16060ba38072520a42e5bf69a0a31bc"),
        base16_literal("887c829cef8a804953b2ca06424ad7c23df515610f1a03384d23b670f5621a93"),
        base16_literal("64d7b40d58358aba2480e6a9a9d12ef25737f600c1cac624ab39495d1562759b"),
        base16_literal("a41e78ec7947ae153b4eed54780e249730af3e434fb03cc90d307bf7a0f2a1f9")
    }
};

BOOST_AUTO_TEST_CASE(ring_signature__digest__always__valid)
{
    const data_chunk message{ 0x42 };
    const auto expected_digest = base16_literal("0095df12de02a8e238fdb0e0420a294914d687d3021c36af672c251d23ebfd31");
    BOOST_REQUIRE(digest(message, valid_public_rings) == expected_digest);
}

BOOST_AUTO_TEST_CASE(ring_signature__sign__valid__expected_proofs)
{
    ring_signature signature;
    signature.proofs = valid_proofs;
    BOOST_REQUIRE(sign(signature, valid_secrets, valid_public_rings, valid_digest, valid_salts));

    BOOST_REQUIRE(std::equal(signature.proofs[0][0].begin(), signature.proofs[0][0].end(), valid_proofs[0][0].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[0][1].begin(), signature.proofs[0][1].end(), valid_proofs[0][1].begin()));

    BOOST_REQUIRE(std::equal(signature.proofs[1][0].begin(), signature.proofs[1][0].end(), valid_proofs[1][0].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[1][1].begin(), signature.proofs[1][1].end(), valid_proofs[1][1].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[1][2].begin(), signature.proofs[1][2].end(), valid_proofs[1][2].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[1][3].begin(), signature.proofs[1][3].end(), valid_proofs[1][3].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[1][4].begin(), signature.proofs[1][4].end(), valid_proofs[1][4].begin()));

    BOOST_REQUIRE(std::equal(signature.proofs[2][0].begin(), signature.proofs[2][0].end(), valid_proofs[2][0].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[2][1].begin(), signature.proofs[2][1].end(), valid_proofs[2][1].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[2][2].begin(), signature.proofs[2][2].end(), valid_proofs[2][2].begin()));
    BOOST_REQUIRE(std::equal(signature.proofs[2][3].begin(), signature.proofs[2][3].end(), valid_proofs[2][3].begin()));
}

BOOST_AUTO_TEST_CASE(ring_signature__verify__valid__round_trip)
{
    ring_signature signature;
    signature.proofs = valid_proofs;
    BOOST_REQUIRE(sign(signature, valid_secrets, valid_public_rings, valid_digest, valid_salts));
    BOOST_REQUIRE(verify(valid_public_rings, valid_digest, signature));
}

const hash_digest negative_digest
{
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x01, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03
};
const ec_secret negative_challenge = base16_literal("a836477d97b909236d6d964db1c214249de7708a45ce25f1259b4a1056a72c5e");
const secret_list negative_secrets
{
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("243adb50939b958ab7f23ea51d8a6192566d209165327a5f5e2b4e3c0e288cdb"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140")
};
const key_rings negative_public_rings
{
    {
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")
    },
    {
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")
    },
    {
        base16_literal("03ffffffffff01000000fec3070000000000007efefffffdffff7f000000007c00"),
        base16_literal("03ffffffffffff1f00000000781ffc7f00000000ffffffffffffffff0f00000000"),
        base16_literal("03000000feffffffff0f00000000000002f0ffffffffffffffffffff3f0000f0ff"),
        base16_literal("0300000000000000000000000000fce0ff0300000000f8ff0000c0ffffffffff00"),
        base16_literal("027ff0ffffffffffffffffffff0100000000e0fffffffffffffffff77f000000e0"),
        base16_literal("0380ffffff07f801ffff3f000000fcffffffffffffffff9fffffffffffffffffff"),
        base16_literal("03000000ffffffffffff03f0ffffffffffffffffff0300000000000080ffff0100"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")
    },
    {
        base16_literal("020ba7bcef897d0bb189df8ade24875be00d59f90c2e0711a69f578b70578f1673"),
        base16_literal("02000000000000fcffffffffffff1f00000000fcffffffffffffffffffffff0f00")
    },
    {
        base16_literal("0300e0ffffff07000000000000000000000000000000000020ffffffff7ff00f00"),
        base16_literal("0300f8fff8ffffffffffffffffffff7f000000401c000000c00780ffff1ff083ff"),
        base16_literal("0200f0ffff7f00000000feffffffffffffffffff0100000000000000000000ffff"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("02ffffffffff01000000f0ffffffff03c0ffffffffffffff0f000f0000f0ff0f00"),
        base16_literal("03ffffffffff7f1f00000000feffffffffffffffffffffff3ff8ffffffffff3f00")
    },
    {
        base16_literal("020000000000e0dfffff0700000000c0ffffffff0700000000000000c0ffffffff"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("02000000e0ffff1f0000000000c001000000000000e0ffff0f0000f0ffffffffff"),
        base16_literal("03000000000080ffff0780ffffffffffffffffffffffff3f000000000000000000"),
        base16_literal("02ffff03000000000000000000ff7f80ff1f00000000ffffffffffffffffffff07")
    },
    {
        base16_literal("020010000000000000000000feff03000000e0ffffffff0f00c0ffffffffffff00"),
        base16_literal("0300ffffffffff07f080ff00f80300ffffffffff7f1f80ff000000feffffffffe0"),
        base16_literal("020080ff7ff8ffffffffffffffffffffffffff07ffffffff01ffffffffffffffff"),
        base16_literal("03ffffff7f000000f0ffffffe0ffffffffffffffffffffffc7ffffcf7f00000000"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")
    },
    {
        base16_literal("02ffffffffffffffffff0100e01f0000fcff000000f0ffffffffffffffffffffff"),
        base16_literal("03ffffff0ff8ffffffffffff3f00801f00f83f0000feffffff03feffffff0100fc"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("020000000000e0ffffffffff0000fcffffff0f00000000000000000000e00f0000"),
        base16_literal("02ffffffffffffffffffffffff030000c0ffffffffffff1f00000000fcff000000")
    }
};
const secret_list negative_salts
{
    base16_literal("9af5c0ace6b29c160f2ebd6622a179166e2ac0db780e4ff4ed7f124edb773bee"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("b02fd19f60451740f49f44c5c3debb1339fd2d7c269f424c11982cb079659f80"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140")
};
const ring_signature::proof_list negative_proofs
{
    {
        base16_literal("62c5c496fffc77445f82fe8194bf6b98d705b6c64411fdef42de76bdbd063d30")
    },
    {
        base16_literal("7b2c9ff6fe007e5bc8eb676183cff7482de9bd430776bf53c0895e9c39d529d7")
    },
    {
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("0000000000000000000000000000000000000000000000000000000000000001"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("8f75ca7bf400d5327faceaf568ae0e7eae393ee09d8ccce0bac9623bb33bff6b"),
        base16_literal("5af9c197a5f557fa6f5de5b53b2869f380fb8cbaa41f97b73fe6f094adff072f")
    },
    {
        base16_literal("374633c78568287a243efdbf541d8ce2bf8c2f11181dfce2317322ab64c9adc2"),
        base16_literal("e1264d2ad0628dc80b3e17d66d70f4af490b080f3fbefb5b5ca52d2d884be6ed")
    },
    {
        base16_literal("05c945a3efcdf53b288e10bfed8d73905ae0dd6c9e92bf9b6eaa09a1fa085131"),
        base16_literal("d8e7484a4b36bc45ab81f59031f7d4b2c3f2598281ebab85520b2fd08168b146"),
        base16_literal("b87cc5518c9caf33508ea997c0ce5fa5aff0297e18284f79d0005c37f41268ff"),
        base16_literal("e68ec17083ffa4c35075c66e482c3d8b800fe1dc26afc02686728714462fa82d"),
        base16_literal("19798cd7a538a8d340f594b3a7f425584a1d0b372d649fcdc69d9f3bc8f95cb6"),
        base16_literal("598f3d7fc1b2ca08164069d49b16667a067315643adc35a867b516ae0f436118")
    },
    {
        base16_literal("ef3ba8f8ee60b27c61364d5095a9f0c06ee21b24e0c8de215469d5b4fe7cdca9"),
        base16_literal("5ed183cb45347cf1c7aeb225996497bde1ed5a5b7ad50036069d52a064b1a7e5"),
        base16_literal("57182423e0c13298fa2012843fe5d207afbf801d45c8ccd5793d51b006311ac8"),
        base16_literal("8ae7373739b960c7cc3306afc0c6491d7bd842c99032e502d692eeb5f5787277"),
        base16_literal("d04a2b7608004c4453b4c41f62c615356cb495c9186662e7798b257f1bbbb71e")
    },
    {
        base16_literal("87ad1098fbf8df45ae812c877caf25bdc990da3a7c5866a00b62689f1f4bcf00"),
        base16_literal("12983d2e21c8010ce96b8b5d6b1e80f5a2f0c3ab9e63099b3a7b5920206bd74e"),
        base16_literal("3604b87027d8ee01b06c8453b6b97e7b4cb3a891145b8f634aab34b855a39366"),
        base16_literal("36f4510553dfc5fd2e23606f611add0437271e3b3896658f3d9b2e483bd85873"),
        base16_literal("c536ed86506ff11132189e7b2d994217c45177f2b01f18fb54243dc0bb7e73ec")
    },
    {
        base16_literal("09751fb8229462c9239e224e6fa2fbc13dcda8a696714dc4a32056385e5e6719"),
        base16_literal("7f82130379fc31c64cdd29d653e18eea66698c43da7b4fa3301e4cf4335c736e"),
        base16_literal("2344cd1fda5965f4fef9116b2eb4dedd1fcc76a8dcbea576f3c3fd6a5c3f1340"),
        base16_literal("ce3502240efe247b6999464201eb268cb63777927d5da5351fbe1c0e8381287f"),
        base16_literal("4434a6c164af4c178b32f8cae800f3b43b409b046e8e2206455d75853ad4ebf0")
    }
};

BOOST_AUTO_TEST_CASE(ring_signature__verify__negative__false)
{
    ring_signature signature;
    signature.challenge = negative_challenge;
    signature.proofs = negative_proofs;
    BOOST_REQUIRE(!verify(negative_public_rings, negative_digest, signature));
}

const hash_digest faulty_digest
{
    0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0x01, 0x80, 0x9f,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00
};
const ec_secret faulty_challenge = base16_literal("45b5179e0d0d2333028e07ff067a610ecdcd997af8b07ef394142152eb98cf0e");
const secret_list faulty_secrets
{
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140")
};
const key_rings faulty_public_rings
{
    {
        base16_literal("02003f000000ffffffffffffffff07000000000f0000000000000000000000e01f"),
        base16_literal("03000000f8ffff0f00f807ffffffffffffffffff7f00fcffffffffffff00000000"),
        base16_literal("030000000000f8ffffff010000000780ffffffffffff00000000001f0000000000"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("030000000000000000000000fc0f1e00000000feffffffffc1ff1f00f0ffff3f00"),
        base16_literal("03e0ffffffffffff0fe0ff000000e0ffffffffff3f000000fe7f7f00c0ffff3f00"),
        base16_literal("02ffffff1ffc0f00000000feffffff0f000000c0ffffffffffffffffffffffffff")
    },
    {
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("0300e0ff000000000000000080ffff3f00000000000000000000f0ffffffff1f00"),
        base16_literal("02ffff0700000000000000f8ffffffffff7fc07fe0ffffffffff1ff8ffffffffff"),
        base16_literal("021f0000000000e0ff070000000000ffffffffff070000c0f0ffffffffffffffff"),
        base16_literal("02ffffffffffffffff03000000ff0000000000fcffffffff0f00000080ffffffff")
    },
    {
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("03ffffffffffffffff0100000000000000000000000000000000000000feffffff"),
        base16_literal("020300000000010000000000f0ffffffff7f00000000f03f1c0000e0ffffff0700"),
        base16_literal("0300000000000000fcffc31f0000000000000000f0ffffffff1f00e00f0000ff07")
    },
    {
        base16_literal("03c0ff0000000080ffffffffffffffff3f00000000000000f0ffffff7f000080ff"),
        base16_literal("03ffffff3f000080ff07000000000000000000fcfcfff8ffff0f0000000f000000"),
        base16_literal("02ff7f00feff030000f00300000080ffff0300e00300000000fcff1f0000fc0000"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("02ffffff1fc0ffff0f0000000000feffffffffff07000000000000000000000000"),
        base16_literal("033f0000e0010000f8ffffffffff0f000000000000ffff3f00803ffcffffffffff"),
        base16_literal("03000000e00fe0ffffffffffffffffffffffff1f000000000000ffffffffff01f0"),
        base16_literal("03ffff1f0000000000000000000000f8fffffffffffffff8ffffffff0ff0ffffff")
    },
    {
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("030000000000000000000000c0ffff07000000000000f03f000000f0ffffffff0f")
    },
    {
        base16_literal("021f000000000000e0ff1f0000000000000000000000ffffff01000000000000e0"),
        base16_literal("0379be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        base16_literal("033e000000c0ffffffffffff010080ffffffff03c0ffffffffffffff0f00fcffff"),
        base16_literal("0200f8ffffffff3fc0ffff1fffffffff07000000000000f8ffffffffffffffffff"),
        base16_literal("02ff07000000fcffffff81ffffffff0f00000000000000f8ffffffffffffffff07"),
        base16_literal("03153507217dd616467c0d7fac4ba26e6a19301ec42af90fcced2d8c21587fc94c"),
        base16_literal("02ffffffff3ffefeffffff030000ffffffffff01f8ffffffffffffffffffffffff")
    }
};
const secret_list faulty_salts
{
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("adf0884a284fd7cc3d5fbc9ac7f11444d191ed755c7efce9de2ed32419c077a8"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
    base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140")
};
const ring_signature::proof_list faulty_proofs
{
    {
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("9a1bf083238f72fb8a462c8a95a6c57e2c7604e62b1fcb11cd77f83f2d5ae948"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364140"),
        base16_literal("77874189e8c61c8368fa03788b4c4135dc3d0e3b3072e106d73c0c245b61bb05")
    },
    {
        base16_literal("fab475465357ee38935b207cae07bc9a7c772602ef8ac8f9455c8dcadf4193a8"),
        base16_literal("528406a16c2b97c3a2ce7ad2acdbe661e66870a3ea62a1eee03a6339a0766f86"),
        base16_literal("0834d917161710bb878fcf3d893f057b9fa93fdf7fec6ab14d97d54dac0fbbc9"),
        base16_literal("37ce142bb62527f00e2420f2306f82a3dfafa5aca9efa41de8028cf18a26cdb8"),
        base16_literal("3d904603764dd3b0fae6add16ef56d8eb92adef06d6ce1ac35952534fc03d306")
    },
    {
        base16_literal("b75c75d4173361eb79b352bce9a28f560ba244dc32898960cb8498213fe80913"),
        base16_literal("d11629471c239d91a4062e24b615d90da8dfac413a20adc1b75a8f8cfc0a9667"),
        base16_literal("8b2c569f006c328a65309274e228410aad7af708008459d1e6f991ec3b759d22"),
        base16_literal("f048b32158025c56eda95a52f560ae495f0bba2f42df57f6c4da1b70e1713cff")
    },
    {
        base16_literal("8f4611080e923802f677edaf88ae45065da6153ccb553b7d8546eccd189a0070"),
        base16_literal("e63f5e66e9d02ac72037bbe09fd4754237b124941bb310d0c750aecb8908fad8"),
        base16_literal("24880a1db35a7b5979a9e1e6b459d0b57876b97c17fb963c2fb1d9ffc46712d3"),
        base16_literal("c7f112bd31779769e54f6b7c9229d5aab525344ccbe1ce9e3a5d481f0304901f"),
        base16_literal("13072b73be9ba83c3c079275f8cfe11b186070ea810cd7b593a3a6679305767b"),
        base16_literal("7e7403b1a54435a912dfb20364bc4245abcaf5e61ce4d0cc9c7e426091f9566f"),
        base16_literal("a6870ec4938e59003c16c4ef03032bc8ce2bea16ddc0555d4da3171036904f81"),
        base16_literal("b76ac04fbaf87c6af4b81d70189bd8f3935320004186e5176921ebbf90969d3e")
    },
    {
        base16_literal("7efe7844ff4efcf5704e519bce2daea3864b598a37970fdc02dcddf5d9a9ebd9"),
        base16_literal("39e7d3d66968988b4fabab819b83e81382d57b106e51be2b18c47e956e5061c2")
    },
    {
        base16_literal("5e6ab9d7ff93582ef3e7469e4ad5ce2edac9870377d31917ed4a84f956dbda84"),
        base16_literal("394fdffe8bc4b4e11cab523fc22dca88a046e05a389a5b04966dd20bc070f169"),
        base16_literal("3834e0b1ea78347091dd01695e7d01d73a5b57d488e93240067524a594b7eb6b"),
        base16_literal("837aa228d06e07516bdd5e6b38501909a005de5122c517fc832f3b944b288704"),
        base16_literal("2ccecc768c258d07d62473033a544e034f82fcfbe4486034f56cbf9b601ff6d6"),
        base16_literal("4f6f0cf449f045e5ac2f62aed2597350e04a83f268fef31ece0f12c40e7bcaf7"),
        base16_literal("8d439a379794f5fcb04fe0bd6623e7ccfb0bfc82ebf7cae6bb2732528a6bc496")
    }
};

BOOST_AUTO_TEST_CASE(ring_signature__verify__faulty__false)
{
    ring_signature signature;
    signature.challenge = faulty_challenge;
    signature.proofs = faulty_proofs;
    BOOST_REQUIRE(!verify(faulty_public_rings, faulty_digest, signature));
}

BOOST_AUTO_TEST_SUITE_END()
