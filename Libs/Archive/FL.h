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
  constexpr static void CleanString(std::string &input) noexcept
  {
    if (std::size(input) > 4) {
      if (input.at(input.size() - 1) == '\r') { input.pop_back(); }// remove the carriage return character
      if (Tools::iEquals(std::string_view(input.c_str(), 3), "c:\\")) {
        input.erase(0, 3);// remove c:\ from the start of the strings.
      }
      Tools::replaceSlashes(input);
    }
  }


public:
  constexpr const static auto Ext = std::string_view(".FL");

  [[nodiscard]] static auto GetAllEntriesData(const std::filesystem::path &path,
    const std::string &data,
    const size_t &offset,
    const size_t count = 0)
  {

    auto vector = std::vector<std::pair<unsigned int, std::string>>();
    const auto process = [&count, &vector, &offset](auto &cont) {
      if (!cont.seekg(static_cast<long>(offset))) { return; }
      if (count > 0) vector.reserve(count);
      unsigned int id = 0;

      std::basic_string<char> innerPath;
      while (std::getline(cont, innerPath, '\n')) {
        CleanString(vector.emplace_back(std::make_pair(id++, std::move(innerPath))).second);
      }
      std::sort(vector.begin(), vector.end(), [](const auto &left, const auto &right) {
        if (left.second.length() < right.second.length()) { return true; }
        if (left.second.length() > right.second.length()) { return false; }
        return left.second < right.second;
      });
    };
    if (data.empty()) {
      auto fp = std::ifstream(path, std::ios::in);

      if (fp.is_open()) {
        process(fp);
        fp.close();
      }
    } else {
      if (data[0] != '\0') {
        auto ss = std::stringstream(data);
        process(ss);
      } else {
        std::cout << "\033[1;31mFL Data is null!\033[0m\n";
      }
    }
    vector.shrink_to_fit();
    return vector;
  }
  [[maybe_unused]] [[nodiscard]] static auto
    GetAllEntries(const std::filesystem::path &path, const size_t &offset, const size_t &count = 0)
  {
    auto tmp = std::string();
    return GetAllEntriesData(path, tmp, offset, count);
  }
  [[nodiscard]] static auto
    GetEntry(const std::filesystem::path &path, const std::string_view &needle, const size_t &offset)
  {
    std::ifstream fp = std::ifstream(path, std::ios::in);

    fp.seekg(static_cast<long>(offset));
    std::string innerPath;
    for (unsigned int i = 0; std::getline(fp, innerPath); i++) {
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