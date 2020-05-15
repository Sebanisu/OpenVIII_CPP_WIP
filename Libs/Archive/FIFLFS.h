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
    std::filesystem::path path{};
    size_t offset{};
    T data{};
    std::string base{};

    // Assigns basename and returns it.
    std::string GetBaseName() { return base = FIFLFS::GetBaseName(path); }
    operator bool() const { return (!path.empty() && std::filesystem::exists(path)) || !std::empty(data); }
  };
  ds_<std::vector<char>> fi_{};
  ds_<std::vector<char>> fs_{};
  ds_<std::basic_string<char>> fl_{};// this is char because the file contains strings.
  size_t count_{};
  void FI(const std::filesystem::path &path, const size_t &offset = 0U, size_t size = 0U)
  {
    fi_.path = path;
    fi_.offset = offset;
    if (size == 0U && std::filesystem::exists(fi_.path)) { size = std::filesystem::file_size(fi_.path); }
    count_ = FI::GetCount(size);
  }
  void GetCount(size_t size = 0U)
  {
    if (size == 0U && std::filesystem::exists(fi_.path)) { size = std::filesystem::file_size(fi_.path); }
    if (size == 0U && !fi_.data.empty()) size = fi_.data.size();
    count_ = FI::GetCount(size);
  }

  void FS(const std::filesystem::path &path, const size_t &offset = 0U)
  {
    fs_.path = path;
    fs_.offset = offset;
  }

  void FL(const std::filesystem::path &path, const size_t &offset = 0U)
  {
    fl_.path = path;
    fl_.offset = offset;
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

  [[nodiscard]] auto GetEntry(const unsigned int &id) const
  {
    if (id < count_ || count_ == 0) {
      if (!fi_.data.empty()) {
        return FI::GetEntry(fi_.data, id, fi_.offset);
      } else {
        return FI::GetEntry(fi_.path, id, fi_.offset);
      }
    }
    return OpenVIII::Archive::FI();
  }


  operator bool() const { return AllSet(); }
  /*
   * 0 = didn't add
   * 1 = added
   * 2 = added and all set
   * */

  char TryAdd(const std::filesystem::path &fileEntry)
  {
    switch (CheckExtension(fileEntry)) {
    case 1:
      FL(fileEntry, 0U);
      fl_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 2:
      FS(fileEntry, 0U);
      fs_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 3:
      FI(fileEntry, 0U);
      GetCount();
      fi_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    default:
      break;
    }

    return 0;
  }
  template<typename srcT = std::vector<char>, typename datT = Archive::FI>
  char TryAddNested(const srcT &src, const size_t srcOffset, const std::filesystem::path &fileEntry, const datT &fi)
  {

    switch (CheckExtension(fileEntry)) {
    case 1:
      FL(fileEntry, 0U);
      FS::GetEntry(src, fi, srcOffset, fl_.data);
      //remove carriage returns
      fl_.data.erase(std::remove(fl_.data.begin(),fl_.data.end(),'\r'),fl_.data.end());
      //change slashes to preferred
      Tools::replaceSlashes(fl_.data);
      fl_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 2:
      FS(fileEntry, 0U);
      FS::GetEntry(src, fi, srcOffset, fs_.data);
      fs_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 3:
      FI(fileEntry, 0U);
      FS::GetEntry(src, fi, srcOffset, fi_.data);
      GetCount();
      fi_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    default:
      break;
    }
    return 0;
  }
  void Test() const
  {
    if (!std::filesystem::exists(fl_.path)) { std::cout << "internal file!\n"; }
    std::cout << *this << std::endl;
    std::cout << "Getting Filenames from : " << fl_.path << std::endl;
    FIFLFS archive{};
    for (const auto &item : Archive::FL::GetAllEntriesData(fl_.path, fl_.data, fl_.offset, count_)) {
      const auto &[id, strPath] = item;
      auto fi = GetEntry(id);
      {
        std::filesystem::path path(strPath);
        char retVal;
        if (!fs_.data.empty()) {
          retVal = archive.TryAddNested(fs_.data, fs_.offset, path, fi);

        } else {
          retVal = archive.TryAddNested(fs_.path, fs_.offset, path, fi);
        }
        if (retVal == 1) { continue; }
        if (retVal == 2) {
          archive.Test();
          archive = {};
          continue;
        };
      }
      {
        std::vector<char> buffer{};
        if (!fs_.data.empty()) {
          FS::GetEntry(fs_.data, fi, fs_.offset, buffer);
        } else {
          FS::GetEntry(fs_.path, fi, fs_.offset, buffer);
        }
        if (buffer.empty()) {
          std::cout << '{' << id << ", "
                    << "Empty!"
                    << ", " << strPath << "}" << fi << std::endl;
          if (fi.UncompressedSize() > 0) { exit(EXIT_FAILURE); }
        }
        if (fi.UncompressedSize() != buffer.size()) { exit(EXIT_FAILURE); }
        std::cout << '{' << id << ", " << buffer.size() << ", " << strPath << "}" << std::endl;
        Tools::WriteBuffer(buffer, strPath);
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



  void compareBaseNames()
  {
    if ((fl_.base == fs_.base && fi_.base == fs_.base) || std::empty(fl_.base) || std::empty(fi_.base)
        || std::empty(fs_.base)) {
      return;
    }
    if (fl_.base != fs_.base || fi_.base != fs_.base) {
      if (fl_.base == fi_.base) {
        std::cerr << "Removing FS Data: " << fs_ << "\n";
        fs_ = {};
        exit(EXIT_FAILURE);
      }
    } else if (fi_.base != fl_.base) {
      if (fl_.base == fs_.base) {
        std::cerr << "Removing FL Data: " << fl_ << "\n";
        fi_ = {};
        exit(EXIT_FAILURE);
      } else if (fi_.base == fs_.base) {
        std::cerr << "Removing FI Data: " << fi_ << "\n";
        fl_ = {};
        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "No files matched!\n";
      exit(EXIT_FAILURE);
    }
  }

  // todo move get files to here
  using FIFLFSmap = std::map<std::string, OpenVIII::Archive::FIFLFS>;

  [[nodiscard]] static FIFLFSmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    return GetFilesFromIterator(std::filesystem::directory_iterator(path, options));// todo may need sorted.
  }
  template<class iter_t>[[nodiscard]] static FIFLFSmap GetFilesFromIterator(iter_t iter)
  {
    auto tmp = FIFLFSmap();
    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &fileEntry : iter) {
      if (archive.TryAdd(fileEntry)) {
        if (archive.AllSet()) {// todo confirm basename matches right now i'm assuming the 3 files are together.
          // todo check for language codes to choose correct files
          tmp.insert(std::make_pair(archive.GetBaseName(), archive));
          archive = OpenVIII::Archive::FIFLFS();
        }
      }
    }
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
    std::string name;
    for (const auto &path : { fi_.path, fl_.path, fs_.path }) {
      if (!path.string().empty()) {
        name = path.filename().stem().string();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        break;
      }
    }
    return name;
  }

  static void testPair(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair)
  {
    const auto &[name, paths] = pair;
    std::cout << paths << '\n';
    paths.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
  // todo make get files allow recursive archives
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FIFLFS_H
