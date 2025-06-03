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
#ifndef VIIIARCHIVE_MAP_HPP
#define VIIIARCHIVE_MAP_HPP
#include "Mim.hpp"
#include "open_viii/tools/Tools.hpp"
#include "Pupu.hpp"
#include "Tile1.hpp"
#include "Tile2.hpp"
#include "Tile3.hpp"
#include "tl/write.hpp"
#include <limits>
#include <ranges>
#include <utility>
#include <variant>
namespace open_viii::graphics::background {
/**
 * @brief Concept to determine if a type represents a tile.
 *
 * A type satisfies `is_tile` if it has a nested `impl_type` and is derived from
 * it.
 *
 * @tparam T The type to check.
 */
template<typename T>
concept is_tile = requires { typename std::remove_cvref_t<T>::impl_type; }
               && std::derived_from<
                    std::remove_cvref_t<T>,
                    typename std::remove_cvref_t<T>::impl_type>;

/**
 * @brief Concept to determine if a type is a range of tiles.
 *
 * A type satisfies `is_tiles` if it is a range and its elements satisfy
 * `is_tile`.
 *
 * @tparam T The type to check.
 */
template<typename T>
concept is_tiles = is_tile<std::ranges::range_value_t<std::remove_cvref_t<T>>>
                && std::ranges::range<std::remove_cvref_t<T>>;

/**
 * @brief Concept to determine if a type is a contiguous and sized range of
 * tiles.
 *
 * A type satisfies `is_contiguous_sized_tiles` if it meets the `is_tiles`
 * requirement and is both a contiguous range and a sized range.
 *
 * @tparam T The type to check.
 */
template<typename T>
concept is_contiguous_sized_tiles
  = is_tiles<std::remove_cvref_t<T>>
 && std::ranges::contiguous_range<std::remove_cvref_t<T>>
 && std::ranges::sized_range<std::remove_cvref_t<T>>;

struct Map
{
  using variant_tiles = std::variant<
    std::monostate,
    std::vector<Tile1>,
    std::vector<Tile2>,
    std::vector<Tile3>>;

  using variant_tile = std::variant<std::monostate, Tile1, Tile2, Tile3>;

private:
  variant_tiles       m_tiles{};
  /**
   * offset holds the original position of canvas.
   */
  Point<std::int16_t> m_offset{};

public:
  /**
   * @brief Visits `m_tiles` and applies the provided lambda if it contains
   * valid tiles.
   *
   * If `m_tiles` satisfies `is_contiguous_sized_tiles`, the lambda is applied
   * to it. Otherwise, an empty `std::vector<Tile1>` is passed to the lambda.
   *
   * @tparam Lambda The type of the callable object.
   * @param lambda A callable object to be applied to `m_tiles` if it contains
   * valid tiles.
   * @return The result of the lambda function.
   */
  auto
    visit_tiles(auto &&lambda) const
  {
    return std::visit(
      [&lambda](const auto &tiles) {
        using tiles_type = std::remove_cvref_t<decltype(tiles)>;
        if constexpr (is_contiguous_sized_tiles<tiles_type>) {
          return lambda(tiles);
        }
        else {
          static const std::vector<Tile1> empty{};
          return lambda(empty);// what if default is bad.
        }
      },
      m_tiles);
  }

  /**
   * @brief Visits `m_tiles` and applies the provided lambda if it contains
   * valid tiles.
   *
   * If `m_tiles` satisfies `is_contiguous_sized_tiles`, the lambda is applied
   * to it. Otherwise, an empty `std::vector<Tile1>` is passed to the lambda.
   *
   * @tparam Lambda The type of the callable object.
   * @param lambda A callable object to be applied to `m_tiles` if it contains
   * valid tiles.
   * @return The result of the lambda function.
   */
  auto
    visit_tiles(auto &&lambda)
  {
    return std::visit(
      [&lambda](auto &tiles) {
        using tiles_type = std::remove_cvref_t<decltype(tiles)>;
        if constexpr (is_contiguous_sized_tiles<tiles_type>) {
          return lambda(tiles);
        }
        else {
          std::vector<Tile1> empty{};
          return lambda(empty);// what if default is bad.
        }
      },
      m_tiles);
  }

