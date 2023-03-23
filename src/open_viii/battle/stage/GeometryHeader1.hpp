//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRYHEADER1_HPP
#define VIIIARCHIVE_GEOMETRYHEADER1_HPP
/**
 * @brief Represents the header of a geometry object in the battle stage.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 */
namespace open_viii::battle::stage {
struct GeometryHeader1
{
private:
  static constexpr std::uint16_t CONSTANT_VALUE = 0x0100;
  std::uint16_t m_always_1_0_1{ CONSTANT_VALUE };///< @brief Always 0x0100.
  std::uint16_t m_always_1_0_2{ CONSTANT_VALUE };///< @brief Always 0x0100.
  std::uint16_t m_number_vertices{};             ///< @brief Number of vertices.

public:
  /**
   * @brief default constructor.
   */
  GeometryHeader1() = default;

  /**
   * @brief Construct a new GeometryHeader1 object.
   * @param number_vertices The number of vertices in the geometry object.
   */
  explicit GeometryHeader1(std::uint16_t number_vertices)
    : m_number_vertices(number_vertices)
  {}

  /**
   * @brief Get the number of vertices in the geometry object.
   * @return The number of vertices.
   */
  [[nodiscard]] const auto &
    number_vertices() const noexcept
  {
    return m_number_vertices;
  }

  /**
   * @brief Check if the geometry header is valid.
   * @return true if the header is valid, false otherwise.
   */
  [[nodiscard]] bool
    test() const noexcept
  {
    return m_always_1_0_1 == CONSTANT_VALUE && m_always_1_0_2 == CONSTANT_VALUE
        && m_number_vertices > 0;
  }

  static constexpr std::size_t EXPECTED_SIZE
    = 6U;///< @brief Expected size of the GeometryHeader1 struct in bytes.
};
static_assert(sizeof(GeometryHeader1) == GeometryHeader1::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GEOMETRYHEADER1_HPP
