#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FI.hpp"
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace open_viii::archive;
  using namespace open_viii;
  [[maybe_unused]] suite tests = [] {
    static constexpr auto check_fi
      = [](
          const std::uint32_t               &size,
          const std::uint32_t               &offset,
          const open_viii::CompressionTypeT &compression_type) {
          const FI fi{ size, offset, compression_type };
          expect(eq(fi.uncompressed_size(), size));
          expect(eq(fi.offset(), offset));
          expect(eq(
            static_cast<std::uint32_t>(fi.compression_type()),
            static_cast<std::uint32_t>(compression_type)));
        };
    static constexpr auto check_count = [](const std::size_t &expected_count) {
      expect(eq(FI::get_count(expected_count * FI::SIZE), expected_count));
    };
    "FI check if returned values = the values we put in."_test = [] {
      check_fi(0U, 1U, CompressionTypeT::none);
      check_fi(10U, 15U, CompressionTypeT::none);
      check_fi(65U, 16U, CompressionTypeT::none);
    };
    "FI check count"_test = [] {
      // theres a version of get count that takes a file path, it just gets the
      // filesize and passes it to the other function so I don't think I need to
      // write a test for it.
      expect(eq(FI::SIZE, sizeof(FI)));
      check_count(0U);
      check_count(1U);
      check_count(40U);
      check_count(60U);
      check_count(55U);
    };
    "FI entry offset"_test = [] {
      expect(eq(FI::get_fi_entry_offset(5U, 5U), 65U));
      expect(eq(FI::get_fi_entry_offset(45U, 3U), 543U));
      expect(eq(FI::get_fi_entry_offset(90U, 1U), 1081U));
      expect(eq(FI::get_fi_entry_offset(2U, 9U), 33U));
      expect(eq(FI::get_fi_entry_offset(1U, 0U), 12U));
      expect(eq(FI::get_fi_entry_offset(19U, 55U), 283U));
    };
    "FI append"_test = [] {
      std::vector<char> buffer{};
      append_entry(buffer, FI(5U, 10U));
      expect(eq(std::size(buffer), 12U));
      const auto local_fi = FI(buffer, 0U, 0U);
      expect(eq(local_fi.offset(), 10U));
      expect(eq(local_fi.uncompressed_size(), 5U));
    };
  };
}