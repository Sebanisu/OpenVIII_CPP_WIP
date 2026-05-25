// NormalizedSourceTile.cpp

#include <boost/ut.hpp>

#include "open_viii/graphics/background/NormalizedSourceTile.hpp"
#include "open_viii/graphics/background/Tile1.hpp"

int
  main()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  using namespace open_viii::graphics::background;

  [[maybe_unused]] suite tests = [] {
    "normalized_source_tile default construction"_test = [] {
      normalized_source_tile tile{};

      expect(eq(tile.m_animation_state, 0U));
      expect(
        eq(tile.m_animation_id, (std::numeric_limits<std::uint8_t>::max)()));
    };

    "normalized_source_tile constructs from Tile1"_test = [] {
      Tile1 source{};

      source = source.with_texture_id(3)
                 .with_palette_id(7)
                 .with_layer_id(2)
                 .with_source_xy(32, 48)
                 .with_animation_id(5)
                 .with_animation_state(1)
                 .with_blend_mode(BlendModeT::add)
                 .with_blend(true)
                 .with_depth(4)
                 .with_draw(true);

      normalized_source_tile normalized{ source };

      expect(eq(normalized.m_tex_id_buffer.id(), 3U));
      expect(eq(normalized.m_palette_id.id(), 7U));
      expect(eq(normalized.m_layer_id.id(), 2U));
      expect(eq(normalized.m_source_xy.x(), 32U));
      expect(eq(normalized.m_source_xy.y(), 48U));
      expect(eq(normalized.m_animation_id, 5U));
      expect(eq(normalized.m_animation_state, 1U));
      expect(eq(normalized.m_blend_mode, BlendModeT::add));
    };

    "normalized_source_tile assignment from Tile1"_test = [] {
      Tile1 source{};

      source = source.with_texture_id(9)
                 .with_palette_id(4)
                 .with_layer_id(1)
                 .with_source_xy(16, 64);

      normalized_source_tile normalized{};

      normalized = source;

      expect(eq(normalized.m_tex_id_buffer.id(), 9U));
      expect(eq(normalized.m_palette_id.id(), 4U));
      expect(eq(normalized.m_layer_id.id(), 1U));
      expect(eq(normalized.m_source_xy.x(), 16U));
      expect(eq(normalized.m_source_xy.y(), 64U));
    };

    "normalized_source_tile equality"_test = [] {
      Tile1 source{};

      source = source.with_texture_id(1)
                 .with_palette_id(2)
                 .with_layer_id(3)
                 .with_source_xy(16, 32);

      normalized_source_tile a{ source };
      normalized_source_tile b{ source };

      expect(eq(a, b));
    };

    "normalized_source_tile inequality"_test = [] {
      Tile1 lhs{};
      Tile1 rhs{};

      lhs = lhs.with_texture_id(1);
      rhs = rhs.with_texture_id(2);

      normalized_source_tile a{ lhs };
      normalized_source_tile b{ rhs };

      expect(neq(a, b));
    };

    "normalized_source_animated_tile strips blend"_test = [] {
      Tile1 source{};

      source
        = source.with_texture_id(5).with_blend(true).with_depth(2).with_draw(
          true);

      normalized_source_animated_tile animated{ source };

      expect(eq(animated.m_tex_id_buffer.blend(), false));
      expect(eq(animated.m_tex_id_buffer.id(), 5U));
      expect(eq(animated.m_tex_id_buffer.depth(), 2U));
      expect(eq(animated.m_tex_id_buffer.draw(), true));
    };

    "normalized_source_animated_tile preserves palette when not animated"_test
      = [] {
          Tile1 source{};

          source = source.with_palette_id(9);

          normalized_source_animated_tile animated{ source };

          expect(eq(animated.m_palette_id.id(), 9U));
        };

    "normalized_source_animated_tile masks palette when animated"_test = [] {
      Tile1 source{};

      source = source.with_palette_id(4).with_animation_id(2);

      normalized_source_animated_tile animated{ source };

      expect(eq(
        animated.m_palette_id.id(),
        (std::numeric_limits<std::uint8_t>::max)()));
    };

    "normalized_source_animated_tile equality ignores blend"_test = [] {
      Tile1 lhs{};
      Tile1 rhs{};

      lhs = lhs.with_texture_id(1).with_blend(true).with_source_xy(16, 16);

      rhs = rhs.with_texture_id(1).with_blend(false).with_source_xy(16, 16);

      normalized_source_animated_tile a{ lhs };
      normalized_source_animated_tile b{ rhs };

      expect(eq(a, b));
    };

    "normalized_source_animated_tile assignment"_test = [] {
      Tile1 source{};

      source = source.with_texture_id(7)
                 .with_palette_id(3)
                 .with_layer_id(6)
                 .with_source_xy(48, 80);

      normalized_source_animated_tile animated{};

      animated = source;

      expect(eq(animated.m_tex_id_buffer.id(), 7U));
      expect(eq(animated.m_palette_id.id(), 3U));
      expect(eq(animated.m_layer_id.id(), 6U));
      expect(eq(animated.m_source_xy.x(), 48U));
      expect(eq(animated.m_source_xy.y(), 80U));
    };
  };
}