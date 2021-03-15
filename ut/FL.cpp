#include <boost/ut.hpp>// single header
// import boost.ut;        // single module (C++20)
#include "open_viii/archive/FL.hpp"
#include "tl/utility.hpp"
#include <string>
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace std::string_literals;
  [[maybe_unused]] suite tests = [] {
    "clean_path_string"_test = [] {};
    "clean_buffer"_test      = [] {};
    "get_max"_test           = [] {};
    "sort_entries"_test      = [] {};
    "get_all_entries"_test   = [] {};
    "get_entry"_test         = [] {};
  };
}