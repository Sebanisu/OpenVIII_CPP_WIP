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
// FL files contain internal file structure paths. As a flat text file.
// This class is used to search the strings for a filename or
// grab all the entries. The entry will be a string paired with an int
// that is the line number. This is used to ID the FI entries.
struct FL
{
private:
  // Remove the C:\ from the start, remove the \r from the end,
  // and change \ to the correct slash.
  // added skipFixed if data is set then i probably fixed slashes already.
  constexpr static void CleanString(std::string &input, const bool &skipFixed = true) noexcept
  {
    if (std::size(input) > 4) {
      if (Tools::iEquals(std::string_view(input.c_str(), 2), "c:")) {
        input.erase(0, 3);// remove c:\ from the start of the strings.
      }
      if (skipFixed) {
        if (input.at(input.size() - 1) == '\r') { input.pop_back(); }// remove the carriage return character
        Tools::replaceSlashes(input);
      }
    }
  }


public:
  constexpr const static auto Ext = std::string_view(".FL");

  // Get All entries sorted from file or data buffer.
  [[nodiscard]] static auto GetAllEntriesData(const std::filesystem::path &path,
    const std::string &data,
    const size_t &offset,
    const size_t count = 0U,
    const std::string_view needle = {})
  {

    auto vector = std::vector<std::pair<unsigned int, std::string>>();
    const auto process = [&count, &vector, &offset, &needle, &data](auto &cont) {
      if (!cont.seekg(static_cast<long>(offset))) { return; }
      if (count > 0) vector.reserve(count);
      // id numerical order is same order as fi data. So need to keep the id so we can reference the fi correctly.
      std::basic_string<char> innerPath;
      for (unsigned int id = 0; std::getline(cont, innerPath, '\n'); id++) {
        if (!needle.empty() && !Tools::iFind(innerPath, needle)) continue;// filter value by string if need.
        CleanString(vector.emplace_back(std::make_pair(id, std::move(innerPath))).second, data.empty());
      }
      // sort the strings. to make it easier to choose the correct string first.
      // shorter length and then what ever str < str2 does.
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
  // Get all entries from the FL file sorted and cleaned.
  [[maybe_unused]] [[nodiscard]] static auto GetAllEntries(const std::filesystem::path &path,
    const size_t &offset,
    const size_t &count = 0,
    const std::string_view needle = {})
  {
    auto tmp = std::string();
    return GetAllEntriesData(path, tmp, offset, count, needle);
  }
  // Get a single entry that is the first match for needle.
  [[nodiscard]] static auto
    GetEntry(const std::filesystem::path &path, const std::string_view &needle, const size_t &offset)
  {// Maybe should search all entries instead of using this because this is not sorted. Sorting matters when the
   // strings are similar. Though this might be faster if only getting a few files from an archive.
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