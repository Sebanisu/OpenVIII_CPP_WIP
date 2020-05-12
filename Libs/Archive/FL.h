#ifndef OPENVIII_FL_H
#define OPENVIII_FL_H

#include <set>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include <cassert>
#include "Tools/Tools.h"

namespace OpenVIII::Archive {
struct FL
{
private:
  constexpr static void CleanString(std::string &input)
  {
    // std::cout << input << '\n';
    if (std::size(input) > 4) {
      if (input[input.size() - 1] == '\r') { input.erase(input.size() - 1); }

      input.erase(0, 3);
      Tools::replaceAll(input, '\\', std::filesystem::path::preferred_separator);
    }
  }


public:
  constexpr const static auto Ext = std::string_view(".FL");

  [[nodiscard]] static auto GetAllEntriesData(const std::filesystem::path &path,
    const std::string &data,
    const size_t &offset,
    const size_t count = 0)
  {

    std::ifstream fp{};
    auto vector = std::vector<std::pair<unsigned int, std::string>>();
    if (!data.empty() || (fp = std::ifstream(path, std::ios::in)).is_open()) {
      if (count > 0) vector.reserve(count);

      const auto loop = [&vector](auto &cont) {
        unsigned int id = 0;

        std::string innerPath;
        while (std::getline(cont, innerPath, '\n')) {
          CleanString(innerPath);
          vector.emplace_back(std::make_pair(id++, std::move(innerPath)));
        }
      };

      if (fp.is_open()) {
        fp.seekg(static_cast<long>(offset));
        loop(fp);
        if (!fp.is_open()) { fp.close(); }
      } else {
        if (data[0] == '\0') {
          std::cout << "\033[1;31mFL Data is null!\033[0m\n";
          // exit(EXIT_FAILURE);
          return vector;
        }
        auto ss = std::stringstream(data);
        loop(ss);
      }

      std::sort(vector.begin(),
        vector.end(),
        [](const auto &left, const auto &right) {
          if (left.second.length() < right.second.length()) { return true; }
          if (left.second.length() > right.second.length()) { return false; }
          return left.second < right.second;
        });
      // no real way to know the number of lines here. FI can know the number because the count is the filesize /12.
      // pass the count as an argument to reserve the correct number of elements.
      vector.shrink_to_fit();
    }
    return vector;
  }
  [[maybe_unused]] [[nodiscard]] static auto GetAllEntries(const std::filesystem::path &path, const size_t &offset)
  {
    auto tmp = std::string();
    return GetAllEntriesData(path, tmp, offset);
  }
  [[nodiscard]] static auto
    GetEntry(const std::filesystem::path &path, const std::string_view &needle, const size_t &offset)
  {
    std::ifstream fp = std::ifstream(path, std::ios::in);
    unsigned int i = 0;
    fp.seekg(static_cast<long>(offset));
    std::string innerPath;
    while (std::getline(fp, innerPath)) {
      if (Tools::iFind(innerPath, needle)) {
        CleanString(innerPath);
        fp.close();
        return (std::make_pair(i, std::move(innerPath)));
      }
      i++;
    }
    fp.close();
    return std::make_pair(0U, std::string(""));
  }
};
// namespace OpenVIII::Archive
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FL_H