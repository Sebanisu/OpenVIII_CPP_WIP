#ifndef _LZSS_H
#define _LZSS_H

#include <cstring>
#include <iostream>
#include <vector>
#include <array>

namespace OpenVIII::Compression {


    struct LZSS {
    private:
        constexpr static const unsigned int flagsMask = 0x100U;
        constexpr static const unsigned int flagsBits = 0xFF00U;
        constexpr static const unsigned int offsetMask = 0xF0U;
        constexpr static const unsigned int countMask = 0x0FU;
        
        constexpr static const auto F = 18U;
        constexpr static const auto N = 4096U;
        constexpr static const auto N_ = N - 1U;
        constexpr static const auto THRESHOLD = 2U;
    public:
        // NEW LZSS
        // LZSS.C -- A Data Compression Program
        // (tab = 4 spaces)
        // 4/6/1989 Haruhiko Okumura
        // Use, distribute, and modify this program freely.
        // Please send me your improved versions.
        // PC-VAN SCIENCE
        // NIFTY-Serve PAF01022
        // CompuServe  74050,1022
        // Code borrowed from Java's implementation of LZSS by antiquechrono
        // ClangTidy doesn't like the signed bitwise. says bitwise ops should be unsigned i guess.
        // I updated the code to unsigned where anything used a bit op
        // If you know the dstSize you can reserve that space. If not we'll start with an empty vector.
        static auto Decompress(const std::vector<unsigned char> &src,size_t dstSize = 0) {
            auto dst = std::vector<unsigned char>();
            dst.reserve(dstSize);
            auto iterator = src.begin();
            const auto &srcEnd = src.end();
            unsigned int current{0};
            auto textBuf = std::array<unsigned int, N_ + F>();
            // ring buffer of size N, with extra F-1 bytes to facilitate string comparison

            auto r = N - F;
            auto flags = 0U;
            while (iterator < srcEnd) {
                if (((flags >>= 1U) & flagsMask) == 0) {
                    if (iterator + 1 >= srcEnd) { break; }
                    flags = *iterator++ | flagsBits;     // uses higher byte cleverly to Count eight
                }
                if ((flags & 1U) == 1) { // raw value
                    if (iterator + 1 >= srcEnd) { break; }
                    current = *iterator++;
                    dst.push_back(static_cast<unsigned char>(current));
                    textBuf.at(r++) = current;
                    r &= N_;
                } else { // value previously read
                    //get bounds of ring buffer
                    if (iterator + 1 >= srcEnd) { break; }
                    decltype(current) offset = *iterator++;
                    if (iterator + 1 >= srcEnd) { break; }
                    decltype(current) count = *iterator++;
                    offset |= ((count & offsetMask) << 4U);
                    count = (count & countMask) + THRESHOLD;
                    //read from ring buffer
                    for (decltype(current) k = 0; k <= count; k++) {
                        //get value
                        current = textBuf.at((offset + k) & N_);
                        //assign value
                        dst.push_back(static_cast<unsigned char>(current));
                        textBuf.at(r++) = current;
                        r &= N_;
                    }
                }
            }
            return dst;
        }
        //todo add compression method
    };
}
#endif