  /**
   * @brief Provides a filtering lambda to remove invalid tiles.
   *
   * This lambda checks if a tile's `x` coordinate is not equal to the maximum
   * possible `std::int16_t` value. If the type is not a tile, it is always
   * valid.
   *
   * @return A lambda function for filtering invalid tiles.
   */
  static constexpr auto
    filter_invalid() noexcept
  {
    return [](const auto &...args) {
      static constexpr auto end_x{ (std::numeric_limits<std::int16_t>::max)() };
      return (
        ([](const auto &arg) {
          if constexpr (is_tile<decltype(arg)>) {
            return std::cmp_not_equal(arg.x(), end_x);
          }
          else {
            return true;
          }
        }(args))
        && ...);
    };
  }

  /**
   * @brief Returns a range view that filters out invalid tiles.
   *
   * Uses `filter_invalid()` to create a `std::ranges::views::filter` view.
   *
   * @return A filter view that removes invalid tiles.
   */
  static constexpr auto
    filter_view_invalid() noexcept
  {
    return std::ranges::views::filter(Map::filter_invalid());
  }

  /**
   * @brief Gets the offset value of the map.
   *
   * @return The offset value.
   */
  [[nodiscard]] auto
    offset() const noexcept
  {
    return m_offset;
  }

  /**
   * @brief Compares two `Map` objects for equality.
   *
   * Two maps are considered equal if they have the same `m_tiles` index
   * and their tile contents match.
   *
   * @param other The `Map` instance to compare against.
   * @return `true` if the maps are equal, `false` otherwise.
   */
  bool
    operator==(const Map &other) const noexcept
  {
    return (m_tiles.index() == m_tiles.index())
        && other.visit_tiles([this](const is_tiles auto &other_tiles) -> bool {
             return visit_tiles(
               [&other_tiles](const is_tiles auto &tiles) -> bool {
                 if constexpr (std::is_same_v<
                                 std::remove_cvref<decltype(tiles)>,
                                 std::remove_cvref<decltype(other_tiles)>>) {
                   return std::ranges::equal(tiles, other_tiles);
                 }
                 else {
                   return false;
                 }
               });
           });
  }

  /**
   * @brief Checks if two `Map` objects are not equal.
   *
   * @param other The `Map` instance to compare against.
   * @return `true` if the maps are not equal, `false` otherwise.
   */
  bool
    operator!=(const Map &other) const noexcept
  {
    return !(*this == other);
  }

  /**
   * @brief Compares two `Map` objects to determine if the first is less than
   * the second.
   *
   * If the types of `tiles` and `other_tiles` don't match, returns `false`
   * (cannot determine order). An assertion is added in debug mode to warn that
   * this function may not work as expected if types don't match.
   *
   * @param other The `Map` instance to compare against.
   * @return `true` if this map is less than the other, `false` otherwise.
   */
  bool
    operator<(const Map &other) const noexcept
  {
    return visit_tiles([&other](const is_tiles auto &tiles) -> bool {
      return other.visit_tiles(
        [&tiles](const is_tiles auto &other_tiles) -> bool {
          if constexpr (std::is_same_v<
                          std::remove_cvref_t<decltype(tiles)>,
                          std::remove_cvref_t<decltype(other_tiles)>>) {
            return std::ranges::lexicographical_compare(tiles, other_tiles);
          }
          else {
            // Assertion to help detect mismatched types in debug mode
            assert(false && "this function won't work as expected if both tiles and other_tiles aren't the same type");
            return false;// Return false if types don't match
          }
        });
    });
  }

  /**
   * @brief Compares two `Map` objects to determine if the first is greater than
   * the second.
   *
   * If the types of `tiles` and `other_tiles` don't match, returns `false`
   * (cannot determine order).
   *
   * @param other The `Map` instance to compare against.
   * @return `true` if this map is greater than the other, `false` otherwise.
   */
  bool
    operator>(const Map &other) const noexcept
  {
    return other < *this;
  }

