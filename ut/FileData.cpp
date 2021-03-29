#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FI.hpp"
#include "open_viii/archive/FileData.hpp"
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace boost::ut::bdd;
  using namespace std::string_view_literals;
  using namespace open_viii::archive;
  using namespace open_viii;
  [[maybe_unused]] suite tests = [] {
    static constexpr std::string_view sample_hex =
      "\x0B\x00\x00\x00\x63\x72\x65\x64\x69\x74\x73\x2E\x61\x76\x69\x2E\xA5\x0C"
      "\x00\x00\x00\x00\x00\x9F\xE3\x96\x03\x0F\x00\x00\x00\x64\x61\x74\x61\x5C"
      "\x64\x69\x73\x6B\x5C\x64\x69\x73\x6B\x31\xCD\x88\xA3\x03\x00\x00\x00\x00"
      "\x02\x00\x00\x00\x0F\x00\x00\x00\x64\x61\x74\x61\x5C\x64\x69\x73\x6B\x5C"
      "\x64\x69\x73\x6B\x32\xCF\x88\xA3\x03\x00\x00\x00\x00\x02\x00\x00\x00\x0F"
      "\x00\x00\x00\x64\x61\x74\x61\x5C\x64\x69\x73\x6B\x5C\x64\x69\x73\x6B\x33"
      "\xD1\x88\xA3\x03\x00\x00\x00\x00\x02\x00\x00\x00\x0F\x00\x00\x00\x64\x61"
      "\x74\x61\x5C\x64\x69\x73\x6B\x5C\x64\x69\x73\x6B\x34\xD3\x88\xA3\x03\x00"
      "\x00\x00\x00\x02\x00\x00\x00"sv;
    static std::stringstream ss{};
    ss.write(std::data(sample_hex), std::size(sample_hex));
    "FileData test"_test = [] {
      // make a file data using the different constructors make sure we get
      // the same values.?
      static constexpr auto expects =
        [](const FileData & fd,
           std::string_view expected_path              = {},
           std::size_t      expected_total_size        = 16U,
           std::size_t      expected_uncompressed_size = {},
           std::size_t      expected_offset            = {},
           bool             expected_empty             = true) {
          expect(eq(fd.get_path_string(), expected_path));
          expect(eq(fd.total_size(), expected_total_size));
          expect(
            eq(static_cast<std::uint32_t>(fd.compression_type()),
               static_cast<std::uint32_t>(open_viii::CompressionTypeT::none)));
          expect(eq(fd.uncompressed_size(), expected_uncompressed_size));
          expect(eq(fd.offset(), expected_offset));
          expect(eq(fd.empty(), expected_empty));
        };
      static constexpr auto check_fd_sample = [](auto &v) {
        then("5 values in sample") = [&v] {
          expects(FileData(v), "credits.avi"sv, 27U, 60220319U, 828718U, false);
          expects(FileData(v), "data/disk/disk1"sv, 31U, 2U, 61049037U, false);
          expects(FileData(v), "data/disk/disk2"sv, 31U, 2U, 61049039U, false);
          expects(FileData(v), "data/disk/disk3"sv, 31U, 2U, 61049041U, false);
          expects(FileData(v), "data/disk/disk4"sv, 31U, 2U, 61049043U, false);
        };
        then("Next one will be empty because there isn't anymore data") = [&v] {
          expects(FileData(v));
        };
      };
      given("I have a string stream") = [] {
        check_fd_sample(ss);
      };

      given("Giving it manual data") = [] {
        then("Just a size and an offset") = [] {
          expects(FileData(252U, 658U), {}, 16U, 658U, 252U, true);
        };
        then("A string, a size and an offset") = [] {
          expects(FileData(R"(test\test)"sv, 252U, 658U),
                  "test/test"sv,
                  25U,
                  658U,
                  252U,
                  false);
        };
        then("Port Data from FI") = [] {
          expects(FileData(FI(658U, 252U, CompressionTypeT::none)),
                  {},
                  16U,
                  658U,
                  252U,
                  true);
          expect(throws([] {
            FileData(FI(658U, 252U, CompressionTypeT::lzss));
          }));
          expect(throws([] {
            FileData(FI(658U, 252U, CompressionTypeT::lz4));
          }));
        };
      };
      given("raw_buffer") = [] {
        auto input = tl::read::input(sample_hex, true);
        check_fd_sample(input);
      };
    };
  };
}