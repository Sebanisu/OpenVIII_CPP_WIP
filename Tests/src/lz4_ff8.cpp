#include "open_viii/compression/L4Z.hpp"
#include "tl/read.hpp"
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include "tl/input.hpp"
std::ofstream
  open_file(std::string_view out)
{
  std::ofstream os{};
  os.open(std::filesystem::path(out));
  if (!os.is_open()) {
    std::cerr << "failed to open " << out << " for writing.";
    std::exit(1);
  }
  return os;
}
void
  compress(std::string_view in, std::string_view out)
{
  std::ofstream os           = open_file(out);
  const auto    decompressed = tl::read::entire_file(in, std::vector<char>());
  const auto    compressed =
    open_viii::compression::l4z::compress<std::vector<char>>(decompressed);
  std::array<char, 4> tmp{};
  std::uint32_t       i = static_cast<uint32_t>(std::size(compressed) + 8U);
  std::memcpy(&tmp, &i, 4);
  os.write(std::data(tmp), 4);
  std::string_view str = "4ZL_";
  os.write(std::data(str), 4);
  i = static_cast<uint32_t>(std::size(compressed));
  std::memcpy(&tmp, &i, 4);
  os.write(std::data(tmp), 4);
  os.write(std::data(compressed), static_cast<long>(std::size(compressed)));
}
void
  decompress(std::string_view   in,
             std::string_view   out,
             unsigned long long expected_size)
{
  std::ofstream os         = open_file(out);
  const auto    compressed = tl::read::entire_file(in, std::vector<char>());
  const auto input = tl::read::input(compressed);
  const auto comp_size = input.output<std::uint32_t>();
  if (std::size(compressed) != comp_size+4U)
  {
      std::cerr << "Size of file missmatch: per header file size " << std::size(compressed) << " should equal " << comp_size + 4U << " !!\n";
      std::exit(1);
  }
  const auto    decompressed =
    open_viii::compression::l4z::decompress<std::vector<char>>(
      std::span(compressed).subspan(12), expected_size);
  os.write(std::data(decompressed), static_cast<long>(std::size(decompressed)));
}
int
  main(int argc, [[maybe_unused]] char *argv[])
{
  if (argc == 2) {
    compress(argv[0], argv[1]);
    return 0;
  }
  if (argc == 4 && std::string_view(argv[0]) == std::string_view("-d")) {
    decompress(argv[1], argv[2], std::stoull(std::string(argv[3])));
    return 0;
  }
  std::cerr << "Use arguments: [-d] \"input\" \"output\" [expected_size]\n";
  std::cerr << "\t-d\t\t--\t required for decompress\n";
  std::cerr << "\tinput\t\t--\t input path\n";
  std::cerr << "\toutput\t\t--\t output path\n";
  std::cerr << "\texpected_size\t--\t required for decompress, size in bytes after decompressing\n";
  std::exit(1);
}