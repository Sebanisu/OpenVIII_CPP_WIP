#ifndef CF53C804_EE24_4949_8AA8_FE0182BFCCC1
#define CF53C804_EE24_4949_8AA8_FE0182BFCCC1
#include "Map.hpp"
#include "Tile1.hpp"
#include <array>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <type_traits>
#include <utility>

namespace open_viii::graphics::background {

/**
 * @brief A class for tracking and resolving conflicts between source tiles.
 *
 * The `source_tile_conflicts` class is used to manage and analyze conflicts
 * between tiles in a grid. It provides methods to:
 * - Track the number of tiles at each location.
 * - Identify locations with conflicts (multiple tiles in the same spot).
 * - Determine empty and occupied locations.
 * - Retrieve tile locations and manage their indexes.
 *
 * This class is marked as `[[nodiscard]]` to indicate that the results of its
 * functions should not be ignored.
 *
 * @note The class is marked `final` to prevent inheritance.
 */
class [[nodiscard]] source_tile_conflicts final
{
public:
  /**
   * @brief Represents a 3D location in the grid.
   *
   * This structure stores the coordinates of a tile in the grid, including:
   * - `x`: The horizontal coordinate.
   * - `y`: The vertical coordinate.
   * - `t`: The texture page (layer) the tile belongs to.
   */
  struct location
  {
    std::uint8_t x{};///< The x-coordinate of the tile.
    std::uint8_t y{};///< The y-coordinate of the tile.
    std::uint8_t t{};///< The texture page of the tile.
  };

  /**
   * @brief The width of each tile grid in units.
   *
   * This constant represents the number of horizontal tiles in the grid.
   */
  static constexpr auto X_SIZE    = 16U;

  /**
   * @brief The height of each tile grid in units.
   *
   * This constant represents the number of vertical tiles in the grid.
   */
  static constexpr auto Y_SIZE    = 16U;

  /**
   * @brief The number of texture pages in the grid.
   *
   * This constant represents the total number of texture pages (layers) in
   * the grid.
   */
  static constexpr auto T_SIZE    = 14U;

  /**
   * @brief The total number of tiles in a single texture page.
   *
   * This constant is calculated as the product of the grid's width
   * (`X_SIZE`) and height (`Y_SIZE`).
   */
  static constexpr auto GRID_SIZE = X_SIZE * Y_SIZE;
#ifdef UT_source_tile_conflicts_test
  // Code specific to UT_source_tile_conflicts_test
public:
#else
  // Code for other cases
private:
#endif

  /**
   * @brief Alias for the underlying grid storage.
   *
   * This type represents a multidimensional grid that stores vectors of tile
   * difference types. It is defined as a `std::array` where each element
   * corresponds to a specific position in a 3D grid (x, y, t). The vectors
   * store `difference_type` values, which represent offsets or indexes
   * within the tile data.
   *
   * @note The use of `std::vector<std::uint8_t>::difference_type` ensures
   * compatibility across different tile types, avoiding the need for
   * templates. A static assertion can be added to confirm the
   * `difference_type` is consistent across all potential tile types.
   */
  using grid_array = std::array<
    std::vector<std::vector<std::uint8_t>::difference_type>,
    X_SIZE * Y_SIZE * T_SIZE>;
  using grid_array_ptr  = std::shared_ptr<grid_array>;

  /**
   * @brief The grid storage for tracking tile indexes.
   *
   * This member variable is an instance of `grid_array`, which keeps track
   * of the tile indexes present in each location of the 3D grid (x, y, t).
   * It allows you to determine:
   * - How many tiles are in each location.
   * - Which locations are empty.
   * - The specific tiles occupying each location.
   *
   * Each entry in the grid corresponds to a specific location and contains a
   * vector of tile indexes, which can be used for further analysis or
   * operations.
   */
  grid_array_ptr m_grid = std::make_shared<grid_array>();

