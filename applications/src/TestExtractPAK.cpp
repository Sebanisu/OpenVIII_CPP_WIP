// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "TestExtractPAK.hpp"
#include "open_viii/pak/Pak.hpp"
#include "open_viii/paths/Paths.hpp"

void scan_paths(const std::filesystem::path& dest_path) {
    open_viii::Paths::for_each_path([&dest_path](const std::filesystem::path& path) {
        std::cout << path << std::endl;
        open_viii::tools::execute_on_directory(
            path, {}, { ".pak" }, [&dest_path](const std::filesystem::path& p) {
                std::cout << p << std::endl;
                open_viii::pak::Pak pak{ p };
                std::cout << pak << std::endl;
                pak.extract(dest_path);
            });
        open_viii::tools::execute_on_directory(
            path, {}, { ".cam" }, [](const std::filesystem::path& p) {
                std::cout << p << std::endl;
                open_viii::tools::read_from_file(
                    [](std::istream& fp) {
                        open_viii::pak::Cam cam{ fp };
                        std::cout << cam << std::endl;
                    },
                    p);
            });
        });
}

void process_pak_file(const std::filesystem::path& pak_path, const std::filesystem::path& dest_path) {
    std::cout << "Processing file: " << pak_path << std::endl;
    open_viii::pak::Pak pak{ pak_path };
    std::cout << pak << std::endl;
    pak.extract(dest_path);
}

bool validate_and_create_directory(const std::filesystem::path& path) {
    std::error_code ec;
    if (!std::filesystem::exists(path, ec)) {
        if (!std::filesystem::create_directories(path, ec)) {
            std::cerr << "Failed to create directory: " << path << " - " << ec.message() << std::endl;
            return false;
        }
    }
    else if (!std::filesystem::is_directory(path, ec)) {
        std::cerr << "Path is not a directory: " << path << " - " << ec.message() << std::endl;
        return false;
    }
    return true;
}

int main() {
    std::string choice;
    std::cout << "Enter 'scan' to scan known paths or enter the path to a .pak file: ";
    std::getline(std::cin, choice);

    std::string dest_path_input;
    std::cout << "Enter the destination path for extraction (leave blank for 'movies'): ";
    std::getline(std::cin, dest_path_input);

    std::filesystem::path dest_path = dest_path_input.empty() ? "movies" : dest_path_input;

    if (!validate_and_create_directory(dest_path)) {
        return 1; // Exit if the destination path is invalid
    }

    if (choice == "scan") {
        scan_paths(dest_path);
    }
    else {
        std::filesystem::path pak_path{ choice };
        if (std::filesystem::exists(pak_path) && pak_path.extension() == ".pak") {
            process_pak_file(pak_path, dest_path);
        }
        else {
            std::cerr << "Invalid .pak file path." << std::endl;
        }
    }

    return 0;
}
