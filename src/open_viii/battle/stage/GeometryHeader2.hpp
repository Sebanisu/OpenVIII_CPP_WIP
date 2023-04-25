//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRYHEADER2_HPP
#define VIIIARCHIVE_GEOMETRYHEADER2_HPP
/**
 * @brief Represents the second header of a geometry object in the battle stage.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 */
namespace open_viii::battle::stage {
struct GeometryHeader2
{
private:
  std::uint16_t m_triangle_count{};///< @brief Number of triangles.
  std::uint16_t m_quad_count{};    ///< @brief Number of quads.
  std::uint32_t m_padding{};       ///< @brief Padding to maintain alignment.

public:
  /**
   * @brief Get the number of triangles in the geometry object.
   * @return The number of triangles.
   */
  [[nodiscard]] const auto &
    triangle_count() const noexcept
  {
    return m_triangle_count;
  }

  /**
   * @brief Get the number of quads in the geometry object.
   * @return The number of quads.
   */
  [[nodiscard]] const auto &
    quad_count() const noexcept
  {
    return m_quad_count;
  }

  [[nodiscard]] const auto &
    padding() const noexcept
  {
    return m_padding;
  }

  static constexpr std::size_t EXPECTED_SIZE
    = 8U;///< @brief Expected size of the GeometryHeader2 struct in bytes.
};
static_assert(sizeof(GeometryHeader2) == GeometryHeader2::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GEOMETRYHEADER2_HPP
