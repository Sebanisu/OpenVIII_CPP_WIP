// SourceTileConflicts.cpp

#include <boost/ut.hpp>

#define UT_source_tile_conflicts_test

#include "open_viii/graphics/background/SourceTileConflicts.hpp"
#include "open_viii/graphics/background/Tile1.hpp"

#include <ranges>
#include <vector>

int
  main()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  using namespace open_viii::graphics::background;

  [[maybe_unused]] suite tests = [] {
    "source_tile_conflicts default empty state"_test = [] {
      source_tile_conflicts conflicts{};

      const auto            occupied = conflicts.range_of_occupied_locations()
                                     | std::ranges::to<std::vector>();

      const auto conflict_range      = conflicts.range_of_conflicts_flattened();

      expect(occupied.empty());
      expect(conflict_range.empty());
    };

    "source_tile_conflicts tracks occupied tile"_test = [] {
      source_tile_conflicts conflicts{};

      Tile1                 tile{};

      tile = tile.with_texture_id(1).with_source_xy(16, 32);

      conflicts(tile).push_back(0);

      const auto occupied = conflicts.range_of_occupied_locations()
                          | std::ranges::to<std::vector>();

      expect(eq(occupied.size(), 1U));

      expect(eq(occupied.front().x, 16U));
      expect(eq(occupied.front().y, 32U));
      expect(eq(occupied.front().t, 1U));
    };

    "source_tile_conflicts detects conflicts"_test = [] {
      source_tile_conflicts conflicts{};

      Tile1                 tile{};

      tile = tile.with_texture_id(2).with_source_xy(32, 48);

      conflicts(tile).push_back(1);
      conflicts(tile).push_back(2);

      const auto flattened = conflicts.range_of_conflicts_flattened();

      expect(eq(flattened.size(), 2U));
      expect(eq(flattened[0], 1));
      expect(eq(flattened[1], 2));
    };

    "source_tile_conflicts separates texture pages"_test = [] {
      source_tile_conflicts conflicts{};

      Tile1                 a{};
      Tile1                 b{};

      a = a.with_texture_id(0).with_source_xy(16, 16);

      b = b.with_texture_id(1).with_source_xy(16, 16);

      conflicts(a).push_back(10);
      conflicts(b).push_back(20);

      const auto flattened = conflicts.range_of_conflicts_flattened();

      expect(flattened.empty());

      const auto occupied = conflicts.range_of_occupied_locations()
                          | std::ranges::to<std::vector>();

      expect(eq(occupied.size(), 2U));
    };

    "source_tile_conflicts non conflicts flattened"_test = [] {
      source_tile_conflicts conflicts{};

      Tile1                 a{};
      Tile1                 b{};

      a = a.with_texture_id(0).with_source_xy(0, 0);

      b = b.with_texture_id(0).with_source_xy(16, 0);

      conflicts(a).push_back(3);
      conflicts(b).push_back(4);

      const auto non_conflicts = conflicts.range_of_non_conflicts_flattened()
                               | std::ranges::to<std::vector>();

      expect(eq(non_conflicts.size(), 2U));
      expect(eq(non_conflicts[0], 3));
      expect(eq(non_conflicts[1], 4));
    };

    "source_tile_conflicts operator access by coordinates"_test = [] {
      source_tile_conflicts conflicts{};

      conflicts(16, 32, 1).push_back(99);

      expect(eq(conflicts(16, 32, 1).size(), 1U));
      expect(eq(conflicts(16, 32, 1).front(), 99));
    };

    "source_tile_conflicts multiple conflicts stay grouped"_test = [] {
      source_tile_conflicts conflicts{};

      Tile1                 tile{};

      tile = tile.with_texture_id(3).with_source_xy(48, 48);

      conflicts(tile).push_back(1);
      conflicts(tile).push_back(2);
      conflicts(tile).push_back(3);

      const auto grouped
        = conflicts.range_of_conflicts() | std::ranges::to<std::vector>();

      expect(eq(grouped.size(), 1U));
      expect(eq(grouped.front().size(), 3U));
    };

    "source_tile_conflicts empty locations shrink after insert"_test = [] {
      source_tile_conflicts conflicts{};

      const auto            before
        = conflicts.range_of_empty_locations() | std::ranges::to<std::vector>();

      Tile1 tile{};

      tile = tile.with_texture_id(0).with_source_xy(0, 0);

      conflicts(tile).push_back(1);

      const auto after
        = conflicts.range_of_empty_locations() | std::ranges::to<std::vector>();

      expect(eq(after.size() + 1U, before.size()));
    };

    "source_tile_conflicts calculate/reverse round trip"_test = [] {
      for (std::uint8_t t = 0; t < source_tile_conflicts::T_SIZE; ++t) {
        for (std::uint16_t y = 0; y < source_tile_conflicts::GRID_SIZE;
             y += source_tile_conflicts::Y_SIZE) {
          for (std::uint16_t x = 0; x < source_tile_conflicts::GRID_SIZE;
               x += source_tile_conflicts::X_SIZE) {
            const auto index = source_tile_conflicts::calculate_index(x, y, t);

            const auto location = source_tile_conflicts::reverse_index(index);

            expect(eq(location.x, x)) << "x mismatch for index " << index;

            expect(eq(location.y, y)) << "y mismatch for index " << index;

            expect(eq(location.t, t)) << "t mismatch for index " << index;
          }
        }
      }

      "source_tile_conflicts calculate_index produces unique indices"_test =
        [] {
          constexpr auto total_size = source_tile_conflicts::X_SIZE
                                    * source_tile_conflicts::Y_SIZE
                                    * source_tile_conflicts::T_SIZE;

          std::array<bool, total_size> seen{};

          std::size_t                  visited_count = 0U;

          for (std::uint8_t t = 0; t < source_tile_conflicts::T_SIZE; ++t) {
            for (std::uint16_t y = 0; y < source_tile_conflicts::GRID_SIZE;
                 y += source_tile_conflicts::Y_SIZE) {
              for (std::uint16_t x = 0; x < source_tile_conflicts::GRID_SIZE;
                   x += source_tile_conflicts::X_SIZE) {
                const auto index
                  = source_tile_conflicts::calculate_index(x, y, t);

                expect(index < total_size) << "index out of bounds: " << index;

                expect(!seen[index])
                  << "duplicate index detected: " << index << " from x=" << x
                  << " y=" << y << " t=" << static_cast<int>(t);

                seen[index] = true;
                ++visited_count;
              }
            }
          }

          expect(eq(visited_count, total_size));

          expect(std::ranges::all_of(seen, [](bool v) {
            return v;
          }));
        };
    };
  };
}