// TileOperations.cpp

#include "open_viii/graphics/background/TileOperations.hpp"
#include "open_viii/graphics/background/Map.hpp"

#include <boost/ut.hpp>
#include <ranges>

int
  main()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  using namespace open_viii::graphics::background;
  using namespace open_viii::graphics::background::tile_operations;

  [[maybe_unused]] suite<"TileOperations Tile1"> tests1 = [] {
    "X getter extracts x coordinate Tile1"_test = [] {
      constexpr auto tile  = Tile1{}.with_x(123);

      constexpr auto value = X{}(tile);

      expect(eq(value, 123));
    };

    "Y getter extracts y coordinate Tile1"_test = [] {
      constexpr auto tile  = Tile1{}.with_y(456);

      constexpr auto value = Y{}(tile);

      expect(eq(value, 456));
    };

    "XMatch matches tile x Tile1"_test = [] {
      constexpr auto tile = Tile1{}.with_x(321);

      expect(XMatch{ 321 }(tile));
      expect(!XMatch{ 111 }(tile));
    };

    "YMatch matches tile y Tile1"_test = [] {
      constexpr auto tile = Tile1{}.with_y(654);

      expect(YMatch{ 654 }(tile));
      expect(!YMatch{ 999 }(tile));
    };

    "WithX transforms tile x Tile1"_test = [] {
      constexpr auto tile    = Tile1{}.with_x(10);

      auto           updated = WithX<Tile1>{ 42 }(tile);

      expect(eq(updated.x(), 42));
      expect(eq(updated.y(), tile.y()));
    };

    "WithY transforms tile y Tile1"_test = [] {
      constexpr auto tile    = Tile1{}.with_y(10);

      auto           updated = WithY<Tile1>{ 77 }(tile);

      expect(eq(updated.y(), 77));
      expect(eq(updated.x(), tile.x()));
    };

    "pipe operator applies transform Tile1"_test = [] {
      constexpr auto tile    = Tile1{}.with_x(1);

      auto           updated = tile | WithX<Tile1>{ 999 };

      expect(eq(updated.x(), 999));
    };

    "TranslateWithX offsets x coordinate Tile1"_test = [] {
      constexpr auto tile       = Tile1{}.with_x(100);

      auto           translated = TranslateWithX<Tile1>{ 100, 200 }(tile);

      expect(eq(translated.x(), 200));
    };

    "TranslateWithY offsets y coordinate Tile1"_test = [] {
      constexpr auto tile       = Tile1{}.with_y(50);

      auto           translated = TranslateWithY<Tile1>{ 50, 75 }(tile);

      expect(eq(translated.y(), 75));
    };

    "NotInvalidTile accepts normal tiles Tile1"_test = [] {
      constexpr auto tile = Tile1{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles Tile1"_test = [] {
      constexpr auto tile = Tile1{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };

    "XGroup stores current value Tile1"_test = [] {
      constexpr XGroup<Tile1> group{ 123 };

      expect(eq(group.current, 123));
    };

    "XGroup constructs from tile Tile1"_test = [] {
      constexpr auto          tile = Tile1{}.with_x(789);

      constexpr XGroup<Tile1> group{ tile };

      expect(eq(group.current, 789));
    };

    "XGroup exposes min/max values Tile1"_test = [] {
      expect(eq(
        XGroup<Tile1>::min_value,
        (std::numeric_limits<XGroup<Tile1>::value_type>::min)()));
      expect(XGroup<Tile1>::max_value >= XGroup<Tile1>::min_value);
    };

    "filter invalid tiles with ranges Tile1"_test = [] {
      constexpr std::array<Tile1, 3> tiles{
        Tile1{}.with_x(1),
        Tile1{}.with_x(0x7FFF),
        Tile1{}.with_x(2),
      };

      auto filtered = tiles | std::views::filter(NotInvalidTile{});

      auto it       = filtered.begin();

      expect(eq(it->x(), 1));
      ++it;
      expect(eq(it->x(), 2));
    };

    "NotInvalidTile accepts normal tiles Tile1"_test = [] {
      constexpr auto tile = Tile1{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles Tile1"_test = [] {
      constexpr auto tile = Tile1{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };
  };

  [[maybe_unused]] suite<"TileOperations Tile2"> tests2 = [] {
    "X getter extracts x coordinate Tile2"_test = [] {
      constexpr auto tile  = Tile2{}.with_x(123);

      constexpr auto value = X{}(tile);

      expect(eq(value, 123));
    };

    "Y getter extracts y coordinate Tile2"_test = [] {
      constexpr auto tile  = Tile2{}.with_y(456);

      constexpr auto value = Y{}(tile);

      expect(eq(value, 456));
    };

    "XMatch matches tile x Tile2"_test = [] {
      constexpr auto tile = Tile2{}.with_x(321);

      expect(XMatch{ 321 }(tile));
      expect(!XMatch{ 111 }(tile));
    };

    "YMatch matches tile y Tile2"_test = [] {
      constexpr auto tile = Tile2{}.with_y(654);

      expect(YMatch{ 654 }(tile));
      expect(!YMatch{ 999 }(tile));
    };

    "WithX transforms tile x Tile2"_test = [] {
      constexpr auto tile    = Tile2{}.with_x(10);

      auto           updated = WithX<Tile2>{ 42 }(tile);

      expect(eq(updated.x(), 42));
      expect(eq(updated.y(), tile.y()));
    };

    "WithY transforms tile y Tile2"_test = [] {
      constexpr auto tile    = Tile2{}.with_y(10);

      auto           updated = WithY<Tile2>{ 77 }(tile);

      expect(eq(updated.y(), 77));
      expect(eq(updated.x(), tile.x()));
    };

    "pipe operator applies transform Tile2"_test = [] {
      constexpr auto tile    = Tile2{}.with_x(1);

      auto           updated = tile | WithX<Tile2>{ 999 };

      expect(eq(updated.x(), 999));
    };

    "TranslateWithX offsets x coordinate Tile2"_test = [] {
      constexpr auto tile       = Tile2{}.with_x(100);

      auto           translated = TranslateWithX<Tile2>{ 100, 200 }(tile);

      expect(eq(translated.x(), 200));
    };

    "TranslateWithY offsets y coordinate Tile2"_test = [] {
      constexpr auto tile       = Tile2{}.with_y(50);

      auto           translated = TranslateWithY<Tile2>{ 50, 75 }(tile);

      expect(eq(translated.y(), 75));
    };

    "NotInvalidTile accepts normal tiles Tile2"_test = [] {
      constexpr auto tile = Tile2{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles Tile2"_test = [] {
      constexpr auto tile = Tile2{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };

    "XGroup stores current value Tile2"_test = [] {
      constexpr XGroup<Tile2> group{ 123 };

      expect(eq(group.current, 123));
    };

    "XGroup constructs from tile Tile2"_test = [] {
      constexpr auto          tile = Tile2{}.with_x(789);

      constexpr XGroup<Tile2> group{ tile };

      expect(eq(group.current, 789));
    };

    "XGroup exposes min/max values Tile2"_test = [] {
      expect(eq(
        XGroup<Tile2>::min_value,
        (std::numeric_limits<XGroup<Tile2>::value_type>::min)()));
      expect(XGroup<Tile2>::max_value >= XGroup<Tile2>::min_value);
    };

    "filter invalid tiles with ranges Tile2"_test = [] {
      constexpr std::array<Tile2, 3> tiles{
        Tile2{}.with_x(1),
        Tile2{}.with_x(0x7FFF),
        Tile2{}.with_x(2),
      };

      auto filtered = tiles | std::views::filter(NotInvalidTile{});

      auto it       = filtered.begin();

      expect(eq(it->x(), 1));
      ++it;
      expect(eq(it->x(), 2));
    };

    "NotInvalidTile accepts normal tiles Tile2"_test = [] {
      constexpr auto tile = Tile2{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles Tile2"_test = [] {
      constexpr auto tile = Tile2{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };
  };

  [[maybe_unused]] suite<"TileOperations Tile3"> tests3 = [] {
    "X getter extracts x coordinate Tile3"_test = [] {
      constexpr auto tile  = Tile3{}.with_x(123);

      constexpr auto value = X{}(tile);

      expect(eq(value, 123));
    };

    "Y getter extracts y coordinate Tile3"_test = [] {
      constexpr auto tile  = Tile3{}.with_y(456);

      constexpr auto value = Y{}(tile);

      expect(eq(value, 456));
    };

    "XMatch matches tile x Tile3"_test = [] {
      constexpr auto tile = Tile3{}.with_x(321);

      expect(XMatch{ 321 }(tile));
      expect(!XMatch{ 111 }(tile));
    };

    "YMatch matches tile y Tile3"_test = [] {
      constexpr auto tile = Tile3{}.with_y(654);

      expect(YMatch{ 654 }(tile));
      expect(!YMatch{ 999 }(tile));
    };

    "WithX transforms tile x Tile3"_test = [] {
      constexpr auto tile    = Tile3{}.with_x(10);

      auto           updated = WithX<Tile3>{ 42 }(tile);

      expect(eq(updated.x(), 42));
      expect(eq(updated.y(), tile.y()));
    };

    "WithY transforms tile y Tile3"_test = [] {
      constexpr auto tile    = Tile3{}.with_y(10);

      auto           updated = WithY<Tile3>{ 77 }(tile);

      expect(eq(updated.y(), 77));
      expect(eq(updated.x(), tile.x()));
    };

    "pipe operator applies transform Tile3"_test = [] {
      constexpr auto tile    = Tile3{}.with_x(1);

      auto           updated = tile | WithX<Tile3>{ 999 };

      expect(eq(updated.x(), 999));
    };

    "TranslateWithX offsets x coordinate Tile3"_test = [] {
      constexpr auto tile       = Tile3{}.with_x(100);

      auto           translated = TranslateWithX<Tile3>{ 100, 200 }(tile);

      expect(eq(translated.x(), 200));
    };

    "TranslateWithY offsets y coordinate Tile3"_test = [] {
      constexpr auto tile       = Tile3{}.with_y(50);

      auto           translated = TranslateWithY<Tile3>{ 50, 75 }(tile);

      expect(eq(translated.y(), 75));
    };

    "NotInvalidTile accepts normal tiles Tile3"_test = [] {
      constexpr auto tile = Tile3{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles Tile3"_test = [] {
      constexpr auto tile = Tile3{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };

    "XGroup stores current value Tile3"_test = [] {
      constexpr XGroup<Tile3> group{ 123 };

      expect(eq(group.current, 123));
    };

    "XGroup constructs from tile Tile3"_test = [] {
      constexpr auto          tile = Tile3{}.with_x(789);

      constexpr XGroup<Tile3> group{ tile };

      expect(eq(group.current, 789));
    };

    "XGroup exposes min/max values Tile3"_test = [] {
      expect(eq(
        XGroup<Tile3>::min_value,
        (std::numeric_limits<XGroup<Tile3>::value_type>::min)()));
      expect(XGroup<Tile3>::max_value >= XGroup<Tile3>::min_value);
    };

    "filter invalid tiles with ranges Tile3"_test = [] {
      constexpr std::array<Tile3, 3> tiles{
        Tile3{}.with_x(1),
        Tile3{}.with_x(0x7FFF),
        Tile3{}.with_x(2),
      };

      auto filtered = tiles | std::views::filter(NotInvalidTile{});

      auto it       = filtered.begin();

      expect(eq(it->x(), 1));
      ++it;
      expect(eq(it->x(), 2));
    };


    "NotInvalidTile accepts normal tiles Tile3"_test = [] {
      constexpr auto tile = Tile3{}.with_x(0);

      expect(NotInvalidTile{}(tile));
    };

    "NotInvalidTile rejects sentinel tiles Tile3"_test = [] {
      constexpr auto tile = Tile3{}.with_x(0x7FFF);

      expect(!NotInvalidTile{}(tile));
    };
  };
}