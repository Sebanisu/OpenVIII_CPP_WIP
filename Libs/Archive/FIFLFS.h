#ifndef OPENVIII_FIFLFS_H
#define OPENVIII_FIFLFS_H

#include "FI.hpp"
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
struct FIFLFS
{
private:
  template<typename T> struct ds_
  {
    mutable std::filesystem::path path{};
    mutable size_t offset{};
    mutable size_t size{};// if forced otherwise 0;
    mutable T data{};
    mutable std::string base{};

    //    // Assigns basename and returns it.
    [[maybe_unused]] std::string GetBaseName() const noexcept { return base = FIFLFS::GetBaseName(path); }
    operator bool() const
    {
      return (!path.empty() && std::filesystem::exists(path)) || !std::empty(data)
             || (!path.empty() && (offset > 0 || size > 0));
    }
  };
  ds_<std::vector<char>> fi_{};
  ds_<std::vector<char>> fs_{};
  ds_<std::basic_string<char>> fl_{};// this is char because the file contains strings.
  mutable size_t count_{};
  void GetCount(size_t size = 0U) const
  {
    if (size == 0U) size = fi_.size;
    if (size == 0U && std::filesystem::exists(fi_.path)) { size = std::filesystem::file_size(fi_.path); }
    if (size == 0U && !fi_.data.empty()) size = fi_.data.size();
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
    os << '{' << data.GetBaseName() << ", " << '{' << data.fi_.path << ", " << data.fl_.path << ", " << data.fs_.path
       << "}}";
    return os;
  }

  [[nodiscard]] Archive::FI GetEntry(const unsigned int &id) const
  {
    if (count_ == 0 || id < count_) {
      if (!fi_.data.empty()) {
        return Archive::FI(fi_.data, id, fi_.offset);
      } else {
        return Archive::FI(fi_.path, id, fi_.offset);
      }
    }
    return {};
  }


  operator bool() const noexcept { return AllSet(); }
  /*
   * 0 = didn't add
   * 1 = added
   * 2 = added and all set
   * */

  char TryAdd(const std::filesystem::path &existingFilePath,
    const std::filesystem::path &virtualFilePath = "",
    size_t offset = 0U,
    size_t size = 0U) const
  {
    const auto set = [&existingFilePath, &offset, &virtualFilePath, &size](auto &ds) {
      ds.path = existingFilePath;
      ds.offset = offset;
      ds.size = size;
      if (virtualFilePath.has_stem()) ds.base = GetBaseName(virtualFilePath);
      else
        ds.GetBaseName();
    };
    switch (virtualFilePath.has_extension() ? CheckExtension(virtualFilePath) : CheckExtension(existingFilePath)) {
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
      // remove carriage returns
      fl_.data.erase(std::remove(fl_.data.begin(), fl_.data.end(), '\r'), fl_.data.end());
      // change slashes to preferred
      Tools::replaceSlashes(fl_.data);
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
    if (!std::filesystem::exists(fl_.path)) { std::cout << "nested file!\n"; }
    std::cout << *this << std::endl;
    std::cout << "Getting Filenames from : " << fl_.path << std::endl;
    FIFLFS archive{};
    auto items = Archive::FL::GetAllEntriesData(fl_.path, fl_.data, fl_.offset, fl_.size, count_);
    for (const auto &item : items) {
      const auto &[id, strVirtualPath] = item;
      // std::cout << "TryAddNested: {" << id << ", " << strVirtualPath << "}\n";

      auto fi = GetEntry(id);
      {
        std::filesystem::path virtualPath(strVirtualPath);
        char retVal;
        if (!fs_.data.empty()) {
          retVal = archive.TryAddNested(fs_.data, fs_.offset, virtualPath, fi);

        } else {
          if (fi.CompressionType() == Archive::CompressionTypeT::None) {
            retVal = archive.TryAdd(fs_.path, virtualPath, fs_.offset + fi.Offset(), fi.UncompressedSize());
            if(retVal!=0)
            {
              std::cout << virtualPath.filename() << " is uncompressed pointing at location in actual file!\n";
            }
          } else {
            retVal = archive.TryAddNested(fs_.path, fs_.offset, virtualPath, fi);
          }
        }
        if (retVal == 1) { continue; }
        if (retVal == 2) {
          archive.Test();
          archive = {};
          continue;
        };
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
        if (fi.UncompressedSize() != buffer.size()) { exit(EXIT_FAILURE); }
        std::cout << '{' << id << ", " << buffer.size() << ", " << strVirtualPath << "}" << std::endl;
        Tools::WriteBuffer(buffer, strVirtualPath);
      }
    }
  }

  char static CheckExtension(const std::filesystem::path &path)
  {

    char i = 0;// 0 does not match;
    if (path.has_extension())
      for (const auto &ext : { OpenVIII::Archive::FL::Ext, OpenVIII::Archive::FS::Ext, OpenVIII::Archive::FI::Ext }) {
        i++;
        if (OpenVIII::Tools::iEquals(path.extension().string(), ext)) { return i; }
      }

    return 0;
  }


  void compareBaseNames() const
  {
    if ((fl_.base == fs_.base && fi_.base == fs_.base) || std::empty(fl_.base) || std::empty(fi_.base)
        || std::empty(fs_.base)) {
      return;
    }
    if (fl_.base != fs_.base || fi_.base != fs_.base) {
      if (fl_.base == fi_.base) {
        std::cerr << "base name mismatch FS Data: " << fs_ << "\n";
        // fs_ = {};
        exit(EXIT_FAILURE);
      }
    } else if (fi_.base != fl_.base) {
      if (fl_.base == fs_.base) {
        std::cerr << "base name mismatch FL Data: " << fl_ << "\n";
        // fi_ = {};
        exit(EXIT_FAILURE);
      } else if (fi_.base == fs_.base) {
        std::cerr << "base name mismatch FI Data: " << fi_ << "\n";
        // fl_ = {};
        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "No basename matched!\n";
      exit(EXIT_FAILURE);
    }
  }

  // todo move get files to here
  using FIFLFSmap = std::vector<std::pair<std::string, OpenVIII::Archive::FIFLFS>>;

  [[nodiscard]] static FIFLFSmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    return GetFilesFromIterator(std::filesystem::directory_iterator(path, options));// todo may need sorted.
  }
  template<class iter_t>[[nodiscard]] static FIFLFSmap GetFilesFromIterator(iter_t iter)
  {
    auto tmp = FIFLFSmap();
    tmp.reserve(6U);// battle, field, magic, main, menu, world
    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &fileEntry : iter) {
      if (archive.TryAdd(fileEntry)) {
        if (archive.AllSet()) {// todo confirm basename matches right now i'm assuming the 3 files are together.
          // todo check for language codes to choose correct files
          // auto key = archive.GetBaseName();
          tmp.emplace_back(std::make_pair(archive.GetBaseName(), std::move(archive)));
          archive = {};
        }
      }
    }
    tmp.shrink_to_fit();// if there is more than 6 it'll collapse the vector
    return tmp;
  }
  [[nodiscard]] std::string static GetBaseName(const std::filesystem::path &path)
  {
    if (path.string().empty()) return std::string{};
    auto name = path.filename().stem().string();
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    return name;
  }
  [[nodiscard]] std::string GetBaseName() const
  {
    for (const auto &path : { fi_.base, fl_.base, fs_.base }) {
      if (!path.empty()) { return path; }
    }
    return {};
  }

  static void testPair(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair)
  {
    const auto &[name, paths] = pair;
    std::cout << paths << '\n';
    paths.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FIFLFS_H