  /**
   * @brief Compares two `Map` objects to determine if the first is less than or
   * equal to the second.
   *
   * @param other The `Map` instance to compare against.
   * @return `true` if this map is less than or equal to the other, `false`
   * otherwise.
   */
  bool
    operator<=(const Map &other) const noexcept
  {
    return !(other < *this);
  }

  /**
   * @brief Compares two `Map` objects to determine if the first is greater than
   * or equal to the second.
   *
   * @param other The `Map` instance to compare against.
   * @return `true` if this map is greater than or equal to the other, `false`
   * otherwise.
   */
  bool
    operator>=(const Map &other) const noexcept
  {
    return !(*this < other);
  }

private:
  /**
   * @brief Calls the provided lambda if `m_tiles` is not a valid tile
   * container.
   *
   * This function checks if `m_tiles` does not satisfy the `is_tiles` concept.
   * If it doesn't, the given lambda is invoked, typically to initialize
   * `m_tiles`.
   *
   * @tparam Lambda The type of the callable object.
   * @param lambda A callable object to be executed if `m_tiles` is not a valid
   * tile container.
   */
  void
    visit_not_tiles(auto &&lambda) const
  {
    std::visit(
      [&lambda](auto &&tiles) {
        using tiles_type = std::remove_cvref_t<decltype(tiles)>;
        if constexpr (!is_tiles<tiles_type>) {
          lambda();
        }
      },
      m_tiles);
  }
  /**
   * @brief Computes the minimum and maximum elements of `m_tiles` using a
   * custom comparison.
   *
   * Filters out invalid tiles before applying `std::ranges::minmax_element`. If
   * `m_tiles` is not a valid tile container, the `fail` function is called.
   *
   * @tparam Compare A callable for comparing elements.
   * @tparam Get A callable to process the result of minmax_element.
   * @tparam Fail A callable to handle the failure case.
   * @param compare A comparison function.
   * @param get A function to extract values from minmax_element result.
   * @param fail A function to return a fallback value when `m_tiles` is
   * invalid.
   * @return The processed result from `get(tmp)` or `fail()`.
   */
  [[nodiscard]] constexpr auto
    minmax_generic(auto compare, auto get, auto fail) const noexcept
  {
    return std::visit(
      [&compare, &get, &fail](auto &&tiles) {
        using tiles_type = std::remove_cvref_t<decltype(tiles)>;
        if constexpr (is_tiles<tiles_type>) {
          auto filtered = tiles | std::views::filter(filter_invalid());
          auto tmp      = std::ranges::minmax_element(filtered, compare);
          return get(tmp);
        }
        else {
          return fail();
        }
      },
      m_tiles);
  }

  /**
   * @brief Computes the minimum element of `m_tiles` using a custom comparison.
   *
   * Filters out invalid tiles before applying `std::ranges::min_element`.
   * If `m_tiles` is not a valid tile container, the `fail` function is called.
   *
   * @tparam Compare A callable for comparing elements.
   * @tparam Get A callable to process the result of min_element.
   * @tparam Fail A callable to handle the failure case.
   * @param compare A comparison function.
   * @param get A function to extract values from min_element result.
   * @param fail A function to return a fallback value when `m_tiles` is
   * invalid.
   * @return The processed result from `get(tmp)` or `fail()`.
   */
  [[nodiscard]] constexpr auto
    min_generic(auto compare, auto get, auto fail) const noexcept
  {
    return std::visit(
      [&compare, &get, &fail](auto &&tiles) {
        using tiles_type = std::remove_cvref_t<decltype(tiles)>;
        if constexpr (is_tiles<tiles_type>) {
          auto filtered = tiles | std::views::filter(filter_invalid());
          auto tmp      = std::ranges::min_element(filtered, compare);
          return get(tmp);
        }
        else {
          return fail();
        }
      },
      m_tiles);
  }

