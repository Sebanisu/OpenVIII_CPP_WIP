#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FL.hpp"
#include "tl/utility.hpp"
#include <string>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_literals;
  [[maybe_unused]] suite tests = [] {
    "remove drive letter"_test = [] {
      const auto test_drive_letter = [](std::string &&start) {
        const auto start_size = std::ranges::size(start);
        const auto end =
          open_viii::archive::fl::remove_drive_letter(std::move(start));
        const auto end_size = std::ranges::size(end);
        expect(gt(start_size, end_size));
        expect(eq(start_size - 3U, end_size));
      };
      const auto test_drive_letters = [&test_drive_letter](auto &&...start) {
        (test_drive_letter(std::move(start)), ...);
      };
      static constexpr auto all_lowercase_letters =
        tl::utility::sequence<'a', 'z' - 'a'>();
      static constexpr auto all_UPPERCASE_letters =
        tl::utility::sequence<'A', 'Z' - 'A'>();
      const auto test_all_drive_letters = [&test_drive_letter]() {
        all_lowercase_letters.operator()([&test_drive_letter]<auto... I>() {
          (test_drive_letter(std::string{ char(I), ':', '\\' }), ...);
        });
        all_UPPERCASE_letters.operator()([&test_drive_letter]<auto... I>() {
          (test_drive_letter(std::string{ char(I), ':', '\\' }), ...);
        });
        all_lowercase_letters.operator()([&test_drive_letter]<auto... I>() {
          (test_drive_letter(std::string{ char(I), ':', '/' }), ...);
        });
        all_UPPERCASE_letters.operator()([&test_drive_letter]<auto... I>() {
          (test_drive_letter(std::string{ char(I), ':', '/' }), ...);
        });
      };
      test_drive_letters(R"(c:\)"s,
                         R"(d:\)"s,
                         R"(e:\)"s,
                         R"(c:\test\a)"s,
                         R"(c:\test\b)"s,
                         R"(c:\test\c)"s);
      test_all_drive_letters();
    };
  };
}