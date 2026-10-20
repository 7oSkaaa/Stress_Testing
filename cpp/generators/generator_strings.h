#pragma once
#include <string>
#include <random>
#include <cassert>
#include <cctype>
#include <vector>

namespace gen_strings {
    enum class CaseType { Lower, Upper, Mixed };

    inline char random_char (CaseType caseType, std::mt19937_64 &rng, char l = 'a', char r = 'z') {
        char ch = std::uniform_int_distribution<int>(l, r)(rng);
        bool upper = caseType == CaseType::Upper || (caseType == CaseType::Mixed && std::uniform_int_distribution<int>(0, 1)(rng));
        return upper ? std::toupper(ch) : std::tolower(ch);
    }

    inline std::string random (int len, CaseType caseType, std::mt19937_64 &rng, char l = 'a', char r = 'z') {
        assert(len >= 0);
        std::string s(len, 'a');
        for (auto &ch : s) ch = random_char (caseType, rng, l, r);
        return s;
    }

    inline std::string palindrome (int len, CaseType caseType, std::mt19937_64 &rng, char l = 'a', char r = 'z') {
        assert(len >= 0);
        std::string s(len, 'a');
        for (int i = 0, j = len - 1; i <= j; ++i, --j) {
            char ch = random_char (caseType, rng, l, r);
            s[i] = s[j] = ch;
        }
        return s;
    }

    // Generate a random string with digits, letters, or both
    inline std::string random_alphanum (int len, bool letters, bool digits, CaseType caseType, std::mt19937_64 &rng) {
        assert(letters || digits);
        std::string s;
        s.reserve(len);
        for (int i = 0; i < len; ++i) {
            if (letters && digits) {
                if (std::uniform_int_distribution<int>(0, 1)(rng))
                    s += random_char (caseType, rng);
                else
                    s += '0' + std::uniform_int_distribution<int>(0, 9)(rng);
            } else if (letters) {
                s += random_char (caseType, rng);
            } else {
                s += '0' + std::uniform_int_distribution<int>(0, 9)(rng);
            }
        }
        return s;
    }

    // Generate a random string with a custom alphabet
    inline std::string random_custom (int len, const std::string &alphabet, std::mt19937_64 &rng) {
        assert(!alphabet.empty());
        std::string s(len, ' ');
        for (auto &ch : s) ch = alphabet[std::uniform_int_distribution<int>(0, (int)alphabet.size() - 1)(rng)];
        return s;
    }

    // Generate a vector of random strings
    inline std::vector < std::string > random_strings (int count, int len, CaseType caseType, std::mt19937_64 &rng) {
        std::vector < std::string > v(count);
        for (auto &s : v) s = random (len, caseType, rng);
        return v;
    }

    // Generate a vector of palindromes
    inline std::vector < std::string > palindromes (int count, int len, CaseType caseType, std::mt19937_64 &rng) {
        std::vector < std::string > v(count);
        for (auto &s : v) s = palindrome (len, caseType, rng);
        return v;
    }
}
