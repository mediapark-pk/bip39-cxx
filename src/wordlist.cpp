#include "wordlist.h"
#include <algorithm>
#include <fstream>

std::map<std::string, Wordlist> Wordlist::instances = {};

Wordlist::Wordlist(const std::string& language) : m_language{language}, m_count{0}
{
    const std::string wordListFile = language + ".txt";
    std::ifstream file(wordListFile);
    std::string word;
    while (std::getline(file, word)) {
        m_words.emplace_back(word);
        ++m_count;
    }

    if (m_count != 2048) {
        throw WordlistException("BIP39 words list file must have precise 2048 entries");
    }
}

Wordlist Wordlist::getLanguage(const std::string& language)
{
    auto it = instances.find(language);
    if (it != instances.end()) {
        return (*it).second;
    }
    auto wordList = Wordlist(language);
    instances[language] = wordList;
    return wordList;
}

Wordlist Wordlist::english()
{
    return getLanguage("english");
}

Wordlist Wordlist::french()
{
    return getLanguage("french");
}

Wordlist Wordlist::italian()
{
    return getLanguage("italian");
}

Wordlist Wordlist::spanish()
{
    return getLanguage("spanish");
}

std::string Wordlist::language() const
{
    return m_language;
}

std::string Wordlist::getWord(int index)
{
    if (index > (int)m_words.size())
        return "";
    return m_words.at(index);
}

int Wordlist::findIndex(const std::string& searchWord)
{
    auto pos = std::find(m_words.begin(), m_words.end(), searchWord);
    int index = std::distance(m_words.begin(), pos);
    return index;
}
