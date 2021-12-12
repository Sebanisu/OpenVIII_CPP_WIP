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
#ifndef VIIIARCHIVE_COMPRESSIONTYPET_HPP
#define VIIIARCHIVE_COMPRESSIONTYPET_HPP
namespace open_viii {
enum class CompressionTypeT : std::uint32_t
{
  none = 0,
  lzss = 1,
  lz4  = 2,
  // below are custom values i was setting for files that are
  // lzss compressed but aren't auto decompressed by the library.
  // LZSS_UnknownSize = 3, //some entries are compressed with lzss but it is
  // decompressed later LZSS_LZSS = 4, //some entries are compressed twice.
};
}// namespace open_viii
#endif// VIIIARCHIVE_COMPRESSIONTYPET_HPP
