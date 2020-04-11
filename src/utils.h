#ifndef UTILS_H
#define UTILS_H

#include <iterator>
#include <sstream>
#include <string>

namespace BIP39_Utils
{
bool isHex(const std::string& str);

template <typename Range, typename Value = typename Range::value_type>
std::string Join(Range const& elements, const char* const delimiter)
{
    std::ostringstream os;
    auto b = begin(elements), e = end(elements);

    if (b != e) {
        std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
        b = prev(e);
    }
    if (b != e) {
        os << *b;
    }

    return os.str();
}

const char* hex_char_to_bin(char c);

char bin_str_to_hex(uint32_t j);

int hexValue(char hex_digit);

std::string base16Decode(const std::string& input);

std::string base16Encode(const std::string& input);

bool hashEquals(const std::string& known, const std::string& user);

}    // namespace BIP39_Utils

#endif // UTILS_H
