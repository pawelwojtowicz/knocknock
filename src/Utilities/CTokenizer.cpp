#include "CTokenizer.h"
#include <cctype>

namespace knocknock
{
tStringList CTokenizer::Tokenize(const std::string& str, char delimiter)
{
    tStringList tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

tStringList CTokenizer::TokenizeTrimmed(const std::string& str, char delimiter)
{
    tStringList tokens;
    for (const auto& token : Tokenize(str, delimiter)) {
        std::string trimmed = Trim(token);
        if (!trimmed.empty()) {
            tokens.push_back(std::move(trimmed));
        }
    }
    return tokens;
}

std::string CTokenizer::Trim(const std::string& str)
{
    auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; };

    size_t start = 0;
    while (start < str.size() && isSpace(str[start])) {
        ++start;
    }
    size_t end = str.size();
    while (end > start && isSpace(str[end - 1])) {
        --end;
    }
    return str.substr(start, end - start);
}
}