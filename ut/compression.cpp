#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/compression/L4Z.hpp"
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
// This is the type which holds sequences
template<std::size_t... Ns> struct sequence
{
  template<typename lambdaT>
  void
    operator()(const lambdaT &op) const
  {
    (op.template operator()<Ns>(), ...);
  }
};
// First define the template signature
template<std::size_t... Ns> struct seq_gen;
// Recursion case
template<std::size_t I, std::size_t... Ns> struct seq_gen<I, Ns...>
{
  // Take front most number of sequence,
  // decrement it, and prepend it twice.
  // First I - 1 goes into the counter,
  // Second I - 1 goes into the sequence.
  using type = typename seq_gen<I - 1U, I - 1U, Ns...>::type;
};
// Recursion abort
template<std::size_t... Ns> struct seq_gen<1U, Ns...>
{
  using type = sequence<Ns...>;
};
template<size_t N> using sequence_t = typename seq_gen<N + 1U>::type;
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  static constexpr sequence_t<100U> run{};
  [[maybe_unused]] suite            tests = [] {
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