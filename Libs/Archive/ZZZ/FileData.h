//
// Created by pcvii on 4/23/2020.
//

#ifndef VIIICPPTEST_FILEDATA_H
#define VIIICPPTEST_FILEDATA_H
#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../FI.hpp"
#include "Tools/Tools.h"
namespace OpenVIII::Archive {
struct [[maybe_unused]] FileData
{
private:
  //unsigned int filenameLength_{};
  unsigned long offset_{};
  std::string filename_{};
  unsigned int size_{};

public:
  struct [[maybe_unused]] Comparator{ bool operator()(const FileData &left, const FileData &right)
      const { if (std::size(left.filename_) < std::size(right.filename_)){ return true;
}
if (std::size(left.filename_) > std::size(right.filename_)) { return false; }
return left.filename_ < right.filename_;
}// namespace OpenVIII::Archive
}
;
constexpr FileData() = default;
[[maybe_unused]] FileData(const std::string_view &filename, const unsigned long offset, unsigned int size)
{
  filename_ = filename;
  Tools::replaceAll(filename_, '\\', std::filesystem::path::preferred_separator);
  //filenameLength_ = static_cast<unsigned int>(std::size(filename_));
  offset_ = offset;
  size_ = size;
  //std::cout << '{'<< offset<<", " << size<< ", " <<filename << "}\n";
}
[[maybe_unused]] static FileData GetEntry(std::ifstream &fp)
{
  unsigned int filenameLength{};
  decltype(offset_) offset{};
  decltype(size_) size{};
  if (!fp.is_open()) { return FileData(); }
  Tools::ReadVal(fp,filenameLength);
  auto filename = std::basic_string<char>();
  filename.resize(filenameLength);
  fp.read(filename.data(), filenameLength);
  Tools::ReadVal(fp,offset);
  Tools::ReadVal(fp,size);
  return FileData(filename, offset, size);
}
// size of all the data in this struct
[[maybe_unused]] [[nodiscard]] constexpr auto TotalSize()
{
  return sizeof(unsigned int) + std::size(filename_) + sizeof(offset_) + sizeof(size_);
}
// gets path as a std::filesystem::path
[[maybe_unused]] [[nodiscard]] auto GetPath() const { return std::filesystem::path(filename_); }
// convert to FI. (loss of precision size_t to unsigned int)
[[maybe_unused]] [[nodiscard]] constexpr auto GetFI() const
{
  return FI(static_cast<unsigned int>(size_), static_cast<unsigned int>(offset_));
}
// get size of file
[[maybe_unused]] [[nodiscard]] constexpr auto Size() const { return size_; }
// get offset of file
[[maybe_unused]] [[nodiscard]] constexpr auto Offset() const { return offset_; }
// gets path as a std::string_view
[[maybe_unused]] [[nodiscard]] auto GetPathString() const { return std::string_view(filename_); }
[[maybe_unused]] auto GetTuple() const { return std::make_tuple(GetPath(), Offset(), Size()); }
}
;
}
#endif// VIIICPPTEST_FILEDATA_H
