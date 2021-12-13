//
// Created by pcvii on 2/15/2021.
//
#ifndef VIIIARCHIVE_FOR_EACH_HPP
#define VIIIARCHIVE_FOR_EACH_HPP
#include <iostream>
namespace open_viii::tools {
/**
 * for_each x and y value where max x and max y are different values.
 * @tparam lambdaT
 * @param max_x
 * @param max_y
 * @param lambda
 */
template<std::unsigned_integral intT, std::invocable<intT, intT> lambdaT>
static constexpr void
  for_each_xy(const intT max_x, const intT max_y, const lambdaT &lambda)
{
  constexpr intT start{};
  for (auto y = start; y != max_y; ++y) {
    for (auto x = start; x != max_x; ++x) {
      if constexpr (std::is_invocable_r_v<bool, lambdaT, intT, intT>) {
        if (lambda(x, y)) {
          return;// short circuit
        }
      }
      else {
        lambda(x, y);
      }
    }
  }
}
/**
 * test for_each_xy by adding everything together
 * @param max_x max x
 * @param max_y max y
 * @return total of all x and y values
 */
static consteval auto
  test_for_each_xy(std::size_t max_x, std::size_t max_y)
{
  std::size_t total{};
  for_each_xy(max_x, max_y, [&total](const auto x, const auto y) {
    total += x + y;
  });
  return total;
}
static_assert(test_for_each_xy(5U, 1U) == 10U);
static_assert(test_for_each_xy(1U, 5U) == 10U);
template<std::unsigned_integral intT, std::invocable<intT, intT> lambdaT>
/**
 * for_each x and y value where max x and max y are the same value.
 * @tparam lambdaT
 * @param max_xy max x and max y.
 * @param lambda lambda to execute f(x,y)
 */
static constexpr void
  for_each_xy(const intT &max_xy, const lambdaT &lambda)
{
  for_each_xy(max_xy, max_xy, lambda);
}
/**
 * test for_each_xy by adding everything together
 * @param max_xy max x and max y.
 * @return total of all x and y values
 */
static consteval auto
  test_for_each_xy(std::size_t max_xy)
{
  return test_for_each_xy(max_xy, max_xy);
}
static_assert(test_for_each_xy(5U) == 100U);
/**
 *
 * @tparam UnaryOperationT
 * @tparam BinaryOperationT
 * @param directory
 * @param unary_function
 * @param binary_function
 * @todo add tests
 */
template<
  std::invocable<std::filesystem::path> UnaryOperationT,
  typename BinaryOperationT>
static void
  execute_on_directory(const std::filesystem::path &directory, UnaryOperationT unary_function, BinaryOperationT binary_function = true) requires(
    (std::is_same_v<std::decay<BinaryOperationT>, bool>)
    || (std::is_invocable_r_v<bool, BinaryOperationT, std::filesystem::path>))
{
  const std::filesystem::directory_options options
    = std::filesystem::directory_options::skip_permission_denied;
  std::error_code ec{};
  std::ranges::for_each(
    std::filesystem::directory_iterator(directory, options, ec),
    [&unary_function, &binary_function](const auto &item) {
      const auto path = item.path();
      if constexpr (std::is_same_v<std::decay<BinaryOperationT>, bool>) {
        if (!binary_function) {
          return;
        }
      }
      else if constexpr (std::is_invocable_r_v<
                           bool,
                           BinaryOperationT,
                           std::filesystem::path>) {
        if (!binary_function(path)) {
          return;
        }
      }
      unary_function(path);
    });
  if (ec) {
    std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - " << ec.value()
              << ": " << ec.message() << std::endl;
    ec.clear();
  }
}
/**
 *
 * @tparam lambdaT
 * @param dir
 * @param filenames
 * @param extensions
 * @param lambda
 * @todo add tests
 */
template<typename lambdaT>
requires(std::invocable<lambdaT, std::filesystem::path>)
  [[maybe_unused]] static void execute_on_directory(
    const std::filesystem::path            &dir,
    std::initializer_list<std::string_view> filenames,
    std::initializer_list<std::string_view> extensions,
    const lambdaT                          &lambda)
{
  execute_on_directory(
    dir,
    lambda,
    [&filenames, &extensions](const std::filesystem::path &path) {
      return std::filesystem::is_regular_file(path)
          && (std::ranges::empty(extensions)
              || (path.has_extension() && i_ends_with_any(path.extension().string(), extensions)))
          && (std::ranges::empty(filenames)
              || (path.has_stem() && i_find_any(path.stem().string(), filenames)));
    });
}
/**
 * Will find and execute lambda on the dir if path_contains a valid value.
 * @tparam lambdaT
 * @param dir
 * @param path_contains list of valid values {} = all.
 * @param lambda f(std::filesystem:: path)
 * @todo add tests
 */
template<typename lambdaT>
requires(std::invocable<lambdaT, std::filesystem::path>)
  [[maybe_unused]] static void execute_on_directories(
    const std::filesystem::path            &dir,
    std::initializer_list<std::string_view> path_contains,
    const lambdaT                          &lambda)
{
  execute_on_directory(
    dir,
    lambda,
    [&path_contains](const std::filesystem::path &path) {
      return std::filesystem::is_directory(path)
          && tools::i_find_any(path.string(), path_contains);
    });
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_FOR_EACH_HPP
