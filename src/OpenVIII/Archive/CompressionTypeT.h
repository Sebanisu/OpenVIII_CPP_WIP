//
// Created by pcvii on 6/22/2020.
//

#ifndef VIIIARCHIVE_COMPRESSIONTYPET_H
#define VIIIARCHIVE_COMPRESSIONTYPET_H
#include <cstdint>
namespace OpenVIII::Archive {
enum class CompressionTypeT : std::uint32_t {
  None = 0,
  LZSS = 1,
  LZ4 = 2,
  // below are custom values i was setting for files that are
  // lzss compressed but aren't auto decompressed by the library.
  // LZSS_UnknownSize = 3, //some entries are compressed with lzss but it is decompressed later
  // LZSS_LZSS = 4, //some entries are compressed twice.
};
}
#endif// VIIIARCHIVE_COMPRESSIONTYPET_H
