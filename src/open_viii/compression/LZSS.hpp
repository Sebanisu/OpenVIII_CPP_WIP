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
#include <algorithm>
#include <ranges>
#include <array>
#include <vector>
namespace open_viii::compression {
struct LZSS
{
private:
  static constexpr std::uint32_t R_SIZE          = 4078U;
  static constexpr std::uint32_t MATCH_MASK      = 0xF0U;
  static constexpr std::uint32_t P_OFFSET        = 4097U;
  static constexpr std::uint32_t FLAGS_MASK      = 0x100U;
  static constexpr std::uint32_t FLAGS_BITS      = 0xFF00U;
  static constexpr std::uint32_t OFFSET_MASK     = MATCH_MASK;
  static constexpr std::uint32_t COUNT_MASK      = 0x0FU;
  static constexpr std::uint32_t NOT_USED        = 4096U;
  static constexpr std::uint32_t NODE_SIZE       = 18U;
  static constexpr auto          F               = 18U;
  static constexpr auto          F_MINUS1        = F - 1;
  static constexpr auto          N               = NOT_USED;
  static constexpr auto          N_MINUS1        = N - 1U;
  static constexpr auto          THRESHOLD       = 2U;
  static constexpr auto          RIGHT_SIDE_SIZE = 4353;
  static constexpr int           N_PLUS1         = N + 1U;
  static constexpr int           N_PLUS2         = N + 1U;
  static constexpr int           N_PLUS17        = N + F_MINUS1;
  /**
   * @note I was getting a complexity warning as all the code was in one
   * function. So I moved the code into a struct. So I could change the lambda's
   * into member functions.
   */
  struct CompressImpl
  {
    [[nodiscard]] [[maybe_unused]] auto
      operator()(std::span<const char> src)
    {
      std::uint32_t cur_result{};
      size_t        size_alloc = src.size() / 2U;
      auto          code_buf   = std::array<std::uint8_t, F_MINUS1>();
      const auto    data_end   = std::ranges::cend(src);
      auto          data       = std::ranges::cbegin(src);
      auto          result     = std::vector<char>();
      result.reserve(size_alloc);
      //    if(result.size() < sizeAlloc) {
      //      result.resize(sizeAlloc);
      //    }
      /* quint32
              textsize = 0,//text size counter
              codesize = 0;//code size counter */
      // initialize trees
      /* For i = 0 to NMinus1, rightSide[i] and leftSide[i] will be the right
      and left children of node i. These nodes need not be initialized. Also,
      parent[i] is the parent of node i. These are initialized to NotUsed which
      stands for 'not used.' For i = 0 to 255, rightSide[4097 + i] is the root
      of the tree for strings that begin with character i.
      These are initialized to NotUsed. Note there are 256 trees. */
      // for(i=4097 ; i<=4352 ; ++i)	rightSide[i] = NotUsed;
      // for(i=0 ; i<N ; ++i)	parent[i] = NotUsed;
      std::ranges::fill(m_parent, NOT_USED);
      std::ranges::fill(std::span(m_right_side).subspan(N_PLUS1), NOT_USED);
      code_buf[0]       = 0;// code_buf[1..16] saves eight units of code, and
                      // code_buf[0] works as eight flags, "1" representing
      // that the unit is an unencoded letter (1 byte), "0" a
      // position-and-length pair (2 bytes). Thus, eight units require at most
      // 16 bytes of code.
      std::uint32_t s   = 0;
      std::uint32_t r   = R_SIZE;
      //	for(i=s ; i<r ; ++i)
      //		text_buf[i] = '\x0';//Clear the buffer with  any
      // character that will appear often.
      // memset(text_buf, 0, r); //std::array should init with 0s.
      std::uint32_t len = 0;
      for (; len < NODE_SIZE && data != data_end; ++len, ++data) {
        m_text_buf.at(std::size_t{ r } + len) = static_cast<std::uint8_t>(
          *data);// Read 18 bytes into the last 18 bytes of the buffer
      }
      if (/* (textsize =  */ len /* ) */ == 0) {
        result.clear();
        return result;// text of size zero
      }
      // for (auto i : std::ranges::iota_view(std::uint32_t{ 1 }, NODE_SIZE)) {
      for (std::uint32_t i = 1U; i != NODE_SIZE + 1; ++i) {
        // for (; i <= NODE_SIZE; ++i) {
        insert_node(r - i);// Insert the 18 strings, each of which begins with
                           // one or more 'space' characters.  Note the order in
      }
      // which these strings are inserted.  This way, degenerate trees will be
      // less likely to occur.
      insert_node(r);// Finally, insert the whole string just read.  The global
                     // variables match_length and match_position are set.
      std::uint32_t code_buf_ptr = 1;
      std::uint8_t  mask         = 1;
      do {
        if (m_match_length > len) {
          m_match_length
            = len;// match_length may be spuriously long near the end of text.
        }
        if (m_match_length <= 2) {
          m_match_length = 1; // Not long enough match.  Send one byte.
          code_buf[0] |= mask;//'send one byte' flag
          code_buf.at(code_buf_ptr++) = m_text_buf.at(r);// Send unencoded.
        }
        else {
          code_buf.at(code_buf_ptr++)
            = static_cast<std::uint8_t>(m_match_position);
          code_buf.at(code_buf_ptr++) = static_cast<std::uint8_t>(
            (((m_match_position >> 4U) & MATCH_MASK))
            | (m_match_length - (2 + 1)));// Send position and length pair. Note
                                          // match_length > 2.
        }
        if ((mask = static_cast<std::uint8_t>((mask << 1U))) == 0)// Shift mask
                                                                  // left one
                                                                  // bit.
        {
          //			for(i=0 ; i<code_buf_ptr ; ++i)//Send at most 8
          // units
          // of result.append(code_buf[i]);//code together
          // result.replace(curResult, code_buf_ptr, (char *)code_buf,
          // code_buf_ptr); pos,len,after,after_len if length the same then
          // would be a memcpy. or .insert()
          result.insert(
            result.begin() + cur_result,
            code_buf.begin(),
            code_buf.begin() + code_buf_ptr);
          cur_result += code_buf_ptr;
          code_buf[0]  = 0;
          code_buf_ptr = mask = 1;
        }
        std::uint32_t last_match_length = m_match_length;
        {
          std::uint32_t i{};
          for (; i < last_match_length && data != data_end; ++i) {
            std::uint32_t c = static_cast<std::uint8_t>(*data++);
            delete_node(s);// Delete old strings and
            m_text_buf.at(s) = static_cast<std::uint8_t>(c);// read new bytes
            if (s < F_MINUS1) {
              m_text_buf.at(std::size_t{ s } + N) = static_cast<std::uint8_t>(
                c);// If the position is near the end of buffer, extend the
                   // buffer to make string comparison easier.
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
        }
      }
      while (len > 0);     // until length of string to be processed is zero
      if (code_buf_ptr > 1)// Send remaining code.
      {
        //		for(i = 0; i < code_buf_ptr ; ++i)
        //			result.append(code_buf[i]);
        // result.replace(curResult, code_buf_ptr, (char *)code_buf,
        // code_buf_ptr); pos,len,after,after_len if length the same then would
        // be a memcpy. or .insert()
        result.insert(
          result.begin() + cur_result,
          code_buf.begin(),
          code_buf.begin() + code_buf_ptr);
        cur_result += code_buf_ptr;
      }
      // result.truncate(curResult);
      result.erase(result.begin() + cur_result, result.end());
      return result;
    }
    /**
     * Inserts string of length 18, text_buf[item..item+18-1], into one of the
     * trees (text_buf.at(item)'th tree) and returns the longest-match position
     * and length via the global variables match_position and match_length. If
     * match_length = 18, then removes the old node in favor of the new one,
     * because the old one will be deleted sooner. Note item plays double role,
     * as tree node and position in buffer.
     */
    //[&text_buf, &match_length, &match_position]
    void
      insert_node(const std::uint32_t &item)
    {
      int  cmp              = 1;
      auto key              = std::span<std::uint8_t>(m_text_buf);
      key                   = key.subspan(item);
      // auto key = text_buf.begin() + item;
      std::uint32_t p       = P_OFFSET + key[0];
      m_right_side.at(item) = m_left_side.at(item) = NOT_USED;
      m_match_length                               = 0;
      while (true) {
        if (cmp >= 0) {// if cmp is unsigned this is always true..
          if (m_right_side.at(p) != NOT_USED) {
            p = m_right_side.at(p);
          }
          else {
            m_right_side.at(p) = item;
            m_parent.at(item)  = p;
            return;
          }
        }
        else {
          if (m_left_side.at(p) != NOT_USED) {
            p = m_left_side.at(p);
          }
          else {
            m_left_side.at(p) = item;
            m_parent.at(item) = p;
            return;
          }
        }
        {
          std::uint32_t node_index = 1;
          for (; node_index < NODE_SIZE;
               ++node_index) {
                              
            if (
              (cmp = +key[node_index]
                   - +(m_text_buf.at(std::size_t{ p } + node_index)))
              != 0) {
              break;
            }
          }
          if (node_index > m_match_length) {
            m_match_position = p;
            if ((m_match_length = node_index) >= NODE_SIZE) {
              break;
            }
          }
        }
      }
      m_parent.at(item)               = m_parent.at(p);
      m_left_side.at(item)            = m_left_side.at(p);
      m_right_side.at(item)           = m_right_side.at(p);
      m_parent.at(m_left_side.at(p))  = item;
      m_parent.at(m_right_side.at(p)) = item;
      if (m_right_side.at(m_parent.at(p)) == p) {
        m_right_side.at(m_parent.at(p)) = item;
      }
      else {
        m_left_side.at(m_parent.at(p)) = item;
      }
      m_parent.at(p) = NOT_USED;// remove p
    };
    // deletes node p from tree
    void
      delete_node(auto p)
    {
      // std::uint32_t q = 0;
      if (m_parent.at(p) == NOT_USED) {
        return;// not in tree
      }
      {
        auto q = [&p, this]() -> std::uint32_t {
          if (m_right_side.at(p) == NOT_USED) {
            return m_left_side.at(p);
          }
          else if (m_left_side.at(p) == NOT_USED) {
            return m_right_side.at(p);
          }
          else {
            std::uint32_t q_i = m_left_side.at(p);
            if (m_right_side.at(q_i) != NOT_USED) {
              do {
                q_i = m_right_side.at(q_i);
              }
              while (m_right_side.at(q_i) != NOT_USED);
              m_right_side.at(m_parent.at(q_i)) = m_left_side.at(q_i);
              m_parent.at(m_left_side.at(q_i))  = m_parent.at(q_i);
              m_left_side.at(q_i)               = m_left_side.at(p);
              m_parent.at(m_left_side.at(p))    = q_i;
            }
            m_right_side.at(q_i)            = m_right_side.at(p);
            m_parent.at(m_right_side.at(p)) = q_i;
            return q_i;
          }
        }();
        m_parent.at(q) = m_parent.at(p);
        if (m_right_side.at(m_parent.at(p)) == p) {
          m_right_side.at(m_parent.at(p)) = q;
        }
        else {
          m_left_side.at(m_parent.at(p)) = q;
        }
      }
      m_parent.at(p) = NOT_USED;
    };

  private:
    // left & right children & parents -- These constitute binary search trees.
    std::array<std::uint32_t, N_PLUS2>         m_left_side{};
    std::array<std::uint32_t, RIGHT_SIDE_SIZE> m_right_side{};
    std::array<std::uint32_t, N_PLUS2>         m_parent{};
    std::array<std::uint8_t, N_PLUS17>
      m_text_buf{};// ring buffer of size N, with extra 17 bytes to facilitate
                   // string comparison
    std::uint32_t m_match_length{};
    std::uint32_t m_match_position{};
  };

public:
  /**
   NEW LZSS
   LZSS.C -- A Data compression Program
   (tab = 4 spaces)
   Use, distribute, and modify this program freely.
   Please send me your improved versions.
   PC-VAN SCIENCE
   NIFTY-Serve PAF01022
   CompuServe  74050,1022

   Code borrowed from Java's implementation of LZSS by antiquechrono
   ClangTidy doesn't like the signed bitwise. says bitwise ops should be
   unsigned i guess. I updated the code to unsigned where anything used a bit op
   If you know the dstSize you can reserve that space. If not we'll start with
   an empty vector.

   @date 4/6/1989
   @authors Haruhiko Okumura, antiquechrono, maki, sebanisu
   @see https://github.com/niemasd/PyFF7/wiki/LZSS-Format
   @see https://github.com/niemasd/PyFF7/blob/master/PyFF7/lzss.py
   @see http://wiki.ffrtt.ru/index.php?title=FF7/LZSS_format
   */
  template<typename dstT = std::vector<char>>
  [[nodiscard]] static dstT
      decompress(std::span<const char> src, size_t dst_size = 0)
  {
      // warning C6262: Function uses '16560' bytes of stack:  exceeds
      // /analyze:stacksize '16384'.  Consider moving some data to heap. warning
      // C6262: Function uses '16568' bytes of stack:  exceeds /analyze:stacksize
      // '16384'.  Consider moving some data to heap.
      dstT dst{};
      if (src.empty())
      {
          return dst;
      }
      if (dst_size > 0) {
          dst.reserve(dst_size);
      }
    auto       iterator  = src.begin();
    auto       textBuf   = std::array<std::uint32_t, N_MINUS1 + F>();
    // ring buffer of size N, with extra F-1 bytes to facilitate string
    // comparison
    auto       r         = N - F;
    auto       flags     = 0U;
    const auto testAtEnd = [the_end_minus_1 = std::ranges::end(src)-1, &iterator](){
      return iterator > the_end_minus_1;
    };
    const auto next = [&iterator](){
      return static_cast<std::uint8_t>(*iterator++);
    };
    while (!testAtEnd()) {
      if (((flags >>= 1U) & FLAGS_MASK) == 0) {
        if (testAtEnd()) {
          break;
        }
        flags = next()
              | FLAGS_BITS;// uses higher byte cleverly to Count eight
      }
      if ((flags & 1U) == 1) {// raw value
        if (testAtEnd()) {
          break;
        }
        std::uint32_t current = next();
        // if (dstSize != 0 && dst.size() + 1 >= dstSize) break;
        dst.push_back(static_cast<char>(current));
        textBuf.at(r++) = current;
        r &= N_MINUS1;
      }
      else {// value previously read
        // get bounds of ring buffer
        if (testAtEnd()) {
          break;
        }
        std::uint32_t offset = next();
        if (testAtEnd()) {
          break;
        }
        std::uint32_t count = next();
        offset |= ((count & OFFSET_MASK) << 4U);
        count = (count & COUNT_MASK) + THRESHOLD;
        // read from ring buffer
        // for (std::uint32_t k = 0; k <= count; ++k) {
        // for (auto k : std::ranges::iota_view(std::uint32_t{}, count + 1)) {
        for (const auto k : std::ranges::iota_view(std::uint32_t{}, count + 1)) {
          // get value
          std::uint32_t current = textBuf.at((offset + k) & N_MINUS1);
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
  static auto
    compress(std::span<const char> span)
  {
    // warning C6262: Function uses '54328' bytes of stack:  exceeds
    // /analyze:stacksize '16384'.  Consider moving some data to heap.
    CompressImpl compress_obj{};// complex code moved into abstraction.
    return compress_obj(span);
  }
};
}// namespace open_viii::compression
#endif