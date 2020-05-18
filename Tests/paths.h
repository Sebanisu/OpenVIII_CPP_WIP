//
// Created by pcvii on 5/18/2020.
//

#ifndef VIIICPPTEST_PATHS_H
#define VIIICPPTEST_PATHS_H
struct Paths{
  static auto get() { return
    std::array<std::string, 4>({
      R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII/Data/lang-en)",
      R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)",
      R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)",
      R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII Remastered)"});
  }
};
#endif// VIIICPPTEST_PATHS_H
