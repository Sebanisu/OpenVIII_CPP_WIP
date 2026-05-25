// SourceTileConflicts.cpp

#include <boost/ut.hpp>

#define UT_SourceTileConflicts_test

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

  [[maybe_unused]] suite<"SourceTileConflicts"> tests = [] {
    "SourceTileConflicts default empty state"_test = [] {
      SourceTileConflicts conflicts{};

      const auto          occupied = conflicts.range_of_occupied_locations()
                                   | std::ranges::to<std::vector>();

      const auto conflict_range    = conflicts.range_of_conflicts_flattened();

      expect(occupied.empty());
      expect(conflict_range.empty());
    };

    "SourceTileConflicts tracks occupied tile"_test = [] {
      SourceTileConflicts conflicts{};

      Tile1               tile{};

      tile = tile.with_texture_id(1).with_source_xy(16, 32);

      conflicts(tile).push_back(0);

      const auto occupied = conflicts.range_of_occupied_locations()
                          | std::ranges::to<std::vector>();

      expect(eq(occupied.size(), 1U));

      expect(eq(occupied.front().x, 16U));
      expect(eq(occupied.front().y, 32U));
      expect(eq(occupied.front().t, 1U));
    };

    "SourceTileConflicts detects conflicts"_test = [] {
      SourceTileConflicts conflicts{};

      Tile1               tile{};

      tile = tile.with_texture_id(2).with_source_xy(32, 48);

      conflicts(tile).push_back(1);
      conflicts(tile).push_back(2);

      const auto flattened = conflicts.range_of_conflicts_flattened();

      expect(eq(flattened.size(), 2U));
      expect(eq(flattened[0], 1));
      expect(eq(flattened[1], 2));
    };

    "SourceTileConflicts separates texture pages"_test = [] {
      SourceTileConflicts conflicts{};

      Tile1               a{};
      Tile1               b{};

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

    "SourceTileConflicts non conflicts flattened"_test = [] {
      SourceTileConflicts conflicts{};

      Tile1               a{};
      Tile1               b{};

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

    "SourceTileConflicts operator access by coordinates"_test = [] {
      SourceTileConflicts conflicts{};

      conflicts(16, 32, 1).push_back(99);

      expect(eq(conflicts(16, 32, 1).size(), 1U));
      expect(eq(conflicts(16, 32, 1).front(), 99));
    };

    "SourceTileConflicts multiple conflicts stay grouped"_test = [] {
      SourceTileConflicts conflicts{};

      Tile1               tile{};

      tile = tile.with_texture_id(3).with_source_xy(48, 48);

      conflicts(tile).push_back(1);
      conflicts(tile).push_back(2);
      conflicts(tile).push_back(3);

      const auto grouped
        = conflicts.range_of_conflicts() | std::ranges::to<std::vector>();

      expect(eq(grouped.size(), 1U));
      expect(eq(grouped.front().size(), 3U));
    };

    "SourceTileConflicts empty locations shrink after insert"_test = [] {
      SourceTileConflicts conflicts{};

      const auto          before
        = conflicts.range_of_empty_locations() | std::ranges::to<std::vector>();

      Tile1 tile{};

      tile = tile.with_texture_id(0).with_source_xy(0, 0);

      conflicts(tile).push_back(1);

      const auto after
        = conflicts.range_of_empty_locations() | std::ranges::to<std::vector>();

      expect(eq(after.size() + 1U, before.size()));
    };

    "SourceTileConflicts calculate/reverse round trip"_test = [] {
      for (std::uint8_t t = 0; t < SourceTileConflicts::T_SIZE; ++t) {
        for (std::uint16_t y = 0; y < SourceTileConflicts::GRID_SIZE;
             y += SourceTileConflicts::Y_SIZE) {
          for (std::uint16_t x = 0; x < SourceTileConflicts::GRID_SIZE;
               x += SourceTileConflicts::X_SIZE) {
            const auto index    = SourceTileConflicts::calculate_index(x, y, t);

            const auto location = SourceTileConflicts::reverse_index(index);

            expect(eq(location.x, x)) << "x mismatch for index " << index;

            expect(eq(location.y, y)) << "y mismatch for index " << index;

            expect(eq(location.t, t)) << "t mismatch for index " << index;
          }
        }
      }

      "SourceTileConflicts calculate_index produces unique indices"_test = [] {
        constexpr auto               total_size = SourceTileConflicts::X_SIZE
                                                * SourceTileConflicts::Y_SIZE
                                                * SourceTileConflicts::T_SIZE;

        std::array<bool, total_size> seen{};

        std::size_t                  visited_count = 0U;

        for (std::uint8_t t = 0; t < SourceTileConflicts::T_SIZE; ++t) {
          for (std::uint16_t y = 0; y < SourceTileConflicts::GRID_SIZE;
               y += SourceTileConflicts::Y_SIZE) {
            for (std::uint16_t x = 0; x < SourceTileConflicts::GRID_SIZE;
                 x += SourceTileConflicts::X_SIZE) {
              const auto index = SourceTileConflicts::calculate_index(x, y, t);

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