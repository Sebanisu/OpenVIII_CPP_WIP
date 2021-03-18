#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FileData.hpp"
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace open_viii::archive;
  using namespace open_viii;
  [[maybe_unused]] suite tests = [] {
    "constructors test"_test = [] {
      // make a file data using the different constructors make sure we get the
      // same values.?
    };
    "total size"_test = [] {
      // check if expected size per given values.
      expect(eq(FileData().total_size(), 16U));
    };
    "get path"_test = [] {
      // check if expected path.
      expect(eq(FileData().get_path(), std::filesystem::path()));
    };
    "compression type"_test = [] {
      expect(eq(static_cast<std::uint32_t>(FileData().compression_type()),
                static_cast<std::uint32_t>(open_viii::CompressionTypeT::none)));
    };
    "uncompressed size"_test = [] {
      expect(eq(FileData().uncompressed_size(), std::size_t{}));
    };
    "offset"_test = [] {
      expect(eq(FileData().offset(), std::size_t{}));
    };
    "get path string"_test = [] {
      // check if expected path.
      expect(eq(FileData().get_path_string(), std::string_view("")));
    };
    "empty"_test = [] {
      // check if expected path.
      expect(eq(FileData().empty(), true));
    };
  };
}