#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FL.hpp"
#include "tl/algorithm.hpp"
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
  using namespace open_viii::archive;
  [[maybe_unused]] suite tests = [] {
    {
      static const auto check_path_strings = [](std::string &&input) {
        const auto find_r = [](const char &c) -> bool {
          return c == '\r';
        };
        const auto has_c_drive
          = !tl::string::search(input, "c:\\"sv).has_value()
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
    "get_max"_test = [] {
      static const auto check =
        [](std::size_t count, std::size_t limit, std::size_t expected) {
          expect(eq((open_viii::archive::fl::get_max)(count, limit), expected));
        };
      check(0U, 0U, 0U);
      check(0U, 1U, 1U);
      check(10U, 1U, 1U);
      check(1U, 10U, 1U);
    };
    "sort_entries"_test = [] {
      /* this test will be giving unsorted data and comparing it to the sorted
       * one.*/
      static const auto check
        = [](
            std::vector<std::pair<std::uint32_t, std::string>> &&unsorted,
            std::vector<std::pair<std::uint32_t, std::string>> &&expected) {
            const auto vector
              = open_viii::archive::fl::sort_entries(std::move(unsorted));
            auto f2 = std::ranges::begin(expected);
            for (auto f = std::ranges::begin(vector);
                 f != std::ranges::end(vector);
                 ++f, ++f2) {
              expect(eq(f->second, f2->second));
            }
          };
      check(
        std::vector{ std::make_pair(0U, R"(c:\c)"s),
                     std::make_pair(0U, R"(c:\aa)"s),
                     std::make_pair(0U, R"(c:\b)"s),
                     std::make_pair(0U, R"(c:\cc)"s),
                     std::make_pair(0U, R"(c:\a)"s),
                     std::make_pair(0U, R"(c:\bb)"s) },
        std::vector{ std::make_pair(0U, R"(c:\a)"s),
                     std::make_pair(0U, R"(c:\b)"s),
                     std::make_pair(0U, R"(c:\c)"s),
                     std::make_pair(0U, R"(c:\aa)"s),
                     std::make_pair(0U, R"(c:\bb)"s),
                     std::make_pair(0U, R"(c:\cc)"s) });
    };

    {
      static const auto sample_fl        = R"(C:\ff8\Data\eng\FIELD\mapdata.fi
C:\ff8\Data\eng\FIELD\mapdata\ec.fi
C:\ff8\Data\eng\FIELD\mapdata\te.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bcmin22a.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bccent1a.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bcport1b.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bchtr1a.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bchtl1a.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bcsaka1a.fi
C:\ff8\Data\eng\FIELD\mapdata\bc\bcform1a.fi)"s;
      static constexpr std::size_t count = 10U;
      static const auto            sample_fl_path
        = tl::utility::create_temp_file("sample_fl_test_file.fl", sample_fl);
      "get_all_entries"_test = [] {
        static const auto check
          = [](
              const std::initializer_list<std::string_view> needle = {},
              const std::size_t expected_count                     = 10U) {
              expect(sample_fl_path.has_value());
              const auto from_file = open_viii::archive::fl::get_all_entries(
                sample_fl_path.value(),
                ""s,
                0U,
                0U,
                count,
                needle);
              const auto from_string = open_viii::archive::fl::get_all_entries(
                sample_fl_path.value(),
                sample_fl,
                0U,
                0U,
                0U,
                needle);
              tl::algorithm::for_each(
                [](const auto &a, const auto &b) {
                  expect(eq(a.first, b.first));
                  expect(eq(a.second, b.second));
                },
                from_file,
                from_string);
              expect(eq(std::ranges::size(from_file), expected_count));
              expect(eq(std::ranges::size(from_string), expected_count));
            };
        check();
        check({ "bc"sv }, 7U);
        check({ "form"sv }, 1U);
      };
      "get_entry"_test = [] {
        static const auto check
          = [](
              const std::initializer_list<std::string_view> needle,
              const std::string_view                        expected_result) {
              expect(sample_fl_path.has_value());
              const auto from_file = open_viii::archive::fl::get_entry(
                sample_fl_path.value(),
                ""s,
                needle,
                0U,
                0U,
                count);
              const auto from_string = open_viii::archive::fl::get_entry(
                sample_fl_path.value(),
                sample_fl,
                needle,
                0U,
                0U,
                0U);

              expect(eq(from_file.first, from_string.first));
              expect(eq(from_file.second, from_string.second));
              expect(eq(from_file.second, expected_result));
            };

        if constexpr (std::filesystem::path::preferred_separator == '/') {
          check({ "bc"sv }, "ff8/Data/eng/FIELD/mapdata/bc/bcmin22a.fi"sv);
          check({ "form"sv }, "ff8/Data/eng/FIELD/mapdata/bc/bcform1a.fi"sv);
        }
        else {
          check(
            { "bc"sv },
            "ff8\\Data\\eng\\FIELD\\mapdata\\bc\\bcmin22a.fi"sv);
          check(
            { "form"sv },
            "ff8\\Data\\eng\\FIELD\\mapdata\\bc\\bcform1a.fi"sv);
        }
      };
    }
    "FL append"_test = [] {
      std::string buffer{};
      append_entry(buffer, "test/test1.test"sv);
      append_entry(buffer, "test/test2.test"sv);
      append_entry(buffer, "test/test3.test"sv);
      expect(eq(buffer[7], '\\'));
      expect(eq(std::size(buffer), 60U));
      const auto entries
        = open_viii::archive::fl::get_all_entries("", buffer, 0U);

      if constexpr (std::filesystem::path::preferred_separator == '/') {
        expect(eq(entries.at(0).second, "test/test1.test"sv));
        expect(eq(entries.at(1).second, "test/test2.test"sv));
        expect(eq(entries.at(2).second, "test/test3.test"sv));
      }
      else {
        expect(eq(entries.at(0).second, "test\\test1.test"sv));
        expect(eq(entries.at(1).second, "test\\test2.test"sv));
        expect(eq(entries.at(2).second, "test\\test3.test"sv));
      }
    };

    "get_all_entry_strings"_test = [] {
      static const auto check
        = [](
            const std::string                             &input,
            const std::initializer_list<std::string_view> &needle,
            const std::vector<std::string>                &expected_unix,
            const std::vector<std::string>                &expected_windows) {
            const auto results = open_viii::archive::fl::get_all_entry_strings(
              tl::read::input{ input },
              0U,
              0U,
              0U,
              needle,
              0U);
            expect(eq(results.size(), expected_unix.size()));

            if constexpr (std::filesystem::path::preferred_separator == '/') {
              for (size_t i = 0; i < expected_unix.size(); ++i) {
                expect(eq(results[i], expected_unix[i]));
              }
            }
            else {
              for (size_t i = 0; i < expected_windows.size(); ++i) {
                expect(eq(results[i], expected_windows[i]));
              }
            }
          };
      check(
        "C:\\test\\file1.fi\r\nC:\\test\\file2.fi\r\nC:\\data\\file3.fi\r\n",
        { "test" },
        { "test/file1.fi", "test/file2.fi" },
        { "test\\file1.fi", "test\\file2.fi" });
      check(
        "C:\\test\\file1.fi\r\nC:\\test\\file2.fi\r\nC:\\data\\file3.fi\r\n",
        {},
        { "data/file3.fi", "test/file1.fi", "test/file2.fi" },
        {
          "data\\file3.fi",
          "test\\file1.fi",
          "test\\file2.fi",
        });
    };

    "get_entry"_test = [] {
      static const auto check =
        [](
          const std::string                             &input,
          const std::initializer_list<std::string_view> &needle,
          const std::string                             &expected_unix,
          const std::string                             &expected_windows) {
          const auto result = open_viii::archive::fl::get_entry(input, needle);
          if constexpr (std::filesystem::path::preferred_separator == '/') {
            expect(eq(result.second, expected_unix));
          }
          else {
            expect(eq(result.second, expected_windows));
          }
        };
      check(
        "C:\\test\\file1.fi\r\nC:\\test\\file2.fi\r\nC:\\data\\file3.fi\r\n",
        { "data" },
        "data/file3.fi",
        "data\\file3.fi");
      check(
        "C:\\test\\file1.fi\r\nC:\\test\\file2.fi\r\nC:\\data\\file3.fi\r\n",
        { "test" },
        "test/file1.fi",
        "test\\file1.fi");
    };

    "append_entry"_test = [] {
      static const auto check = [](
                                  const std::filesystem::path &input,
                                  const std::string           &expected_unix,
                                  const std::string &expected_windows) {
        std::string buffer;
        open_viii::archive::append_entry(buffer, input);
        if constexpr (std::filesystem::path::preferred_separator == '/') {
          expect(eq(buffer, expected_unix));
        }
        else {
          expect(eq(buffer, expected_windows));
        }
      };
      check(
        "test/test1.test",
        "C:\\test/test1.test\r\n",
        "C:\\test\\test1.test\r\n");
      check(
        "data/sample.fi",
        "C:\\data/sample.fi\r\n",
        "C:\\data\\sample.fi\r\n");
    };
  };
}