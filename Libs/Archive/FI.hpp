#ifndef OPENVIII_FI_H
#define OPENVIII_FI_H

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <cstring>
#include <iterator>
#include <array>
#include <algorithm>
#include "..\Tools\Tools.h"
namespace OpenVIII::Archive {
enum class CompressionTypeT : unsigned int {
  None = 0,
  LZSS = 1,
  LZ4 [[maybe_unused]] = 2,
  // below are custom values i was setting for files that are
  // lzss compressed but aren't auto decompressed by the library.
  // LZSS_UnknownSize = 3, //some entries are compressed with lzss but it is decompressed later
  // LZSS_LZSS = 4, //some entries are compressed twice.
};

struct FI
{
  // changed to int because libraries require casting to int anyway.
private:
  unsigned int uncompressedSize_{ 0 };
  unsigned int offset_{ 0 };
  CompressionTypeT compressionType_{ CompressionTypeT::None };

  [[nodiscard]] constexpr static size_t GetStart(const unsigned int &id, const size_t &offset = 0U)
  {
    return (id * Size) + offset;
  }

  [[nodiscard]] static FI GetEntry(std::ifstream &fp, const size_t &start = 0U)
  {
    unsigned int offset = 0;
    unsigned int uncompressedSize = 0;
    unsigned int compressionType = 0;
    fp.seekg(static_cast<long>(start), std::ios::beg);

    //    auto readVal = [&fp](auto &v) {
    //      auto tmp = std::array<char, sizeof(v)>{};
    //      fp.read(tmp.data(), sizeof(v));
    //      memcpy(&v, tmp.data(), sizeof(v));// memcpy will be optimized away. It is safer than casting.
    //      // TODO change to bitcast in cpp 20 or read another way?
    //    };
    Tools::ReadVal(fp, uncompressedSize);
    Tools::ReadVal(fp, offset);
    Tools::ReadVal(fp, compressionType);
    return FI(uncompressedSize, offset, static_cast<CompressionTypeT>(compressionType));
  }

public:
  constexpr static const size_t Size = 12U;

  constexpr static const auto Ext = std::string_view(".FI");

  [[nodiscard]] constexpr auto UncompressedSize() const noexcept { return uncompressedSize_; }

  [[nodiscard]] constexpr auto Offset() const noexcept { return offset_; }

  [[nodiscard]] constexpr auto CompressionType() const noexcept { return compressionType_; }

  constexpr FI() = default;

  constexpr FI(const unsigned int &uncompressedSize,
    const unsigned int &offset,
    const CompressionTypeT &compressionType = CompressionTypeT::None)
  {
    uncompressedSize_ = uncompressedSize;
    offset_ = offset;
    compressionType_ = compressionType;
  }

  [[nodiscard]] constexpr static auto GetCount(const size_t fileSize) noexcept { return fileSize / Size; }
  // GetCount which is fileSize/Size if file doesn't exist return 0;
  [[nodiscard]] auto static GetCount(const std::filesystem::path &path)
  {
    if (std::filesystem::exists(path)) { return GetCount(std::filesystem::file_size(path)); }
    return static_cast<uintmax_t>(0U);
  }

  [[nodiscard]] static FI
    GetEntry(const std::vector<char> &data, const unsigned int &id, const size_t &offsetIn, const size_t &size)
  {
    const auto count = GetCount(data.size());
    if (id < count) {

      const auto startOffset = GetStart(id, offsetIn);
      if (size != 0U && startOffset > offsetIn + size - Size) return {};
      const auto *start = data.data() + static_cast<long>(startOffset);
      unsigned int offset = 0;
      unsigned int uncompressedSize = 0;
      unsigned int compressionType = 0;
      std::memcpy(&uncompressedSize, start, sizeof(uncompressedSize));
      start += sizeof(uncompressedSize);
      std::memcpy(&offset, start, sizeof(offset));
      start += sizeof(offset);
      std::memcpy(&compressionType, start, sizeof(compressionType));
      return FI(uncompressedSize, offset, static_cast<CompressionTypeT>(compressionType));
    }

    return FI();
  }

  [[nodiscard]] static FI
    GetEntry(const std::filesystem::path &path, const unsigned int &id, const size_t &offset, const size_t &size)
  {
    const auto count = GetCount(path);
    if (id < count) {
      auto fp = std::ifstream(path, std::ios::in | std::ios::binary);
      if (fp.is_open()) {
        const auto start = GetStart(id, offset);
        if (size != 0U && start > offset + size - Size) return {};
        const FI fi = GetEntry(fp, start);
        fp.close();
        return fi;
      }
      fp.close();
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

#endif// !OPENVIII_FI_H