  /**
   * @brief Calculates the index of a tile based on its coordinates and
   * texture page.
   *
   * This function calculates a unique index for a tile given its `x`, `y`
   * coordinates, and texture page `t`. It ensures the inputs are validated
   * and asserts that the calculated index is within range.
   *
   * @tparam X The type of the x-coordinate (must be an unsigned integral).
   * @tparam Y The type of the y-coordinate (must be an unsigned integral).
   * @tparam T The type of the texture page (must be an unsigned integral).
   * @param x The x-coordinate of the tile.
   * @param y The y-coordinate of the tile.
   * @param t The texture page of the tile.
   * @return The calculated index of the tile.
   */
  template<
    std::unsigned_integral X,
    std::unsigned_integral Y,
    std::unsigned_integral T>
  [[nodiscard]] static constexpr std::unsigned_integral auto
    calculate_index(X x, Y y, T t) noexcept
  {
    validate_inputs(x, y, t);
    auto index = t * (GRID_SIZE) + y + (x / X_SIZE);
    assert(
      std::cmp_less(index, X_SIZE * Y_SIZE * T_SIZE)
      && "the calculated index is out of range.");

    return index;
  }

  /**
   * @brief Validates the inputs for tile index calculation.
   *
   * This function checks that the `x` and `y` coordinates, as well as the
   * texture page `t`, meet the required constraints. It ensures values are
   * within acceptable ranges and that `x` and `y` are multiples of their
   * respective sizes.
   *
   * @tparam X The type of the x-coordinate (must be an unsigned integral).
   * @tparam Y The type of the y-coordinate (must be an unsigned integral).
   * @tparam T The type of the texture page (must be an unsigned integral).
   * @param x The x-coordinate to validate.
   * @param y The y-coordinate to validate.
   * @param t The texture page to validate.
   */
  template<
    std::unsigned_integral X,
    std::unsigned_integral Y,
    std::unsigned_integral T>
  static constexpr void
    validate_inputs(
      [[maybe_unused]] X x,
      [[maybe_unused]] Y y,
      [[maybe_unused]] T t) noexcept
  {
    if constexpr (!std::is_same_v<X, std::uint8_t>) {
      assert(std::cmp_less(x, GRID_SIZE) && "x must be less than 256");
    }
    if constexpr (!std::is_same_v<Y, std::uint8_t>) {
      assert(std::cmp_less(y, GRID_SIZE) && "y must be less than 256");
    }
    assert(std::cmp_equal(x % X_SIZE, 0U) && "x must be a multiple of 16");
    assert(std::cmp_equal(y % Y_SIZE, 0U) && "y must be a multiple of 16");
    assert(
      std::cmp_less(t, T_SIZE)
      && "t must be less than the maximum number of texture pages");
  }

  /**
   * @brief Reverses an index to calculate its tile location.
   *
   * This function takes an index and calculates the corresponding tile
   * location in terms of its `x`, `y` coordinates and texture page `t`.
   *
   * @tparam Index The type of the index (must be an integral type).
   * @param index The index to reverse.
   * @return The location of the tile, containing `x`, `y`, and `t`.
   */
  template<std::integral Index>
  static constexpr location
    reverse_index(Index index) noexcept
  {
    location l;
    if constexpr (std::signed_integral<Index>) {
      assert(std::cmp_greater_equal(index, 0) && " index must be greater than 0");
    }
    assert(
      std::cmp_less(index, X_SIZE * Y_SIZE * T_SIZE)
      && "the index is out of range");
    l.t = static_cast<std::uint8_t>(
      static_cast<std::make_unsigned<Index>::type>(index)
      / GRID_SIZE);// Reverse the t calculation
    const Index remaining = static_cast<std::make_unsigned<Index>::type>(index)
                          % GRID_SIZE;// Remaining part after extracting t
    l.y                   = static_cast<std::uint8_t>(
      (remaining / X_SIZE) * Y_SIZE);// y is the remainder
    l.x = static_cast<std::uint8_t>(
      (remaining % Y_SIZE) * X_SIZE);// Calculate x by reversing the division
    return l;
  }

public:
#if defined(__cpp_multidimensional_subscript)                                  \
  && __cpp_multidimensional_subscript >= 202110L
  /**
   * @brief Accesses the grid element at the specified 3D coordinates (x, y,
   * t).
   *
   * This operator provides mutable access to the element at the given
   * coordinates in the grid. Requires C++23 support for multidimensional
   * subscripting.
   *
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param t The texture page (t-coordinate).
   * @return A reference to the grid element at the specified coordinates.
   */
  [[nodiscard]] constexpr auto &
    operator[](std::uint16_t x, std::uint16_t y, std::uint8_t t) noexcept
  {
    return m_grid.get()->at(calculate_index(x, y, t));
  }

