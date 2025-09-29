#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/compression/L4Z.hpp"
#include "open_viii/compression/LZSS.hpp"
#include "tl/random.hpp"
#include "tl/utility.hpp"
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  static constexpr tl::utility::sequence<1U, 100U> run{};
  [[maybe_unused]] suite                           tests = [] {
    "lzss compress and uncompress"_test = []() {
      const auto run_once = [&]<size_t i>() {
        const auto                  random_char = tl::random::iota<char, i>();
        [[maybe_unused]] const auto compressed_random_char
          = open_viii::compression::LZSS::compress(random_char);
        [[maybe_unused]] const auto decompressed_random_char
          = open_viii::compression::LZSS::decompress(compressed_random_char);
        expect(std::ranges::equal(random_char, decompressed_random_char));
      };
      run(run_once);
    };
    "lz4 compress and uncompress"_test = []() {
      const auto run_once = [&]<size_t i2>() {
        const auto                  random_char = tl::random::iota<char, i2>();
        [[maybe_unused]] const auto compressed_random_char
          = open_viii::compression::l4z::compress(random_char);
        [[maybe_unused]] const auto decompressed_random_char
          = open_viii::compression::l4z::decompress(compressed_random_char, i2);
        expect(std::ranges::equal(random_char, decompressed_random_char));
      };
      run(run_once);
    };
  };
}