  /**
   * @brief Computes the maximum element of `m_tiles` using a custom comparison.
   *
   * Filters out invalid tiles before applying `std::ranges::max_element`.
   * If `m_tiles` is not a valid tile container, the `fail` function is called.
   *
   * @tparam Compare A callable for comparing elements.
   * @tparam Get A callable to process the result of max_element.
   * @tparam Fail A callable to handle the failure case.
   * @param compare A comparison function.
   * @param get A function to extract values from max_element result.
   * @param fail A function to return a fallback value when `m_tiles` is
   * invalid.
   * @return The processed result from `get(tmp)` or `fail()`.
   */
  [[nodiscard]] constexpr auto
    max_generic(auto compare, auto get, auto fail) const noexcept
  {
    return std::visit(
      [&compare, &get, &fail](auto &&tiles) {
        using tiles_type = std::remove_cvref_t<decltype(tiles)>;
        if constexpr (is_tiles<tiles_type>) {
          auto filtered = tiles | std::views::filter(filter_invalid());
          auto tmp      = std::ranges::max_element(filtered, compare);
          return get(tmp);
        }
        else {
          return fail();
        }
      },
      m_tiles);
  }

  /**
   * @brief Computes the min and max X coordinates of the valid tiles.
   * @return A pair of minimum and maximum X values.
   */
  [[nodiscard]] constexpr auto
    minmax_x() const noexcept
  {
    return minmax_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.x() < b.x();
      },
      [](auto &&tmp) {
        return std::pair<const std::int16_t, const std::int16_t>{
          tmp.min->x(),
          tmp.max->x()
        };
      },
      []() {
        return std::pair<const std::int16_t, const std::int16_t>{};
      });
  }

  /**
   * @brief Computes the minimum X coordinate of the valid tiles.
   * @return The minimum X value.
   */
  [[nodiscard]] constexpr auto
    min_x() const noexcept
  {
    return min_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.x() < b.x();
      },
      [](auto &&tmp) {
        return tmp->x();
      },
      []() {
        return std::int16_t{};
      });
  }

  /**
   * @brief Computes the maximum X coordinate of the valid tiles.
   * @return The maximum X value.
   */
  [[nodiscard]] constexpr auto
    max_x() const noexcept
  {
    return max_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.x() < b.x();
      },
      [](auto &&tmp) {
        return tmp->x();
      },
      []() {
        return std::int16_t{};
      });
  }

  /**
   * @brief Computes the min and max Y coordinates of the valid tiles.
   * @return A pair of minimum and maximum Y values.
   */
  [[nodiscard]] constexpr auto
    minmax_y() const noexcept
  {
    return minmax_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.y() < b.y();
      },
      [](auto &&tmp) {
        return std::pair<const std::int16_t, const std::int16_t>{
          tmp.min->y(),
          tmp.max->y()
        };
      },
      []() {
        return std::pair<const std::int16_t, const std::int16_t>{};
      });
  }

  /**
   * @brief Computes the minimum Y coordinate of the valid tiles.
   * @return The minimum Y value.
   */
  [[nodiscard]] constexpr auto
    min_y() const noexcept
  {
    return min_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.y() < b.y();
      },
      [](auto &&tmp) {
        return tmp->y();
      },
      []() {
        return std::int16_t{};
      });
  }

  /**
   * @brief Computes the maximum Y coordinate of the valid tiles.
   * @return The maximum Y value.
   */
  [[nodiscard]] constexpr auto
    max_y() const noexcept
  {
    return max_generic(
      [](const auto &a, const auto &b) -> bool {
        return a.y() < b.y();
      },
      [](auto &&tmp) {
        return tmp->y();
      },
      []() {
        return std::int16_t{};
      });
  }

  /**
   * @brief Initializes the tile variant based on the given MimType.
   *
   * Allocates storage for the appropriate tile type and fills it using the
   * provided buffer.
   *
   * @param mim_type The type of MIM file being processed.
   * @param buffer The raw data buffer to initialize the tiles from.
   * @return A variant containing the initialized tiles or `std::monostate` if
   * the type is unknown.
   */
  static variant_tiles
    init_tiles(const MimType &mim_type, const std::span<const char> buffer)
  {
    variant_tiles tiles{};
    switch (mim_type.type()) {
    case 1:
      tiles = std::vector<Tile1>{};
      break;
    case 2:
      tiles = std::vector<Tile2>{};
      break;
    case 3:
      tiles = std::vector<Tile3>{};
      break;
    default:
      tiles = std::monostate{};// Unknown type.
    }
    std::visit(
      [&buffer](auto &&local_tiles) {
        using tiles_type = std::remove_cvref_t<decltype(local_tiles)>;
        if constexpr (is_contiguous_sized_tiles<tiles_type>) {
          using map_type = typename tiles_type::value_type;
          local_tiles.resize(buffer.size() / sizeof(map_type));
          std::memcpy(local_tiles.data(), buffer.data(), buffer.size());
        }
      },
      tiles);
    return tiles;
  }

