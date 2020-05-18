#ifndef _TOOLS_H
#define _TOOLS_H

#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include "../Archive/FL.hpp"
#include "../Archive/FI.hpp"
#include "../Archive/FS.hpp"
#include "../Archive/FIFLFS.hpp"

namespace OpenVIII {
    struct Tools {
/*
Case Insensitive strings equal
*/
        [[nodiscard]] constexpr static bool
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
        [[maybe_unused]] constexpr static void
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


        //Find Case Insensitive Sub String in a given substring
        [[maybe_unused]] [[nodiscard]] static auto
        iFind(const std::string_view &haystack, const std::string_view &needle) {
            const constexpr auto comp = [](const auto &ch1, const auto &ch2) {
                return ::toupper(ch1) == ::toupper(ch2);
            };
            const auto it = std::search( //clang-tidy wants to make this a pointer. msvc doesn't return a pointer.
                    haystack.begin(), haystack.end(),
                    needle.begin(), needle.end(),
                    comp
            );
            return (it != haystack.end());
            //todo make constexpr in cpp 20
        }

        static auto GetFiles(const std::string_view path) {
            //order must match switch below
            static constexpr const std::array<std::string_view, 3> extensions = {OpenVIII::Archive::FL::Ext,
                                                                                 OpenVIII::Archive::FS::Ext,
                                                                                 OpenVIII::Archive::FI::Ext};
            const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
            auto tmp = std::map<std::string, OpenVIII::Archive::FIFLFS>();
            auto archive = OpenVIII::Archive::FIFLFS();
            for (const auto &fileEntry : std::filesystem::directory_iterator(path, options)) {
                if (fileEntry.is_regular_file()) {
                    unsigned char i = 0;
                    for (const auto &ext : extensions) {
                        if (OpenVIII::Tools::iEquals(fileEntry.path().extension().string(), ext)) {
                            switch (i) {
                                case 0:
                                    archive.FL(fileEntry);
                                    break;
                                case 1:
                                    archive.FS(fileEntry);
                                    break;
                                case 2:
                                    archive.FI(fileEntry);
                                    break;
                                default:
                                    break;
                            }

                            if (archive.AllSet()) {
                                auto name = fileEntry.path().filename().stem().string();
                                std::transform(name.begin(), name.end(), name.begin(), ::toupper);
                                tmp.insert(std::make_pair(name, archive));
                                archive = OpenVIII::Archive::FIFLFS();
                            }
                            break;
                        }
                        i++;
                    }
                }
            }
            return tmp;
        }
    };
}
#endif // !_TOOLS_H
