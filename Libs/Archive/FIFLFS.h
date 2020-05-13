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

public:
  //  [[maybe_unused]] [[nodiscard]] auto FI() const noexcept { return fi_; }
  //
  //  [[maybe_unused]] [[nodiscard]] auto FS() const noexcept { return fs_; }
  //
  //  [[maybe_unused]] [[nodiscard]] auto FL() const noexcept { return fl_; }

  [[maybe_unused]] [[nodiscard]] const auto &FI() const noexcept { return fi_; }

  [[maybe_unused]] [[nodiscard]] const auto &FS() const noexcept { return fs_; }

  [[maybe_unused]] [[nodiscard]] const auto &FL() const noexcept { return fl_; }

  [[maybe_unused]] void FI(const std::filesystem::path &path, const size_t &offset = 0U, size_t size = 0U)
  {
    fi_.path = path;
    fi_.offset = offset;
    if (size == 0U && std::filesystem::exists(fi_.path)) { size = std::filesystem::file_size(fi_.path); }
    count_ = FI::GetCount(size);
  }
  void FI(const std::filesystem::path &path, const std::vector<char> &vector, const size_t &offset = 0U)
  {
    FI(path, offset, vector.size());
    if (!vector.empty()) { fi_.data = vector; }// todo std::move into this.
  }

  void FS(const std::filesystem::path &path, const size_t &offset = 0U)
  {
    fs_.path = path;
    fs_.offset = offset;
  }
  void FS(const std::filesystem::path &path, const std::vector<char> &vector, const size_t &offset = 0U)
  {
    FS(path, offset);
    if (!vector.empty()) { fs_.data = vector; }// todo std::move into this.
  }

  void FL(const std::filesystem::path &path, const size_t &offset = 0U)
  {
    fl_.path = path;
    fl_.offset = offset;
  }

  void FL(const std::filesystem::path &path, const std::vector<char> &vector, const size_t &offset = 0U)
  {
    // todo change vector to std::string
    FL(path, offset);
    if (!vector.empty()) {
      fl_.data =
        std::basic_string<char>(vector.begin(), vector.end());// todo std::move after getting string instead of vector.
      // check for garbage
      auto index =
        fl_.data.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_\n\r\\:/.-");
      if (index != std::string::npos) {
        std::cerr << "\nError: Invalid character (" << static_cast<int>(fl_.data.at(index))
                  << ") found in FL data! = " << fl_.path << '\n';
        exit(EXIT_FAILURE);
      }
    }
  }

  [[maybe_unused]] void
    FL(const std::filesystem::path &path, const std::string_view &stringView, const size_t &offset = 0U)
  {
    FL(path, offset);
    if (!stringView.empty()) { fl_.data = stringView; }// todo std::move after getting string instead of stringView.
  }

  [[nodiscard]] bool AllSet() const { return fi_ && fs_ && fl_; }

  FIFLFS() = default;

  FIFLFS(const std::filesystem::path &fi,
    const std::filesystem::path &fs,
    const std::filesystem::path &fl,
    const size_t &fiOffset = 0U,
    const size_t &fsOffset = 0U,
    const size_t &flOffset = 0U)
  {
    fi_.path = fi;
    fs_.path = fs;
    fl_.path = fl;
    fi_.offset = fiOffset;
    fl_.offset = flOffset;
    fs_.offset = fsOffset;
    // todo should i have this constructor? is it even used? the reason i think not setting all possible values and why
    // would we want it to...
  }

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

  [[maybe_unused]] [[nodiscard]] auto GetEntry(const std::string_view &needle) const
  {
    auto found = FL::GetEntry(fl_.path, needle, fi_.offset);
    const auto &[id, path] = found;
    return GetEntry(id);
  }
  auto TryAdd(const std::string_view &strPath, const std::vector<char> &buffer)
  {
    const auto directoryEntry = std::filesystem::directory_entry(strPath);
    return TryAdd(directoryEntry, buffer);
  }
  auto static TryAddTestReset(FIFLFS &archive, const std::string_view &strPath, const std::vector<char> &buffer)
  {
    switch (archive.TryAdd(strPath, buffer)) {
    case 1:
      return true;
    case 2:
      std::cout << archive << std::endl;
      archive.Test();
      archive = FIFLFS();
      return true;
    }
    return false;
  }
  void Test() const
  {
    std::cout << "Getting Filenames from : " << fl_.path << std::endl;
    const auto entries = Archive::FL::GetAllEntriesData(fl_.path, fl_.data, fl_.offset, count_);
    auto archive = FIFLFS();
    for (const auto &item : entries) {
      const auto &[id, strPath] = item;
      auto fi = GetEntry(id);

      std::vector<char> buffer{};
      if (fi.UncompressedSize() > 0) {
        if (!fs_.data.empty()) {
          buffer = FS::GetEntry(fs_.data, fi, fs_.offset);
        } else {
          buffer = FS::GetEntry(fs_.path, fi, fs_.offset);
        }
      }

      if (buffer.size() == 0) {
        std::cout << '{' << id << ", "
                  << "Empty!"
                  << ", " << strPath << "}" << fi << std::endl;
        if (fi.UncompressedSize() > 0) { exit(EXIT_FAILURE); }
      } else {
        if (TryAddTestReset(archive, strPath, buffer)) { continue; }
        std::cout << '{' << id << ", " << buffer.size() << ", " << strPath << "}" << std::endl;
        Tools::WriteBuffer(buffer, strPath);
      }
      if (fi.UncompressedSize() != buffer.size()) { exit(EXIT_FAILURE); }
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
  /*
   * 0 = didn't add
   * 1 = added
   * 2 = added and all set
   * */
  char TryAdd(const std::filesystem::directory_entry &fileEntry, const std::vector<char> &vector)
  {
    switch (CheckExtension(fileEntry.path())) {
    case 1:
      FL(fileEntry, vector, 0U);
      fl_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 2:
      FS(fileEntry, vector, 0U);
      fs_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 3:
      FI(fileEntry, vector, 0U);
      fi_.GetBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    default:
      break;
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

  [[maybe_unused]] static FIFLFSmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    return GetFilesFromIterator(std::filesystem::directory_iterator(path, options));// todo may need sorted.
  }
  template<class iter_t>[[nodiscard]] static FIFLFSmap GetFilesFromIterator(iter_t iter)
  {
    auto tmp = FIFLFSmap();
    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &fileEntry : iter) {
      if (archive.TryAdd(fileEntry, std::vector<char>())) {
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

  [[maybe_unused]] static void testPair(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair)
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