public:
  constexpr static auto EXT = std::string_view(".map");

  Map()                     = default;

  /**
   * @brief Constructs a `Map` from a raw buffer.
   *
   * @param mim_type The type of MIM file being processed.
   * @param buffer A span of raw character data representing the tile
   * information.
   * @param shift Whether to shift the tiles to the origin after loading
   * (default: true).
   */
  explicit Map(
    const MimType              &mim_type,
    const std::span<const char> buffer,
    bool                        shift = true)
    : m_tiles(init_tiles(mim_type, buffer))
  {
    if (shift) {
      shift_to_origin();
    }
  }

  /**
   * @brief Constructs a `Map` from a file.
   *
   * Reads the file contents into a buffer and constructs the map.
   *
   * @param mim_type The type of MIM file being processed.
   * @param path The file path to read from.
   * @param shift Whether to shift the tiles to the origin after loading
   * (default: true).
   */
  Map(
    const MimType               &mim_type,
    const std::filesystem::path &path,
    bool                         shift = true)
    : Map(mim_type, tools::read_entire_file(path), shift)
  {}

  /**
   * @brief Constructs a `Map` from various integral types and a buffer.
   *
   * This constructor allows for flexible parameter passing by converting
   * integral types to `MimType` and using a `std::span` to reference the raw
   * tile data.
   *
   * @tparam T Integral type convertible to `MimType`.
   * @param mim_type The MIM type value.
   * @param buffer A span containing the raw tile data.
   * @param sort_remove Whether to sort and remove redundant tiles (default:
   * true).
   * @param shift Whether to shift the tiles to the origin after loading
   * (default: true).
   */
  Map(
    std::integral auto        &&mim_type,
    const std::span<const char> buffer,
    bool                        sort_remove = true,
    bool                        shift       = true)
    : Map(static_cast<MimType>(mim_type), buffer, sort_remove, shift)
  {}

  /**
   * @brief Constructs a `Map` using a tile-generating function.
   *
   * The function is repeatedly called to generate tiles until it returns
   * `std::monostate`.
   *
   * @tparam tile_funcT A callable that returns a variant containing a tile or
   * `std::monostate`.
   * @param tile_func The function used to generate tiles.
   */
  template<typename tile_funcT>
    requires(std::is_invocable_r_v<variant_tile, tile_funcT>)
  explicit Map(tile_funcT tile_func) : m_tiles(std::monostate())
  {

    bool       on   = true;
    const auto push = [this, &on](const auto &item) {
      using map_type = std::remove_cvref_t<decltype(item)>;
      if constexpr (std::is_same_v<map_type, std::monostate>) {
        on = false;
      }
      else {
        visit_not_tiles([this]() {
          m_tiles = std::vector<map_type>{};
        });
        visit_tiles([&on, &item](auto &&tiles) {
          using tiles_type     = std::remove_cvref_t<decltype(tiles)>;
          using local_map_type = typename tiles_type::value_type;
          if constexpr (std::is_same_v<map_type, local_map_type>) {
            tiles.push_back(item);
          }
          else {
            on = false;
          }
        });
      }
    };
    while (on) {
      std::visit(push, tile_func());
    }
  }

  /**
   * @brief Retrieves the type of the tile variant.
   *
   * @return A numeric value representing the type of tiles stored.
   */
  std::uint8_t
    get_type() const noexcept
  {

    if (m_tiles.index() == 3U) {
      return static_cast<std::uint8_t>(0U);
    }
    return static_cast<std::uint8_t>(m_tiles.index() + 1U);
  }

  /**
   * @brief Retrieves the stored tile container.
   *
   * @return A reference to the variant holding the tiles.
   */
  [[nodiscard]] const auto &
    tiles() const noexcept
  {
    return m_tiles;
  }

  /**
   * @brief Provides access to the stored tile container.
   *
   * @return A pointer to the variant holding the tiles.
   */
  [[nodiscard]] const auto *
    operator->() const noexcept
  {
    return &m_tiles;
  }

  /**
   * @brief Computes a rectangle large enough to contain all tiles.
   *
   * @return A rectangle encompassing all tiles.
   */
  [[nodiscard]] Rectangle<std::int32_t>
    canvas() const noexcept
  {
    const auto l_minmax_x                                  = minmax_x();
    const auto l_minmax_y                                  = minmax_y();
    const auto &[l_min_x, l_max_x]                         = l_minmax_x;
    const auto &[l_min_y, l_max_y]                         = l_minmax_y;
    constexpr static auto                        tile_size = 16;
    open_viii::graphics::Rectangle<std::int32_t> l_canvas{
      l_min_x,
      l_min_y,
      std::abs(l_max_x) + std::abs(l_min_x) + tile_size,
      std::abs(l_max_y) + std::abs(l_min_y) + tile_size
    };
    return l_canvas;
  }

  /**
   * @brief Shifts all tile positions by a given offset.
   *
   * @param x Horizontal shift.
   * @param y Vertical shift.
   */
  void
    shift(const std::int16_t &x, const std::int16_t &y) noexcept
  {
    const auto xy = Point(x, y);
    visit_tiles([&xy](auto &tiles) {
      auto filtered = tiles | std::views::filter(filter_invalid());
      std::ranges::transform(
        filtered,
        std::ranges::begin(tiles),
        [&xy](const auto &t) {
          return t.shift_xy(xy);
        });
    });
  }

  /**
   * @brief Shifts all tiles so that their minimum X and Y coordinates start
   * from (0,0).
   *
   * Some tiles are drawn off-screen to create a texture; this function shifts
   * all tiles to ensure the minimum coordinates are at least (0,0).
   */
  void
    shift_to_origin() noexcept
  {
    if (m_offset != decltype(m_offset){})// already shifted
    {
      return;
    }
    m_offset = Point(min_x(), min_y());
    if (m_offset.x() < 0 || m_offset.y() < 0) {
      shift(m_offset.abs());
    }
  }

  /**
   * @brief Shifts all tile positions by a given point.
   *
   * @param point The shift offset as a Point object.
   */
  void
    shift(const Point<std::int16_t> &point) noexcept
  {
    shift(point.x(), point.y());
  }

  /**
   * @brief Saves the tile data as a CSV file.
   *
   * @param in_path The output file path.
   */
  void
    save_csv(const std::string_view &in_path) const
  {
    auto path = std::filesystem::path(in_path);
    tools::write_buffer(
      [this](std::ostream &os) {
        os
          << R"("Index","Raw bytes","Draw","BPP","Blend Mode","Blend Other","Layer","Texture Page","Palette","Animation","Animation Frame","Source X","Source Y","X","Y","Z")"
          << '\n';
        std::size_t i{};
        visit_tiles([&i, &os](auto &&tiles) {
          std::for_each(
            std::ranges::cbegin(tiles),
            std::ranges::cend(tiles),
            [&os, &i](const auto &t) {
              os << i++ << ',' << '"';
              t.to_hex(os);
              os << "\"," << t.draw() << ',' << int{ t.depth() } << ",\"" <<
                [&t]() {
                  switch (t.blend_mode()) {
                  case BlendModeT::half_add:
                    return "Half Add";
                  case BlendModeT::add:
                    return "Add";
                  case BlendModeT::subtract:
                    return "Subtract";
                  case BlendModeT::quarter_add:
                    return "Quarter Add";
                  case BlendModeT::none:
                  default:
                    return "None";
                  }
                }()
                 << "\"," << static_cast<uint16_t>(t.blend()) << ','
                 << static_cast<uint16_t>(t.layer_id()) << ','
                 << static_cast<uint16_t>(t.texture_id()) << ','
                 << static_cast<uint16_t>(t.palette_id()) << ','
                 << static_cast<uint16_t>(t.animation_id()) << ','
                 << static_cast<uint16_t>(t.animation_state()) << ','
                 << static_cast<uint16_t>(t.source_x()) << ','
                 << static_cast<uint16_t>(t.source_y()) << ','
                 << static_cast<int16_t>(t.x()) << ','
                 << static_cast<int16_t>(t.y()) << ','
                 << static_cast<int16_t>(t.z()) << ','//<< Pupu(t)
                 << '\n';// std::nouppercase << std::dec << std::setw(0U)
            });
        });
      },
      (path.parent_path() / path.stem()).string() + ".csv");
  }

  /**
   * @brief Saves the map data to a file.
   *
   * Writes the tile data to a `.map` file using the specified path.
   *
   * @param in_path The output file path.
   */
  void
    save_map(const std::filesystem::path &in_path) const
  {
    if (in_path.empty()) {
      std::cerr << "Warning: Output file path is empty. Skipping save.\n";
      return;
    }

    if (!in_path.has_filename()) {
      std::cerr
        << "Warning: Output file path has no filename. Skipping save.\n";
      return;
    }

    if (!in_path.has_stem()) {
      std::cerr << "Warning: Output file path has no stem (base name). "
                   "Skipping save.\n";
      return;
    }

    tools::write_buffer(
      [this](std::ostream &os) {
        visit_tiles([&os](auto &&tiles) {
          std::ranges::for_each(tiles, [&os](auto &&tile) {
            tl::write::append(os, tile);
          });
        });
      },
      (in_path.parent_path() / in_path.stem()).string() + ".map");
  }
};

