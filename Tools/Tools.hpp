#ifndef _TOOLS_H
#define _TOOLS_H

#include <string>
#include <algorithm>
#include <cctype>

namespace OpenVIII::Tools {
/*
Case Insensitive strings equal
*/
    [[nodiscard]] constexpr inline static bool
    iEquals(const std::string_view &str1, const std::string_view &str2) {
        const constexpr auto iEqual = [](const auto &c1, const auto &c2) {
            return ::toupper(c1) == ::toupper(c2);
        };
        if (str1.length() != str2.length()) {
            return false;
        }
        for (size_t i = 0; i < str1.length(); i++) {
            if (!iEqual(str1.at(i), str2.at(i))) {
                return false;
            }
        }
        return true;
        //return std::equal(str1.begin(), str1.end(), str2.begin(), iEqual);
        //todo std::equal constexpr in cpp20
    }

    template<typename T>
    [[maybe_unused]] constexpr inline static void
    replaceAll(std::string &haystack, const T &needle, const T &replacement) noexcept {
        if (needle == replacement) {
            return;
        }
        auto replace = [needle, replacement](const T &input) {
            return input == needle ? replacement : input;
        };
        std::transform(haystack.begin(), haystack.end(), haystack.begin(), replace);
    }
/*
Find Case Insensitive Sub String in a given substring (version returns location in string and allows offset)
*/
//[[nodiscard]] inline static size_t iFind(std::string haystack, std::string needle, size_t offset = 0)
//{
//	// Convert complete given String to lower case
//	std::transform(haystack.begin(), haystack.end(), haystack.begin(), ::toupper);
//	// Convert complete given Sub String to lower case
//	std::transform(needle.begin(), needle.end(), needle.begin(), ::toupper);
//	// Find sub string in given string
//	return haystack.find(needle, offset);
//}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-qualified-auto"

    //Find Case Insensitive Sub String in a given substring
    [[maybe_unused]] [[nodiscard]] inline static auto
    iFind(const std::string_view &haystack, const std::string_view &needle) {
        const constexpr auto comp = [](const auto &ch1, const auto &ch2) { return ::toupper(ch1) == ::toupper(ch2); };
        const auto it = std::search( //clang-tidy wants to make this a pointer. msvc doesn't return a pointer.
                haystack.begin(), haystack.end(),
                needle.begin(), needle.end(),
                comp
        );
        return (it != haystack.end());
        //todo make constexpr in cpp 20
    }
#pragma clang diagnostic pop
}
#endif // !_TOOLS_H
