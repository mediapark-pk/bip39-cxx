#include "wordlist.h"
#include <algorithm>
#include <fstream>

std::map<std::string, Wordlist> Wordlist::instances = {};

bool Wordlist::Init(const std::string& language) noexcept
{
    const std::string wordListFile = language + ".txt";
    std::ifstream file(wordListFile);
    std::string word;
    m_words.reserve(2048);
    m_count = 0;
    while (std::getline(file, word)) {
        m_words.emplace_back(word);
        ++m_count;
    }

    if (m_count != 2048) {
        m_words.clear();
        m_count = 0;
        return false;
    }
    return true;
}

Wordlist* Wordlist::getLanguage(const char* language) noexcept
{
    auto it = instances.find(language);
    if (it != instances.end()) {
        return &(*it).second;
    }
    auto wordList = Wordlist();
    if (wordList.Init(language)) {
        instances[language] = wordList;
        return &instances[language];
    }
    return nullptr;
}

Wordlist* Wordlist::english() noexcept
{
    return getLanguage("english");
}

Wordlist* Wordlist::french() noexcept
{
    return getLanguage("french");
}

Wordlist* Wordlist::italian() noexcept
{
    return getLanguage("italian");
}

Wordlist* Wordlist::spanish() noexcept
{
    return getLanguage("spanish");
}

std::string Wordlist::language() const noexcept
{
    return m_language;
}

std::string Wordlist::getWord(int index) noexcept
{
    if (index < 0 || index > (int)m_words.size())
        return "";
    return m_words.at(index);
}

int Wordlist::findIndex(const std::string& searchWord)
{
    auto el = std::lower_bound(m_words.begin(), m_words.end(), searchWord);
    int index = std::distance(m_words.begin(), el);
    return index;
}

bool Wordlist::empty() const noexcept
{
    return m_words.empty();
}
