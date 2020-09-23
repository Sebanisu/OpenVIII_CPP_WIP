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

#ifndef VIIIARCHIVE_TEXHEADER2_VERSION2_HPP
#define VIIIARCHIVE_TEXHEADER2_VERSION2_HPP
#include <cstdint>

namespace open_viii::graphics {

/**
 * This appends to texHeader2 if the file is version 2. used by ff8 only.
 */
struct TexHeader2Version2
{
private:
  /**
   * Unknown 11 (TEX version 2 only)
   */
  std::uint32_t m_unknown_11{};

public:
  /**
   * Unknown 11 (TEX version 2 only)
   */
  [[maybe_unused]] [[nodiscard]] std::uint32_t unknown_11() const noexcept
  {
    return m_unknown_11;
  }
};


}// namespace open_viii::graphics

#endif// VIIIARCHIVE_TEXHEADER2_VERSION2_HPP
