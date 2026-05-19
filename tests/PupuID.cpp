// PupuID.cpp

#include "open_viii/graphics/background/PupuID.hpp"
#include "open_viii/graphics/background/BlendModeT.hpp"
#include <boost/ut.hpp>
#include <fmt/format.h>
#include <string>

int
  main()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  using namespace open_viii::graphics::background;

  [[maybe_unused]] suite tests = [] {
    "PupuID raw constructor"_test = [] {
      constexpr std::uint32_t raw = 0x1234ABCDu;

      const PupuID            id{ raw };

      expect(eq(id.raw(), raw));
    };

    "PupuID formatter outputs uppercase hex"_test = [] {
      const PupuID id{ 0x1234ABCDu };
      using namespace std::string_literals;
      expect(eq(fmt::format("{}", id), "1234ABCD"s));
    };

    "PupuID offset arithmetic"_test = [] {
      const PupuID base{ 0x1000u };

      const auto   next = base + 1U;

      expect(eq(next.raw(), base.raw() + 1U));
    };

    "PupuID equality"_test = [] {
      const PupuID a{ 0xCAFEBABEu };
      const PupuID b{ 0xCAFEBABEu };
      const PupuID c{ 0xDEADBEEFu };

      expect(eq(a, b));
      expect(neq(a, c));
    };

    "PupuID field extraction"_test = [] {
      constexpr std::uint32_t x_not_aligned = 1u << 31u;
      constexpr std::uint32_t layer         = 5u << 24u;
      constexpr std::uint32_t y_not_aligned = 1u << 23u;
      constexpr std::uint32_t blend
        = static_cast<std::uint32_t>(BlendModeT::half_add) << 20u;
      constexpr std::uint32_t animation_id    = 12u << 12u;
      constexpr std::uint32_t animation_state = 34u << 4u;
      constexpr std::uint32_t offset          = 7u;

      constexpr std::uint32_t raw = x_not_aligned | layer | y_not_aligned
                                  | blend | animation_id | animation_state
                                  | offset;

      const PupuID            id{ raw };

      expect(eq(id.layer_id(), 5U));
      expect(eq(
        static_cast<std::uint32_t>(id.blend_mode()),
        static_cast<std::uint32_t>(BlendModeT::half_add)));
      expect(eq(id.animation_id(), 12U));
      expect(eq(id.animation_state(), 34U));
      expect(eq(id.offset(), 7U));

      expect(id.is_x_not_aligned_to_grid());
      expect(id.is_y_not_aligned_to_grid());
    };

    "PupuID summary is not empty"_test = [] {
      const PupuID id{ 0x12345678u };

      expect(!id.create_summary().empty());
    };
  };
}