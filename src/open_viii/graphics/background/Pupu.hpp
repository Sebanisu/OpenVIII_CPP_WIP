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
#include <cstdint>
#include <compare>
#include <bit>
#include "Tile1.hpp"
#include "Tile2.hpp"
#include "Tile3.hpp"
namespace open_viii::graphics::background {
/*
 * A disambiguation ID based on Pupu an application for descrambling and rescrambling the tiles. The values in here
 * should be enough to keep from having collisions. I'm storing the seperate values for easy access on post
 */
struct Pupu
{
private:
  std::uint16_t m_z{};
  BPPT m_depth{};
  std::uint8_t m_layer_id{};
  BlendModeT m_blend_mode{};
  std::uint8_t m_animation_id{};
  std::uint8_t m_animation_state{};
  constexpr static int HEX_BASE = 16;

public:
  Pupu() = default;
  template<typename tileT>
  requires(
    std::is_convertible_v<tileT,
      Tile1> || std::is_convertible_v<tileT, Tile2> || std::is_convertible_v<tileT, Tile3>) constexpr explicit Pupu(tileT
      tile)
    : m_z(tile.z()), m_depth(tile.depth()), m_layer_id(tile.layer_id()), m_blend_mode(tile.blend_mode()),
      m_animation_id(tile.animation_id()), m_animation_state(tile.animation_state())
  {}

  auto operator<=>(const Pupu &) const = default;
  auto operator==(const Pupu &right) const { return nullptr == (*this <=> right); }
  friend auto operator<=>(const Pupu &left, const BPPT &right) { return left.m_depth <=> right; }
  friend auto operator<=>(const BPPT &right, const Pupu &left) { return left.m_depth <=> right; }
  friend auto operator==(const Pupu &left, const BPPT &right) { return nullptr == (left.m_depth <=> right); }
  friend auto operator==(const BPPT &right, const Pupu &left) { return nullptr == (left.m_depth <=> right); }

