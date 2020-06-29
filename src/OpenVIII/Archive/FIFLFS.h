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
#ifndef VIIIARCHIVE_FIFLFS_H
#define VIIIARCHIVE_FIFLFS_H

#include "FI.h"
#include "FL.h"
#include "FS.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <iterator>

namespace OpenVIII::Archive {
template<bool HasNested = false> struct FIFLFS
{
private:
  template<typename T> struct Grouping
  {
  public:
    mutable std::filesystem::path path{};
    mutable size_t offset{};
    mutable size_t size{};// if forced otherwise 0;
    mutable T data{};
    mutable std::string base{};
    mutable std::filesystem::path nestedPath{};

    //    // Assigns basename and returns it.
    [[maybe_unused]] std::string GetBaseName() const noexcept { return base = FIFLFS::GetBaseName(path); }
    explicit operator bool() const
    {
      return (!path.empty() && std::filesystem::exists(path)) || !std::empty(data)
             || (!path.empty() && (offset > 0 || size > 0));
    }
  };
  Grouping<std::vector<char>> fi_{};
  Grouping<std::vector<char>> fs_{};
  Grouping<std::basic_string<char>> fl_{};// this is char because the file contains strings.
  mutable size_t count_{};
  void GetCount(size_t size = 0U) const
  {
    if (size == 0U) {
      size = fi_.size;
    }
    if (size == 0U && std::filesystem::exists(fi_.path)) {
      size = std::filesystem::file_size(fi_.path);
    }
    if (size == 0U && !fi_.data.empty()) {
      size = fi_.data.size();
    }
    count_ = FI::GetCount(size);
  }

public:
  [[maybe_unused]] [[nodiscard]] const auto &FI() const noexcept { return fi_; }

  [[maybe_unused]] [[nodiscard]] const auto &FS() const noexcept { return fs_; }

  [[nodiscard]] const auto &FL() const noexcept { return fl_; }


  [[nodiscard]] bool AllSet() const { return fi_ && fs_ && fl_; }

  FIFLFS() = default;

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FIFLFS &data)
  {
    const auto str = [](auto fiflfs) {
      return std::empty(fiflfs.nestedPath) ? fiflfs.path : fiflfs.path / fiflfs.nestedPath;
    };
    return os << '{' << data.GetBaseName() << " {" << data.count_ << " File Entries from : " << str(data.fi_) << ", "
              << str(data.fl_) << ", " << str(data.fs_) << "}}";
  }

  [[nodiscard]] Archive::FI GetEntryIndex(const unsigned int &id) const
  {
    if (count_ == 0 || id < count_) {
      if (!fi_.data.empty()) {
        return Archive::FI(fi_.data, id, fi_.offset);
      }
      return Archive::FI(fi_.path, id, fi_.offset);
    }
    return {};
  }


  explicit operator bool() const noexcept { return AllSet(); }
  /*
   * 0 = didn't add
   * 1 = added
   * 2 = added and all set
   * */

