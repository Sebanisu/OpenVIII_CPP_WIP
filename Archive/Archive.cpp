// Archive.cpp : Defines the entry point for the application.
//
#include "Archive.hpp"

int main()
{
  std::string path = R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)";
  OpenVIII::Tools::replaceAll(path, '\\', std::filesystem::path::preferred_separator);

  if (!std::filesystem::exists(path)) { return 0; }
  const auto files = OpenVIII::Archive::FIFLFS::GetFilesFromPath(path);
  std::for_each(files.begin(), files.end(), &OpenVIII::Archive::FIFLFS::testPAIR);
  return 0;
}
