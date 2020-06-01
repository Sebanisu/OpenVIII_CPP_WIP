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
#ifndef VIIIARCHIVE_ZZZ_H
#define VIIIARCHIVE_ZZZ_H
#include <string>
#include "FI.h"
#include "External/VIIITools/Tools.h"
#include <filesystem>
#include <set>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "FIFLFS.h"
#include "FileData.h"
namespace OpenVIII::Archive {

struct [[maybe_unused]] ZZZ
{


private:
  // stored at top of zzz file to tell how many files are stored inside
  // unsigned int count_{};
  // file data inside zzz file.
  std::vector<FileData> data_{};
  std::filesystem::path path_{};

public:
  constexpr static auto Ext = ".zzz";
  [[maybe_unused]] [[nodiscard]] auto &Data() const noexcept { return data_; }
  [[maybe_unused]] [[nodiscard]] auto &Path() const noexcept { return path_; }
  ZZZ(const ZZZ &) = default;
  ZZZ &operator=(const ZZZ &) = default;
  ZZZ(ZZZ &&) = default;
  ZZZ &operator=(ZZZ &&) = default;
  ~ZZZ() = default;
  constexpr ZZZ() = default;
  explicit ZZZ(const std::filesystem::path &path)
  {
    if (!(path.has_extension() && Tools::iEquals(path.extension().string(), Ext)) || !std::filesystem::exists(path)) {
      return;
    }

    unsigned int count{};
    auto fp = std::ifstream(path, std::ios::binary | std::ios::in);
    if (!fp.is_open()) {
      fp.close();
      return;
    }
    path_ = path;
    Tools::ReadVal(fp, count);
    std::cout << "Getting " << count << " File Entries from : " << path << std::endl;
    data_.reserve(count);
    for (auto i = 0U; fp.is_open() && !fp.eof() && i < count; i++) {
      if ((data_.emplace_back(FileData(fp)).empty())) {
        std::cerr << "empty element detected and removed\n";
        data_.pop_back();
      }
    }
    std::sort(data_.begin(), data_.end(), [](const FileData &left, const FileData &right) {
      const auto &rightString = right.GetPathString();
      const auto &leftString = left.GetPathString();
      {
        const auto &rightSize = std::size(rightString);
        const auto &leftSize = std::size(leftString);
        if (leftSize < rightSize) {
          return true;
        }
        if (leftSize > rightSize) {
          return false;
        }
      }
      return leftString < rightString;
    });
    fp.close();
    data_.shrink_to_fit();
  }
  void SaveEntry(const FileData &item, const std::string_view &strPath) const
  {
    auto buffer = FS::GetEntry(path_, item, 0U);
    std::cout << '{' << buffer.size() << ", " << strPath << "}\n";
    Tools::WriteBuffer(buffer, strPath);
  }
  void Test() const
  {

    FIFLFS archive{};
    {
      auto beg = data_.begin();
      auto end = data_.end();
      for (auto cur = beg; cur < end; cur++) {
        // const auto &item : data_
        const auto &item = *cur;
        const auto &[strPath, zzzOffset, zzzSize] = item.GetTuple();
        {
          const auto &next =
            cur + 1 < end && (FIFLFS<true>::CheckExtension((*(cur + 1)).GetPathString()) != 0) ? *(cur + 1) : FileData();
          const auto &prev =
            cur - 1 > beg && (FIFLFS<true>::CheckExtension((*(cur - 1)).GetPathString()) != 0) ? *(cur - 1) : FileData();
          // getting a prev and next element to check vs cur item. To make sure at least 2 of them match so we don't add
          // an orphan to the FIFLFS archive.
          std::cout << '{' << zzzOffset << ", " << zzzSize << ", " << strPath << "}\n";
          if ((FIFLFS<true>::CheckExtension(strPath) != 0)
              && ((!next.empty() && FIFLFS<true>::GetBaseName(strPath) == FIFLFS<true>::GetBaseName(next.GetPathString())) ||

                  (!prev.empty() && FIFLFS<true>::GetBaseName(strPath) == FIFLFS<true>::GetBaseName(prev.GetPathString())))) {

            std::filesystem::path fsPath(strPath);
            {
              // char retVal = archive.TryAddNested(path_, 0U, fsPath, item);
              char retVal = archive.TryAdd(path_, fsPath, item.Offset(), item.UncompressedSize());
              if (retVal == 1) {
                continue;
              }
              if (retVal == 2) {
                archive.Test();
                archive = {};
                continue;
              }
            }
          }
        }
        SaveEntry(item, strPath);
      }
    }
  }
  using ZZZmap = std::vector<std::pair<std::string, OpenVIII::Archive::ZZZ>>;

  [[nodiscard]] static ZZZmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;

    auto tmp = ZZZmap();
    tmp.reserve(2);// main and other
    auto archive = OpenVIII::Archive::ZZZ();
    int i{};
    for (const auto &fileEntry : std::filesystem::directory_iterator(path, options)) {
      if (!(fileEntry.path().has_extension() && Tools::iEquals(fileEntry.path().extension().string(), Ext))) {
        continue;
      }
      // todo check for language codes to choose correct files
      auto basename = fileEntry.path().filename().stem().string();
      if (std::empty(basename)) {
        tmp.emplace_back(std::make_pair("__" + std::to_string(i++), std::move(archive)));
      }
      std::transform(basename.begin(), basename.end(), basename.begin(), ::toupper);
      archive = OpenVIII::Archive::ZZZ(fileEntry);
      tmp.emplace_back(std::make_pair(basename, archive));
    }
    tmp.shrink_to_fit();
    return tmp;
  }
  static void testPair(const std::pair<std::string_view, OpenVIII::Archive::ZZZ> &pair)
  {
    const auto &[name, zzz] = pair;
    std::cout << '{' << name << ", " << zzz.path_ << "}\n";
    zzz.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
};

}// namespace OpenVIII::Archive
#endif// VIIIARCHIVE_ZZZ_H