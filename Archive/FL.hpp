#ifndef _FL_H
#define _FL_H

#include <set>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <cassert>
#include "../Tools/Tools.hpp"

namespace OpenVIII::Archive {
    struct FL {
    private:
        constexpr static void CleanString(std::string &input) {
            if (std::size(input) > 4) {
                if (input[input.size() - 1] == '\r') {
                    input.erase(input.size() - 1);
                }

                input.erase(0, 3);
                Tools::replaceAll(input, '\\', std::filesystem::path::preferred_separator);
            }
        }

    public:
        constexpr const static auto Ext = std::string_view(".FL");

        //Get a sorted set of all entries.
        [[nodiscard]]  static auto GetAllEntriesData(const std::filesystem::path &path, const std::vector<char> & data, const size_t offset = 0U) {
            struct comp {
                bool operator()
                        (
                                const std::pair<int, std::string> &left,
                                const std::pair<int, std::string> &right
                        ) const {
                    if (left.second.length() < right.second.length()) {
                        return true;
                    }
                    if (left.second.length() > right.second.length()) {
                        return false;
                    }
                    return left.second < right.second;
                    // string < string caused assert failure.
                }
            };

            auto set = std::set<std::pair<int, std::string>, comp>();
            std::ifstream fp;

            int i = 0;
            std::string innerPath;
            auto loop = [&i,&offset,&fp,&innerPath,&set]()mutable {
                fp.seekg(offset, std::ios::beg);
                while (std::getline(fp, innerPath)) {
                    CleanString(innerPath);
                    set.insert(std::pair(i++, innerPath));
                }
                fp.close();
                return set;
            };
            if (!data.empty()) {
                if(data[0]=='\0')
                {
                    std::cout << "\033[1;31mFL Data is null!\033[0m\n";
                    return set;
                }
                fp = std::ifstream();
                fp.rdbuf()->pubsetbuf(const_cast<char *>(data.data()), data.size());//todo remove const_cast probably need a memcpy
            } else {
                fp = std::ifstream(path, std::ios::in);
            }
            loop();
            return set;
        }

        [[maybe_unused]] [[nodiscard]]  static auto GetAllEntries(const std::filesystem::path &path, const size_t offset = 0U) {
            auto tmp = std::vector<char>();
            return GetAllEntriesData(path, tmp, offset);
        }

        //Get entry the contains a string.
        [[maybe_unused]] [[nodiscard]] static auto
        GetEntry(const std::filesystem::path &path, const std::string_view &needle, const size_t offset = 0U) {
            std::ifstream fp = std::ifstream(path, std::ios::in);
            int i = 0;
            fp.seekg(offset, std::ios::beg);
            std::string innerPath;
            while (std::getline(fp, innerPath)) {
                if (Tools::iFind(innerPath, needle)) {
                    CleanString(innerPath);
                    fp.close();
                    return (std::make_pair(i, innerPath));
                }
                i++;
            }
            fp.close();
            return std::make_pair(-1, std::string(""));
        }
    };
}
#endif // !_FL_H