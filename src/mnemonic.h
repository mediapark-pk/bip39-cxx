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
    Mnemonic(const std::string& entropy);
    std::string generateSeed(const std::string& passphrase = "", int bytes = 0);

    std::string m_entropy;
    std::vector<int> m_wordsIndex;
    std::vector<std::string> m_words;
    std::vector<std::bitset<11>> m_rawBinaryChunks;
    int m_wordsCount;

    std::string entropy() const;

    std::vector<std::string> words() const;

private:
    static constexpr int BIP39_SEED_LEN_512 = 64;
};

#endif // MNEMONIC_H
