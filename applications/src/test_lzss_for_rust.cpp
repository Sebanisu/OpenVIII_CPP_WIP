#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "open_viii/compression/LZSS.hpp"


void test_compress_decompress() {
    // Test data
    std::vector<std::string> original_data_values = {
        "",                                                         // Empty string
        "Hello",                                                    // String with length 5
        "1234567890",                                               // String with length 10                     
        "Lorem ips", // Longer string
        "Lorem ipsum d", // Longer string        
        "Lorem ipsum do", // Longer string      
        "Lorem ipsum dol", // Longer string      
        "Lorem ipsum dolor", // Longer string    
        "Lorem ipsum dolor s", // Longer string
        "Lorem ipsum dolor sit", // Longer string
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit." // Longer string
    };
    std::cout << "[";
    // Iterate over each original data value
    for (const auto& original_data : original_data_values) {
        // Compress the data
        std::vector<char> compressed_data = open_viii::compression::LZSS::compress(original_data);

        std::cout << "(\""<< std::string_view(original_data.data(), original_data.size()) << "\", \n";
        std::cout << "vec![";
        for (const auto& value : compressed_data) {
            std::cout << +static_cast<uint8_t>(value) << ", ";
        }
        std::cout << "]),\n";

        // Decompress the data
        std::vector<char> decompressed_data = open_viii::compression::LZSS::decompress(compressed_data, original_data.size());

        // Assert that decompressed data matches original data
        if (std::string_view(decompressed_data.data(),decompressed_data.size()) != original_data) {
            std::cerr << "Decompressed data does not match original data\n";
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "]\n";
}

int main() {
    test_compress_decompress();
    std::cout << "Compression and decompression successful\n";
    return 0;
}

