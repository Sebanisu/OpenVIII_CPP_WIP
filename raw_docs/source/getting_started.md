Getting Started
======

## Prerequisites

* [cmake version 3.18.0+](https://apt.kitware.com/)
* [ninja-build](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages)

### Compiler

* [gcc-10 & g++-10](https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/ppa)
  * [update-alternatives](https://linuxconfig.org/how-to-switch-between-multiple-gcc-and-g-compiler-versions-on-ubuntu-20-04-lts-focal-fossa)

### Documentation Prerequisites

* [doxygen](https://www.tutorialspoint.com/how-to-install-doxygen-on-ubuntu) or [download](https://www.doxygen.nl/download.html)
* [graphviz](https://graphviz.org/download/)
* [python3-dev & python3-pip](https://www.digitalocean.com/community/tutorials/how-to-install-python-3-and-set-up-a-programming-environment-on-an-ubuntu-20-04-server)

#### Python Packages

* [sphinx](https://pypi.org/project/Sphinx/)
* [sphinx-inline-tabs](https://pypi.org/project/sphinx-inline-tabs/)
* [furo](https://pypi.org/project/furo/)
* [breathe](https://pypi.org/project/breathe/)
* [myst_parser](https://pypi.org/project/myst-parser/)
* [linkify-it-py](https://pypi.org/project/linkify-it-py/)

## Checkout the repository:

```shell
git clone https://github.com/Sebanisu/OpenVIII_CPP_WIP.git --recursive
```

## Build Tests and Applications

### Run Cmake

```shell
cd OpenVIII_CPP_WIP
mkdir build
```

__If you only want tests and applications built__

```shell
cmake -G "Ninja Multi-Config" -B build
```

__If you want documentation built as well__

```shell
cmake -G "Ninja Multi-Config" -B build -DDOCS:STRING=YES
```

    You might need to clear out the `docs` directory before building. As it won't overwrite some files if unless it detects a change. Documentation is checked in,
    so it shows on GitHub pages. `.nojekyl` is required to work on GitHub pages, and cmake will generate it.

### Run Ninja

__To build debug__

```shell
ninja -C build
```

__To build release__

```shell
ninja -C build -f build-Release.ninja
```

### Run Tests

```shell
ninja -C build test
```

__For more verbose output__

This is recommended, as most of the [test framework](https://github.com/boost-ext/ut) print out extra info.

```shell
ninja -C build
cd build 
ctest -C debug --verbose 
cd ..
```

### Run Documentation

```shell
cd docs
# open index.html
```

### Run Applications

```shell
cd build
cd applications
# dir or ls
```

## Windows MSVC

### Run cmake

```dosbatch
cd OpenVIII_CPP_WIP
mkdir build
cmake -G "Visual Studio 16 2019" -B build
```

### Run tests

__For more verbose output__

```dosbatch
MSBuild build\ALL_BUILD.vcxproj 
cd build 
ctest -C debug --verbose 
cd ..
```