  char TryAdd(const std::filesystem::path &existingFilePath,
    const std::filesystem::path &nestedPath = "",
    size_t offset = 0U,
    size_t size = 0U) const
  {
    const auto set = [&existingFilePath, &offset, &nestedPath, &size, this](auto &ds) {
      ds.path = existingFilePath;
      ds.offset = offset;
      ds.size = size;
      ds.nestedPath = nestedPath;
      if (nestedPath.has_stem()) {
        ds.base = GetBaseName(nestedPath);
      } else {
        ds.GetBaseName();
      }
    };
    switch (nestedPath.has_extension() ? CheckExtension(nestedPath) : CheckExtension(existingFilePath)) {
    case 1: {
      set(fl_);
      compareBaseNames();
      return AllSet() ? 2 : 1;
    }
    case 2: {
      set(fs_);
      compareBaseNames();
      return AllSet() ? 2 : 1;
    }
    case 3: {
      set(fi_);
      GetCount();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    }
    default:
      break;
    }

    return 0;
  }
  template<typename srcT = std::vector<char>, typename datT = Archive::FI>
  char
    TryAddNested(const srcT &src, const size_t srcOffset, const std::filesystem::path &fileEntry, const datT &fi) const
  {

    const auto set = [&fileEntry, &srcOffset](auto &ds) {
      ds.path = fileEntry;
      ds.offset = 0U;// the offset is 0 because we are getting the truncated data below.
      ds.GetBaseName();
    };
    switch (CheckExtension(fileEntry)) {
    case 1:
      set(fl_);
      fl_.data = FS::GetEntry<decltype(fl_.data)>(src, fi, srcOffset);
      FL::CleanBuffer(fl_.data);
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 2:
      set(fs_);
      fs_.data = FS::GetEntry(src, fi, srcOffset);
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 3:
      set(fi_);
      fi_.data = FS::GetEntry(src, fi, srcOffset);
      GetCount();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    default:
      break;
    }
    return 0;
  }
  void Test() const
  {
    if (!std::filesystem::exists(fl_.path)) {
      std::cout << "nested file!\n";
    }
    std::cout << *this << std::endl;
    std::cout << "Getting Filenames from : " << fl_.path << std::endl;
    FIFLFS archive{};
    auto items = Archive::FL::GetAllEntriesData(fl_.path, fl_.data, fl_.offset, fl_.size, count_);
    for (const auto &item : items) {
      const auto &[id, strVirtualPath] = item;
      // std::cout << "TryAddNested: {" << id << ", " << strVirtualPath << "}\n";

      auto fi = GetEntryIndex(id);
      {
        char retVal = [this, &archive, &fi, &strVirtualPath]() {
          std::filesystem::path virtualPath(strVirtualPath);
          if (!fs_.data.empty()) {
            return archive.TryAddNested(fs_.data, fs_.offset, virtualPath, fi);
          }

          if (fi.CompressionType() == Archive::CompressionTypeT::None) {
            auto localRetVal = archive.TryAdd(fs_.path, virtualPath, fs_.offset + fi.Offset(), fi.UncompressedSize());
            if (localRetVal != 0) {
              std::cout << virtualPath.filename() << " is uncompressed pointing at location in actual file!\n";
            }
            return localRetVal;
          }
          return archive.TryAddNested(fs_.path, fs_.offset, virtualPath, fi);
        }();
        if (retVal == 1) {
          continue;
        }
        if (retVal == 2) {
          archive.Test();
          archive = {};
          continue;
        }
      }
      {
        std::vector<char> buffer;
        if (!fs_.data.empty()) {
          buffer = FS::GetEntry(fs_.data, fi, fs_.offset);
        } else {
          buffer = FS::GetEntry(fs_.path, fi, fs_.offset);
        }
        if (buffer.empty()) {
          std::cout << '{' << id << ", "
                    << "Empty!"
                    << ", " << strVirtualPath << "}" << fi << std::endl;
          if (!(fi.UncompressedSize() == 0 && fi.CompressionType() == Archive::CompressionTypeT::None)) {
            exit(EXIT_FAILURE);
          }
        }
        if (fi.UncompressedSize() != buffer.size()) {
          exit(EXIT_FAILURE);
        }
        std::cout << '{' << id << ", " << buffer.size() << ", " << strVirtualPath << "}" << std::endl;
        Tools::WriteBuffer(buffer, strVirtualPath);
      }
    }
  }
  using FIFLFSmap = std::vector<std::pair<std::string, OpenVIII::Archive::FIFLFS<true>>>;

