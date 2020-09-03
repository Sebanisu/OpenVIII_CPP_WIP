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

#ifndef VIIICOMPRESSION_LZSS_H
#define VIIICOMPRESSION_LZSS_H

#include <vector>
#include <array>
#include <span>
#include <algorithm>
namespace open_viii::compression {
struct LZSS
{
private:
  constexpr static const unsigned int R_SIZE = 4078U;
  constexpr static const unsigned int MATCH_MASK = 0xF0U;
  constexpr static const unsigned int P_OFFSET = 4097U;
  constexpr static const unsigned int FLAGS_MASK = 0x100U;
  constexpr static const unsigned int FLAGS_BITS = 0xFF00U;
  constexpr static const unsigned int OFFSET_MASK = MATCH_MASK;
  constexpr static const unsigned int COUNT_MASK = 0x0FU;

  constexpr static const unsigned int NOT_USED = 4096U;

  constexpr static const unsigned int NODE_SIZE = 18U;
  constexpr static const auto F = 18U;
  constexpr static const auto F_MINUS1 = F - 1;
  constexpr static const auto N = NOT_USED;
  constexpr static const auto N_MINUS1 = N - 1U;
  constexpr static const auto THRESHOLD = 2U;

  constexpr static const auto RIGHT_SIDE_SIZE = 4353;


  constexpr static const int N_PLUS1 = N + 1U;
  constexpr static const int N_PLUS2 = N + 1U;
  constexpr static const int N_PLUS17 = N + F_MINUS1;

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
  // Info at https://github.com/niemasd/PyFF7/wiki/LZSS-Format
  // https://github.com/niemasd/PyFF7/blob/master/PyFF7/lzss.py
  // http://wiki.ffrtt.ru/index.php?title=FF7/LZSS_format

  template<typename dstT = std::vector<char>>
  [[nodiscard]] static dstT decompress(std::span<const char> src, size_t dst_size = 0)
  {
    // todo replace src type with a std::span so you can pass any container.
    // string view might work if nulls aren't detected as end and size is respected.
    // and string view is const. I mostly want to just have something I can hold the pointers in.
    dstT dst{};
    if (dst_size > 0) {
      dst.reserve(dst_size);
    }
    auto iterator = src.begin();
    const auto &srcEnd = src.end();
    unsigned int current{ 0 };
    auto textBuf = std::array<unsigned int, N_MINUS1 + F>();
    // ring buffer of size N, with extra F-1 bytes to facilitate string comparison

    auto r = N - F;
    auto flags = 0U;
    const auto testAtEnd = [&iterator, &srcEnd]() { return iterator + 1 > srcEnd; };
    while (iterator < srcEnd /*&& (dstSize == 0 || dst.size() < dstSize)*/) {
      if (((flags >>= 1U) & FLAGS_MASK) == 0) {
        if (testAtEnd()) {
          break;
        }
        flags = static_cast<unsigned char>(*iterator++) | FLAGS_BITS;// uses higher byte cleverly to Count eight
      }
      if ((flags & 1U) == 1) {// raw value
        if (testAtEnd()) {
          break;
        }
        current = static_cast<unsigned char>(*iterator++);
        // if (dstSize != 0 && dst.size() + 1 >= dstSize) break;
        dst.push_back(static_cast<char>(current));
        textBuf.at(r++) = current;
        r &= N_MINUS1;
      } else {// value previously read
        // get bounds of ring buffer
        if (testAtEnd()) {
          break;
        }
        decltype(current) offset = static_cast<unsigned char>(*iterator++);
        if (testAtEnd()) {
          break;
        }
        decltype(current) count = static_cast<unsigned char>(*iterator++);
        offset |= ((count & OFFSET_MASK) << 4U);
        count = (count & COUNT_MASK) + THRESHOLD;
        // read from ring buffer
        for (decltype(current) k = 0; k <= count; k++) {
          // get value
          current = textBuf.at((offset + k) & N_MINUS1);
          // assign value
          // if (dstSize != 0 && dst.size() + 1 >= dstSize) return dst;
          dst.push_back(static_cast<char>(current));
          textBuf.at(r++) = current;
          r &= N_MINUS1;
        }
      }
    }
    return dst;
  }
  // todo add compression method

  /****************************************************************************
   ** Deling Final Fantasy VIII Field Editor
   ** Copyright (C) 2009-2012 Arzel Jérôme <myst6re@gmail.com>
   **
   ** This program is free software: you can redistribute it and/or modify
   ** it under the terms of the GNU General Public License as published by
   ** the Free Software Foundation, either version 3 of the License, or
   ** (at your option) any later version.
   **
   ** This program is distributed in the hope that it will be useful,
   ** but WITHOUT ANY WARRANTY; without even the implied warranty of
   ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   ** GNU General Public License for more details.
   **
   ** You should have received a copy of the GNU General Public License
   ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ****************************************************************************/
  /***************************************************************
          4/6/1989 Haruhiko Okumura
          Use, distribute, and modify this program freely.
          Please send me your improved versions.
                  PC-VAN		SCIENCE
                  NIFTY-Serve	PAF01022
                  CompuServe	74050,1022
  *************************************************************
   * Porting to STL - sebanisu.
   * */

