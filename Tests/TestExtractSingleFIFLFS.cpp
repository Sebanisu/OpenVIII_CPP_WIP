//
// Created by pcvii on 5/12/2020.
//

#include "TestExtractSingleFIFLFS.h"
int main()
{
  // test FIFLFS
  std::string path = R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)";
  OpenVIII::Tools::replaceSlashes(path);
  if (!std::filesystem::exists(path)) { return 1; }
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  OpenVIII::Tools::replaceSlashes(needle);
  if (needle.empty()) { return 1; }
  const auto files = OpenVIII::Archive::FIFLFS::GetFilesFromPath(path);
  for (const auto &entry : files) {
    const auto &fiflfs = entry.second;
    std::cout << "Searching " << fiflfs << '\n';
    const auto &fl = fiflfs.FL();
    const auto &[id, innerPath] = OpenVIII::Archive::FL::GetEntry(fl.path, needle, fl.offset);
    if (!innerPath.empty()) { std::cout << "single match: {" << id << ", " << innerPath << "}\n"; }
    const auto vector = OpenVIII::Archive::FL::GetAllEntries(fl.path, fl.offset, 0U, needle);
    if (!vector.empty()) {
      std::cout << "filtered sorted vector: \n";
      for (const auto &ventry : vector) {
        const auto &[vid, vinnerPath] = ventry;
        if (!vinnerPath.empty()) { std::cout << '{' << vid << ", " << vinnerPath << "}\n"; }
      }
    }
  }
  // std::for_each(std::begin(files), std::end(files), &OpenVIII::Archive::FIFLFS::testPair);
  return 0;
}
