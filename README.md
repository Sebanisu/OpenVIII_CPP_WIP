# OpenVIII CPP WIP

[![Image of CI passing or not.](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)](https://ci.appveyor.com/project/Sebanisu/openviii-cpp-wip)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/sebanisu/OpenVIII_CPP_WIP/issues) 
[![GPL v3 License](https://img.shields.io/badge/License-GPL_V3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html) 
[![C++ 20](https://img.shields.io/badge/C++%20-20-ff69b4.svg)](https://en.cppreference.com/w/cpp/20) 
[![GitHub followers](https://img.shields.io/github/followers/sebanisu.svg?style=social&label=Follow)](https://github.com/sebanisu?tab=followers) 
[![GitHub stars](https://img.shields.io/github/stars/sebanisu/ToolsLibrary.svg?style=social&label=Star)](https://GitHub.com/sebanisu/OpenVIII_CPP_WIP/stargazers/) 
[![Documentation](https://codedocs.xyz/Sebanisu/OpenVIII_CPP_WIP.svg)](https://codedocs.xyz/Sebanisu/OpenVIII_CPP_WIP/)

Credits:
 - https://github.com/MaKiPL
 - https://github.com/myst6re
 - https://github.com/antiquechrono
 - https://github.com/DarkShinryu

Reference:
 - http://wiki.ffrtt.ru/index.php?title=FF8/PC_Media
 - http://wiki.ffrtt.ru/index.php?title=FF7/LZSS_format
 - https://github.com/MaKiPL/OpenVIII-monogame
 - https://github.com/myst6re/deling
 - https://github.com/DarkShinryu/doomtrain
 - https://github.com/DarkShinryu/doomtrain/wiki
 
Requires:
 - cpp20 gcc currently. I'm using some features visual studio doesn't support yet.
 - https://github.com/lz4/lz4

This is WIP and everything is subject to change.

This is a port of OpenVIII to C++. It's kinda an experiment. I want to keep is a modular as possible so you don't need to include what you don't want.

The Plan:
  - The doing mostly back end work at first.
  - Support for each of the file formats
  - Tests to make sure the code works as I go. Dumping data to check to see if I can read correctly.
  - Ability to quickly get at what you want.
  - Later on may write tests to quickly render things. Much like our debug menu on OpenVIII-monogame. I'll be able to open up maps and view them make sure the drawing logic is good. But like it'll be it's own lightweight exe that just populates a list of maps or something for each type I wanna test.
  - Last thing I'll worry about is the game. 
