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
#ifndef VIIIARCHIVE_FI_H
#define VIIIARCHIVE_FI_H

#include <fstream>
#include <iostream>
#include <vector>
#include <span>
#include <filesystem>
#include <cstring>
#include <iterator>
#include <array>
#include <algorithm>
#include "OpenVIII/Tools/Tools.h"
#include "OpenVIII/CompressionTypeT.h"
namespace OpenVIII::Archive {
/**
 * FI is the file index for the FL and FS files.
 */
struct FI
{
  // changed to int because libraries require casting to int anyway.
private:
  std::uint32_t uncompressedSize_{};
  std::uint32_t offset_{};
  CompressionTypeT compressionType_{ CompressionTypeT::None };

  [[nodiscard]] constexpr static size_t GetStartOffset(const unsigned int &id, const size_t &offset = 0U)
  {
    return (id * Size) + offset;
  }

public:
  constexpr static const size_t Size = 12U;

  constexpr static const auto Ext = std::string_view(".FI");

  [[nodiscard]] constexpr auto UncompressedSize() const noexcept { return uncompressedSize_; }

  [[nodiscard]] constexpr auto Offset() const noexcept { return offset_; }

  [[nodiscard]] constexpr auto CompressionType() const noexcept { return compressionType_; }

  constexpr FI() noexcept = default;

  template<FI_Like fiT> constexpr explicit FI(const fiT &fi)
  {
    uncompressedSize_ = static_cast<decltype(uncompressedSize_)>(fi.UncompressedSize());
    offset_ = static_cast<decltype(offset_)>(fi.Offset());
    compressionType_ = static_cast<decltype(compressionType_)>(fi.CompressionType());
  }
  constexpr FI(const unsigned int &uncompressedSize,
    const unsigned int &offset,
    const CompressionTypeT &compressionType = CompressionTypeT::None) noexcept
  {
    uncompressedSize_ = uncompressedSize;
    offset_ = offset;
    compressionType_ = compressionType;
  }

  explicit FI(std::ifstream &&fp, const long &startOffset = 0, bool close = false)
  {
    // unsure if this is correct but passing from ifstream is an rvalue
    // so umm it won't let me use a normal reference unless it's const.
    if (!fp.is_open()) {
      return;
    }
    if (startOffset < 0) {
      return;// shouldn't be less than 0;
    }
    fp.seekg(startOffset);

    Tools::ReadVal(fp, uncompressedSize_);
    if (uncompressedSize_ > 0) {// if size is 0 than no point in reading more.
      Tools::ReadVal(fp, offset_);
      Tools::ReadVal(fp, compressionType_);
    }
    if (close) {
      fp.close();
    }
  }

  FI(const std::filesystem::path &path, const unsigned int &id, const size_t &offset)
    : FI(std::ifstream(path, std::ios::in | std::ios::binary), static_cast<long>(GetStartOffset(id, offset)), true)
  {}

  explicit FI(std::span<const char> buffer, const size_t &startOffset = 0U)
  {
    if (startOffset + Size > std::ranges::size(buffer)) {
      return;
    }
    buffer = buffer.subspan(startOffset, Size);

    std::memcpy(&uncompressedSize_, std::ranges::data(buffer), sizeof(uncompressedSize_));
    if (uncompressedSize_ > 0) {// if size is 0 than no point in reading more.
      buffer = buffer.subspan(sizeof(uncompressedSize_));
      std::memcpy(&offset_, std::ranges::data(buffer), sizeof(offset_));
      buffer = buffer.subspan(sizeof(offset_));
      std::memcpy(&compressionType_, std::ranges::data(buffer), sizeof(compressionType_));
    }
  }

  FI(const std::vector<char> &buffer, const unsigned int &id, const size_t &offset)
    : FI(buffer, GetStartOffset(id, offset))
  {}

  [[nodiscard]] constexpr static size_t GetCount(const size_t fileSize) noexcept { return fileSize / Size; }
  // GetCount which is fileSize/Size if file doesn't exist return 0;
  [[maybe_unused]] [[nodiscard]] size_t static GetCount(const std::filesystem::path &path)
  {
    if (std::filesystem::exists(path)) {
      return GetCount(std::filesystem::file_size(path));
    }
    return {};
  }

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FI &data)
  {
    os << '{' << data.uncompressedSize_ << ", " << data.offset_ << ", "
       << static_cast<unsigned int>(data.compressionType_) << '}';
    return os;
  }
};
}// namespace OpenVIII::Archive

#endif// !VIIIARCHIVE_FI_H