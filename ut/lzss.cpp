#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/compression/LZSS.hpp"
#include <random>
template<typename T> struct random_impl
{
private:
  std::random_device                                             rd{};
  mutable std::mt19937                                           gen{ rd() };
  mutable std::uniform_int_distribution<std::make_unsigned_t<T>> dis{
    std::numeric_limits<std::make_unsigned_t<T>>::min(),
    std::numeric_limits<std::make_unsigned_t<T>>::max()
  };
public:
  auto
    operator()([[maybe_unused]] const T &unused) const
  {
    return static_cast<T>(dis(gen));
  }
};
template<typename T, std::size_t count>
auto
  random_iota()
{
  static const auto    random_T = random_impl<T>();
  std::array<T, count> out{};
  std::ranges::transform(
    out, std::begin(out), []([[maybe_unused]] const auto &unused) -> T {
      return random_T(unused);
    });
  return out;
}
template<size_t max, size_t i = 1U, typename lambdaT>
void
  run(const lambdaT &op)
{
  if constexpr (i < max + 1U) {
    if constexpr (i >= 1U) {
      op.template operator()<i>();
    }
    run<max, i + 1U>(op);
  }
}
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  [[maybe_unused]] suite tests = [] {
    "lzss compress and uncompress"_test = []() {
      const auto run_once = [&]<size_t i>() {
        const auto                  random_char = random_iota<char, i>();
        [[maybe_unused]] const auto compressed_random_char =
          open_viii::compression::LZSS::compress(random_char);
        [[maybe_unused]] const auto decompressed_random_char =
          open_viii::compression::LZSS::decompress(compressed_random_char);
        expect(std::ranges::equal(random_char, decompressed_random_char));
      };
      run<100U>(run_once);
    };
  };
}