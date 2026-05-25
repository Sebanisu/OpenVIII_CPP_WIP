// PupuID.cpp

#include "open_viii/graphics/background/PupuID.hpp"
#include "open_viii/graphics/background/BlendModeT.hpp"
#include <boost/ut.hpp>
#include <fmt/format.h>
#include <limits>
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

    "hsv2rgb produces red"_test = [] {
      constexpr auto rgb = PupuID::hsv2rgb(0.0f, 1.0f, 1.0f);

      expect(eq(static_cast<int>(rgb[0] * 255.0f), 255));
      expect(eq(static_cast<int>(rgb[1] * 255.0f), 0));
      expect(eq(static_cast<int>(rgb[2] * 255.0f), 0));
    };

    "hsv2rgb produces green"_test = [] {
      constexpr auto rgb = PupuID::hsv2rgb(1.0f / 3.0f, 1.0f, 1.0f);

      expect(eq(static_cast<int>(rgb[0] * 255.0f), 0));
      expect(eq(static_cast<int>(rgb[1] * 255.0f), 255));
      expect(eq(static_cast<int>(rgb[2] * 255.0f), 0));
    };

    "hsv2rgb produces blue"_test = [] {
      constexpr auto rgb = PupuID::hsv2rgb(2.0f / 3.0f, 1.0f, 1.0f);

      expect(eq(static_cast<int>(rgb[0] * 255.0f), 0));
      expect(eq(static_cast<int>(rgb[1] * 255.0f), 0));
      expect(eq(static_cast<int>(rgb[2] * 255.0f), 255));
    };

    "fract removes integer portion"_test = [] {
      expect(eq(PupuID::fract(1.25f), 0.25f));
      expect(eq(PupuID::fract(5.75f), 0.75f));
    };

    "clamp constrains values"_test = [] {
      expect(eq(PupuID::clamp(-1.0f, 0.0f, 1.0f), 0.0f));
      expect(eq(PupuID::clamp(0.5f, 0.0f, 1.0f), 0.5f));
      expect(eq(PupuID::clamp(2.0f, 0.0f, 1.0f), 1.0f));
    };

    "mix interpolates values"_test = [] {
      expect(eq(PupuID::mix(0.0f, 10.0f, 0.0f), 0.0f));
      expect(eq(PupuID::mix(0.0f, 10.0f, 0.5f), 5.0f));
      expect(eq(PupuID::mix(0.0f, 10.0f, 1.0f), 10.0f));
    };

    "encode_uint_to_rgba is deterministic"_test = [] {
      constexpr auto a = PupuID::encode_uint_to_rgba(123u);
      constexpr auto b = PupuID::encode_uint_to_rgba(123u);

      expect(eq(a.r(), b.r()));
      expect(eq(a.g(), b.g()));
      expect(eq(a.b(), b.b()));
      expect(eq(a.a(), b.a()));
    };

    "encode_uint_to_rgba produces opaque alpha"_test = [] {
      constexpr auto color = PupuID::encode_uint_to_rgba(42u);

      expect(eq(color.a(), std::uint8_t{ 255u }));
    };

    "encode_uint_to_rgba differentiates nearby ids"_test = [] {
      constexpr auto a     = PupuID::encode_uint_to_rgba(100u);
      constexpr auto b     = PupuID::encode_uint_to_rgba(101u);

      const bool different = a.r() != b.r() || a.g() != b.g() || a.b() != b.b();

      expect(different);
    };
  };
}