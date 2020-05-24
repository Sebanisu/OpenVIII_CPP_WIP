#ifndef VIIIARCHIVE_FI_H
#define VIIIARCHIVE_FI_H

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <cstring>
#include <iterator>
#include <array>
#include <algorithm>
#include "External/VIIITools/Tools.h"
namespace OpenVIII::Archive {
enum class CompressionTypeT : unsigned int {
  None = 0,
  LZSS = 1,
  LZ4 = 2,
  // below are custom values i was setting for files that are
  // lzss compressed but aren't auto decompressed by the library.
  // LZSS_UnknownSize = 3, //some entries are compressed with lzss but it is decompressed later
  // LZSS_LZSS = 4, //some entries are compressed twice.
};

struct FI
{
  // changed to int because libraries require casting to int anyway.
private:
  unsigned int uncompressedSize_{};
  unsigned int offset_{};
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

  constexpr FI(const unsigned int &uncompressedSize,
    const unsigned int &offset,
    const CompressionTypeT &compressionType = CompressionTypeT::None) noexcept
  {
    uncompressedSize_ = uncompressedSize;
    offset_ = offset;
    compressionType_ = compressionType;
  }

  FI(std::ifstream &&fp, const long &startOffset = 0, bool close = false)
  {
    // unsure if this is correct but passing from ifstream is an rvalue
    // so umm it won't let me use a normal reference unless it's const.
    if (!fp.is_open()) return;
    if (startOffset < 0) return;// shouldn't be less than 0;
    fp.seekg(startOffset);

    Tools::ReadVal(fp, uncompressedSize_);
    if (uncompressedSize_ > 0) {// if size is 0 than no point in reading more.
      Tools::ReadVal(fp, offset_);
      Tools::ReadVal(fp, compressionType_);
    }
    if (close) fp.close();
  }

  FI(const std::filesystem::path &path, const unsigned int &id, const size_t &offset)
    : FI(std::ifstream(path, std::ios::in | std::ios::binary), static_cast<long>(GetStartOffset(id, offset)), true)
  {}

  FI(const std::vector<char> &buffer, const size_t &startOffset = 0U)
  {
    auto bufferPointer = buffer.data();
    if (bufferPointer + startOffset + Size > bufferPointer + buffer.size()) return;
    bufferPointer += startOffset;
    std::memcpy(&uncompressedSize_, bufferPointer, sizeof(uncompressedSize_));
    if (uncompressedSize_ > 0) {// if size is 0 than no point in reading more.
      bufferPointer += sizeof(uncompressedSize_);
      std::memcpy(&offset_, bufferPointer, sizeof(offset_));
      bufferPointer += sizeof(offset_);
      std::memcpy(&compressionType_, bufferPointer, sizeof(compressionType_));
    }
  }

  FI(const std::vector<char> &buffer, const unsigned int &id, const size_t &offset)
    : FI(buffer, GetStartOffset(id, offset))
  {}

  [[nodiscard]] constexpr static auto GetCount(const size_t fileSize) noexcept { return fileSize / Size; }
  // GetCount which is fileSize/Size if file doesn't exist return 0;
  [[maybe_unused]] [[nodiscard]] auto static GetCount(const std::filesystem::path &path)
  {
    if (std::filesystem::exists(path)) { return GetCount(std::filesystem::file_size(path)); }
    return static_cast<uintmax_t>(0U);
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