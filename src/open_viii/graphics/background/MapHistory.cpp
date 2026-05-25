//
// Created by pcvii on 6/2/2022.
//

#include "MapHistory.hpp"
// #include "gui/colors.hpp"
namespace open_viii::graphics::background {
using map_t = Map;

/**
 * @brief Calculates the Pupu IDs for the tiles in a given map.
 *
 * This function processes all tiles in the provided map and generates a
 * unique `PupuID` (an unsigned 64-bit integer) for each tile. The `PupuID` is
 * used to prevent conflicts or overlapping tiles in the unswizzled or
 * rendered images, allowing modders to modify and re-import tiles without
 * conflicts.
 *
 * The `UniquifyPupu` logic performs the following:
 * - Initially, a `PupuID` is generated for each tile, which includes 4 offset
 * bits.
 * - After the initial generation, a second pass is made where overlapping
 * tiles are detected.
 * - For overlapping tiles, the `PupuID` is incremented to resolve the
 * conflict, ensuring each tile has a unique ID even when they occupy the same
 * location.
 *
 * The `PupuID` is designed to wrap around the unsigned 64-bit integer,
 * allowing you to reverse the ID back to the original values, much like
 * `SourceTileConflicts` handles the swizzle or input locations.
 *
 * The generated `PupuID`s can be used to:
 * - Dump unswizzled images with unique filenames containing the raw hex value
 * of each `PupuID`.
 * - Allow modders to upscale or modify the images.
 * - Re-import the modified images back into the system without conflicts.
 *
 * @param map The map containing the tiles to process.
 * @return A vector of `PupuID` objects corresponding to the tiles in the map.
 */
static std::vector<PupuID>
  calculate_pupu(const map_t &map)
{
  return map.visit_tiles([](const auto &tiles) {
    std::vector<PupuID> pupu_ids = {};
    UniquifyPupu const  pupu_map = {};
    pupu_ids.reserve(std::ranges::size(tiles));

    std::ranges::transform(
      tiles | Map::filter_view_invalid(),
      std::back_insert_iterator(pupu_ids),
      pupu_map);
    return pupu_ids;
  });
}

/**
 * @brief Generates a vector of unique PupuIDs from the input vector.
 *
 * This function processes a vector of `PupuID` values, removing any duplicate
 * entries while preserving the order of unique elements (after sorting). The
 * steps include:
 * - Sorting the input vector to group duplicates together.
 * - Removing consecutive duplicates using `std::ranges::unique`.
 * - Erasing the duplicates from the resulting vector to ensure all elements
 * are unique.
 *
 * @param input A vector of `PupuID` values that may contain duplicates.
 * @return A new vector containing only the unique `PupuID` values from the
 * input.
 *
 * @note The function operates on a copy of the input, leaving the original
 * vector unmodified. The output is sorted due to the use of
 * `std::ranges::sort` prior to deduplication.
 */
static std::vector<PupuID>
  make_unique_pupu(const std::vector<PupuID> &input)
{
  // Copies the input
  std::vector<PupuID> output = input;

  // Sorts the vector to prepare for unique removal
  std::ranges::sort(output);

  // Removes consecutive duplicates
  const auto last = std::ranges::unique(output);

  // Erases the duplicates from the vector
  output.erase(last.begin(), last.end());
  return output;
}

/**
 * @brief Calculates source tile location conflicts for a given map.
 *
 * This function identifies and tracks conflicts between tiles in the provided
 * map. A conflict is defined as multiple tiles occupying the same grid
 * location. The result is a `SourceTileConflicts` object, which stores
 * information about conflicting tiles and their locations.
 *
 * @param map The map containing the tiles to process.
 * @return A `SourceTileConflicts` object representing the tile conflicts in
 * the map.
 */
[[nodiscard]] static SourceTileConflicts
  calculate_conflicts(const map_t &map)
{
  return map.visit_tiles([](const auto &tiles) {
    SourceTileConflicts stc{};
    std::ranges::for_each(
      tiles | Map::filter_view_invalid(),
      [&](const auto &tile) {
        stc(tile).push_back(std::ranges::distance(&tiles.front(), &tile));
      });
    return stc;
  });
}

/**
 * @brief Populates a map with the count of similar tiles within a given range
 * of tile indices.
 *
 * This function traverses a range of tile indices within the map and counts
 * the frequency of normalized tiles. The function returns a map where each
 * key is a normalized tile, and the corresponding value is the count of its
 * occurrences within the specified range of tile indices.
 *
 * The count of tiles is incremented for each occurrence of a normalized tile
 * in the provided range. This is useful for identifying tiles that appear
 * multiple times in the range, potentially for highlighting or providing
 * additional feedback to the user.
 *
 * @param map The map object that contains the tiles to be processed.
 * @param range_of_tile_indices A range of indices specifying the tiles to be
 * counted. The range can be any iterable sequence of indices that corresponds
 * to the tiles within the map.
 *
 * @return A map of `NormalizedSourceTile` to `std::uint8_t` where the key is
 * the normalized tile and the value is the count of its occurrences within
 * the specified range of tile indices.
 *
 * @note This function relies on `std::ranges::advance` to retrieve the tile
 * at each index, ensuring that the correct tile is processed for each index
 * in the given range.
 */
[[nodiscard]] static std::map<NormalizedSourceTile, std::uint8_t>
  populate_similar_tile_count(
    const map_t              &map,
    std::ranges::range auto &&range_of_tile_indices)
{

  return map.visit_tiles([&](const auto &tiles) {
    std::map<NormalizedSourceTile, std::uint8_t> ret = {};
    const auto to_tile                               = [&](const auto index) {
      assert(
        std::cmp_less(index, std::ranges::size(tiles))
        && "index out of range!");
      auto begin = std::ranges::cbegin(tiles);
      std::ranges::advance(begin, index);
      return *begin;
    };
    auto transform_view
      = range_of_tile_indices | std::ranges::views::transform(to_tile);
    for (const auto tile :
         transform_view) {// Increment the count for the current tile (default
                          // initializes to 0 if not found)
      ++ret[tile];
    }
    return ret;
  });
}

/**
 * @brief Populates a map with the count of animated tiles within a given
 * range of tile indices.
 *
 * This function processes a range of tile indices within the map and counts
 * the frequency of normalized animated tiles. The result is a map where each
 * key is a `NormalizedSourceAnimatedTile`, and the corresponding value
 * represents the count of its occurrences in the specified range.
 *
 * This function is useful for identifying frequently used animated tiles
 * within the range, which may help with optimization, debugging, or providing
 * visual feedback to users.
 *
 * @param map The map object containing tiles to process.
 * @param range_of_tile_indices A range of indices specifying the animated
 * tiles to be counted. The range can be any iterable sequence of indices
 * corresponding to tiles in the map.
 *
 * @return A map of `NormalizedSourceAnimatedTile` to `std::uint8_t` where
 * the key is the normalized animated tile and the value is the count of its
 * occurrences within the specified range of tile indices.
 *
 * @note This function ensures that tile indices are within bounds using an
 * `assert` statement. It leverages `std::ranges` to transform the range of
 * indices into tiles and count their occurrences.
 */
[[nodiscard]] static std::map<NormalizedSourceAnimatedTile, std::uint8_t>
  populate_animation_tile_count(
    const map_t              &map,
    std::ranges::range auto &&range_of_tile_indices)
{

  return map.visit_tiles([&](const auto &tiles) {
    std::map<NormalizedSourceAnimatedTile, std::uint8_t> ret = {};
    const auto to_tile = [&](const auto index) {
      assert(
        std::cmp_less(index, std::ranges::size(tiles))
        && "index out of range!");
      auto begin = std::ranges::cbegin(tiles);
      std::ranges::advance(begin, index);
      return *begin;
    };
    auto transform_view
      = range_of_tile_indices | std::ranges::views::transform(to_tile);
    for (const auto &tile :
         transform_view) {// Increment the count for the current tile (default
                          // initializes to 0 if not found)
      ++ret[tile];
    }
    return ret;
  });
}

// /**
//  * @brief Generates a map of normalized source animated tiles to their
//  counts based on similar tiles.
//  *
//  * This function processes a subset of tiles (`similar_tiles`) and creates
//  a new map where
//  * each normalized source animated tile is associated with its total count.
//  It aggregates counts
//  * from the given similar tiles map.
//  *
//  * @param similar_tiles A map containing normalized source tiles and their
//  counts.
//  * @return A map of normalized source animated tiles to their total counts.
//  */
// [[nodiscard]] static std::map<NormalizedSourceAnimatedTile, std::uint8_t>
//   populate_animation_tile_count(const MapHistory::nst_map
//   &similar_tiles)
// {
//      std::map<NormalizedSourceAnimatedTile, std::uint8_t> ret = {};
//      for (const auto &[tile, count] : similar_tiles)
//      {
//           ret[tile] += count;
//      }
//      return ret;
// }

/**
 * @brief Disambiguates conflicts between similar and animation tile counts.
 *
 * This function ensures that tiles are either marked as similar or animation
 * tiles, but not both. It resolves conflicts by prioritizing one category
 * over the other:
 * - If a tile's "similar" count is greater than 1, its animation count is set
 * to 0, as similar tiles are considered more specific.
 * - Otherwise, the "similar" count is set to 0, prioritizing animation tiles.
 *
 * The function assumes that the number of elements in
 * `working_similar_counts` is greater than or equal to the number of elements
 * in `working_animation_counts`. It uses the key from
 * `working_similar_counts` to access and modify corresponding values in
 * `working_animation_counts`.
 *
 * @param working_similar_counts A reference to a map of normalized source
 * tiles and their similar counts. This map must have a superset of the keys
 * in `working_animation_counts`.
 * @param working_animation_counts A reference to a map of normalized source
 * animated tiles and their counts. Keys in this map may be a subset of those
 * in `working_similar_counts`.
 *
 * @note This function uses an `assert` to ensure that the size of
 * `working_similar_counts` is greater than or equal to the size of
 * `working_animation_counts`.
 */
static void
  disambiguate_normalized_tiles(
    MapHistory::nst_map  &working_similar_counts,
    MapHistory::nsat_map &working_animation_counts)
{
  assert(
    std::cmp_greater_equal(
      std::ranges::size(working_similar_counts),
      std::ranges::size(working_animation_counts)));
  std::ranges::for_each(working_similar_counts, [&](auto &similar) {
    if (
      std::cmp_equal(
        similar.first.m_animation_id,
        (std::numeric_limits<std::uint8_t>::max)())) {
      if (similar.second > 1) {
        auto &animated = working_animation_counts[similar.first];
        animated       = 0;// similar is more specific
      }
      else {
        similar.second = 0;// assume animation is more important
      }
      return;
    }

    similar.second = 0;// assume animation is more important
  });
}

static std::vector<Color32RGBA>
  create_colors_from_indexes(std::size_t count)
{
  return std::views::iota(0u, static_cast<std::uint32_t>(count))
       | std::views::transform([&](const auto index) {
           return PupuID::encode_uint_to_rgba(
             static_cast<std::uint32_t>(index));
         })
       | std::ranges::to<std::vector>();
}

MapHistory::MapHistory(map_t map)
  : m_original(std::move(map)), m_working(m_original),
    m_original_pupu(calculate_pupu(m_original)),
    m_working_pupu(m_original_pupu),
    m_original_unique_pupu(make_unique_pupu(m_original_pupu)),
    m_working_unique_pupu(m_original_unique_pupu),
    m_original_unique_pupu_color(
      create_colors_from_indexes(std::ranges::size(m_original_unique_pupu))),
    m_working_unique_pupu_color(m_original_unique_pupu_color),
    m_original_conflicts(calculate_conflicts(m_original)),
    m_working_conflicts(calculate_conflicts(m_original)),
    m_working_similar_counts(populate_similar_tile_count(
      m_working,
      m_working_conflicts.range_of_conflicts_flattened())),
    m_working_animation_counts(populate_animation_tile_count(
      m_working,
      m_working_conflicts.range_of_conflicts_flattened()))
{
  disambiguate_normalized_tiles(
    m_working_similar_counts,
    m_working_animation_counts);
}

void
  MapHistory::refresh_original_all(bool force) const
{
  if (!m_original_changed && !force) {
    return;
  }
  refresh_original_pupu();
  refresh_original_conflicts();
  m_original_changed = false;
}

void
  MapHistory::refresh_working_all(bool force) const
{
  if (!m_working_changed && !force) {
    return;
  }
  refresh_working_conflicts();
  refresh_working_pupu();
  m_working_changed = false;
}

void
  MapHistory::refresh_original_pupu() const
{
  m_original_pupu        = calculate_pupu(m_original);
  m_original_unique_pupu = make_unique_pupu(m_original_pupu);
  if (
    std::ranges::size(m_original_unique_pupu)
    != std::ranges::size(m_original_unique_pupu_color)) {
    m_original_unique_pupu_color
      = create_colors_from_indexes(std::ranges::size(m_original_unique_pupu));
  }
}

void
  MapHistory::refresh_working_pupu() const
{
  m_working_pupu        = calculate_pupu(m_working);
  m_working_unique_pupu = make_unique_pupu(m_working_pupu);
  if (
    std::ranges::size(m_original_unique_pupu)
    != std::ranges::size(m_working_unique_pupu_color)) {
    m_working_unique_pupu_color
      = create_colors_from_indexes(std::ranges::size(m_working_unique_pupu));
  }
}

void
  MapHistory::refresh_original_conflicts() const
{
  m_original_conflicts = calculate_conflicts(m_original);
}

void
  MapHistory::refresh_working_conflicts() const
{
  m_working_conflicts      = calculate_conflicts(m_working);
  m_working_similar_counts = populate_similar_tile_count(
    m_working,
    m_working_conflicts.range_of_conflicts_flattened());
  m_working_animation_counts = populate_animation_tile_count(
    m_working,
    m_working_conflicts.range_of_conflicts_flattened());
  disambiguate_normalized_tiles(
    m_working_similar_counts,
    m_working_animation_counts);
}

const std::vector<PupuID> &
  MapHistory::original_pupu() const noexcept
{
  return m_original_pupu;
}

const std::vector<PupuID> &
  MapHistory::working_pupu() const noexcept
{
  return m_working_pupu;
}

const std::vector<PupuID> &
  MapHistory::original_unique_pupu() const noexcept
{
  return m_original_unique_pupu;
}

const std::vector<PupuID> &
  MapHistory::working_unique_pupu() const noexcept
{
  return m_working_unique_pupu;
}

const std::vector<Color32RGBA> &
  MapHistory::original_unique_pupu_color() const noexcept
{
  return m_original_unique_pupu_color;
}

const std::vector<Color32RGBA> &
  MapHistory::working_unique_pupu_color() const noexcept
{
  return m_working_unique_pupu_color;
}

const SourceTileConflicts &
  MapHistory::original_conflicts() const noexcept
{
  return m_original_conflicts;
}

const SourceTileConflicts &
  MapHistory::working_conflicts() const noexcept
{
  return m_working_conflicts;
}

const MapHistory::nst_map &
  MapHistory::working_similar_counts() const noexcept
{
  return m_working_similar_counts;
}

const MapHistory::nsat_map &
  MapHistory::working_animation_counts() const noexcept
{
  return m_working_animation_counts;
}

std::size_t
  MapHistory::count() const
{
  return m_undo_history.size();
}

std::size_t
  MapHistory::redo_count() const
{
  return m_redo_history.size();
}

const map_t &
  MapHistory::original() const
{
  return m_original;
}

map_t &
  MapHistory::original_mutable()
{
  return m_original;
}

map_t &
  MapHistory::working()
{
  return m_working;
}

const map_t &
  MapHistory::working() const
{
  return m_working;
}

map_t &
  MapHistory::copy_original(std::string description)
{
  if (m_in_multi_frame_operation) {
    return original_mutable();
  }
  //(void)debug_count_print();
  clear_redo();
  m_undo_original_or_working.push_back(pushed::original);
  m_undo_history.push_back(original());
  m_undo_change_descriptions.push_back(std::move(description));
  m_original_changed = true;
  return original_mutable();
}

map_t &
  MapHistory::copy_working(std::string description)
{
  if (m_in_multi_frame_operation) {
    return working();
  }
  //(void)debug_count_print();
  clear_redo();
  m_undo_original_or_working.push_back(pushed::working);
  m_undo_history.push_back(working());
  m_undo_change_descriptions.push_back(std::move(description));
  m_working_changed = true;
  return working();
}

map_t &
  MapHistory::begin_multi_frame_working(std::string description)
{
  if (!m_in_multi_frame_operation) {
    auto &result               = copy_working(std::move(description));
    m_in_multi_frame_operation = true;
    return result;
  }
  return working();
}

void
  MapHistory::end_multi_frame_working(std::string description)
{
  m_in_multi_frame_operation = false;
  if (!description.empty() && !m_undo_change_descriptions.empty()) {
    m_undo_change_descriptions.back() = std::move(description);
  }
  m_working_changed = true;
  // if nothing changes remove undo till there are no matches.
  std::ignore       = remove_duplicate();
}

void
  MapHistory::clear_redo()
{
  m_redo_history.clear();
  m_redo_original_or_working.clear();
}

bool
  MapHistory::remove_duplicate()
{
  bool ret = false;
  while (undo_enabled() &&
         ((m_undo_original_or_working.back() == pushed::working
           && m_undo_history.back() == m_working)
          || (m_undo_original_or_working.back() == pushed::original
              && m_undo_history.back() == m_original)))
     {
    (void)undo(true);
    ret = true;
  }
  return ret;
}

const map_t &
  MapHistory::first_to_original(std::string description)
{
  //(void)debug_count_print();
  clear_redo();
  m_undo_history.push_back(original());
  m_undo_original_or_working.push_back(pushed::original);
  m_undo_change_descriptions.push_back(std::move(description));
  m_original         = m_undo_history.front();
  m_original_changed = true;
  return original();
}

const map_t &
  MapHistory::first_to_working(std::string description)
{
  //(void)debug_count_print();
  clear_redo();
  m_undo_history.push_back(working());
  m_undo_original_or_working.push_back(pushed::working);
  m_undo_change_descriptions.push_back(std::move(description));
  m_working         = m_undo_history.front();
  m_working_changed = true;
  return working();
}

const map_t &
  MapHistory::copy_working_to_original(std::string description)
{
  //(void)debug_count_print();
  clear_redo();
  m_undo_history.push_back(original());
  m_undo_original_or_working.push_back(pushed::original);
  m_undo_change_descriptions.push_back(std::move(description));
  // todo do we want to recalculate the pupu?
  m_original         = working();
  m_original_changed = true;
  return original();
}

bool
  MapHistory::redo()
{
  // const auto count = debug_count_print();
  if (!redo_enabled()) {
    return false;
  }
  const pushed last = m_redo_original_or_working.back();
  m_undo_original_or_working.push_back(last);
  m_redo_original_or_working.pop_back();

  (void)m_undo_change_descriptions.emplace_back(
    std::move(m_redo_change_descriptions.back()));
  m_redo_change_descriptions.pop_back();
  if (last == pushed::working) {
    (void)m_undo_history.emplace_back(std::move(m_working));
    m_working         = std::move(m_redo_history.back());
    m_working_changed = true;
    m_redo_history.pop_back();
    return true;
  }
  (void)m_undo_history.emplace_back(std::move(m_original));
  m_original         = std::move(m_redo_history.back());
  m_original_changed = true;
  m_redo_history.pop_back();
  return true;
}

bool
  MapHistory::undo(bool skip_redo)
{
  // const auto count = debug_count_print();
  if (!undo_enabled()) {
    return false;
  }
  const pushed last = m_undo_original_or_working.back();
  if (!skip_redo) {
    m_redo_original_or_working.push_back(last);
    (void)m_redo_change_descriptions.emplace_back(
      std::move(m_undo_change_descriptions.back()));
  }
  m_undo_original_or_working.pop_back();
  m_undo_change_descriptions.pop_back();
  if (last == pushed::working) {
    if (!skip_redo) {
      m_redo_history.emplace_back(std::move(m_working));
    }
    m_working         = std::move(m_undo_history.back());
    m_working_changed = true;
    m_undo_history.pop_back();
    return true;
  }
  if (!skip_redo) {
    m_redo_history.emplace_back(std::move(m_original));
  }
  m_original         = std::move(m_undo_history.back());
  m_original_changed = true;
  m_undo_history.pop_back();
  return true;
}
void
  MapHistory::undo_all()
{
  while (undo()) {}
}
void
  MapHistory::redo_all()
{
  while (redo()) {}
}
bool
  MapHistory::redo_enabled() const
{
  return !std::ranges::empty(m_redo_history)
      && !std::ranges::empty(m_redo_original_or_working);
}

bool
  MapHistory::undo_enabled() const
{
  return !std::ranges::empty(m_undo_history)
      && !std::ranges::empty(m_undo_original_or_working);
}

const map_t &
  MapHistory::const_working() const
{
  return m_working;
}

template<is_tile TileT>
[[nodiscard]] PupuID
  MapHistory::get_pupu_from_working(const TileT &tile) const
{
  return m_original_pupu[static_cast<std::size_t>(
    get_offset_from_working(tile))];
}

// Explicit instantiation for Tiles
template PupuID
  MapHistory::get_pupu_from_working(const Tile1 &) const;
template PupuID
  MapHistory::get_pupu_from_working(const Tile2 &) const;
template PupuID
  MapHistory::get_pupu_from_working(const Tile3 &) const;

template<open_viii::graphics::background::is_tile TileT>
std::vector<TileT>::difference_type
  MapHistory::get_offset_from_working(const TileT &tile) const
{
  return working().visit_tiles(
    [&](const auto &tiles) -> std::vector<TileT>::difference_type {
      if constexpr (
        std::is_same_v<
          std::ranges::range_value_t<std::remove_cvref_t<decltype(tiles)>>,
          TileT>) {
        return static_cast<std::vector<TileT>::difference_type>(
          std::ranges::distance(&tiles.front(), &tile));
      }
      else {
        return {};
      }
    });
}

template std::vector<Tile1>::difference_type
  MapHistory::get_offset_from_working(const Tile1 &) const;
template std::vector<Tile2>::difference_type
  MapHistory::get_offset_from_working(const Tile2 &) const;
template std::vector<Tile3>::difference_type
  MapHistory::get_offset_from_working(const Tile3 &) const;

std::string_view
  MapHistory::current_undo_description() const
{
  if (std::ranges::empty(m_undo_change_descriptions)) {
    return {};
  }
  return m_undo_change_descriptions.back();
}

std::string_view
  MapHistory::current_redo_description() const
{
  if (std::ranges::empty(m_redo_change_descriptions)) {
    return {};
  }
  return m_redo_change_descriptions.back();
}

[[nodiscard]] MapHistory::pushed
  MapHistory::current_undo_pushed() const
{
  if (std::ranges::empty(m_undo_original_or_working)) {
    return {};
  }
  return m_undo_original_or_working.back();
}

[[nodiscard]] MapHistory::pushed
  MapHistory::current_redo_pushed() const
{
  if (std::ranges::empty(m_redo_original_or_working)) {
    return {};
  }
  return m_redo_original_or_working.back();
}
}// namespace open_viii::graphics::background