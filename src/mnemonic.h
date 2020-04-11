#ifndef MNEMONIC_H
#define MNEMONIC_H

#include <bitset>
#include <string>
#include <vector>

class MnemonicException
{
public:
    MnemonicException(std::string&& desc) : m_desc{std::move(desc)} {}
    std::string what() const
    {
        return m_desc;
    }

private:
    std::string m_desc;
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
