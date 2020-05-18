//
// Created by pcvii on 4/27/2020.
//
#include "TestExtractFIFLFS.h"
#include "paths.h"
int main()
{
//      //test FIFLFS
//      std::array<std::string, 2> paths ({
//        R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII/Data/lang-en)",
//        R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)"});

for(auto path : Paths::get()) {
  OpenVIII::Tools::replaceSlashes(path);
  if (!std::filesystem::exists(path)) { continue; }
  const auto files = OpenVIII::Archive::FIFLFS::GetFilesFromPath(path);
  std::for_each(std::begin(files), std::end(files), &OpenVIII::Archive::FIFLFS::testPair);
}
  return 0;

}