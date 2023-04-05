//
// Created by pcvii on 4/5/2023.
//

#ifndef OPENVIII_CPP_WIP_DATQUAD_HPP
#define OPENVIII_CPP_WIP_DATQUAD_HPP
#include "DatTriangle.hpp"
#include "open_viii/battle/stage/Shapes.hpp"
namespace open_viii::battle::dat {
struct DatQuad
{
private:
  std::array<FaceWrapper, 4U>
    m_raw_face_indeces{};// vertex_indexes[0] &= 0xFFF, other bits are
                         // unknown
  open_viii::graphics::Point<std::uint8_t> m_uv1;
  TextureIDWrapper m_raw_texture_id{};///< m_raw_texture_id >> 6 & 0b111U
  open_viii::graphics::Point<std::uint8_t> m_uv2;
  std::uint16_t                            m_u{};// textureID_related2
  open_viii::graphics::Point<std::uint8_t> m_uv3;
  open_viii::graphics::Point<std::uint8_t> m_uv4;

public:
  [[nodiscard]] std::uint16_t
    u() const
  {
    return m_u;
  }

  std::uint16_t &
    u()
  {
    return m_u;
  }
  [[nodiscard]] TextureIDWrapper
    texture_id() const
  {
    return m_raw_texture_id;
  }
  TextureIDWrapper &
    texture_id()
  {
    return m_raw_texture_id;
  }
  [[nodiscard]] auto
    uvs() const
  {
    return std::views::iota(0, 4)
         | std::views::transform([this](int index) -> auto {
             switch (index) {
             default:
             case 0:
               return m_uv1;
             case 1:
               return m_uv2;
             case 2:
               return m_uv3;
             case 3:
               return m_uv4;
             }
           });
  }

  [[nodiscard]] auto
    uvs()
  {
    return std::views::iota(0, 4)
         | std::views::transform([this](int index) -> auto & {
             switch (index) {
             default:
             case 0:
               return m_uv1;
             case 1:
               return m_uv2;
             case 2:
               return m_uv3;
             case 3:
               return m_uv4;
             }
           });
  }
  [[nodiscard]] const auto &
    indeces() const
  {
    return m_raw_face_indeces;
  }
  [[nodiscard]] auto &
    indeces()
  {
    return m_raw_face_indeces;
  }
  static constexpr std::size_t COUNT = 4U;
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
    return m_raw_face_indeces[I];
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
    return m_raw_face_indeces[I];
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
    else if constexpr (I == 3U) {
      return m_uv4;
    }
  }

  /**
   * @brief Get the UV value for a specific vertex.
   * @tparam I The index of the vertex.
   * @return The UV value for the specified vertex.
   */
  template<std::size_t I>
    requires(I < COUNT)
  [[nodiscard]] constexpr auto &
    uv() noexcept
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
    else if constexpr (I == 3U) {
      return m_uv4;
    }
  }
  auto
    get_triangles() const
  {
    auto triangles            = common_quad_to_triangle<DatTriangle>(*this);
    triangles[0].texture_id() = texture_id();
    triangles[0].u()          = u();
    triangles[1].texture_id() = texture_id();
    triangles[1].u()          = u();
    return triangles;
  }
  static constexpr std::size_t EXPECTED_SIZE = 20U;
};
static_assert(sizeof(DatQuad) == DatQuad::EXPECTED_SIZE);
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_DATQUAD_HPP
