//
// Created by pcvii on 4/27/2020.
//
#include "TestExtractZZZ.h"
#include "paths.h"
int main()
{
  // test ZZZ extract with FIFLFS
  for(auto path : Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);

    if (!std::filesystem::exists(path)) { continue; }
    const auto files = OpenVIII::Archive::ZZZ::GetFilesFromPath(path);
    std::for_each(files.begin(), files.end(), &OpenVIII::Archive::ZZZ::testPair);
  }
  return 0;
}