//
// Created by pcvii on 4/23/2020.
//
#ifndef VIIICPPTEST_ZZZ_HPP
#define VIIICPPTEST_ZZZ_HPP
#include <string>
#include "FI.hpp"
#include "../Tools/Tools.hpp"
#include <filesystem>
#include <set>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "FIFLFS.hpp"
#include "ZZZ/FileData.hpp"
namespace OpenVIII::Archive {

struct [[maybe_unused]] ZZZ
{


private:
  // stored at top of zzz file to tell how many files are stored inside
  //  [[maybe_unused]] unsigned int count_{};
  // file data inside zzz file.
  std::set<FileData, FileData::Comparator> data_{};

  [[maybe_unused]] constexpr static auto Ext = ".zzz";

  std::filesystem::path path_{};

public:
  constexpr ZZZ() = default;
  //[[maybe_unused]] explicit ZZZ(const std::set<FileData, FileData::Comparator> &data);
  //  {
  //    data_ = data;
  //    count_ = static_cast<unsigned int>(std::size(data));
  //  }
  //  [[maybe_unused]] constexpr auto size() { return std::size(data_); }
  //  [[maybe_unused]] auto begin() { return std::begin(data_); }
  //  [[maybe_unused]] auto end() { return std::end(data_); }
  [[maybe_unused]] [[nodiscard]] static auto GetAllEntries(const std::filesystem::path &path)
  {
    auto data = std::set<FileData, FileData::Comparator>();
    if (!(path.has_extension() && Tools::iEquals(path.extension().string(), Ext)) || !std::filesystem::exists(path)) {
      return data;
    }


    unsigned int count{};
    auto fp = std::ifstream(path, std::ios::binary | std::ios::in);
    if (!fp.is_open()) {
      fp.close();
      return data;
    }
    std::cout << "Getting File Entries from : " << path << std::endl;
    std::array<char, sizeof(count)> tmp{};
    fp.read(tmp.data(), tmp.size());
    memcpy(&count, tmp.data(), sizeof(count));
    for (auto i = 0U; fp.is_open() && !fp.eof() && i < count; i++) {
      auto fd = FileData::GetEntry(fp);
      if (!fd.GetPathString().empty()) { data.insert(fd); }
    }
    fp.close();
    return data;
  }
  [[maybe_unused]] explicit ZZZ(const std::filesystem::path &path)
  {
    path_ = path;
    data_ = GetAllEntries(path_);
  }
  static auto GetEntry(const std::filesystem::path &path, const FileData &data)
  {
    if (!(path.has_extension() && Tools::iEquals(path.extension().string(), Ext)) || !std::filesystem::exists(path)) {
      return std::vector<unsigned char>();
    }
    auto fp = std::ifstream(path, std::ios::binary | std::ios::in);
    if (!fp.is_open()) {
      fp.close();
      return std::vector<unsigned char>();
    }

    auto buffer = std::vector<unsigned char>(data.Size());
    fp.seekg(static_cast<long>(data.Offset()));
    auto tmp = std::vector<char>(data.Size());
    fp.read(tmp.data(), static_cast<long>(data.Size()));
    fp.close();
    memcpy(buffer.data(), tmp.data(), data.Size());
    // todo in cpp20 use bitcast instead. or find another way to write data.
    return buffer;
  }
  auto GetEntry(const FileData &data) const { return GetEntry(path_, data); }

  [[maybe_unused]] void Test() const
  {

    auto archive = OpenVIII::Archive::FIFLFS();
    std::filesystem::path path;
    std::filesystem::path dir;
    std::filesystem::path filename;
    std::basic_ofstream<char> fp;
    for (const auto &item : data_) {
      const auto &[zzzPath, zzzOffset, zzzSize] = item.GetTuple();
      const auto directoryEntry = std::filesystem::directory_entry(zzzPath);
      auto buffer = GetEntry(item);
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
        fp.write(tmp.data(), static_cast<long>(buffer.size()));
        // todo in cpp20 use bitcast instead. or find another way to write data.
      };
      const auto &size{ buffer.size() };
      std::cout << '{' << size << ", " << zzzPath << "}\n";
      if (size == 0) { continue; }
      if (buffer.empty()) { continue; }
      path = zzzPath;
      dir = std::filesystem::path("tmp");
      filename = dir / path;
      std::filesystem::create_directories(filename.parent_path());
      fp = std::ofstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
      if (fp.is_open()) { writeBuffer(); }
      fp.close();
    }
  }
  using ZZZmap = std::map<std::string, OpenVIII::Archive::ZZZ>;

  static ZZZmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;

    auto tmp = ZZZmap();
    auto archive = OpenVIII::Archive::ZZZ();
    for (const auto &fileEntry : std::filesystem::directory_iterator(path, options)) {
      if(!(fileEntry.path().has_extension() && Tools::iEquals(fileEntry.path().extension().string(),Ext))) {continue;}
      // todo check for language codes to choose correct files
      auto basename = fileEntry.path().filename().stem().string();
      std::transform(basename.begin(),basename.end(), basename.begin(),::toupper);
      archive = OpenVIII::Archive::ZZZ(fileEntry);
      tmp.insert(std::make_pair(basename, archive));
    }
    return tmp;
  }
  static void testPAIR(const std::pair<std::string_view, OpenVIII::Archive::ZZZ> &pair)
  {
    const auto &[name, zzz] = pair;
    std::cout << '{' << name << ", " << zzz.path_ << "}\n";
    zzz.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
};

}// namespace OpenVIII::Archive
#endif// VIIICPPTEST_ZZZ_HPP