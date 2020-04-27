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
      return std::vector<char>();
    }
    auto fp = std::ifstream(path, std::ios::binary | std::ios::in);
    if (!fp.is_open()) {
      fp.close();
      return std::vector<char>();
    }

    auto buffer = std::vector<char>(data.Size());
    fp.seekg(static_cast<long>(data.Offset()));
    fp.read(buffer.data(), static_cast<long>(data.Size()));
    fp.close();
    // todo in cpp20 use bitcast instead. or find another way to write data.
    return buffer;
  }
  auto GetEntry(const FileData &data) const { return GetEntry(path_, data); }

  [[maybe_unused]] void Test() const
  {

    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &item : data_) {
      const auto &[zzzPath, zzzOffset, zzzSize] = item.GetTuple();
      auto buffer = GetEntry(item);
      if (FIFLFS::TryAddTestReset(archive, zzzPath.string(), buffer)) { continue; }
      std::cout << '{' << buffer.size() << ", " << zzzPath << "}\n";
      Tools::WriteBuffer(buffer, zzzPath.string());
    }
  }
  using ZZZmap = std::map<std::string, OpenVIII::Archive::ZZZ>;

  [[maybe_unused]] [[nodiscard]] static ZZZmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;

    auto tmp = ZZZmap();
    auto archive = OpenVIII::Archive::ZZZ();
    int i{};
    for (const auto &fileEntry : std::filesystem::directory_iterator(path, options)) {
      if (!(fileEntry.path().has_extension() && Tools::iEquals(fileEntry.path().extension().string(), Ext))) {
        continue;
      }
      // todo check for language codes to choose correct files
      auto basename = fileEntry.path().filename().stem().string();
      if (std::empty(basename)) tmp.insert(std::make_pair("__"+std::to_string(i++),archive));
      std::transform(basename.begin(), basename.end(), basename.begin(), ::toupper);
      archive = OpenVIII::Archive::ZZZ(fileEntry);
      tmp.insert(std::make_pair(basename, archive));
    }
    return tmp;
  }
  [[maybe_unused]] static void testPair(const std::pair<std::string_view, OpenVIII::Archive::ZZZ> &pair)
  {
    const auto &[name, zzz] = pair;
    std::cout << '{' << name << ", " << zzz.path_ << "}\n";
    zzz.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
};

}// namespace OpenVIII::Archive
#endif// VIIICPPTEST_ZZZ_HPP