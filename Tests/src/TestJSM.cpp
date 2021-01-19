//
// Created by pcvii on 1/18/2021.
//
#include "TestJSM.hpp"
#include "open_viii/field/scripts/jsm.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
         std::cout << path << std::endl;
         const auto archives =
           open_viii::archive::Archives<open_viii::LangT::en>(path);

         const open_viii::archive::FIFLFS<true> & field = archives.get<open_viii::archive::ArchiveTypeT::field>();
         field.execute_with_nested({},[]([[maybe_unused]] const std::vector<char> & buffer, const std::string &in_path){
           std::cout << in_path << std::endl;
           [[maybe_unused]]auto jsm = open_viii::field::scripts::jsm(buffer);
         },{"jsm"});
  });
  const auto end = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
}