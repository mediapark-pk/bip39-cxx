#include "bip39.h"
#include "mnemonic.h"
#include "pbkdf2_sha512/sha2.hpp"
#include "utils.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <linux/random.h>
#include <sstream>
#include <sys/syscall.h>
#include <unistd.h>

BIP39::BIP39(int wordCount)
{
    if (wordCount < 12 || wordCount > 24) {
        //        throw
    } else if (wordCount % 3 != 0) {
        //        throw
    }
    // Actual words count
    m_wordsCount = wordCount;
    // Overall entropy bits (ENT+CS)
    m_overallBits = m_wordsCount * 11;
    // Checksum Bits are 1 bit per 3 words, starting from 12 words with 4 CS bits
    m_checksumBits = ((m_wordsCount - 12) / 3) + 4;
    // Entropy Bits (ENT)
    m_entropyBits = m_overallBits - m_checksumBits;
}

Mnemonic BIP39::Entropy(const std::string& entropy)
{
    BIP39::validateEntropy(entropy);

    auto entropyBits = entropy.length() + 4;
    auto checksumBits = ((entropyBits - 128) / 32) + 4;
    auto wordsCount = (entropyBits + checksumBits) / 11;
    return BIP39(wordsCount).useEntropy(entropy).wordList(Wordlist::english()).mnemonic();
}

Mnemonic BIP39::Generate(int wordCount)
{
    return BIP39(wordCount).generateSecureEntropy().wordList(Wordlist::english()).mnemonic();
}

void BIP39::validateEntropy(const std::string& entropy)
{
    if (!BIP39_Utils::isHex(entropy)) {
        return;
    }

    auto entropyBits = entropy.length() + 4;
    static constexpr std::array<int, 5> temp = {128, 160, 192, 224, 256};
    auto found = std::find(temp.begin(), temp.end(), entropyBits);
    if (found == temp.end())
        return;
}

Mnemonic BIP39::Words(const std::string& words, Wordlist wordlist, bool verifyChecksum)
{
    std::istringstream w{words};
    std::string word;
    std::vector<std::string> spWords;
    while (std::getline(w, word, ' ')) {
        spWords.emplace_back(word);
    }

    auto wordCount = spWords.size();
    return BIP39(wordCount).wordList(wordlist).reverse(spWords, verifyChecksum);
}

BIP39 BIP39::useEntropy(const std::string& entropy)
{
    BIP39::validateEntropy(entropy);
    m_entropy = entropy;
    m_checksum = checksum(entropy, m_checksumBits);
    auto str = hex2bits(m_entropy) + m_checksum;
    for (size_t i = 0; i < str.length(); i += 11) {
        std::string word11 = str.substr(i, 11);
        m_rawBinaryChunks.emplace_back(word11);
    }
    //    m_rawBinaryChunks = str_split(hex2bits(m_entropy).$this->checksum, 11);
    return *this;
}

BIP39 BIP39::generateSecureEntropy()
{
    size_t read_bytes = 0;
    ssize_t n;
    size_t size = m_entropyBits / 8;
    char* bytes = (char*)malloc(size);
    while (read_bytes < size) {
        size_t amount_to_read = size - read_bytes;
        n = syscall(SYS_getrandom, bytes + read_bytes, amount_to_read, 0);
        if (n == -1) {
            if (errno == ENOSYS) {
                /* This can happen if PHP was compiled against a newer kernel where getrandom()
                 * is available, but then runs on an older kernel without getrandom(). If this
                 * happens we simply fall back to reading from /dev/urandom. */
                break;
            } else if (errno == EINTR || errno == EAGAIN) {
                /* Try again */
                continue;
            } else {
                /* If the syscall fails, fall back to reading from /dev/urandom */
                break;
            }
        }
        read_bytes += (size_t)n;
    }
    std::string bin_rand{bytes};
    auto hex_rand = BIP39_Utils::base16Encode(bin_rand);
    useEntropy(hex_rand);
    return *this;
}

Mnemonic BIP39::mnemonic()
{
    if (m_entropy.empty()) {
        //        throw
    }
    if (m_wordList.empty()) {
        //        throw
    }

    auto _mnemonic = Mnemonic();
    for (const auto& bit : m_rawBinaryChunks) {
        auto index = BIP39_Utils::binToDec(bit);
        _mnemonic.appendWordIndex(index);
        _mnemonic.appendWord(m_wordList.getWord(index));
        _mnemonic.appendBinaryChunk(bit);
        _mnemonic.incrementCount();
    }
    return _mnemonic;
}

BIP39 BIP39::wordList(Wordlist wordlist)
{
    m_wordList = std::move(wordlist);
    return *this;
}

Mnemonic BIP39::reverse(std::vector<std::string> words, bool verifyChecksum)
{
    if (m_wordList.empty()) {
        return Mnemonic();
        //        throw new MnemonicException('Wordlist is not defined');
    }

    auto mnemonic = Mnemonic();
    int pos = 0;
    for (const auto& word : words) {
        ++pos;
        auto index = m_wordList.findIndex(word);
        if (index == 0) {
            return mnemonic;
        }

        mnemonic.appendWord(word);
        mnemonic.appendWordIndex(index);
        mnemonic.incrementCount();
        std::bitset<32> b(index);
        auto s = b.to_string();
        mnemonic.appendBinaryChunk(BIP39_Utils::str_pad(s, 11, "0", BIP39_Utils::STR_PAD_LEFT));

        auto rawBinary = BIP39_Utils::Join(mnemonic.rawBinaryChunks(), "");
        auto entropyBits = rawBinary.substr(0, m_entropyBits);
        auto checksumBits = rawBinary.substr(m_entropyBits, m_checksumBits);

        mnemonic.setEntropy(bits2hex(entropyBits));

        // Verify Checksum?
        if (verifyChecksum) {
            if (!BIP39_Utils::hashEquals(checksumBits, checksum(mnemonic.entropy(), m_checksumBits))) {
                std::cout << "Entropy checksum match failed!\n";
            }
        }
    }

    return mnemonic;
}

std::string BIP39::hex2bits(const std::string& hex)
{
    std::string bits;
    int len = hex.length();
    bits.reserve(len * 4);
    for (int i = 0; i < len; ++i) {
        bits += BIP39_Utils::hex_char_to_bin(hex.at(i));
    }
    return bits;
}

std::string BIP39::bits2hex(const std::string& bits)
{
    std::string hex;
    int len = bits.size();
    hex.reserve(len / 4);
    for (int i = 0; i < len; i += 4) {
        hex += BIP39_Utils::bin_str_to_hex(bits.substr(i, i + 4));
    }
    return hex;
}

std::string BIP39::checksum(const std::string& entropy, int bits)
{
    std::vector<uint8_t> out;
    out.resize(SHA256_DIGEST_STRING_LENGTH);
    auto entrop = BIP39_Utils::base16Decode(entropy);
    sha256_Raw(reinterpret_cast<const uint8_t*>(entrop.c_str()), entropy.size(), &out[0]);
    std::string hash{(char*)out.data(), out.size()};

    int checksumChar = hash.at(0);

    std::string checksumStr;
    for (int i = 0; i < bits; ++i) {
        checksumStr += checksumChar >> (7 - i) & 1;
    }

    return checksumStr;
}
