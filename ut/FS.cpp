#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/CompressionTypeT.hpp"
#include "open_viii/archive/FI.hpp"
#include "open_viii/archive/FS.hpp"
#include "open_viii/compression/L4Z.hpp"
#include "open_viii/compression/LZSS.hpp"
#include "tl/utility.hpp"
#include <algorithm>
#include <cstdint>
#include <ranges>
#include <sstream>
#include <string_view>
// TODO export the FS writing code into the FS.hpp, I had to write the code to
// write FS to have something for the read tests to use.
/**
 * Mock span data, this is grouping values together for tests. The raw data is
 * in a string_view and the span points to the same data. Because if you pass a
 * string_view to the code it thinks it's a path. Fi has offsets and size info
 * to track where the data is.
 */
struct mock_span_data
{
private:
  std::string_view       m_string_view{};
  std::span<const char>  m_span{};
  open_viii::archive::FI m_fi{};
public:
  constexpr mock_span_data(std::string_view            string_view,
                           std::uint32_t               offset = 0,
                           std::uint32_t               size   = 0,
                           open_viii::CompressionTypeT compression_type =
                             open_viii::CompressionTypeT::none)
    : m_string_view([&size, &offset, &string_view]() -> std::string_view {
        if (offset > 0) {
          string_view = string_view.substr(offset);
        }
        if (size > 0) {
          string_view = string_view.substr(0, size);
        }
        if (size == 0) {
          size = static_cast<std::uint32_t>(std::size(string_view));
        }
        return string_view;
      }()),
      m_span(m_string_view),
      m_fi(size, offset, compression_type)
  {}
  constexpr mock_span_data(std::span<const char>       string_view,
                           std::uint32_t               offset = 0,
                           std::uint32_t               size   = 0,
                           open_viii::CompressionTypeT compression_type =
                             open_viii::CompressionTypeT::none)
    : mock_span_data(
      std::string_view(std::data(string_view), std::size(string_view)),
      offset,
      size,
      compression_type)
  {}
  constexpr auto
    string_view() const noexcept
  {
    return m_string_view;
  }
  constexpr auto
    span() const noexcept
  {
    return m_span;
  }
  constexpr auto
    fi() const noexcept
  {
    return m_fi;
  }
};
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace open_viii::archive;
  using namespace open_viii::compression;
  [[maybe_unused]] suite tests = [] {
    static constexpr std::string_view test_string("HELLO WORLD! 0123456789");
    static constexpr mock_span_data   mock0(test_string);
    static constexpr mock_span_data   mock1(test_string, 0, 5);
    static constexpr mock_span_data   mock2(test_string, 6, 6);
    static constexpr mock_span_data   mock3(test_string,
                                          std::size(test_string) - 10);
    const auto                        temp_file =
      tl::utility::create_temp_file("FS_test_uncompressed.tmp", test_string);
    static constexpr auto check_fs = [](auto                  full_data,
                                        const mock_span_data &mock) {
      const auto value = FS::get_entry(full_data, mock.fi());
      expect(std::ranges::equal(value, mock.string_view()));
    };
    static constexpr auto check_fs2 =
      [](auto full_data, const mock_span_data &mock, const FI &fi) {
        const auto value = FS::get_entry(full_data, fi);
        expect(std::ranges::equal(value, mock.string_view()));
      };
    "read entire span"_test = [] {
      check_fs(mock0.span(), mock0);
      check_fs(mock0.span(), mock1);
      check_fs(mock0.span(), mock2);
      check_fs(mock0.span(), mock3);
    };
    "read entire file"_test = [&temp_file] {
      expect(temp_file.has_value());
      expect(std::filesystem::exists(temp_file.value()));
      check_fs(temp_file.value(), mock0);
      check_fs(temp_file.value(), mock1);
      check_fs(temp_file.value(), mock2);
      check_fs(temp_file.value(), mock3);
    };
    {
      std::vector<FI> fi_buffer{};
      fi_buffer.reserve(3);
      std::vector<char> uncompressed_buffer{};
      const auto        add_uncompressed_data =
        [&uncompressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(
            FS::append_entry(uncompressed_buffer, mock.span(), mock.fi()));
        };
      add_uncompressed_data(mock1);
      add_uncompressed_data(mock2);
      add_uncompressed_data(mock3);
      "read entire span"_test = [&uncompressed_buffer, &fi_buffer] {
        check_fs2(uncompressed_buffer, mock1, fi_buffer[0]);
        check_fs2(uncompressed_buffer, mock2, fi_buffer[1]);
        check_fs2(uncompressed_buffer, mock3, fi_buffer[2]);
      };
    }
    {
      // write compressed LZSS data to buffer
      std::vector<FI> fi_buffer{};
      fi_buffer.reserve(3);
      std::vector<char> compressed_buffer{};
      const auto        add_lzss_data = [&compressed_buffer,
                                  &fi_buffer](const mock_span_data &mock) {
        fi_buffer.push_back(FS::append_entry(
          compressed_buffer, mock.span(), open_viii::CompressionTypeT::lzss));
      };
      add_lzss_data(mock1);
      add_lzss_data(mock2);
      add_lzss_data(mock3);
      const auto compressed_temp_file = tl::utility::create_temp_file(
        "FS_test_Lzss.tmp",
        std::string_view(std::data(compressed_buffer),
                         std::size(compressed_buffer)));
      "read entire span"_test = [&compressed_buffer, &fi_buffer] {
        check_fs2(compressed_buffer, mock1, fi_buffer[0]);
        check_fs2(compressed_buffer, mock2, fi_buffer[1]);
        check_fs2(compressed_buffer, mock3, fi_buffer[2]);
      };
      "read entire file"_test = [&compressed_temp_file, &fi_buffer] {
        expect(compressed_temp_file.has_value());
        expect(std::filesystem::exists(compressed_temp_file.value()));
        check_fs2(compressed_temp_file.value(), mock1, fi_buffer[0]);
        check_fs2(compressed_temp_file.value(), mock2, fi_buffer[1]);
        check_fs2(compressed_temp_file.value(), mock3, fi_buffer[2]);
      };
    }
    {
      // write compressed L4Z data to buffer
      std::vector<FI> fi_buffer{};
      fi_buffer.reserve(3);
      std::vector<char> compressed_buffer{};
      const auto        add_l4z_data = [&compressed_buffer,
                                 &fi_buffer](const mock_span_data &mock) {
        fi_buffer.push_back(FS::append_entry(
          compressed_buffer, mock.span(), open_viii::CompressionTypeT::lz4));
      };
      add_l4z_data(mock1);
      add_l4z_data(mock2);
      add_l4z_data(mock3);
      const auto compressed_temp_file = tl::utility::create_temp_file(
        "FS_test_Lz4.tmp",
        std::string_view(std::data(compressed_buffer),
                         std::size(compressed_buffer)));
      "read entire span"_test = [&compressed_buffer, &fi_buffer] {
        check_fs2(compressed_buffer, mock1, fi_buffer[0]);
        check_fs2(compressed_buffer, mock2, fi_buffer[1]);
        check_fs2(compressed_buffer, mock3, fi_buffer[2]);
      };
      "read entire file"_test = [&compressed_temp_file, &fi_buffer] {
        expect(compressed_temp_file.has_value());
        expect(std::filesystem::exists(compressed_temp_file.value()));
        check_fs2(compressed_temp_file.value(), mock1, fi_buffer[0]);
        check_fs2(compressed_temp_file.value(), mock2, fi_buffer[1]);
        check_fs2(compressed_temp_file.value(), mock3, fi_buffer[2]);
      };
    }
  };
}