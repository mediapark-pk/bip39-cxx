#ifndef MNEMONIC_H
#define MNEMONIC_H

#include <bitset>
#include <stdexcept>
#include <string>
#include <vector>

class MnemonicException : public std::runtime_error
{
public:
    MnemonicException(std::string&& msg) : std::runtime_error{std::move(msg)} {}
    const char* what() const noexcept
    {
        return std::runtime_error::what();
    }
};

class Mnemonic
{
public:
    Mnemonic() = default;
    std::vector<uint8_t> generateSeed(const std::string& passphrase = "");

    std::string entropy;
    std::vector<int> wordsIndex;
    std::vector<std::string> words;
    std::vector<std::bitset<11>> rawBinaryChunks;
    int m_wordsCount{};

private:
    static constexpr int BIP39_SEED_LEN_512 = 64;
};

#endif // MNEMONIC_H