  [[nodiscard]] [[maybe_unused]] static auto compress(std::span<const char> src)
  {
    // todo pass a std::span in cpp 20 instead of vector.

    unsigned int match_length = {};
    unsigned int match_position{};

    // left & right children & parents -- These constitute binary search trees.
    auto left_side = std::array<unsigned int, N_PLUS2>();
    auto right_side = std::array<unsigned int, RIGHT_SIDE_SIZE>();
    auto parent = std::array<unsigned int, N_PLUS2>();

    auto text_buf = std::array<unsigned char,
      N_PLUS17>();// ring buffer of size N, with extra 17 bytes to facilitate string comparison
    unsigned int cur_result{};
    size_t size_alloc = src.size() / 2U;
    auto code_buf = std::array<unsigned char, F_MINUS1>();

    const auto data_end = src.end();
    auto data = src.begin();

    auto result = std::vector<char>();
    result.reserve(size_alloc);
    /**
     * Inserts string of length 18, text_buf[item..item+18-1], into one of the trees (text_buf.at(item)'th tree) and
     * returns the longest-match position and length via the global variables match_position and match_length. If
     * match_length = 18, then removes the old node in favor of the new one, because the old one will be deleted sooner.
     * Note item plays double role, as tree node and position in buffer.
     */
    const auto insert_node = [&text_buf, &right_side, &left_side, &match_length, &parent, &match_position](
                               const unsigned int &item) {
      /*  */

      int cmp = 1U;
      auto key = std::span<unsigned char>(text_buf);
      key = key.subspan(item);

      // auto key = text_buf.begin() + item;// todo replace with std::span.
      unsigned int p = P_OFFSET + key[0];

      right_side.at(item) = left_side.at(item) = NOT_USED;
      match_length = 0;

      while (true) {
        if (cmp >= 0) {// if cmp is unsigned this is always true..
          if (right_side.at(p) != NOT_USED) {
            p = right_side.at(p);
          } else {
            right_side.at(p) = item;
            parent.at(item) = p;
            return;
          }
        } else {
          if (left_side.at(p) != NOT_USED) {
            p = left_side.at(p);
          } else {
            left_side.at(p) = item;
            parent.at(item) = p;
            return;
          }
        }
        unsigned int node_index = 1;
        for (; node_index < NODE_SIZE; node_index++) {
          if ((cmp = key.subspan(node_index)[0] - (text_buf.at(p + node_index))) != 0) {
            break;
          }// todo need std::span to remove pointer math.
        }

        if (node_index > match_length) {
          match_position = p;
          if ((match_length = node_index) >= NODE_SIZE) {
            break;
          }
        }
      }

      parent.at(item) = parent.at(p);
      left_side.at(item) = left_side.at(p);
      right_side.at(item) = right_side.at(p);

      parent.at(left_side.at(p)) = item;
      parent.at(right_side.at(p)) = item;

      if (right_side.at(parent.at(p)) == p) {
        right_side.at(parent.at(p)) = item;
      } else {
        left_side.at(parent.at(p)) = item;
      }
      parent.at(p) = NOT_USED;// remove p
    };
    // deletes node p from tree
    const auto delete_node = [&parent, &right_side, &left_side](auto p) {
      // unsigned int q = 0;
      if (parent.at(p) == NOT_USED) {
        return;// not in tree
      }

      unsigned int q{};
      if (right_side.at(p) == NOT_USED) {
        q = left_side.at(p);
      } else if (left_side.at(p) == NOT_USED) {
        q = right_side.at(p);
      } else {
        q = left_side.at(p);
        if (right_side.at(q) != NOT_USED) {
          do {
            q = right_side.at(q);
          } while (right_side.at(q) != NOT_USED);

          right_side.at(parent.at(q)) = left_side.at(q);
          parent.at(left_side.at(q)) = parent.at(q);
          left_side.at(q) = left_side.at(p);
          parent.at(left_side.at(p)) = q;
        }
        right_side.at(q) = right_side.at(p);
        parent.at(right_side.at(p)) = q;
      }
      parent.at(q) = parent.at(p);

      if (right_side.at(parent.at(p)) == p) {
        right_side.at(parent.at(p)) = q;
      } else {
        left_side.at(parent.at(p)) = q;
      }
      parent.at(p) = NOT_USED;
    };

    //    if(result.size() < sizeAlloc) {
    //      result.resize(sizeAlloc);
    //    }

    /* quint32
            textsize = 0,//text size counter
            codesize = 0;//code size counter */

    // initialize trees
    /* For i = 0 to NMinus1, rightSide[i] and leftSide[i] will be the right and left children of node i. These nodes
    need not be initialized. Also, parent[i] is the parent of node i. These are initialized to NotUsed which stands for
    'not used.' For i = 0 to 255, rightSide[4097 + i] is the root of the tree for strings that begin with character i.
    These are initialized to NotUsed. Note there are 256 trees. */

    // for(i=4097 ; i<=4352 ; ++i)	rightSide[i] = NotUsed;
    // for(i=0 ; i<N ; ++i)	parent[i] = NotUsed;
    parent.fill(NOT_USED);
    std::fill(right_side.begin() + N_PLUS1, right_side.end(), NOT_USED);

    code_buf[0] = 0;// code_buf[1..16] saves eight units of code, and code_buf[0] works as eight flags, "1" representing
                    // that the unit is an unencoded letter (1 byte), "0" a position-and-length pair (2 bytes). Thus,
                    // eight units require at most 16 bytes of code.


    unsigned int s = 0;
    unsigned int r = R_SIZE;

    //	for(i=s ; i<r ; ++i)
    //		text_buf[i] = '\x0';//Clear the buffer with  any character that will appear often.
    // memset(text_buf, 0, r); //std::array should init with 0s.
    unsigned int len = 0;
    for (; len < NODE_SIZE && data < data_end; ++len) {
      text_buf.at(r + len) = static_cast<unsigned char>(*data++);// Read 18 bytes into the last 18 bytes of the buffer
    }
    if (/* (textsize =  */ len /* ) */ == 0) {
      result.clear();
      return result;// text of size zero
    }
    unsigned int i = 1;
    for (; i <= NODE_SIZE; ++i) {
      insert_node(
        r - i);// Insert the 18 strings, each of which begins with one or more 'space' characters.  Note the order in
    }
    // which these strings are inserted.  This way, degenerate trees will be less likely to occur.

    insert_node(
      r);// Finally, insert the whole string just read.  The global variables match_length and match_position are set.

    unsigned int code_buf_ptr = 1;
    unsigned char mask = 1;

    do {
      if (match_length > len) {
        match_length = len;// match_length may be spuriously long near the end of text.
      }


      if (match_length <= 2) {
        match_length = 1;// Not long enough match.  Send one byte.
        code_buf[0] |= mask;//'send one byte' flag
        code_buf.at(code_buf_ptr++) = text_buf.at(r);// Send unencoded.
      } else {
        code_buf.at(code_buf_ptr++) = static_cast<unsigned char>(match_position);
        code_buf.at(code_buf_ptr++) = static_cast<unsigned char>(
          (((match_position >> 4U) & MATCH_MASK))
          | (match_length - (2 + 1)));// Send position and length pair. Note match_length > 2.
      }

      if ((mask = static_cast<unsigned char>((mask << 1U))) == 0)// Shift mask left one bit.
      {
        //			for(i=0 ; i<code_buf_ptr ; ++i)//Send at most 8 units of
        //				result.append(code_buf[i]);//code together
        // result.replace(curResult, code_buf_ptr, (char *)code_buf, code_buf_ptr);
        // pos,len,after,after_len if length the same then would be a memcpy. or .insert()
        result.insert(result.begin() + cur_result, code_buf.begin(), code_buf.begin() + code_buf_ptr);
        cur_result += code_buf_ptr;
        code_buf[0] = 0;
        code_buf_ptr = mask = 1;
      }

      unsigned int last_match_length = match_length;
      for (i = 0; i < last_match_length && data < data_end; ++i) {
        unsigned int c = static_cast<unsigned char>(*data++);
        delete_node(s);// Delete old strings and
        text_buf.at(s) = static_cast<unsigned char>(c);// read new bytes

        if (s < F_MINUS1) {
          text_buf.at(s + N) = static_cast<unsigned char>(
            c);// If the position is near the end of buffer, extend the buffer to make string comparison easier.
        }

        s = (s + 1) & (N_MINUS1);
        r = (r + 1) & (N_MINUS1);
        // Since this is a ring buffer, increment the position modulo N.
        insert_node(r);// Register the string in text_buf[r..r+18-1]
      }

      while (i++ < last_match_length)// After the end of text,
      {
        delete_node(s);// no need to read, but
        s = (s + 1) & (N_MINUS1);
        r = (r + 1) & (N_MINUS1);
        if (--len != 0U) {
          insert_node(r);// buffer may not be empty.
        }
      }

    } while (len > 0);// until length of string to be processed is zero

    if (code_buf_ptr > 1)// Send remaining code.
    {
      //		for(i = 0; i < code_buf_ptr ; ++i)
      //			result.append(code_buf[i]);
      // result.replace(curResult, code_buf_ptr, (char *)code_buf, code_buf_ptr);
      // pos,len,after,after_len if length the same then would be a memcpy. or .insert()
      result.insert(result.begin() + cur_result, code_buf.begin(), code_buf.begin() + code_buf_ptr);
      cur_result += code_buf_ptr;
    }
    // result.truncate(curResult);
    result.erase(result.begin() + cur_result, result.end());
    return result;
  }
};
}// namespace open_viii::compression
#endif