  template<typename tileT>
  requires(std::is_convertible_v<tileT,
             Tile1> || std::is_convertible_v<tileT, Tile2> || std::is_convertible_v<tileT, Tile3>) friend auto
    operator==(const Pupu &left, const tileT &right)
  {
    return left.m_z == right.z() && left.m_depth == right.depth() && left.m_layer_id == right.layer_id()
           && left.m_blend_mode == right.blend_mode() && left.m_animation_id == right.animation_id()
           && left.m_animation_state == right.animation_state();
  }
  template<typename tileT>
  requires(std::is_convertible_v<tileT,
             Tile1> || std::is_convertible_v<tileT, Tile2> || std::is_convertible_v<tileT, Tile3>) friend auto
    operator==(const tileT &left, const Pupu &right)
  {
    return right.m_z == left.z() && right.m_depth == left.depth() && right.m_layer_id == left.layer_id()
           && right.m_blend_mode == left.blend_mode() && right.m_animation_id == left.animation_id()
           && right.m_animation_state == left.animation_state();
  }
  template<typename tileT>
  requires(
    std::is_convertible_v<tileT,
      Tile1> || std::is_convertible_v<tileT, Tile2> || std::is_convertible_v<tileT, Tile3>) bool AllButZ(const tileT
      &left) const noexcept
  {
    return m_depth == left.depth() && m_layer_id == left.layer_id() && m_blend_mode == left.blend_mode()
           && m_animation_id == left.animation_id() && m_animation_state == left.animation_state();
  }
  /**
   * Computes the ID from the values.
   * @return all the values as one uint64
   */
  [[nodiscard]] std::uint64_t generate() const noexcept
  {
    constexpr auto bits_per_long = static_cast<int>(sizeof(std::uint64_t) * 8);
    auto bits = bits_per_long;
    std::uint64_t pupu_id{};
    [[maybe_unused]] const auto add_common = [&bits, &pupu_id](
                                               const std::uint64_t value, const int &shift, const uint64_t &mask) {
      bits -= shift;
      pupu_id |= std::rotl((value & mask), bits);
    };
    [[maybe_unused]] const auto add_crumb = [&add_common](const auto &value) {
      static constexpr auto bits_per_crumb = static_cast<int>(sizeof(std::uint8_t) * 8) / 4;// 0-3
      static constexpr auto crumb_mask = 0x3U;
      add_common(value, bits_per_crumb, crumb_mask);
    };
    [[maybe_unused]] const auto add_nibble = [&add_common](const auto &value) {
      static constexpr auto bits_per_nibble = static_cast<int>(sizeof(std::uint8_t) * 8) / 2;// 0-15
      static constexpr auto nibble_mask = 0xFU;
      add_common(static_cast<std::uint64_t>(value), bits_per_nibble, nibble_mask);
    };
    [[maybe_unused]] const auto add_byte = [&add_common](const auto &value) {
      static constexpr auto bits_per_byte = static_cast<int>(sizeof(std::uint8_t) * 8);// 0-255
      static constexpr auto byte_mask = 0xFFU;
      add_common(value, bits_per_byte, byte_mask);
    };
    [[maybe_unused]] const auto add_short = [&add_common](const auto &value) {
      static constexpr auto bits_per_short = static_cast<int>(sizeof(std::uint16_t) * 8);
      [[maybe_unused]] static constexpr auto short_mask = 0xFFFFU;
      add_common(value, bits_per_short, short_mask);
    };
    add_crumb(m_depth.raw());
    add_nibble(m_layer_id);
    add_nibble(m_blend_mode);
    add_byte(m_animation_id);
    add_byte(m_animation_state);
    add_short(m_z);
    return pupu_id;
  }
  [[nodiscard]] explicit operator std::uint64_t() const { return generate(); }
  friend std::ostream &operator<<(std::ostream &os, const Pupu &input)
  {
    return os << std::uppercase << std::hex << std::setfill('0') << std::setw(16U) << input.generate()
              << std::nouppercase << std::dec << std::setfill(' ') << std::setw(0U);
  }
  constexpr explicit Pupu(std::uint64_t pupu_id) noexcept
  {
    if (pupu_id == 0) {
      return;
    }
    constexpr auto bits_per_long = static_cast<int>(sizeof(std::uint64_t) * 8);

    int bits = bits_per_long;

    [[maybe_unused]] const auto extract_common = [&bits, &pupu_id](const int &shift, const uint64_t &mask) {
      bits -= shift;
      return std::rotr(pupu_id, bits) & mask;
    };
    [[maybe_unused]] const auto extract_crumb = [&extract_common]() -> std::uint8_t {
      static constexpr auto bits_per_crumb = static_cast<int>(sizeof(std::uint8_t) * 8) / 4;// 0-3
      [[maybe_unused]] static constexpr auto crumb_mask = 0x3U;
      return static_cast<std::uint8_t>(extract_common(bits_per_crumb, crumb_mask));
    };
    [[maybe_unused]] const auto extract_nibble = [&extract_common]() -> std::uint8_t {
      static constexpr auto bits_per_nibble = static_cast<int>(sizeof(std::uint8_t) * 8) / 2;// 0-15
      [[maybe_unused]] static constexpr auto nibble_mask = 0xFU;
      return static_cast<std::uint8_t>(extract_common(bits_per_nibble, nibble_mask));
    };

    [[maybe_unused]] const auto extract_byte = [&extract_common]() -> std::uint8_t {
      static constexpr auto bits_per_byte = static_cast<int>(sizeof(std::uint8_t) * 8);// 0-255
      [[maybe_unused]] static constexpr auto byte_mask = 0xFFU;
      return static_cast<std::uint8_t>(extract_common(bits_per_byte, byte_mask));
    };
    [[maybe_unused]] const auto extract_short = [&extract_common]() -> std::uint16_t {
      static constexpr auto bits_per_short = static_cast<int>(sizeof(std::uint16_t) * 8);
      [[maybe_unused]] constexpr static auto short_mask = 0xFFFFU;
      return static_cast<std::uint16_t>(extract_common(bits_per_short, short_mask));
    };
    using namespace literals;
    const auto extract_depth = [&extract_crumb]() {
      switch (extract_crumb()) {
      default:
        return 4_bpp;
      case 1:
        return 8_bpp;
      case 2:
        return 16_bpp;
      case 3:
        return 24_bpp;
      }
    };
    m_depth = extract_depth();
    m_layer_id = extract_nibble();
    m_blend_mode = static_cast<BlendModeT>(extract_nibble());
    m_animation_id = extract_byte();
    m_animation_state = extract_byte();
    m_z = extract_short();
  }

  explicit Pupu(std::string_view hex) : Pupu(std::strtoull(std::ranges::data(hex), nullptr, HEX_BASE)) {}

  [[nodiscard]] const std::uint16_t &z() const noexcept { return m_z; }
  [[nodiscard]] const BPPT &depth() const noexcept { return m_depth; }
  [[nodiscard]] const std::uint8_t &layer_id() const noexcept { return m_layer_id; }
  [[nodiscard]] const BlendModeT &blend_mode() const noexcept { return m_blend_mode; }
  [[nodiscard]] const std::uint8_t &animation_id() const noexcept { return m_animation_id; }
  [[nodiscard]] const std::uint8_t &animation_state() const noexcept { return m_animation_state; }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_PUPU_HPP
