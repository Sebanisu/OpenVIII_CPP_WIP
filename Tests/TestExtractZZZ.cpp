//
// Created by pcvii on 4/27/2020.
//
#include "TestExtractZZZ.h"
int main()
{
  // test ZZZ extract with FIFLFS
  std::string path = R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)";
  OpenVIII::Tools::replaceAll(path, '\\', std::filesystem::path::preferred_separator);

  if (!std::filesystem::exists(path)) { return 0; }
  const auto files = OpenVIII::Archive::ZZZ::GetFilesFromPath(path);
  std::for_each(files.begin(), files.end(), &OpenVIII::Archive::ZZZ::testPair);
  return 0;
}