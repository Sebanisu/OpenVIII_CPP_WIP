#include "open_viii/compression/L4Z.hpp"
#include "tl/input.hpp"
#include "tl/read.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
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
  const auto decompressed = tl::read::entire_file(in, std::vector<char>());
  if (std::empty(decompressed)) {
    std::cerr << "file not found or unable to read " << in << '\n';
    std::exit(1);
  }
  std::ofstream os = open_file(out);
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
  const auto compressed = tl::read::entire_file(in, std::vector<char>());
  if (std::empty(compressed) || std::size(compressed) <= 12U) {
    std::cerr << "file not found or unable to read: " << std::size(compressed) << ", " << in << '\n';
    std::exit(1);
  }
  std::ofstream os = open_file(out);
   const auto input = tl::read::input(compressed);
   const auto comp_size = input.output<std::uint32_t>();
   if (std::size(compressed) != comp_size+4U)
  {
      std::cerr << "Size of file missmatch: per header file size " <<
      std::size(compressed) << " should equal " << comp_size + 4U << " !!\n";
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
  if (argc == 3) {
    compress(argv[1], argv[2]);
    return 0;
  }
  if (argc == 5 && std::string_view(argv[1]) == std::string_view("-d")) {
    decompress(argv[2], argv[3], std::stoull(std::string(argv[4])));
    return 0;
  }
  std::cerr << "Use arguments: [-d] \"input\" \"output\" [expected_size]\n";
  std::cerr << "\t-d\t\t--\t required for decompress\n";
  std::cerr << "\tinput\t\t--\t input path\n";
  std::cerr << "\toutput\t\t--\t output path\n";
  std::cerr << "\texpected_size\t--\t required for decompress, size in bytes "
               "after decompressing\n";
  std::exit(1);
}