#include "mnemonic.h"
#include "pbkdf2_sha512/pbkdf2.hpp"
#include "utils.h"

#include <cstring>

std::vector<uint8_t> Mnemonic::generateSeed(const std::string& passphrase)
{
    std::string pass{BIP39_Utils::Join(words, " ")};
    std::string salt{"mnemonic" + passphrase};
    std::vector<uint8_t> output(BIP39_SEED_LEN_512);

    static constexpr int rounds = 2048;

    pbkdf2_hmac_sha512(
        reinterpret_cast<const uint8_t*>(pass.c_str()),
        pass.length(),
        reinterpret_cast<const uint8_t*>(salt.c_str()),
        salt.length(),
        rounds,
        &output[0]);
    return output;
}
