#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/compression/L4Z.hpp"
#include "open_viii/compression/LZSS.hpp"
#include <random>
/**
 * Random holder should be defined as a static value.
 * @tparam T type of value returned by operator()
 * @see if we can make this passable to a std::transform
 * @todo maybe a form of random_impl should be moved to toolslibrary.
 */
template<typename T> struct random_impl
{
private:
  static_assert(std::is_integral_v<T>);
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
/**
 * Get a std::array with random values.
 * @tparam T type stored in std::array
 * @tparam count is the total number of values in the array.
 * @return std::array<T,count>
 */
template<typename T, std::size_t count>
auto
  random_iota()
{
  static_assert(std::is_integral_v<T>);
  static const auto    random_T = random_impl<T>();
  std::array<T, count> out{};
  std::ranges::transform(
    out, std::begin(out), []([[maybe_unused]] const auto &unused) -> T {
      return random_T(unused);
    });
  return out;
}
/**
 * Holds number sequence and has operator to pass a lambda that takes 1 of the
 * sequence as a template parameter.
 * @tparam number_sequence
 */
template<std::size_t... number_sequence> struct sequence_impl
{
  template<typename lambdaT>
  constexpr void
    operator()(const lambdaT &op) const
  {
    (op.template operator()<number_sequence>(), ...);
  }
};
/**
 *
 * @tparam Min min value of I.
 * @tparam I is the current number in sequence
 * @tparam Ns sequence is number of values.
 * @return
 * @see https://stackoverflow.com/a/49673314/2588183
 */
template<std::size_t Min, std::size_t I, std::size_t... Ns>
auto
  make_sequence_impl()
{
  static_assert(I >= Min);
  if constexpr (I == Min)
    return sequence_impl<Ns...>();
  else
    return make_sequence_impl<Min, I - 1U, I - 1U, Ns...>();
}
/**
 * Get sequence_impl from Min to Max numbers
 * @tparam Min number
 * @tparam Max number
 */
template<size_t Min, size_t Count>
using sequence = std::decay_t<decltype(make_sequence_impl<Min, Count + Min>())>;
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  static constexpr sequence<1U, 100U> run{};
  [[maybe_unused]] suite              tests = [] {
    "lzss compress and uncompress"_test = []() {
      const auto run_once = [&]<size_t i>() {
        const auto                  random_char = random_iota<char, i>();
        [[maybe_unused]] const auto compressed_random_char =
          open_viii::compression::LZSS::compress(random_char);
        [[maybe_unused]] const auto decompressed_random_char =
          open_viii::compression::LZSS::decompress(compressed_random_char);
        expect(std::ranges::equal(random_char, decompressed_random_char));
      };
      run(run_once);
    };
    "lz4 compress and uncompress"_test = []() {
      const auto run_once = [&]<size_t i>() {
        const auto                  random_char = random_iota<char, i>();
        [[maybe_unused]] const auto compressed_random_char =
          open_viii::compression::l4z::compress(random_char);
        [[maybe_unused]] const auto decompressed_random_char =
          open_viii::compression::l4z::decompress(compressed_random_char, i);
        expect(std::ranges::equal(random_char, decompressed_random_char));
      };
      run(run_once);
    };
  };
}