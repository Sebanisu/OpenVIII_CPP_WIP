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
    {
      static const auto check_path_strings = [](std::string &&input) {
        const auto find_r = [](const char &c) -> bool {
          return c == '\r';
        };
        const auto has_c_drive =
          !tl::string::search(input, "c:\\"sv).has_value()
          && !tl::string::search(input, "C:\\"sv).has_value();
        const auto has_r = std::ranges::none_of(input, find_r);
        if constexpr (std::filesystem::path::preferred_separator == '/') {
          const auto find_slash = [](const char &c) -> bool {
            return c == '\\';
          };
          /* on windows the \ stays on linux it's a /
           * so the only things the same are removing \r and c:\ */
          const auto has_slash = std::ranges::none_of(input, find_slash);
          expect(has_slash);
        }
        expect(has_c_drive);
        expect(has_r);
      };
      "clean_path_string"_test = [] {
        static const auto check = [](std::string &&input) {
          check_path_strings(
            open_viii::archive::fl::clean_path_string(std::move(input)));
        };
        check(R"(c:\test\blah\blah\blah)"s);
        check("c:\\test\\blah\\blah\\blah\r\r\r\r\r\r\r\r\r\r\r\r"s);
        check(R"(C:\test\blah\blah\blah)"s);
        check("C:\\test\\blah\\blah\\blah\r\r\r\r\r\r\r\r\r\r\r\r"s);
      };
      "clean_buffer"_test = [] {
        static const auto check = [](std::string &&input) {
          check_path_strings(
            open_viii::archive::fl::clean_buffer(std::move(input)));
        };
        check(R"(c:\test1\
c:\test2\test3\
C:\test4\test5\
C\test5\test6\)"s);
        check(
          "(c:\\test1\\\r\nc:\\test2\\test3\\\r\nC:\\test4\\test5\\\r\nC:\\test5\\test6\\\r\n"s);
      };
    }
    "get_max"_test         = [] {
      static const auto check = [](std::size_t count, std::size_t limit, std::size_t expected)
      {
        expect(eq(open_viii::archive::fl::get_max(count,limit),expected));
      };
      check(0U,0U,0U);
      check(0U,1U,1U);
      check(10U,1U,1U);
      check(1U,10U,1U);
    };
    "sort_entries"_test    = [] {
      /* this test will be giving unsorted data and comparing it to the sorted one.*/
    };
    "get_all_entries"_test = [] {};
    "get_entry"_test       = [] {};
  };
}