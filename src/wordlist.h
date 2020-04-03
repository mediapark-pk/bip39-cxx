#ifndef WORDLIST_H
#define WORDLIST_H

#include <map>
#include <string>
#include <vector>

class WordlistException
{
public:
    WordlistException(std::string s) : m_desc{std::move(s)} {}
    std::string what() const
    {
        return m_desc;
    }

private:
    std::string m_desc;
};

class Wordlist
{
public:
    Wordlist(const std::string& language);

    static Wordlist getLanguage(const std::string& language);
    static Wordlist english();
    static Wordlist french();
    static Wordlist italian();
    static Wordlist spanish();

    std::string language() const;
    std::string getWord(int index);
    int findIndex(const std::string& searchWord);

private:
    static std::map<std::string, Wordlist> instances;
    std::vector<std::string> m_words;
    std::string m_language;
    int m_count;
};

#endif // WORDLIST_H
