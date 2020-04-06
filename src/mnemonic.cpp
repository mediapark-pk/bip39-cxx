#include "mnemonic.h"
#include "pbkdf2_sha512/pbkdf2.hpp"
#include "utils.h"

#include <cstring>

Mnemonic::Mnemonic(const std::string& entropy) : m_entropy{entropy}, m_wordsCount{0} {}

std::string Mnemonic::generateSeed(const std::string& passphrase, int bytes)
{
    std::string pass = BIP39_Utils::Join(m_words, " ");
    std::string salt = (std::string{"mnemonic"} + passphrase);
    std::vector<uint8_t> output;
    output.resize(BIP39_SEED_LEN_512);

    static constexpr int rounds = 2048;

    pbkdf2_hmac_sha512(
        reinterpret_cast<const uint8_t*>(pass.c_str()),
        pass.length(),
        reinterpret_cast<const uint8_t*>(salt.c_str()),
        salt.length(),
        rounds,
        &output[0]);

    std::string out{output.begin(), output.end()};
    return out;
}

void Mnemonic::setWords(const std::vector<std::string>& words)
{
    m_words = words;
}

std::vector<std::string> Mnemonic::words() const
{
    return m_words;
}

std::vector<std::bitset<11>> Mnemonic::rawBinaryChunks() const
{
    return m_rawBinaryChunks;
}

std::vector<int> Mnemonic::wordsIndex() const
{
    return m_wordsIndex;
}

void Mnemonic::setWordsIndex(const std::vector<int>& wordsIndex)
{
    m_wordsIndex = wordsIndex;
}

void Mnemonic::appendWord(std::string word)
{
    m_words.emplace_back(std::move(word));
}

void Mnemonic::appendWordIndex(int wordIndex)
{
    m_wordsIndex.emplace_back(wordIndex);
}

void Mnemonic::appendBinaryChunk(std::bitset<11> chunk)
{
    m_rawBinaryChunks.emplace_back(std::move(chunk));
}

void Mnemonic::incrementCount()
{
    ++m_wordsCount;
}

std::string Mnemonic::entropy() const
{
    return m_entropy;
}

void Mnemonic::setEntropy(const std::string &entropy)
{
    m_entropy = entropy;
}
