# OpenVIII CPP WIP

[![Image of CI passing or not.](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)](https://ci.appveyor.com/project/Sebanisu/openviii-cpp-wip)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/sebanisu/OpenVIII_CPP_WIP/issues) 
[![GPL v3 License](https://img.shields.io/badge/License-GPL_V3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html) 
[![C++ 20](https://img.shields.io/badge/C++%20-20-ff69b4.svg)](https://en.cppreference.com/w/cpp/20) 
[![GitHub followers](https://img.shields.io/github/followers/sebanisu.svg?style=social&label=Follow)](https://github.com/sebanisu?tab=followers) 
[![GitHub stars](https://img.shields.io/github/stars/sebanisu/ToolsLibrary.svg?style=social&label=Star)](https://GitHub.com/sebanisu/OpenVIII_CPP_WIP/stargazers/) 
[![Documentation](https://codedocs.xyz/Sebanisu/OpenVIII_CPP_WIP.svg)](https://codedocs.xyz/Sebanisu/OpenVIII_CPP_WIP/)

Credits:
 - [MaKiPL](https://github.com/MaKiPL)
 - [myst6re](https://github.com/myst6re)
 - [antiquechrono](https://github.com/antiquechrono)
 - [DarkShinryu](https://github.com/DarkShinryu)
 - [julianxhokaxhiu](https://github.com/julianxhokaxhiu)

Reference:
 - [Final Fantasy Inside Wiki - FF8](http://wiki.ffrtt.ru/index.php?title=FF8)
 - [Final Fantasy Inside Wiki - LZSS format](http://wiki.ffrtt.ru/index.php?title=FF7/LZSS_format)
 - [Doomtrain Wiki](https://github.com/DarkShinryu/doomtrain/wiki)
 - [OpenVIII-monogame](https://github.com/MaKiPL/OpenVIII-monogame)
 - [Deling](https://github.com/myst6re/deling), [Hyne](https://github.com/myst6re/hyne)
 - [Doomtrain](https://github.com/DarkShinryu/doomtrain)
 - [FFNx](https://github.com/julianxhokaxhiu/FFNx)
 
Requires:
 - cpp20 gcc currently. I'm using some features visual studio doesn't support yet.
 - https://github.com/lz4/lz4

This is WIP and everything is subject to change.

This is a port of OpenVIII to C++. It's kinda an experiment. I want to keep is a modular as possible so you don't need to include what you don't want.

Only things directly related to accessing FF8 should be in this project. If it's code that could be more generally licensed it should probably be in another library. This library has to be GPL because it refers to projects and references projects under the GPL. Not all the dependencies need to be GPL. I started [Tools Library](https://github.com/Sebanisu/ToolsLibrary) for things that don't need to be in this project.

The Plan:
  - **TESTS!!!!** - I'm using [ut](https://github.com/boost-ext/ut) for my testing framework. This was overdue. I have test apps that just dump to console. But I needed unit tests for a long time. If I'm going to alter a file I want to try to make sure it's under test before I change it.
    - I'll probably move the apps that I wrote into their own project using this as a dependency. Like the one that extracts all the archives. 
  - I need to add a configuration library, maybe the one FFNx uses.
    - This will be to store the path[s] to FF8 and such. Right now I'm using a hardcoded list. I could maybe have a windows only app that fetches the FF8 path from the registry. Though I wouldn't write one of those directly in the library because this will be **cross platform**.
  - The doing mostly back end work at first.
  - Support for each of the file formats
  - Tests to make sure the code works as I go. Dumping data to check to see if I can read correctly.
  - Ability to quickly get at what you want.
  - Later on may write tests to quickly render things. Much like our debug menu on OpenVIII-monogame. I'll be able to open up maps and view them make sure the drawing logic is good. But like it'll be it's own lightweight exe that just populates a list of maps or something for each type I wanna test.
  - Last thing I'll worry about is the game. 
