#ifndef BIP39_H
#define BIP39_H

#include <string>
#include <vector>

#include "wordlist.h"


class Mnemonic;

class BIP39
{
public:
    BIP39(int wordCount = 12);

    static Mnemonic Entropy(const std::string& entropy);
    static Mnemonic Generate(int wordCount);
    static void validateEntropy(const std::string& entropy);
    static Mnemonic Words(const std::string& words, Wordlist wordlist, bool verifyChecksum = true);
    Mnemonic reverse(std::vector<std::string> words, bool verifyChecksum = true);

    BIP39 useEntropy(const std::string& entropy);

    BIP39 generateSecureEntropy();

    Mnemonic mnemonic();

    BIP39 wordList(Wordlist wordlist);

    std::string hex2bits(const std::string& hex);
    std::string bits2hex(const std::string& bits);
    std::string checksum(const std::string& entropy);
    static constexpr size_t len_to_mask(size_t len) noexcept;

private:
    int m_wordsCount;
    int m_overallBits;
    int m_checksumBits;
    int m_entropyBits;
    std::string m_entropy;
    std::string m_checksum;
    std::vector<std::string> m_rawBinaryChunks;
    std::vector<std::string> m_words;
    Wordlist m_wordList;
};

#endif // BIP39_H
