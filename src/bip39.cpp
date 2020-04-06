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
        throw MnemonicException("Mnemonic words count must be between 12-24");
    } else if (wordCount % 3 != 0) {
        throw MnemonicException("Words count must be generated in multiples of 3");
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
    if (!BIP39::validateEntropy(entropy)) {
        throw MnemonicException("Invalid Entropy: " + entropy);
    }

    auto entropyBits = entropy.length() + 4;
    auto checksumBits = ((entropyBits - 128) / 32) + 4;
    auto wordsCount = (entropyBits + checksumBits) / 11;
    return BIP39(wordsCount).useEntropy(entropy).wordList(Wordlist::english()).mnemonic();
}

Mnemonic BIP39::Generate(int wordCount)
{
    return BIP39(wordCount).generateSecureEntropy().wordList(Wordlist::english()).mnemonic();
}

bool BIP39::validateEntropy(const std::string& entropy)
{
    if (!BIP39_Utils::isHex(entropy)) {
        return false;
    }

    auto entropyBits = entropy.length() * 4;
    switch (entropyBits) {
    case 128:
    case 160:
    case 192:
    case 224:
    case 256:
        return true;
    }
    return false;
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
    if (!BIP39::validateEntropy(entropy)) {
        throw MnemonicException("Invalid Entropy: " + entropy);
    }
    m_entropy = entropy;
    m_checksum = checksum(entropy);
    auto str = hex2bits(m_entropy) + m_checksum;
    for (size_t i = 0; i < str.length(); i += 11) {
        const std::string& word11 = str.substr(i, 11);
        m_rawBinaryChunks.emplace_back(word11);
    }
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
                std::cout << "ENOSYS";
                break;
            } else if (errno == EINTR || errno == EAGAIN) {
                /* Try again */
                continue;
            } else {
                /* If the syscall fails, fall back to reading from /dev/urandom */
                std::cout << "syscall failed\n";
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
        throw MnemonicException("Entropy is empty");
    }
    if (m_wordList.empty()) {
        throw MnemonicException("Wordlist is empty");
    }

    auto _mnemonic = Mnemonic(m_entropy);
    for (const auto& bit : m_rawBinaryChunks) {
        //        auto index = BIP39_Utils::binToDec(bit);
        auto index = bit.to_ulong();
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
        throw MnemonicException("Wordlist is empty");
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
        std::bitset<11> b(index);
        mnemonic.appendBinaryChunk(b);

        //        auto rawBinary = BIP39_Utils::Join(mnemonic.rawBinaryChunks(), "");
        std::string rawBinary;
        rawBinary.reserve(mnemonic.rawBinaryChunks().size() * 11);
        for (const auto bit : mnemonic.rawBinaryChunks()) {
            rawBinary += bit.to_string();
        }
        auto entropyBits = rawBinary.substr(0, m_entropyBits);
        auto checksumBits = rawBinary.substr(m_entropyBits, m_checksumBits);

        mnemonic.setEntropy(bits2hex(entropyBits));

        // Verify Checksum?
        if (verifyChecksum) {
            if (!BIP39_Utils::hashEquals(checksumBits, checksum(mnemonic.entropy()))) {
                throw MnemonicException("Entropy checksum match failed!");
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
        hex += BIP39_Utils::bin_str_to_hex(bits.substr(i, 4));
    }
    return hex;
}

constexpr size_t BIP39::len_to_mask(size_t len) noexcept
{
    switch (len) {
    case 128:
        return 0xf0;
    case 160:
        return 0xf8;
    case 192:
        return 0xfc;
    case 224:
        return 0xfe;
    case 256:
        return 0xff;
    default:
        return 0;
    }
}

std::string BIP39::checksum(const std::string& entropy)
{
    std::vector<uint8_t> out;
    out.resize(SHA256_DIGEST_LENGTH);
    auto entrop = BIP39_Utils::base16Decode(entropy);
    sha256_Raw(reinterpret_cast<const uint8_t*>(entrop.c_str()), entrop.size(), &out[0]);

    auto checksumChar = out.at(0);
    auto mask = len_to_mask(m_entropyBits);
    if (mask == 0xf0)
        return std::bitset<4>((checksumChar & mask) >> 4).to_string();
    else if (mask == 0xf8)
        return std::bitset<5>((checksumChar & mask) >> 4).to_string();
    else if (mask == 0xfc)
        return std::bitset<6>((checksumChar & mask) >> 4).to_string();
    else if (mask == 0xfe)
        return std::bitset<7>((checksumChar & mask) >> 4).to_string();
    else if (mask == 0xff)
        return std::bitset<8>((checksumChar & mask) >> 4).to_string();
    return "";
}
