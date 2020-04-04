#ifndef MNEMONIC_H
#define MNEMONIC_H

#include <string>
#include <vector>

class Mnemonic
{
public:
    Mnemonic() = default;
    Mnemonic(const std::string& entropy);
    std::string generateSeed(const std::string& passphrase = "", int bytes = 0);

    void setWords(const std::vector<std::string>& words);
    std::vector<std::string> words() const;

    std::vector<std::string> rawBinaryChunks() const;
    void setRawBinaryChunks(const std::vector<std::string>& rawBinaryChunks);

    std::vector<int> wordsIndex() const;
    void setWordsIndex(const std::vector<int>& wordsIndex);

    void appendWord(std::string word);
    void appendWordIndex(int wordIndex);
    void appendBinaryChunk(std::string chunk);
    void incrementCount();

    std::string entropy() const;
    void setEntropy(const std::string& entropy);

private:
    std::string m_entropy;
    int m_wordsCount;
    std::vector<int> m_wordsIndex;
    std::vector<std::string> m_words;
    std::vector<std::string> m_rawBinaryChunks;
};

#endif // MNEMONIC_H
