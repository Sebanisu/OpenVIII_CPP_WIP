#include "lz4_lzss_common_ff8.hpp"
#include "open_viii/compression/LZSS.hpp"

void
  compress(std::string_view in, std::string_view out)
{
  const auto decompressed = tl::read::entire_file(in, std::vector<char>());
  if (std::empty(decompressed)) {
    std::cerr << "file not found or unable to read " << in << '\n';
    std::exit(1);
  }
  std::ofstream os      = open_file(out);
  const auto compressed = open_viii::compression::LZSS::compress(decompressed);
  // write int size
  write_value(os, static_cast<std::uint32_t>(std::size(compressed)));
  // write data
  os.write(std::data(compressed), static_cast<long>(std::size(compressed)));
  os.flush();
  os.close();
}
void
decompress(std::string_view in, std::string_view out)
{
    const auto compressed = tl::read::entire_file(in, std::vector<char>());
    if (std::empty(compressed) || std::size(compressed) <= 4U) {
        std::cerr << "file not found or unable to read: " << std::size(compressed)
            << ", " << in << '\n';
        std::exit(1);
    }
    std::ofstream os = open_file(out);
    const auto    input = tl::read::input(compressed);
    const auto    comp_size = input.output<std::uint32_t>();
    if (std::size(compressed) != std::size_t{ comp_size } + std::size_t{ 4U }) {
    std::cerr << "Size of file mismatch: per header file size "
              << std::size(compressed) << " should equal " << comp_size + 4U
              << " !!\n";
    std::exit(1);
  }
  const auto decompressed =
    open_viii::compression::LZSS::decompress<std::vector<char>>(
      std::span(compressed).subspan(4U));
  os.write(std::data(decompressed), static_cast<long>(std::size(decompressed)));
}
int
  main(int argc, [[maybe_unused]] char *argv[])
{
  return common_main(argc, argv);
}