#include "CTokenizer.h"

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
}