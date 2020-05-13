//
// Created by pcvii on 4/27/2020.
//
#include "TestExtractFIFLFS.h"
int main()
{
      //test FIFLFS
    std::string path = R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)";
    OpenVIII::Tools::replaceSlashes(path);

    if (!std::filesystem::exists(path)) { return 0; }
    const auto files = OpenVIII::Archive::FIFLFS::GetFilesFromPath(path);
    std::for_each(std::begin(files), std::end(files), &OpenVIII::Archive::FIFLFS::testPair);
  return 0;
}