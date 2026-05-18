// UniquifyPupu.cpp

#include <boost/ut.hpp>

#include "open_viii/graphics/background/Tile1.hpp"
#include "open_viii/graphics/background/UniquifyPupu.hpp"

int
  main()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  using namespace open_viii::graphics::background;

  [[maybe_unused]] suite tests = [] {
    "UniquifyPupu increments duplicate ids"_test = [] {
      UniquifyPupu uniquify{};

      Tile1        tile{};

      tile              = tile.with_xy(0, 0);

      const auto first  = uniquify(tile);
      const auto second = uniquify(tile);
      const auto third  = uniquify(tile);

      expect(eq(second.raw(), first.raw() + 1U));
      expect(eq(third.raw(), second.raw() + 1U));
    };

    "UniquifyPupu keeps different buckets separate"_test = [] {
      UniquifyPupu uniquify{};

      Tile1        a{};
      Tile1        b{};

      // Different 16x16 bucket
      a               = a.with_xy(0, 0);

      b               = b.with_xy(16, 0);

      const auto id_a = uniquify(a);
      const auto id_b = uniquify(b);

      expect(eq(id_a.raw(), id_b.raw()));
    };

    "UniquifyPupu increments inside same bucket"_test = [] {
      UniquifyPupu uniquify{};

      Tile1        a{};
      Tile1        b{};

      // Same 16x16 bucket

      a                 = a.with_xy(0, 0);

      b                 = b.with_xy(15, 15);

      const auto first  = uniquify(a);
      const auto second = uniquify(b);

      expect(neq(first, second));
      expect(second.is_x_not_aligned_to_grid());
      expect(second.is_y_not_aligned_to_grid());
    };

    "UniquifyPupu different base ids do not collide"_test = [] {
      UniquifyPupu uniquify{};

      Tile1        a{};
      Tile1        b{};

      a               = a.with_layer_id(0);
      b               = b.with_layer_id(1);

      const auto id_a = uniquify(a);
      const auto id_b = uniquify(b);

      expect(neq(id_a, id_b));
    };

    "UniquifyPupu supports repeated calls"_test = [] {
      UniquifyPupu uniquify{};

      Tile1        tile{};

      tile          = tile.with_xy(32, 48);

      auto previous = uniquify(tile);

      for (std::uint32_t i = 1; i != 10; ++i) {
        const auto current = uniquify(tile);

        expect(eq(current.raw(), previous.raw() + 1U));

        previous = current;
      }
    };
  };
}