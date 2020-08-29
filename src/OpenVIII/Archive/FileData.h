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

#ifndef VIIIARCHIVE_FILEDATA_H
#define VIIIARCHIVE_FILEDATA_H
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "FI.h"
#include "OpenVIII/Tools/Tools.h"
namespace OpenVIII::Archive {
struct [[maybe_unused]] FileData
{
private:
  std::basic_string<char> filename_{};// defined as char because some compilers default to wide char.
  std::uint64_t offset_{};
  std::uint32_t size_{};

public:
  constexpr FileData() = default;
  FileData(const FileData &) = default;
  FileData(FileData &&) = default;
  FileData &operator=(const FileData &) = default;
  FileData &operator=(FileData &&) = default;
  ~FileData() = default;
  [[maybe_unused]] FileData(const std::string_view &filename, const unsigned long offset, unsigned int size)
  {
    filename_ = filename;
    Tools::replaceSlashes(filename_);
    offset_ = offset;
    size_ = size;
  }
  bool empty() const noexcept { return size_ == 0 || filename_.empty(); }
  FileData(std::ifstream &fp)
  {
    if (!fp.is_open()) {
      return;
    }
    {
      unsigned int filenameLength{};
      Tools::ReadVal(fp, filenameLength);
      // made as a char basic string to make sure
      // we aren't reading wide chars or something odd.
      filename_.resize(filenameLength);
      fp.read(filename_.data(), filenameLength);
    }
    Tools::replaceSlashes(filename_);// make sure slashes match compiler
    Tools::ReadVal(fp, offset_);
    Tools::ReadVal(fp, size_);
  }


  // size of this file entry in the zzz file.
  [[maybe_unused]] [[nodiscard]] constexpr auto TotalSize()
  {
    return sizeof(unsigned int) + std::size(filename_) + sizeof(offset_) + sizeof(size_);
  }
  // gets path as a std::filesystem::path
  [[maybe_unused]] [[nodiscard]] auto GetPath() const { return std::filesystem::path(filename_); }
  // convert to FI. (loss of precision size_t to unsigned int)
  //[[maybe_unused]] [[nodiscard]] constexpr auto GetFI() const
  //{
  //  return FI(static_cast<unsigned int>(size_), static_cast<unsigned int>(offset_));
  //}
  // get size of file
  [[maybe_unused]] [[nodiscard]] constexpr auto Size() const noexcept { return size_; }
  [[maybe_unused]] [[nodiscard]] constexpr auto CompressionType() const noexcept { return CompressionTypeT::None; }
  // alias for Size that should mirror FI
  [[maybe_unused]] [[nodiscard]] constexpr auto UncompressedSize() const noexcept { return size_; }
  // get offset of file
  [[maybe_unused]] [[nodiscard]] constexpr auto Offset() const noexcept { return offset_; }
  // gets path as a std::string_view
  [[maybe_unused]] [[nodiscard]] auto GetPathString() const { return std::string_view(filename_); }
  [[maybe_unused]] [[nodiscard]] auto GetTuple() const { return std::make_tuple(GetPathString(), Offset(), Size()); }
};
}// namespace OpenVIII::Archive
#endif// VIIIARCHIVE_FILEDATA_H
