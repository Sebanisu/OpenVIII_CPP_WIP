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
  using namespace std::string_view_literals;
  [[maybe_unused]] suite tests = [] {
    "clean_path_string"_test = [] {
      const auto check = [](std::string input) {
        const auto find_r = [](const char &c) -> bool {
          return c == '\r';
        };
        input = open_viii::archive::fl::clean_path_string(std::move(input));
        std::string_view view{ input };
        const auto       start = view.substr(0, 3U);
        const auto       has_r = std::ranges::none_of(input, find_r);
        if constexpr (std::filesystem::path::preferred_separator == '/') {
          const auto find_slash = [](const char &c) -> bool {
            return c == '\\';
          };
          /* on windows the \ stays on linux it's a /
           * so the only things the same are removing \r and c:\ */
          const auto has_slash = std::ranges::none_of(input, find_slash);
          expect(has_slash);
        }
        expect(neq(start, R"(c:\)"sv));
        expect(neq(start, R"(C:\)"sv));
        expect(has_r);
      };
      check(R"(c:\test\blah\blah\blah)"s);
      check("c:\\test\\blah\\blah\\blah\r\r\r\r\r\r\r\r\r\r\r\r"s);
      check(R"(C:\test\blah\blah\blah)"s);
      check("C:\\test\\blah\\blah\\blah\r\r\r\r\r\r\r\r\r\r\r\r"s);
    };
    "clean_buffer"_test    = [] {};
    "get_max"_test         = [] {};
    "sort_entries"_test    = [] {};
    "get_all_entries"_test = [] {};
    "get_entry"_test       = [] {};
  };
}