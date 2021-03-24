#include "lz4_lzss_common_ff8.hpp"
#include "open_viii/compression/L4Z.hpp"

void
  compress(std::string_view in, std::string_view out)
{
  const auto decompressed = tl::read::entire_file(in, std::vector<char>());
  if (std::empty(decompressed)) {
    std::cerr << "file not found or unable to read " << in << '\n';
    std::exit(1);
  }
  std::ofstream os = open_file(out);
  const auto    compressed =
    open_viii::compression::l4z::compress<std::vector<char>>(decompressed);
  // write int size+8
  write_value(os, static_cast<std::uint32_t>(std::size(compressed) + 8U));
  // write string
  constexpr static std::string_view str = "4ZL_";
  os.write(std::data(str), 4);
  // write int size
  write_value(os, static_cast<std::uint32_t>(std::size(decompressed)));
  // write data
  os.write(std::data(compressed), static_cast<long>(std::size(compressed)));
  os.flush();
  os.close();
}
void
  decompress(std::string_view   in,
             std::string_view   out,
             unsigned long long expected_size)
{
  const auto compressed = tl::read::entire_file(in, std::vector<char>());
  if (std::empty(compressed) || std::size(compressed) <= 12U) {
    std::cerr << "file not found or unable to read: " << std::size(compressed)
              << ", " << in << '\n';
    std::exit(1);
  }
  std::ofstream os        = open_file(out);
  const auto    input     = tl::read::input(compressed);
  const auto    comp_size = input.output<std::uint32_t>();
  if (std::size(compressed) != comp_size + 4U) {
    std::cerr << "Size of file mismatch: per header file size "
              << std::size(compressed) << " should equal " << comp_size + 4U
              << " !!\n";
    std::exit(1);
  }
  const auto decompressed =
    open_viii::compression::l4z::decompress<std::vector<char>>(
      std::span(compressed).subspan(12U), expected_size);
  os.write(std::data(decompressed), static_cast<long>(std::size(decompressed)));
}

int
  main(int argc, [[maybe_unused]] char *argv[])
{
  return common_main(argc, argv);
}