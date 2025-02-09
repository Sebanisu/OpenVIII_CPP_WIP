// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_CONCEPTS_HPP
#define VIIIARCHIVE_CONCEPTS_HPP
#include "CompressionTypeT.hpp"
#include "tl/concepts.hpp"
#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
namespace open_viii {
template<typename T>
concept Color_R = requires(T a)
{
  {
    a.r()
    } -> std::integral;
};
template<typename T>
concept Color_G = requires(T a)
{
  {
    a.g()
    } -> std::integral;
};
template<typename T>
concept Color_B = requires(T a)
{
  {
    a.b()
    } -> std::integral;
};
template<typename T>
concept Color_A = requires(T a)
{
  {
    a.a()
    } -> std::integral;
};
template<typename T>
concept is_bool = std::is_same_v<T, bool>;
template<typename T>
concept Color_is_black = requires(T a)
{
  {
    a.is_black()
    } -> is_bool;
};
template<typename T, typename U>
concept struct_of_color32 = requires(T c)
{
  {
    c.r
    } -> std::same_as<U>;
  {
    c.g
    } -> std::same_as<U>;
  {
    c.b
    } -> std::same_as<U>;
  {
    c.a
    } -> std::same_as<U>;
};

template<typename T>
concept struct_of_color32_byte = struct_of_color32<T, std::uint8_t &>;
template<typename T>
concept struct_of_color32_float = struct_of_color32<T, float &>;
template<typename T>
concept Color
  = Color_A<T> && Color_B<T> && Color_G<T> && Color_R<T> && Color_is_black<T>;
template<typename T>
concept Color_types
  = struct_of_color32_byte<T> || struct_of_color32_float<T> || Color<T>;

template<typename T>
concept Number = std::floating_point<T> || std::integral<T>;
template<typename T>
concept FIFLFS_Has_get_entry_data = requires(T a)
{
  a.get_entry_data(std::string_view(""));
};
template<typename T>
concept FI_Like_UncompressedSize = requires(T a)
{
  {
    a.uncompressed_size()
    } -> std::integral;
};
template<typename T>
concept FI_Like_Offset = requires(T a)
{
  {
    a.offset()
    } -> std::integral;
};
template<typename T>
concept FI_Like_CompressionType = requires(T a)
{
  {
    a.compression_type()
    } -> std::convertible_to<CompressionTypeT>;
};
template<typename T>
concept FI_Like = FI_Like_CompressionType<T> && FI_Like_Offset<
  T> && FI_Like_UncompressedSize<T>;
template<typename T> concept Point_Like = (requires (T a)
{{a.x()}->Number;} &&
  requires (T a)
  {{a.y()}->Number;}
);
template<typename T>
concept integral_no_ref_no_cv = std::integral<std::remove_cvref_t<T>>;
template<typename T, std::size_t current = 0U> concept Shape_Like = (
  requires(T a){{a.template uv<current>()}->Point_Like;} &&
    requires(T a){{a.template face_indice<current>()}->integral_no_ref_no_cv;}
  && requires(T a) {{a.COUNT>0U};}

  );
template<typename lambdaT, typename pointT>
concept TakesTwoPointsReturnsPoint = Point_Like<pointT> && std::
  regular_invocable<lambdaT, pointT, pointT> && Point_Like<
    std::invoke_result_t<lambdaT, pointT, pointT>>;
template<typename trivialType>
concept is_trivially_copyable = std::is_trivially_copyable_v<trivialType>;
template<typename trivialType>
concept is_default_constructible = std::is_default_constructible_v<trivialType>;
template<typename trivialType>
concept has_data = requires(trivialType item)
{
  std::ranges::data(item);
};
template<typename trivialType>
concept has_size = requires(trivialType item)
{
  std::ranges::size(item);
};
template<typename trivialType>
concept has_data_and_size = has_data<trivialType> && has_size<trivialType>;
template<typename trivialType>
concept is_trivially_copyable_and_default_constructible = is_trivially_copyable<
  trivialType> && std::is_default_constructible_v<trivialType>;
template<typename trivialType>
concept is_default_constructible_has_data_and_size
  = std::is_default_constructible_v<trivialType> && has_data_and_size<
    trivialType> && !is_trivially_copyable<trivialType>;
template<typename Type>
concept has_resize = requires(Type a)
{
  a.resize(static_cast<std::size_t>(0U));
};
template<typename Type>
concept has_reserve = requires(Type a)
{
  a.reserve(static_cast<std::size_t>(0U));
};
template<typename trivialType>
concept is_default_constructible_has_data_size_resize
  = is_default_constructible_has_data_and_size<trivialType> && has_resize<
    trivialType>;
template<typename lambdaT>
concept Foreach_Archive_Lambda
  = std::invocable<lambdaT, std::vector<char>, std::string>;
// template<typename a_type, typename lambdaT>
// concept Optional_Archive_Execute_On = (
//  Foreach_Archive_Lambda<lambdaT> && requires(a_type a) { a.has_value(); }
//  && requires(a_type a,
//    const std::initializer_list<std::string_view> &filename,
//    lambdaT lambda) { a->execute_on(filename, lambda); });
//
// template<typename a_type, typename lambdaT>
// concept Archive_Execute_On =
//  (Foreach_Archive_Lambda<
//     lambdaT> && requires(a_type a, const
//     std::initializer_list<std::string_view> &filename, lambdaT lambda) {
//    a.execute_on(filename, lambda);
//  });
//
// template<bool nested, typename a_type, typename lambdaT>
// concept Nested_Archive_Execute_On =
//  (nested
//    && Foreach_Archive_Lambda<
//      lambdaT> && requires(a_type a, const
//      std::initializer_list<std::string_view> &filename, lambdaT lambda) {
//         a.execute_with_nested({}, filename, lambda);
//       });

/**
 * concepts used by FS
 */

/**
 * @tparam T Accept things like std::string or std::ostream
 */
template<typename T>
concept is_insertable_or_ostream = tl::concepts::is_contiguous_with_insert<
  T> || std::is_base_of_v<std::ostream, std::decay_t<T>>;
template<typename T>
concept is_monostate = std::same_as<std::decay_t<T>, std::monostate>;
}// namespace open_viii
#endif// VIIIARCHIVE_CONCEPTS_HPP
