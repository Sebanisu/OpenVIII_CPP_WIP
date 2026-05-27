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

      const auto     updated = WithX<TileT>{ 42 }(tile);

      expect(eq(updated.x(), 42));
      expect(eq(updated.y(), tile.y()));
    };

    "WithY transforms tile y"_test = [] {
      constexpr auto tile    = TileT{}.with_y(10);

      const auto     updated = WithY<TileT>{ 77 }(tile);

      expect(eq(updated.y(), 77));
      expect(eq(updated.x(), tile.x()));
    };

    "pipe operator applies transform"_test = [] {
      constexpr auto tile    = TileT{}.with_x(1);

      const auto     updated = tile | WithX<TileT>{ 999 };

      expect(eq(updated.x(), 999));
    };

    "TranslateWithX offsets x coordinate"_test = [] {
      constexpr auto tile       = TileT{}.with_x(100);

      const auto     translated = TranslateWithX<TileT>{ 100, 200 }(tile);

      expect(eq(translated.x(), 200));
    };

    "TranslateWithY offsets y coordinate"_test = [] {
      constexpr auto tile       = TileT{}.with_y(50);

      const auto     translated = TranslateWithY<TileT>{ 50, 75 }(tile);

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

    "XY getter extracts packed coordinates"_test = [] {
      constexpr auto tile = TileT{}.with_x(12).with_y(34);

      const auto     xy   = XY{}(tile);

      expect(eq(xy.x(), 12));
      expect(eq(xy.y(), 34));
    };

    "Z getter extracts z value"_test = [] {
      constexpr auto tile = TileT{}.with_z(55);

      expect(eq(Z{}(tile), 55));
    };

    "SourceX transform updates source x"_test = [] {
      constexpr auto tile    = TileT{}.with_source_x(10);

      const auto     updated = WithSourceX<TileT>{ 99 }(tile);

      expect(eq(updated.source_x(), 99));
    };

    "SourceY match compares source y"_test = [] {
      constexpr auto tile = TileT{}.with_source_y(44);

      expect(SourceYMatch{ 44 }(tile));
      expect(!SourceYMatch{ 12 }(tile));
    };

    "SourceXY getter extracts packed source coordinates"_test = [] {
      constexpr auto tile = TileT{}.with_source_x(7).with_source_y(9);

      const auto     xy   = SourceXY{}(tile);

      expect(eq(xy.x(), 7));
      expect(eq(xy.y(), 9));
    };

    "TextureId transform updates texture id"_test = [] {
      constexpr auto tile    = TileT{}.with_texture_id(1);

      const auto     updated = WithTextureId<TileT>{ 5 }(tile);

      expect(eq(updated.texture_id(), 5));
    };

    if constexpr (has_with_blend_mode<TileT>) {
      "BlendMode transform updates blend mode"_test = [] {
        using blend_mode_type = tile_operations::BlendModeT<TileT>;

        constexpr auto tile   = TileT{}.with_blend_mode(blend_mode_type::add);

        const auto     updated
          = WithBlendMode<TileT>{ blend_mode_type::quarter_add }(tile);

        expect(eq(updated.blend_mode(), blend_mode_type::quarter_add));
      };
    }

    "Blend predicate matches blend flag"_test = [] {
      constexpr auto tile = TileT{}.with_blend(true);

      expect(BlendMatch{ true }(tile));
      expect(!BlendMatch{ false }(tile));
    };

    "Draw transform updates draw flag"_test = [] {
      constexpr auto tile    = TileT{}.with_draw(false);

      const auto     updated = WithDraw<TileT>{ true }(tile);

      expect(updated.draw());
    };

    "Depth getter extracts depth"_test = [] {
      using namespace open_viii::graphics::literals;
      constexpr auto tile = TileT{}.with_depth(4_bpp);

      expect(eq(Depth{}(tile), 4_bpp));
    };

    if constexpr (has_with_layer_id<TileT>) {
      "LayerId transform updates layer id"_test = [] {
        using layer_id_type    = LayerIdT<TileT>;

        constexpr auto tile    = TileT{}.with_layer_id(layer_id_type{ 1 });

        auto           updated = WithLayerId<TileT>{ layer_id_type{ 9 } }(tile);

        expect(eq(updated.layer_id(), layer_id_type{ 9 }));
      };
    }

    if constexpr (has_with_palette_id<TileT>) {
      "PaletteId transform updates palette id"_test = [] {
        using value_type       = PaletteIdT<TileT>;

        constexpr auto tile    = TileT{}.with_palette_id(value_type{ 2 });

        const auto     updated = WithPaletteId<TileT>{ value_type{ 7 } }(tile);

        expect(eq(updated.palette_id(), value_type{ 7 }));
      };
    }

    if constexpr (has_with_animation_id<TileT>) {
      "AnimationId transform updates animation id"_test = [] {
        using value_type    = AnimationIdT<TileT>;

        constexpr auto tile = TileT{}.with_animation_id(value_type{ 3 });

        const auto updated  = WithAnimationId<TileT>{ value_type{ 8 } }(tile);

        expect(eq(updated.animation_id(), value_type{ 8 }));
      };
    }

    if constexpr (has_with_animation_state<TileT>) {
      "AnimationState transform updates animation state"_test = [] {
        using value_type    = AnimationStateT<TileT>;

        constexpr auto tile = TileT{}.with_animation_state(value_type{ 1 });

        const auto updated = WithAnimationState<TileT>{ value_type{ 4 } }(tile);

        expect(eq(updated.animation_state(), value_type{ 4 }));
      };
    }
  };
}

int
  main()
{
  run_tile_operations_tests<Tile1, "Tile1 TileOperations">();
  run_tile_operations_tests<Tile2, "Tile2 TileOperations">();
  run_tile_operations_tests<Tile3, "Tile3 TileOperations">();
}