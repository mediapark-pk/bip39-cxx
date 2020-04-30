#ifndef WORDLIST_H
#define WORDLIST_H

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

class Wordlist
{
public:
    Wordlist() = default;
    bool Init(const std::string& language) noexcept;

    static Wordlist* getLanguage(const char* language) noexcept;
    static Wordlist* english() noexcept;
    static Wordlist* french() noexcept;
    static Wordlist* italian() noexcept;
    static Wordlist* spanish() noexcept;

    std::string language() const noexcept;
    std::string getWord(int index) noexcept;
    int findIndex(const std::string& searchWord);
    bool empty() const noexcept;

private:
    static std::map<std::string, Wordlist> instances;
    std::vector<std::string> m_words;
    std::string m_language;
    int m_count;
};

#endif // WORDLIST_H
