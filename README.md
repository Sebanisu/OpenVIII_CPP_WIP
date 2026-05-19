# OpenVIII CPP WIP

[![Image of CI passing or not.](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)](https://ci.appveyor.com/project/Sebanisu/openviii-cpp-wip)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-yellowgreen.svg?style=flat)](https://github.com/sebanisu/OpenVIII_CPP_WIP/issues)
[![GPL v3 License](https://img.shields.io/badge/License-GPL_V3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)
[![C++ 23](https://img.shields.io/badge/C++%20-23-ff69b4.svg)](https://en.cppreference.com/w/cpp/23)
[![GitHub followers](https://img.shields.io/github/followers/sebanisu.svg?style=social&label=Follow)](https://github.com/sebanisu?tab=followers)
[![Documentation Status](https://readthedocs.org/projects/openviii-cpp-wip/badge/?version=latest)](https://openviii-cpp-wip.readthedocs.io/en/latest/?badge=latest)

## Credits

 - [MaKiPL](https://github.com/MaKiPL)
 - [myst6re](https://github.com/myst6re)
 - [antiquechrono](https://github.com/antiquechrono)
 - [DarkShinryu](https://github.com/DarkShinryu)
 - [julianxhokaxhiu](https://github.com/julianxhokaxhiu)
 - [niemasd](https://github.com/niemasd/PyFF7)

## Reference

 - [Final Fantasy Inside Wiki - FF8](http://wiki.ffrtt.ru/index.php?title=FF8)
 - [Final Fantasy Inside Wiki - LZSS format](http://wiki.ffrtt.ru/index.php?title=FF7/LZSS_format)
 - [Doomtrain Wiki](https://github.com/DarkShinryu/doomtrain/wiki)
 - [OpenVIII-monogame](https://github.com/MaKiPL/OpenVIII-monogame)
 - [Deling](https://github.com/myst6re/deling)
 - [Hyne](https://github.com/myst6re/hyne)
 - [Doomtrain](https://github.com/DarkShinryu/doomtrain)
 - [FFNx](https://github.com/julianxhokaxhiu/FFNx)
 - [PyFF7](https://github.com/niemasd/PyFF7)

## Requires

 - [C++ 23](https://en.cppreference.com/w/cpp/23)
 - [GCC 16](https://gcc.gnu.org/)
   - Currently supported.
 - [MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus/)
   - Some newer language/library features may still lag behind GCC support.
 - [Clang](https://clang.llvm.org/cxx_status.html)
   - Currently untested. It may already work or may require some changes to the library.
 - [lz4](https://github.com/lz4/lz4)
   - Used for remaster archive support.
 - [ut](https://github.com/boost-ext/ut)
   - Unit testing framework.
 - [fmt](https://github.com/fmtlib/fmt)
   - Formatting library.
 - [spdlog](https://github.com/gabime/spdlog)
   - Logging library.
 - [Tools Library](https://github.com/Sebanisu/ToolsLibrary)
   - Used for parsing buffers into types.

## Prefer

 - Back end first.
 - [Lazy operations over eager](https://www.imperva.com/learn/performance/lazy-loading/)
 - [Functional style](https://docs.microsoft.com/en-us/archive/msdn-magazine/2012/august/c-functional-style-programming-in-c)
   - [Immutability](https://www.modernescpp.com/index.php/c-core-guidelines-rules-for-constants-and-immutability)
 - [Constexpr](https://www.youtube.com/watch?v=HMB9oXFobJc)
 - [Algorithms over loops](https://www.drdobbs.com/stl-algorithms-vs-hand-written-loops/184401446)

---

This project started as a port of OpenVIII to modern C++, but gradually became more focused on being a reusable back end library for reading, writing, extracting, and working with FF8 file formats and archives.

The long term goal is still to continue porting functionality from OpenVIII where it makes sense, but most recent development has focused on tooling support and reusable library code instead of rebuilding the engine itself.

The library is intended to stay modular so projects only need to include the pieces they actually use. The primary development targets are Linux and Windows.

This project is currently used by:
 - [Field-Map-Editor](https://github.com/Sebanisu/Field-Map-Editor)

There is still a large amount of functionality from OpenVIII that has not been ported into this library yet.

Only things directly related to accessing or manipulating FF8 data should live in this repository. More general purpose code should be moved into separate libraries when possible.

This library is GPL because it references and derives from GPL related projects and research. Not all dependencies need to be GPL licensed.

Some reusable code has already been moved into:
 - [Tools Library](https://github.com/Sebanisu/ToolsLibrary)

## Current Features

### Archives / Compression

 - FI
 - FS
 - FL
 - ZZZ
 - FIFLFS
 - LZSS
 - LZ4
 - PAK

### Graphics / Image Formats

 - TIM
 - TEX
 - LZS image data
 - PNG export support

### Utilities

 - Archive extraction tools
 - Archive search tools
 - Compression utilities
 - Texture conversion utilities
 - Field rendering/debug utilities

## Testing

The project currently uses:
 - [ut](https://github.com/boost-ext/ut)

Coverage currently includes:
 - FS
 - FI
 - FL
 - LZSS compression
 - LZ4 compression
 - PupuID
 - UniquifyPupu

There are still older test applications that dump results to console. Over time these should move into proper automated unit tests.

## The Plan

 - [ ] Expand unit test coverage
 - [ ] Continue porting OpenVIII functionality where useful
 - [ ] Improve cross platform support
 - [ ] Improve archive editing support
 - [ ] Add configuration support instead of hardcoded paths
 - [ ] Continue separating reusable code into standalone libraries
 - [ ] Add lightweight rendering validation tools
 - [ ] Add more FF8 format support

## Supported Formats

 - [x] FI
 - [x] FS
 - [x] FL
 - [x] ZZZ
 - [x] TIM
 - [x] LZS
 - [x] TEX
 - [x] PAK

## Notes

This project is still WIP and APIs, formats, and internal structures are subject to change.