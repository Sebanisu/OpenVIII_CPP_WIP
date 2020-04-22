#ifndef OPENVIII_FIFLFS_H
#define OPENVIII_FIFLFS_H

#include "FI.hpp"
#include "FL.hpp"
#include "FS.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <set>
#include <iterator>

namespace OpenVIII::Archive {
struct FIFLFS
{
private:
  std::filesystem::path fi_{ "" };
  std::filesystem::path fs_{ "" };
  std::filesystem::path fl_{ "" };
  size_t fiOffset_{ 0U };
  size_t fsOffset_{ 0U };
  size_t flOffset_{ 0U };
  size_t count_{ 0U };
  std::vector<unsigned char> fiData_{};
  std::vector<unsigned char> fsData_{};
  std::basic_string<char> flData_{};// this is char because the file contains strings.
public:
  [[maybe_unused]] [[nodiscard]] auto FI() const { return fi_; }

  [[maybe_unused]] [[nodiscard]] auto FS() const { return fs_; }

  [[maybe_unused]] [[nodiscard]] auto FL() const { return fl_; }

  [[maybe_unused]] [[nodiscard]] auto FIData() const { return fiData_; }

  [[maybe_unused]] [[nodiscard]] auto FSData() const { return fsData_; }

  [[maybe_unused]] [[nodiscard]] auto FLData() const { return flData_; }
  [[maybe_unused]] void FI(const std::filesystem::path &fi, const size_t &offset = 0U, size_t size = 0U)
  {
    fi_ = fi;
    fiOffset_ = offset;
    if (size == 0U) {
      if (std::filesystem::exists(fi_)) { size = std::filesystem::file_size(fi_); }
    }
    count_ = FI::GetCount(size);
  }
  void FI(const std::filesystem::path &fi, const std::vector<unsigned char> &vector, const size_t &offset = 0U)
  {
    FI(fi, offset, vector.size());
    if (!vector.empty()) { fiData_ = vector; }
  }

  void FS(const std::filesystem::path &fs, const size_t &offset = 0U)
  {
    fs_ = fs;
    fsOffset_ = offset;
  }
  void FS(const std::filesystem::path &fs, const std::vector<unsigned char> &vector, const size_t &offset = 0U)
  {
    FS(fs, offset);
    if (!vector.empty()) { fsData_ = vector; }
  }

  void FL(const std::filesystem::path &fl, const size_t &offset = 0U)
  {
    fl_ = fl;
    flOffset_ = offset;
  }

  void FL(const std::filesystem::path &fl, const std::vector<unsigned char> &vector, const size_t &offset = 0U)
  {
    FL(fl, offset);
    if (!vector.empty()) {
      flData_ = std::basic_string<char>();
      flData_.resize(vector.size());
      std::memcpy(flData_.data(), vector.data(), vector.size());// optimizes away safer than cast.
      // check for garbage
      auto index =
        flData_.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_\n\r\\:/.");
      if (index != std::string::npos) { std::cerr << "\nError: Invalid character found in FL data! = " << fl << '\n'; }
    }
  }

  [[maybe_unused]] void FL(const std::filesystem::path &fl, const std::string_view &vector, const size_t &offset = 0U)
  {
    FL(fl, offset);
    if (!vector.empty()) { flData_ = vector; }
  }

  [[nodiscard]] bool AllSet() const { return (fi_ != "") && (fs_ != "") && (fl_ != ""); }

  FIFLFS() = default;

  FIFLFS(const std::filesystem::path &fi,
    const std::filesystem::path &fs,
    const std::filesystem::path &fl,
    const size_t &fiOffset = 0U,
    const size_t &fsOffset = 0U,
    const size_t &flOffset = 0U)
  {
    fi_ = fi;
    fs_ = fs;
    fl_ = fl;
    fiOffset_ = fiOffset;
    flOffset_ = flOffset;
    fsOffset_ = fsOffset;
  }

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FIFLFS &data)
  {
    os << '{' << data.GetBaseName() << ", " << '{' << data.fi_ << ", " << data.fl_ << ", " << data.fs_ << "}}";
    return os;
  }

