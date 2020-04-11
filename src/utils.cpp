#include "utils.h"

#include <iterator>
#include <sstream>
#include <string>

namespace BIP39_Utils
{
const signed char p_util_hexdigit[256] = {
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  0,   1,   2,   3,  4,  5,  6,  7,  8,  9,   -1,  -1,
    -1,  -1,  -1,  -1, -1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, 0xa, 0xb, 0xc,
    0xd, 0xe, 0xf, -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1,  -1,  -1,
    -1,  -1,  -1,  -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1,
};

signed char isHexDigit(char c)
{
    return p_util_hexdigit[(unsigned char)c];
}

bool isHex(const std::string& str)
{
    for (std::string::const_iterator it(str.begin()); it != str.end(); ++it) {
        if (isHexDigit(*it) < 0)
            return false;
    }
    return (!str.empty()) && (str.size() % 2 == 0);
}

std::string str_pad(const std::string& str, int pad_length, std::string pad_string, STR_PAD pad_type)
{
    int i, j, x;
    int str_size = str.size();
    int pad_size = pad_string.size();
    if (pad_length <= str_size || pad_size < 1)
        return str;

    std::string o;
    o.reserve(pad_length);    // allocate enough memory only once

    if (pad_type == STR_PAD_RIGHT) {
        for (i = 0, x = str_size; i < x; i++)
            o.push_back(str[i]);
        for (i = str_size; i < pad_length;)
            for (j = 0; j < pad_size && i < pad_length; j++, i++)
                o.push_back(pad_string[j]);
    } else if (pad_type == STR_PAD_LEFT) {
        int a1 = pad_length - str_size;
        for (i = 0; i < a1;)
            for (j = 0; j < pad_size && i < a1; j++, i++)
                o.push_back(pad_string[j]);
        for (i = 0, x = str_size; i < x; i++)
            o.push_back(str[i]);
    } else if (pad_type == STR_PAD_BOTH) {
        int a1 = (pad_length - str_size) / 2;
        int a2 = pad_length - str_size - a1;
        for (i = 0; i < a1;)
            for (j = 0; j < pad_size && i < a1; j++, i++)
                o.push_back(pad_string[j]);
        for (i = 0, x = str_size; i < x; i++)
            o.push_back(str[i]);
        for (i = 0; i < a2;)
            for (j = 0; j < pad_size && i < a2; j++, i++)
                o.push_back(pad_string[j]);
    }
    return o;
}

const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch (toupper(c)) {
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'A':
        return "1010";
    case 'B':
        return "1011";
    case 'C':
        return "1100";
    case 'D':
        return "1101";
    case 'E':
        return "1110";
    case 'F':
        return "1111";
    }
}

char bin_str_to_hex(const uint32_t j)
{
    return j > 9 ? (char)j + 55 : (char)j + '0';
}

int hexValue(char hex_digit)
{
    switch (hex_digit) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return hex_digit - '0';

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return hex_digit - 'A' + 10;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        return hex_digit - 'a' + 10;
    }
    throw std::invalid_argument("bad hex_digit");
}

std::string base16Decode(const std::string& input)
{
    const auto len = input.length();
    if (len & 1) {
        return "";
        //        throw EncodingException{"base16Decode: Invalid length!"};
    }

    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end();) {
        try {
            int hi = hexValue(*it++);
            int lo = hexValue(*it++);
            output.push_back(hi << 4 | lo);
        } catch (const std::invalid_argument& e) {
            throw e;
        }
    }
    return output;
}

std::string base16Encode(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

bool hashEquals(const std::string& known, const std::string& user)
{
    int result = 0;
    for (size_t i = 0; i < known.length(); ++i) {
        result |= known.at(i) ^ user.at(i);
    }
    return (result == 0);
}

std::string decToBin(int number)
{
    if (number == 0)
        return "0";
    if (number == 1)
        return "1";

    if (number % 2 == 0)
        return decToBin(number / 2) + "0";
    else
        return decToBin(number / 2) + "1";
}

int binToDec(const std::string& number)
{
    int result = 0, pow = 1;
    for (int i = number.length() - 1; i >= 0; --i, pow <<= 1)
        result += (number[i] - '0') * pow;

    return result;
}
}    // namespace BIP39_Utils