/**
 * @brief Creates a tile object based on the given tile type.
 *
 * Selects and constructs the appropriate tile type (`Tile1`, `Tile2`, `Tile3`)
 * based on the `tile_type` parameter. If an invalid tile type is provided, it
 * returns `std::monostate`.
 *
 * @tparam Range A range type containing tile data.
 * @tparam Misc Additional parameters required for tile construction.
 * @param tile_type The type of tile to create (1, 2, or 3).
 * @param data The data range used to construct the tile.
 * @param misc Additional parameters for tile construction.
 * @return A variant containing the created tile or `std::monostate` if an
 * invalid type is provided.
 */
auto
  get_tile(char tile_type, std::ranges::range auto &&data, auto &&...misc) ->
  typename Map::variant_tile
{
  switch (tile_type) {
  case 1:
  case '1':
    return Tile1(
      std::forward<decltype(data)>(data),
      std::forward<decltype(misc)>(misc)...);
    break;
  case 2:
  case '2':
    return Tile2(
      std::forward<decltype(data)>(data),
      std::forward<decltype(misc)>(misc)...);
    break;
  case 3:
  case '3':
    return Tile3(
      std::forward<decltype(data)>(data),
      std::forward<decltype(misc)>(misc)...);
    break;
  }
  return std::monostate{};
}

/**
 * @brief Outputs information about the map to an output stream.
 *
 * Prints the number of tile entries in the map.
 *
 * @param os The output stream.
 * @param m The `Map` instance to display.
 * @return The modified output stream.
 */
inline std::ostream &
  operator<<(std::ostream &os, const Map &m)
{
  m.visit_tiles([&os](auto &&tiles) {
    os << "map contains " << tiles.size() << " tile entries. \n";
  });
  return os;
}

}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MAP_HPP