  //
  //		[[nodiscard]] constexpr auto Count()
  //		{
  //			return count_;
  //		}
  [[nodiscard]] auto GetEntry(const unsigned int &id) const
  {
    auto fi = Archive::FI();
    if (id < count_ || count_ == 0) {
      if (!fiData_.empty()) {
        fi = FI::GetEntry(fiData_, id, fiOffset_);
      } else {
        fi = FI::GetEntry(fi_, id, fiOffset_);
      }
    }

    if (!fsData_.empty()) {
      return FS::GetEntry(fsData_, fi, fsOffset_);
    }
    return FS::GetEntry(fs_, fi, fsOffset_);
  }

  [[maybe_unused]] [[nodiscard]] auto GetEntry(const std::string_view &needle) const
  {
    auto found = FL::GetEntry(fl_, needle, fiOffset_);
    const auto &[id, path] = found;
    return GetEntry(id);
  }

  void Test() const
  {
    std::cout << "Getting Filenames from : " << fl_ << std::endl;
    const auto allFL = Archive::FL::GetAllEntriesData(fl_, flData_, flOffset_);
    auto archive = OpenVIII::Archive::FIFLFS();
    std::filesystem::path path;
    std::filesystem::path dir;
    std::filesystem::path filename;
    std::basic_ofstream<char> fp;
    for (const auto &item : allFL) {
      const auto &[id, strPath] = item;
      const auto directoryEntry = std::filesystem::directory_entry(strPath);
      auto buffer = GetEntry(id);
      if (archive.SetByExtension(directoryEntry, buffer)) {
        if (archive.AllSet()) {
          std::cout << archive << std::endl;
          archive.Test();
          archive = FIFLFS();
        }
        continue;
      }

      const auto writeBuffer = [&fp, &buffer]() {
        auto tmp = std::vector<char>(buffer.size());
        memcpy(tmp.data(), buffer.data(), buffer.size());
        fp.write(tmp.data(), buffer.size());
        // todo in cpp20 use bitcast instead. or find another way to write data.
      };
      const auto &size{ buffer.size() };
      std::cout << '{' << id << ", " << size << ", " << strPath << "}" << std::endl;
      if (size == 0) { continue; }
      if (buffer.empty()) { continue; }
      path = strPath;
      dir = std::filesystem::path("tmp");
      filename = dir / path;
      std::filesystem::create_directories(filename.parent_path());
      fp = std::ofstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
      if (fp.is_open()) { writeBuffer(); }
      fp.close();
    }
  }


  bool SetByExtension(const std::filesystem::directory_entry &fileEntry, const std::vector<unsigned char> &vector)
  {
    if (fileEntry.path().has_extension()) {
      unsigned char i = 0;
      // order to match the switch below
      for (const auto &ext : { OpenVIII::Archive::FL::Ext, OpenVIII::Archive::FS::Ext, OpenVIII::Archive::FI::Ext }) {
        if (OpenVIII::Tools::iEquals(fileEntry.path().extension().string(), ext)) {
          switch (i) {
          case 0:
            FL(fileEntry, vector, 0U);
            return true;
          case 1:
            FS(fileEntry, vector, 0U);
            return true;
          case 2:
            FI(fileEntry, vector, 0U);
            return true;
          default:
            break;
          }
          break;
        }
        i++;
      }
    }
    return false;
  }

  // todo move get files to here
  using FIFLFSmap = std::map<std::string, OpenVIII::Archive::FIFLFS>;

  static FIFLFSmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    return GetFilesFromIterator(std::filesystem::directory_iterator(path, options));// todo may need sorted.
  }

  template<class iter_t> static FIFLFSmap GetFilesFromIterator(iter_t iter)
  {
    //            static_assert(!std::is_same<typename std::iterator_traits<iter_t>::value_type,
    //                    void>::value, "not an iterator!");
    auto tmp = FIFLFSmap();
    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &fileEntry : iter) {
      if (archive.SetByExtension(fileEntry, std::vector<unsigned char>())) {
        if (archive.AllSet()) {// todo confirm basename matches right now i'm assuming the 3 files are together.
          // todo check for language codes to choose correct files
          tmp.insert(std::make_pair(archive.GetBaseName(), archive));
          archive = OpenVIII::Archive::FIFLFS();
        }
      }
    }
    return tmp;
  }

  std::string GetBaseName() const
  {
    std::string name;
    for (const auto &path : { FI(), FL(), FS() }) {
      if (!path.string().empty()) {
        name = path.filename().stem().string();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        break;
      }
    }
    return name;
  }

  static void testPAIR(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair)
  {
    const auto &[name, paths] = pair;
    std::cout << paths << '\n';
    paths.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
  // todo make getfiles allow recursive archives
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FIFLFS_H
