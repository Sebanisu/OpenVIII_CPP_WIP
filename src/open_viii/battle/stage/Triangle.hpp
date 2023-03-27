//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_TRIANGLE_HPP
#define VIIIARCHIVE_TRIANGLE_HPP
#include "GpuFlags.hpp"
#include "open_viii/graphics/Bit4Values.hpp"
#include "open_viii/graphics/Color.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Rectangle.hpp"
#include "RawClut.hpp"
namespace open_viii::battle::stage {
/**
 * @brief A class representing a Triangle structure in FF8 battle stage.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Triangle
 */
struct Triangle
{
private:
  using point = graphics::Point<std::uint8_t>;
  /// Face index A
  std::uint16_t                 m_face_indice_a{};
  /// Face index B
  std::uint16_t                 m_face_indice_b{};
  /// Face index C
  std::uint16_t                 m_face_indice_c{};
  /// UV coordinate 1
  point                         m_uv1{};
  /// UV coordinate 2
  point                         m_uv2{};
  /// Raw CLUT value
  RawClut                       m_raw_clut{};
  /// UV coordinate 3
  point                         m_uv3{};
  /// Raw texture page value
  graphics::Bit4Values          m_raw_texture_page{};
  /// Raw hide value
  std::uint8_t                  m_raw_hide{};
  /// Color value
  graphics::Color24RGB          m_color{};
  /// Raw GPU flags
  GpuFlags                      m_raw_gpu{};
  /// Mask value for 4 bits
  static constexpr std::uint8_t MASK_4_BIT  = 0xFU;
  /// Shift value for 2 bits
  static constexpr int          SHIFT_2_BIT = 2;

public:
  static constexpr std::size_t COUNT = 3U;

  /**
   * @brief Get the CLUT value.
   * @return The CLUT value.
   */
  [[nodiscard]] RawClut
    clut() const noexcept
  {
    return m_raw_clut;
  }
  /**
   * @brief Get the CLUT value.
   * @return The CLUT value.
   */
  [[nodiscard]] RawClut &
    clut() noexcept
  {
    return m_raw_clut;
  }

  /**
   * @brief Get the texture page value.
   * @return The texture page value.
   */
  [[nodiscard]] constexpr std::uint8_t
    texture_page() const noexcept
  {
    return m_raw_texture_page.second;
  }

  /**
   * @brief Get the texture page value.
   * @return The texture page value.
   */
  [[nodiscard]] constexpr graphics::Bit4ValuesProxy<true>
    texture_page() noexcept
  {
    return m_raw_texture_page;
  }

  /**
   * @brief Get the color value.
   * @return The color value.
   */
  [[nodiscard]] constexpr auto
    color() const noexcept
  {
    return m_color;
  }
  /**
   * @brief Get the color value.
   * @return The color value.
   */
  [[nodiscard]] constexpr auto &
    color() noexcept
  {
    return m_color;
  }

  /**
   * @brief Check if the Quad is hidden.
   * @return True if hidden, false otherwise.
   */
  [[nodiscard]] constexpr bool
    hide() const noexcept
  {
    return m_raw_hide != 0U;
  }

  /**
   * @brief Check if the Quad is hidden.
   * @return True if hidden, false otherwise.
   */
  [[nodiscard]] constexpr unsigned char &
    raw_hide() noexcept
  {
    return m_raw_hide;
  }

  /**
   * @brief Check if the Quad is hidden.
   * @return True if hidden, false otherwise.
   */
  [[nodiscard]] constexpr unsigned char
    raw_hide() const noexcept
  {
    return m_raw_hide;
  }

  /**
   * @brief Get the UV value for a specific vertex.
   * @tparam I The index of the vertex.
   * @return The UV value for the specified vertex.
   */
  template<std::size_t I>
    requires(I < COUNT)
  [[nodiscard]] constexpr auto
    uv() const noexcept
  {
    if constexpr (I == 0U) {
      return m_uv1;
    }
    else if constexpr (I == 1U) {
      return m_uv2;
    }
    else if constexpr (I == 2U) {
      return m_uv3;
    }
  }

  /**
   * @brief Get the face index value for a specific vertex.
   * @tparam I The index of the vertex.
   * @return The face index value for the specified vertex.
   */
  template<std::size_t I>
    requires(I < COUNT)
  [[nodiscard]] constexpr auto
    face_indice() const noexcept
  {
    if constexpr (I == 0U) {
      return m_face_indice_a;
    }
    else if constexpr (I == 1U) {
      return m_face_indice_b;
    }
    else if constexpr (I == 2U) {
      return m_face_indice_c;
    }
  }

  /**
   * @brief Get the face index value for a specific vertex.
   * @tparam I The index of the vertex.
   * @return The face index value for the specified vertex.
   */
  template<std::size_t I>
    requires(I < COUNT)
  [[nodiscard]] constexpr auto &
    face_indice() noexcept
  {
    if constexpr (I == 0U) {
      return m_face_indice_a;
    }
    else if constexpr (I == 1U) {
      return m_face_indice_b;
    }
    else if constexpr (I == 2U) {
      return m_face_indice_c;
    }
  }

  /**
   * @brief Generates a range view of face indices for the Quad.
   *
   * @return A range view representing the face indices.
   */
  auto
    face_indices() const noexcept
  {
    return std::views::iota(std::uint8_t{}, std::uint8_t{ 3 })
         | std::views::transform([this](std::uint8_t m_index) -> std::uint16_t {
             {
               switch (m_index) {
               case 0:
                 return face_indice<0U>();
               case 1:
                 return face_indice<1U>();
               case 2:
                 return face_indice<2U>();
               default:
                 return {};// Invalid index, return a
                           // default-constructed value,
                           // handle as desired
               }
             }
           });
  }

  /**
   * @brief Generates a range view of UV coordinates for the Quad.
   *
   * @return A range view representing the UV coordinates.
   */
  auto
    uvs() const noexcept
  {
    return std::views::iota(std::uint8_t{}, std::uint8_t{ 3 })
         | std::views::transform(
             [this](std::uint8_t m_index) -> graphics::Point<std::uint8_t> {
               {
                 switch (m_index) {
                 case 0:
                   return uv<0U>();
                 case 1:
                   return uv<1U>();
                 case 2:
                   return uv<2U>();
                 default:
                   return {};// Invalid index, return a
                             // default-constructed value,
                             // handle as desired
                 }
               }
             });
  }

  /// Raw GPU flags
  GpuFlags &
    gpu() noexcept
  {
    return m_raw_gpu;
  }

  /// Raw GPU flags
  GpuFlags
    gpu() const noexcept
  {
    return m_raw_gpu;
  }

  static constexpr std::size_t EXPECTED_SIZE = 20U;
};
static_assert(sizeof(Triangle) == Triangle::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_TRIANGLE_HPP