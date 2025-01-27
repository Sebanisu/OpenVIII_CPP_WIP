#include "open_viii/compression/LZSS.hpp"
#include "open_viii/tools/Read.hpp"
#include "open_viii/tools/Write.hpp"
#include "open_viii/graphics/Tim.hpp"
#include <filesystem>
#include <iostream>
#include <string>
int
  main()
{
  std::string input;
  std::cout << "Path for .tiz => ";
  std::getline(std::cin, input);
  const std::filesystem::path tiz_path = { input };
  std::error_code             ec       = {};
  bool file_exists                     = std::filesystem::exists(tiz_path, ec);
  if (ec) {
    std::cerr << ec.value() << " " << ec.message() << "\n";
    return 0;
  }
  ec.clear();
  if (!file_exists) {
    std::cerr << tiz_path << " does not exist\n";
    return 0;
  }

  std::cout << tiz_path << " file found\n";

  if (!tiz_path.has_parent_path()) {
    std::cerr << tiz_path << " does not have a parent path\n";
    return 0;
  }
  const auto parent_path = tiz_path.parent_path();

  if (!tiz_path.has_stem()) {
    std::cerr << tiz_path << " does not have a stem\n";
    return 0;
  }

  const auto stem = tiz_path.stem();

  std::cout << stem << " is the stem\n";

  const auto tim_name = stem.string() + ".tim";
  const auto png_name = stem.string() + ".png";

  using namespace open_viii::compression;
  using namespace open_viii::tools;
  using namespace open_viii::graphics;

  const auto tiz_bytes = read_entire_file(tiz_path);
  std::cout << "size of tiz_bytes " << tiz_bytes.size() << "\n";
  auto       tiz_span  = std::span(tiz_bytes);
  const auto magic_value = read_val<uint16_t>(tiz_span);
  const auto length      = read_val<uint32_t>(tiz_span);

  std::cout << "magic_value: " << std::uppercase << std::hex << magic_value << std::dec
            << std::nouppercase << "  (bytes might print reversed)\n";
  std::cout << "file_size:   " << length << "\n";

  const auto uncompressed_bytes = LZSS::decompress(tiz_span);
  bool       success
    = write_buffer(uncompressed_bytes, tim_name, (parent_path).string());
  if (success) {
    std::cout << "file wrote successfully.\n";
  }
  else {
    std::cerr << "file failed to write.\n";
  }

  const auto tim = Tim(uncompressed_bytes);
  tim.save(tiz_path.string());
}