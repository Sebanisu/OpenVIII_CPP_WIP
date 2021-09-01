from conans import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps


class OpenVIIICPPWIP(ConanFile):
    name = "OpenVIII CPP WIP"
    version = "0.1.0"
    settings = "os", "arch", "compiler", "build_type"

    def export_sources(self):
        self.copy("*")

    def requirements(self):
        self.requires("libpng/1.6.37")

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()

    def build(self):
        build_type = self.settings.get_safe("build_type", default="Release")
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.libs = ["mylib"]
