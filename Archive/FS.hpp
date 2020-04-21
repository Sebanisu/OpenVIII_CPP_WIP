#ifndef _FS_H
#define _FS_H

#include "FI.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>
#include <utility>
#include <iterator>
#include <cstring>
#include "../Compression/LZSS.hpp"

namespace OpenVIII::Archive {
    struct FS {
    public:
        static constexpr const auto Ext = std::string_view(".FS");

        [[nodiscard]] static auto GetEntry(const std::filesystem::path &path, const FI &fi, const size_t &offset = 0U) {
            if (fi.UncompressedSize() == 0) {
                return std::vector<unsigned char>();
            }
            std::ifstream fp = std::ifstream(path, std::ios::in | std::ios::binary);
            //if compressed will keep decompressing till get size
            //size compressed isn't quite known with out finding the offset of the next file and finding difference.
            fp.seekg(offset + fi.Offset(), std::ios::beg);
            static const auto readBuffer = [&fp](const auto &s) {
                auto buf = std::vector<unsigned char>(s);
                auto tmp = std::vector<char>(s);
                fp.read(tmp.data(), s); //TODO change to bitcast in cpp 20 or read another way?
                memcpy(buf.data(), tmp.data(), s); //memcpy will be optimized away. It is safer than casting.
                return buf;
            };

            static const auto readVal = [&fp](auto &v) {
                auto tmp = std::array<char, sizeof(v)>();
                fp.read(tmp.data(), sizeof(v));
                memcpy(&v, tmp.data(), sizeof(v)); //memcpy will be optimized away. It is safer than casting.
                //TODO change to bitcast in cpp 20 or read another way?
            };

            std::vector<unsigned char> buffer;
            unsigned int compSize{0};
            switch (fi.CompressionType()) {
                default:
                case TCompressionType::None:
                    buffer = readBuffer(fi.UncompressedSize());
                    fp.close();
                    return buffer;
                case TCompressionType::LZSS:
                    readVal(compSize);
                    buffer = readBuffer(compSize);
                    fp.close();
                    return Compression::LZSS::Decompress(buffer, fi.UncompressedSize());
                case TCompressionType::LZ4: {
                    //todo add support for LZ4
                    fp.close();
                    return std::vector<unsigned char>();
                }
            }
        }
    };
}
#endif // !_FS_H
