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
#ifndef VIIIARCHIVE_PUPU_HPP
#define VIIIARCHIVE_PUPU_HPP
#include "IntegralStorage.hpp"
#include "Tile1.hpp"
#include "Tile2.hpp"
#include "Tile3.hpp"
namespace open_viii::graphics::background {
/*
 * A disambiguation ID based on Pupu an application for descrambling and
 * rescrambling the tiles. The values in here should be enough to keep from
 * having collisions. I'm storing the seperate values for easy access on post
 */
struct Pupu
{
private:
  std::uint16_t        m_z{};
  BPPT                 m_depth{};
  std::uint8_t         m_layer_id{};
  BlendModeT           m_blend_mode{};
  std::uint8_t         m_animation_id{};
  std::uint8_t         m_animation_state{};
  constexpr static int HEX_BASE = 16;
  static constexpr int BIT4     = 4;
  static constexpr int BIT8     = 8;
  static constexpr int BIT16    = 16;

public:
  Pupu() = default;
  template<typename tileT>
    requires(std::is_convertible_v<tileT, Tile1>
             || std::is_convertible_v<tileT, Tile2>
             || std::is_convertible_v<tileT, Tile3>)
  constexpr explicit Pupu(tileT tile)
    : m_z(tile.z()), m_depth(tile.depth()), m_layer_id(tile.layer_id()),
      m_blend_mode(tile.blend_mode()), m_animation_id(tile.animation_id()),
      m_animation_state(tile.animation_state())
  {}
  auto
    operator<=>(const Pupu &) const
    = default;
  auto
    operator==(const Pupu &right) const
  {
    return (*this <=> right) == std::strong_ordering::equal;
  }
  friend auto
    operator<=>(const Pupu &left, const BPPT &right)
  {
    return left.m_depth <=> right;
  }
  friend auto
    operator<=>(const BPPT &right, const Pupu &left)
  {
    return left.m_depth <=> right;
  }
  friend auto
    operator==(const Pupu &left, const BPPT &right)
  {
    return (left.m_depth <=> right) == std::strong_ordering::equal;
  }
  friend auto
    operator==(const BPPT &right, const Pupu &left)
  {
    return (left.m_depth <=> right) == std::strong_ordering::equal;
  }
  template<typename tileT>
    requires(
      std::is_convertible_v<tileT, Tile1> || std::is_convertible_v<tileT, Tile2>
      || std::is_convertible_v<tileT, Tile3>)
  friend auto
    operator==(const Pupu &left, const tileT &right)
  {
    return left.m_z == right.z() && left.m_depth == right.depth()
        && left.m_layer_id == right.layer_id()
        && left.m_blend_mode == right.blend_mode()
        && left.m_animation_id == right.animation_id()
        && left.m_animation_state == right.animation_state();
  }
  template<typename tileT>
    requires(
      std::is_convertible_v<tileT, Tile1> || std::is_convertible_v<tileT, Tile2>
      || std::is_convertible_v<tileT, Tile3>)
  friend auto
    operator==(const tileT &left, const Pupu &right)
  {
    return right.m_z == left.z() && right.m_depth == left.depth()
        && right.m_layer_id == left.layer_id()
        && right.m_blend_mode == left.blend_mode()
        && right.m_animation_id == left.animation_id()
        && right.m_animation_state == left.animation_state();
  }
  template<typename tileT>
    requires(
      std::is_convertible_v<tileT, Tile1> || std::is_convertible_v<tileT, Tile2>
      || std::is_convertible_v<tileT, Tile3>)
  bool
    AllButZ(const tileT &left) const noexcept
  {
    return m_depth == left.depth() && m_layer_id == left.layer_id()
        && m_blend_mode == left.blend_mode()
        && m_animation_id == left.animation_id()
        && m_animation_state == left.animation_state();
  }
  /**
   * Computes the ID from the values.
   * @return all the values as one uint64
   */
  [[nodiscard]] std::uint64_t
    generate() const noexcept
  {
    const integral_storage::Writer<std::uint64_t> pupu_writer{};
    pupu_writer.add_depth(m_depth);
    pupu_writer.add_uint<BIT4>(m_layer_id);
    pupu_writer.add_uint<BIT4>(static_cast<std::uint8_t>(m_blend_mode));
    pupu_writer.add_uint<BIT8>(m_animation_id);
    pupu_writer.add_uint<BIT8>(m_animation_state);
    pupu_writer.add_uint<BIT16>(m_z);
    return pupu_writer.get_id();
  }
  [[nodiscard]] explicit
    operator std::uint64_t() const
  {
    return generate();
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Pupu &input)
  {
    return os << std::uppercase << std::hex << std::setfill('0')
              << std::setw(16U) << input.generate() << std::nouppercase
              << std::dec << std::setfill(' ') << std::setw(0U);
  }
  constexpr explicit Pupu(const std::uint64_t &pupu_id) noexcept
  {
    if (pupu_id == 0) {
      return;
    }
    integral_storage::Reader<std::uint64_t> pupu_reader{ pupu_id };
    m_depth        = pupu_reader.extract_depth();
    m_layer_id     = pupu_reader.extract_uint<BIT4>();
    m_blend_mode   = static_cast<BlendModeT>(pupu_reader.extract_uint<BIT4>());
    m_animation_id = pupu_reader.extract_uint<BIT8>();
    m_animation_state = pupu_reader.extract_uint<BIT8>();
    m_z               = pupu_reader.extract_uint<BIT16>();
  }
  explicit Pupu(std::string_view hex)
    : Pupu(std::strtoull(std::ranges::data(hex), nullptr, HEX_BASE))
  {}
  [[nodiscard]] const std::uint16_t &
    z() const noexcept
  {
    return m_z;
  }
  [[nodiscard]] const BPPT &
    depth() const noexcept
  {
    return m_depth;
  }
  [[nodiscard]] const std::uint8_t &
    layer_id() const noexcept
  {
    return m_layer_id;
  }
  [[nodiscard]] const BlendModeT &
    blend_mode() const noexcept
  {
    return m_blend_mode;
  }
  [[nodiscard]] const std::uint8_t &
    animation_id() const noexcept
  {
    return m_animation_id;
  }
  [[nodiscard]] const std::uint8_t &
    animation_state() const noexcept
  {
    return m_animation_state;
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_PUPU_HPP