  /**
   * @brief Accesses the grid element at the specified 3D coordinates (x, y,
   * t).
   *
   * This operator provides read-only access to the element at the given
   * coordinates in the grid. Requires C++23 support for multidimensional
   * subscripting.
   *
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param t The texture page (t-coordinate).
   * @return The grid element at the specified coordinates.
   */
  [[nodiscard]] constexpr auto
    operator[](std::uint16_t x, std::uint16_t y, std::uint8_t t) const noexcept
  {
    return m_grid.get()->at(calculate_index(x, y, t));
  }

  /**
   * @brief Accesses the grid element corresponding to a given tile.
   *
   * This operator provides mutable access to the grid element using a tile's
   * attributes. Requires C++23 support for multidimensional subscripting.
   *
   * @tparam tile_t The type of the tile, satisfying
   * `open_viii::graphics::background::is_tile`.
   * @param tile The tile object whose attributes specify the grid element.
   * @return A reference to the grid element corresponding to the tile.
   */
  template<open_viii::graphics::background::is_tile tile_t>
  [[nodiscard]] constexpr auto &
    operator[](const tile_t &tile) noexcept
  {
    return m_grid.get()->at(
      calculate_index(tile.source_x(), tile.source_y(), tile.texture_id()));
  }

  /**
   * @brief Accesses the grid element corresponding to a given tile.
   *
   * This operator provides read-only access to the grid element using a
   * tile's attributes. Requires C++23 support for multidimensional
   * subscripting.
   *
   * @tparam tile_t The type of the tile, satisfying
   * `open_viii::graphics::background::is_tile`.
   * @param tile The tile object whose attributes specify the grid element.
   * @return The grid element corresponding to the tile.
   */
  template<open_viii::graphics::background::is_tile tile_t>
  [[nodiscard]] constexpr auto
    operator[](const tile_t &tile) const noexcept
  {
    return m_grid.get()->at(
      calculate_index(tile.source_x(), tile.source_y(), tile.texture_id()));
  }
#endif

  /**
   * @brief Accesses the grid element at the specified 3D coordinates (x, y,
   * t).
   *
   * This operator provides mutable access to the element at the given
   * coordinates in the grid.
   *
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param t The texture page (t-coordinate).
   * @return A reference to the grid element at the specified coordinates.
   */
  [[nodiscard]] constexpr auto &
    operator()(
      const std::uint8_t x,
      const std::uint8_t y,
      const std::uint8_t t) noexcept
  {
    return m_grid.get()->at(calculate_index(x, y, t));
  }

  /**
   * @brief Accesses the grid element at the specified 3D coordinates (x, y,
   * t).
   *
   * This operator provides read-only access to the element at the given
   * coordinates in the grid.
   *
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @param t The texture page (t-coordinate).
   * @return The grid element at the specified coordinates.
   */
  [[nodiscard]] constexpr auto
    operator()(const std::uint8_t x, const std::uint8_t y, const std::uint8_t t)
      const noexcept
  {
    return m_grid.get()->at(calculate_index(x, y, t));
  }

  /**
   * @brief Accesses the grid element corresponding to a given tile.
   *
   * This operator provides mutable access to the grid element using a tile's
   * attributes.
   *
   * @tparam tile_t The type of the tile, satisfying
   * `open_viii::graphics::background::is_tile`.
   * @param tile The tile object whose attributes specify the grid element.
   * @return A reference to the grid element corresponding to the tile.
   */
  template<open_viii::graphics::background::is_tile tile_t>
  [[nodiscard]] constexpr auto &
    operator()(const tile_t &tile) noexcept
  {
    return m_grid.get()->at(
      calculate_index(tile.source_x(), tile.source_y(), tile.texture_id()));
  }

  /**
   * @brief Accesses the grid element corresponding to a given tile.
   *
   * This operator provides read-only access to the grid element using a
   * tile's attributes.
   *
   * @tparam tile_t The type of the tile, satisfying
   * `open_viii::graphics::background::is_tile`.
   * @param tile The tile object whose attributes specify the grid element.
   * @return The grid element corresponding to the tile.
   */
  template<open_viii::graphics::background::is_tile tile_t>
  [[nodiscard]] constexpr auto
    operator()(const tile_t &tile) const noexcept
  {
    return m_grid.get()->at(
      calculate_index(tile.source_x(), tile.source_y(), tile.texture_id()));
  }

