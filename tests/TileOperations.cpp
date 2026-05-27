// TileOperations.cpp

#include "open_viii/graphics/background/TileOperations.hpp"
#include "open_viii/graphics/background/Map.hpp"

#include <boost/ut.hpp>
#include <ranges>

using namespace open_viii::graphics::background;
using namespace open_viii::graphics::background::tile_operations;
template<is_tile TileT, boost::ut::fixed_string suite_title>
void
  run_tile_operations_tests()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  [[maybe_unused]] suite<suite_title> tests1 = [] {
    "X getter extracts x coordinate"_test = [] {
      constexpr auto tile  = TileT{}.with_x(123);

      constexpr auto value = X{}(tile);

      expect(eq(value, 123));
    };

    "Y getter extracts y coordinate"_test = [] {
      constexpr auto tile  = TileT{}.with_y(456);

      constexpr auto value = Y{}(tile);

      expect(eq(value, 456));
    };

    "XMatch matches tile x"_test = [] {
      constexpr auto tile = TileT{}.with_x(321);

      expect(XMatch{ 321 }(tile));
      expect(!XMatch{ 111 }(tile));
    };

    "YMatch matches tile y"_test = [] {
      constexpr auto tile = TileT{}.with_y(654);

      expect(YMatch{ 654 }(tile));
      expect(!YMatch{ 999 }(tile));
    };

    "WithX transforms tile x"_test = [] {
      constexpr auto tile    = TileT{}.with_x(10);

      auto           updated = WithX<TileT>{ 42 }(tile);

      expect(eq(updated.x(), 42));
      expect(eq(updated.y(), tile.y()));
    };

    "WithY transforms tile y"_test = [] {
      constexpr auto tile    = TileT{}.with_y(10);

      auto           updated = WithY<TileT>{ 77 }(tile);

      expect(eq(updated.y(), 77));
      expect(eq(updated.x(), tile.x()));
    };

    "pipe operator applies transform"_test = [] {
      constexpr auto tile    = TileT{}.with_x(1);

      auto           updated = tile | WithX<TileT>{ 999 };

      expect(eq(updated.x(), 999));
    };

    "TranslateWithX offsets x coordinate"_test = [] {
      constexpr auto tile       = TileT{}.with_x(100);

      auto           translated = TranslateWithX<TileT>{ 100, 200 }(tile);

      expect(eq(translated.x(), 200));
    };

    "TranslateWithY offsets y coordinate"_test = [] {
      constexpr auto tile       = TileT{}.with_y(50);

      auto           translated = TranslateWithY<TileT>{ 50, 75 }(tile);

      expect(eq(translated.y(), 75));
    };

    "NotInvalidTile accepts normal tiles"_test = [] {
      constexpr auto tile = TileT{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles"_test = [] {
      constexpr auto tile = TileT{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };

    "XGroup stores current value"_test = [] {
      constexpr XGroup<TileT> group{ 123 };

      expect(eq(group.current, 123));
    };

    "XGroup constructs from tile"_test = [] {
      constexpr auto          tile = TileT{}.with_x(789);

      constexpr XGroup<TileT> group{ tile };

      expect(eq(group.current, 789));
    };

    "XGroup exposes min/max values"_test = [] {
      expect(eq(
        XGroup<TileT>::min_value,
        (std::numeric_limits<typename XGroup<TileT>::value_type>::min)()));
      expect(XGroup<TileT>::max_value >= XGroup<TileT>::min_value);
    };

    "filter invalid tiles with ranges"_test = [] {
      constexpr std::array<TileT, 3> tiles{
        TileT{}.with_x(1),
        TileT{}.with_x(0x7FFF),
        TileT{}.with_x(2),
      };

      auto filtered = tiles | std::views::filter(NotInvalidTile{});

      auto it       = filtered.begin();

      expect(eq(it->x(), 1));
      ++it;
      expect(eq(it->x(), 2));
    };

    "NotInvalidTile accepts normal tiles"_test = [] {
      constexpr auto tile = TileT{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles"_test = [] {
      constexpr auto tile = TileT{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };
  };
}

int
  main()
{
  run_tile_operations_tests<Tile1, "Tile1 TileOperations">();
  run_tile_operations_tests<Tile2, "Tile2 TileOperations">();
  run_tile_operations_tests<Tile3, "Tile3 TileOperations">();
}