  static auto GetFilesFromPath(const std::filesystem::path &path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    auto tmp = FIFLFSmap();
    constexpr auto defaultSize = 6U;// battle, field, magic, main, menu, world
    tmp.reserve(defaultSize);
    auto archive = OpenVIII::Archive::FIFLFS<true>();
    for (const auto &fileEntry : std::filesystem::directory_iterator(path, options)) {
      if (archive.TryAdd(fileEntry) != 0) {
        if (archive.AllSet()) {// todo confirm basename matches right now i'm assuming the 3 files are together.
          // todo check for language codes to choose correct files
          // auto key = archive.GetBaseName();
          tmp.emplace_back(std::piecewise_construct,
            std::forward_as_tuple(archive.GetBaseName()),
            std::forward_as_tuple(std::move(archive)));
          archive = {};
        }
      }
    }
    tmp.shrink_to_fit();// if there is more than 6 it'll collapse the vector
    return tmp;
  }
  auto static CheckExtension(const std::filesystem::path &path)
  {
    return Tools::iEndsWithAny(
      path.string(), { OpenVIII::Archive::FL::Ext, OpenVIII::Archive::FS::Ext, OpenVIII::Archive::FI::Ext });
  }


  void compareBaseNames() const
  {
    if ((fl_.base == fs_.base && fi_.base == fs_.base) || std::empty(fl_.base) || std::empty(fi_.base)
        || std::empty(fs_.base)) {
      return;
    }
    if (fl_.base != fs_.base || fi_.base != fs_.base) {
      if (fl_.base == fi_.base) {
        std::cerr << "base name mismatch FS Data: " << fs_.path << "\n";
        // fs_ = {};
        exit(EXIT_FAILURE);
      }
    } else if (fi_.base != fl_.base) {
      if (fl_.base == fs_.base) {
        std::cerr << "base name mismatch FL Data: " << fl_.path << "\n";
        // fi_ = {};
        exit(EXIT_FAILURE);
      } else if (fi_.base == fs_.base) {
        std::cerr << "base name mismatch FI Data: " << fi_.path << "\n";
        // fl_ = {};
        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "No basename matched!\n";
      exit(EXIT_FAILURE);
    }
  }

  // todo move get files to here

  [[nodiscard]] std::string static GetBaseName(const std::filesystem::path &path)
  {
    if (path.string().empty()) {
      return {};
    }
    auto name = path.filename().stem().string();
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    return name;
  }
  [[nodiscard]] std::string GetBaseName() const
  {
    for (const auto &path : { fi_.base, fl_.base, fs_.base }) {
      if (!path.empty()) {
        return path;
      }
    }
    return {};
  }
  template<typename outT = std::vector<char>>[[nodiscard]] outT GetEntryData(const OpenVIII::Archive::FI &fi) const
  {
    return [this, &fi]() {
      if (std::empty(fs_.data)) {
        return OpenVIII::Archive::FS::GetEntry<outT>(fs_.path, fi, fs_.offset);
      }
      return OpenVIII::Archive::FS::GetEntry<outT>(fs_.data, fi, fs_.offset);
    }();
  }
  template<typename outT = std::vector<char>>[[nodiscard]] outT GetEntryData(const std::string_view &filename) const
  {
    const auto &[id, path] = [this, &filename]() {
      if (std::empty(fl_.data)) {
        return OpenVIII::Archive::FL::GetEntry(fl_.path, { filename }, fl_.offset, fl_.size, count_);
      }
      return OpenVIII::Archive::FL::GetEntryData(fl_.path, fl_.data, { filename }, fl_.offset, fl_.size, count_);
    }();

    //    const auto &fi = [this, &filename, &id]() {
    //      if (std::empty(fi_.data)) {
    //        return OpenVIII::Archive::FI(fi_.path, id, fi_.offset);
    //      }
    //      return OpenVIII::Archive::FI(fi_.data, id, fi_.offset);
    //    }();
    return GetEntryData<outT>(GetEntryIndex(id));
    //    return  [this, &filename, &fi]() {
    //      if(std::empty(fs_.data)) {
    //        return OpenVIII::Archive::FS::GetEntry(fs_.path, fi, fs_.offset);
    //      }
    //      return OpenVIII::Archive::FS::GetEntry(fs_.data,fi,fs_.offset);
    //    }();
  }
};

}// namespace OpenVIII::Archive
#endif// !VIIIARCHIVE_FIFLFS_H