  // /**
  //  * @brief Compares two `source_tile_conflicts` objects for ordering and
  //  equality.
  //  *
  //  * This operator provides default comparison behavior using the
  //  three-way comparison operator.
  //  *
  //  * @param other The other `source_tile_conflicts` object to compare.
  //  * @return A `std::strong_ordering` indicating the result of the
  //  comparison.
  //  */
  // constexpr auto               operator<=>(const source_tile_conflicts &)
  // const noexcept = default;

  /**
   * @brief Retrieves a range of vectors of tile indexes for each conflict.
   *
   * This function filters the grid to find tiles that have conflicts. A
   * conflict is identified as a vector with more than one tile index. The
   * resulting range allows you to identify which tiles are conflicting with
   * each other.
   *
   * @return A range of vectors, where each vector contains tile indexes
   * representing a conflict.
   */
  [[nodiscard]] constexpr auto
    range_of_conflicts() const
  {
    namespace v = std::ranges::views;
    return *m_grid | v::filter([](const auto &v) {
      return std::ranges::size(v) > 1U;
    });
  }

  /**
   * @brief Retrieves a flattened range of tile indexes for each conflict.
   *
   * This function filters the grid to find tiles that have conflicts,
   * similar to `range_of_conflicts`. However, the result is a flattened
   * range of tile indexes, where only the conflicting tile indexes are
   * included, without grouping them into vectors.
   *
   * @return A flattened range of tile indexes representing all conflicts.
   */
  [[nodiscard]] constexpr auto
    range_of_conflicts_flattened() const
  {
    namespace v = std::ranges::views;
    return *m_grid | v::filter([](const auto &v) {
      return std::ranges::size(v) > 1U;
    }) | v::join
         | std::ranges::to<std::vector>();
  }

  /**
   * @brief Retrieves a flattened range of tile indexes for tiles with no
   * conflicts.
   *
   * This function filters the grid to find tiles that have no conflicts. A
   * non-conflict is identified as a vector with exactly one tile index. The
   * resulting range is flattened, containing only the tile indexes for
   * non-conflicting tiles.
   *
   * @return A flattened range of tile indexes representing non-conflicting
   * tiles.
   */
  [[nodiscard]] constexpr auto
    range_of_non_conflicts_flattened() const
  {
    namespace v = std::ranges::views;
    return *m_grid | v::filter([](const auto &v) {
      return std::ranges::size(v) == 1U;
    }) | v::join;
  }

  /**
   * @brief Retrieves a range of tile locations that are empty.
   *
   * This function filters the grid to find locations with no tiles, where
   * the size of the vector at a given location is 0. It then transforms the
   * result into a range of tile locations, calculating the reverse index for
   * each empty location.
   *
   * @return A range of tile locations (as reverse indexes) that are empty.
   */
  [[nodiscard]] constexpr auto
    range_of_empty_locations() const
  {
    namespace v = std::ranges::views;
    return *m_grid | v::filter([](const auto &v) {
      return std::ranges::size(v) == 0U;
    }) | v::transform([&](const auto &v) {
      return reverse_index(std::ranges::distance(&m_grid.get()->front(), &v));
    });
  }

  /**
   * @brief Retrieves a range of tile locations that are occupied.
   *
   * This function filters the grid to find locations with tiles, where the
   * size of the vector at a given location is not 0. It then transforms the
   * result into a range of tile locations, calculating the reverse index for
   * each occupied location.
   *
   * @return A range of tile locations (as reverse indexes) that are
   * occupied.
   */
  [[nodiscard]] constexpr auto
    range_of_occupied_locations() const
  {
    namespace v = std::ranges::views;
    return *m_grid | v::filter([](const auto &v) {
      return std::ranges::size(v) != 0U;
    }) | v::transform([&](const auto &v) {
      return reverse_index(std::ranges::distance(&m_grid.get()->front(), &v));
    });
  }
};
}// namespace open_viii::graphics::background

#endif /* CF53C804_EE24_4949_8AA8_FE0182BFCCC1 */
