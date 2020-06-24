// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_FL_H
#define VIIIARCHIVE_FL_H

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
#include <initializer_list>
#include "External/VIIITools/Tools.h"

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
        if (input.at(input.size() - 1) == '\r') {
          input.pop_back();
        }// remove the carriage return character
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

    const size_t &size = 0U,
    const size_t &count = 0U,
    const std::initializer_list<std::string_view> needle = {},
    const size_t &limit = 0U)
  {
    auto vector = std::vector<std::pair<unsigned int, std::string>>();
    const auto process = [&limit, &count, &size, &vector, &offset, &needle, &data](auto &cont) {
      cont.seekg(0, std::ios::end);
      auto length = cont.tellg();
      if (!cont.seekg(static_cast<long>(offset))) {
        std::cerr << "failed to seek to offset: " << offset << "; length: " << length << ";\n";
        exit(EXIT_FAILURE);
      }
      if (count > 0) {
        if (limit > 0 && count > limit) {
          vector.reserve(limit);
        } else {
          vector.reserve(count);
        }
      }

      // id numerical order is same order as fi data. So need to keep the id so we can reference the fi correctly.
      {
        std::basic_string<char> innerPath;
        for (unsigned int id = 0;
             (count == 0U || vector.size() < count)
               //&& (limit == 0U || vector.size() < limit)
               && (size == 0U || cont.tellg() < static_cast<long>(size + offset)) && [&innerPath, &cont]() -> bool {
               if (cont.seekg(3, std::ios::cur)) {
                 /* skip c:\ */
                 return static_cast<bool>(std::getline(cont, innerPath));
               }
               return false;
             }();
             id++) {
          if (!std::empty(needle)
              && !std::any_of(needle.begin(), needle.end(), [&innerPath](const std::string_view &innerNeedle) {
                   return !innerPath.empty() && Tools::iFind(innerPath, innerNeedle);
                 })) {
            continue;
          }

          // https://youtu.be/oTMSgI1XjF8?t=1727
          CleanString(
            vector
              .emplace_back(
                std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(std::move(innerPath)))
              .second,
            data.empty());
          innerPath = {};
        }
      }
    };
    // sort the strings. to make it easier to choose the correct string first.
    // shorter length and then what ever str < str2 does.
    std::sort(vector.begin(), vector.end(), [](const auto &left, const auto &right) {
      if (left.second.length() < right.second.length()) {
        return true;
      }
      if (left.second.length() > right.second.length()) {
        return false;
      }
      return left.second < right.second;
    });

    if (std::empty(data)) {
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
    const size_t &size = 0,
    const size_t &count = 0,
    const std::initializer_list<std::string_view> needle = {})
  {
    auto tmp = std::string();
    return GetAllEntriesData(path, tmp, offset, size, count, needle);
  }

  // Get a single entry that is the first match for needle.
  template<typename srcT>
  [[nodiscard]] static auto GetEntryData(const std::filesystem::path &path,
    const srcT &data,
    const std::initializer_list<std::string_view> &needle,
    const size_t &offset = 0U,
    const size_t &size = 0U,
    const size_t &count = 0U)
  {// Maybe should search all entries instead of using this because this is not sorted. Sorting matters when the
    // strings are similar. Though this might be faster if only getting a few files from an archive.
    auto buffer = GetAllEntriesData(path, data, offset, size, count, needle, 1);
    if (std::empty(buffer))
      return std::make_pair(0U, std::string(""));
    return buffer.at(0);
  }
  // Get a single entry that is the first match for needle.
  [[nodiscard]] static auto GetEntry(const std::filesystem::path &path,
    const std::initializer_list<std::string_view> &needle,
    const size_t &offset = 0U,
    const size_t &size = 0U,
    const size_t &count = 0U)
  {// Maybe should search all entries instead of using this because this is not sorted. Sorting matters when the
   // strings are similar. Though this might be faster if only getting a few files from an archive.
    auto data = GetAllEntriesData(path, "", offset, size, count, needle, 1);
    if (std::empty(data))
      return std::make_pair(0U, std::string(""));
    return data.at(0);
    //    std::ifstream fp = std::ifstream(path, std::ios::in);
    //
    //    fp.seekg(static_cast<long>(offset));
    //    std::string innerPath;
    //    fp.seekg(3, std::ios::cur);
    //    for (unsigned int i = 0; (count == 0U || i < count) && (size == 0U || fp.tellg() < static_cast<long>(size +
    //    offset))
    //                               && [&innerPath, &fp]() -> bool {
    //           if (fp.seekg(3, std::ios::cur)) {
    //             /* skip c:\ */
    //             return static_cast<bool>(std::getline(fp, innerPath));
    //           }
    //           return false;
    //         }();
    //
    //         i++) {
    //      if (!std::empty(needle)
    //          && std::any_of(needle.begin(), needle.end(), [&innerPath](const std::string_view &innerNeedle) {
    //               return !innerPath.empty() && Tools::iFind(innerPath, innerNeedle);
    //             })) {
    //        CleanString(innerPath);
    //        fp.close();
    //        return (std::make_pair(i, std::move(innerPath)));
    //      }
    //      i++;
    //    }
    //    fp.close();
    //    return std::make_pair(0U, std::string(""));
  }
  static void CleanBuffer(std::string &buffer)
  {
    // remove carriage returns
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
    // change slashes to preferred
    Tools::replaceSlashes(buffer);
  }
};// namespace OpenVIII::Archive
// namespace OpenVIII::Archive
}// namespace OpenVIII::Archive
#endif// !VIIIARCHIVE_FL_H