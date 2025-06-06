#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FI.hpp"
#include "open_viii/archive/FS.hpp"
#include "open_viii/compression/L4Z.hpp"
#include "open_viii/CompressionTypeT.hpp"
#include <algorithm>
#include <cstdint>
#include <span>
#include <string_view>
template<typename fdT>
concept can_be_span_of_char = requires(fdT fd) { std::span<const char>(fd); };
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
  constexpr mock_span_data(
    std::string_view            string_view,
    std::uint32_t               offset = 0,
    std::uint32_t               size   = 0,
    open_viii::CompressionTypeT compression_type
    = open_viii::CompressionTypeT::none)
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
      m_span(m_string_view), m_fi(size, offset, compression_type)
  {}
  constexpr mock_span_data(
    std::span<const char>       string_view,
    std::uint32_t               offset = 0,
    std::uint32_t               size   = 0,
    open_viii::CompressionTypeT compression_type
    = open_viii::CompressionTypeT::none)
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
  static constexpr std::string_view test_string("HELLO WORLD! 0123456789");
  static constexpr mock_span_data   mock0(test_string);
  static constexpr mock_span_data   mock1(test_string, 0, 5);
  static constexpr mock_span_data   mock2(test_string, 6, 6);
  static constexpr mock_span_data   mock3(
    test_string,
    static_cast<std::uint32_t>(std::size(test_string) - 10));
  const auto temp_file
    = tl::utility::create_temp_file("FS_test_uncompressed.tmp", test_string);
  static constexpr auto check_fs
    = [](auto full_data, const mock_span_data &mock) {
        const auto value = FS::get_entry(full_data, mock.fi());
        expect(std::ranges::equal(value, mock.string_view()));
      };
  static constexpr auto check_fs2
    = [](auto full_data, const mock_span_data &mock, const FI &fi) {
        const auto value = [&]() {
          if constexpr (can_be_span_of_char<decltype(full_data)>) {
            return FS::get_entry(std::span<const char>(full_data), fi);
          }
          else {
            return FS::get_entry(full_data, fi);
          }
        }();
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
    const auto        add_uncompressed_data
      = [&uncompressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(
            append_entry(uncompressed_buffer, mock.span(), mock.fi()));
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
    const auto        add_lzss_data
      = [&compressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(append_entry(
            compressed_buffer,
            mock.span(),
            open_viii::CompressionTypeT::lzss));
        };
    add_lzss_data(mock1);
    add_lzss_data(mock2);
    add_lzss_data(mock3);
    const auto compressed_temp_file = tl::utility::create_temp_file(
      "FS_test_Lzss.tmp",
      std::string_view(
        std::data(compressed_buffer),
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
    const auto        add_l4z_data
      = [&compressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(append_entry(
            compressed_buffer,
            mock.span(),
            open_viii::CompressionTypeT::lz4));
        };
    add_l4z_data(mock1);
    add_l4z_data(mock2);
    add_l4z_data(mock3);
    const auto compressed_temp_file = tl::utility::create_temp_file(
      "FS_test_Lz4.tmp",
      std::string_view(
        std::data(compressed_buffer),
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
    // write compressed L4Z data to buffer stream
    std::vector<FI> fi_buffer{};
    fi_buffer.reserve(3);
    std::stringstream compressed_buffer{};
    const auto        add_l4z_data
      = [&compressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(append_entry(
            compressed_buffer,
            mock.span(),
            open_viii::CompressionTypeT::lz4));
        };
    add_l4z_data(mock1);
    add_l4z_data(mock2);
    add_l4z_data(mock3);
    std::string string_buffer = compressed_buffer.str();
    "read entire span"_test   = [&string_buffer, &fi_buffer] {
      check_fs2(string_buffer, mock1, fi_buffer[0]);
      check_fs2(string_buffer, mock2, fi_buffer[1]);
      check_fs2(string_buffer, mock3, fi_buffer[2]);
    };
  }
  {
    // write compressed LZSS data to buffer stream
    std::vector<FI> fi_buffer{};
    fi_buffer.reserve(3);
    std::stringstream compressed_buffer{};
    const auto        add_lzss_data
      = [&compressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(append_entry(
            compressed_buffer,
            mock.span(),
            open_viii::CompressionTypeT::lzss));
        };
    add_lzss_data(mock1);
    add_lzss_data(mock2);
    add_lzss_data(mock3);
    std::string string_buffer = compressed_buffer.str();
    "read entire span"_test   = [&string_buffer, &fi_buffer] {
      check_fs2(string_buffer, mock1, fi_buffer[0]);
      check_fs2(string_buffer, mock2, fi_buffer[1]);
      check_fs2(string_buffer, mock3, fi_buffer[2]);
    };
  }
  {
    // write compressed uncompressed data to buffer stream
    std::vector<FI> fi_buffer{};
    fi_buffer.reserve(3);
    std::stringstream compressed_buffer{};
    const auto        add_uncompressed_data
      = [&compressed_buffer, &fi_buffer](const mock_span_data &mock) {
          fi_buffer.push_back(append_entry(
            compressed_buffer,
            mock.span(),
            open_viii::CompressionTypeT::none));
        };
    add_uncompressed_data(mock1);
    add_uncompressed_data(mock2);
    add_uncompressed_data(mock3);
    std::string string_buffer = compressed_buffer.str();
    "read entire span"_test   = [&string_buffer, &fi_buffer] {
      check_fs2(string_buffer, mock1, fi_buffer[0]);
      check_fs2(string_buffer, mock2, fi_buffer[1]);
      check_fs2(string_buffer, mock3, fi_buffer[2]);
    };
  }
  using namespace open_viii;
  static constexpr std::string_view test_data = "TEST DATA FOR FS";
  static constexpr mock_span_data   mock(test_data);
  "FS get_entry uncompressed"_test = [] {
    std::vector<char> buffer(test_data.begin(), test_data.end());
    FI   fi(static_cast<uint32_t>(buffer.size()), 0, CompressionTypeT::none);
    auto result = FS::get_entry(std::span{ buffer }, fi);
    expect(std::equal(result.begin(), result.end(), test_data.begin()));
  };

  "FS append_entry uncompressed"_test = [] {
    std::vector<char> output;
    FI fi = append_entry(output, mock.span(), CompressionTypeT::none);
    expect(fi.uncompressed_size() == mock.span().size());
    expect(std::equal(output.begin(), output.end(), mock.span().begin()));
  };

  "FS append and read LZSS compressed entry"_test = [] {
    std::vector<char> compressed_output;
    FI                fi
      = append_entry(compressed_output, mock.span(), CompressionTypeT::lzss);
    auto decompressed = FS::get_entry(std::span{ compressed_output }, fi);
    expect(std::equal(
      decompressed.begin(),
      decompressed.end(),
      mock.span().begin()));
  };

  "FS append and read L4Z compressed entry"_test = [] {
    std::vector<char> compressed_output;
    FI fi = append_entry(compressed_output, mock.span(), CompressionTypeT::lz4);
    auto decompressed = FS::get_entry(std::span{ compressed_output }, fi);
    expect(std::equal(
      decompressed.begin(),
      decompressed.end(),
      mock.span().begin()));
  };

  "FS read entry from file"_test = [] {
    const auto    tmp_file = std::filesystem::temp_directory_path() / "test.fs";
    std::ofstream file(tmp_file, std::ios::binary);
    file.write(test_data.data(), test_data.size());
    file.close();
    FI   fi(static_cast<uint32_t>(test_data.size()), 0, CompressionTypeT::none);
    auto result = FS::get_entry(tmp_file, fi);
    expect(std::equal(result.begin(), result.end(), test_data.begin()));
    std::remove(tmp_file.string().c_str());
  };
}