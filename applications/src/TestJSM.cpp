//
// Created by pcvii on 1/18/2021.
//
#include "TestJSM.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/field/scripts/Jsm.hpp"
#include "open_viii/paths/Paths.hpp"
int
  main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    static constexpr auto coo      = open_viii::LangT::en;
    const auto            archives = open_viii::archive::Archives(
      path,
      open_viii::LangCommon::to_string<coo>());
    if (!static_cast<bool>(archives)) {
      std::cerr << "Failed to load path: " << path.string() << '\n';
      return;
    }
    const open_viii::archive::FIFLFS<true> &field
      = archives.get<open_viii::archive::ArchiveTypeT::field>();
    field.execute_with_nested(
      {},
      [](
        [[maybe_unused]] const std::vector<char> &buffer,
        const std::string                        &in_path) {
        std::cout << in_path << std::endl;
        [[maybe_unused]] auto jsm = open_viii::field::scripts::Jsm(buffer);
      },
      { "jsm" });
  });
  const auto end  = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
}