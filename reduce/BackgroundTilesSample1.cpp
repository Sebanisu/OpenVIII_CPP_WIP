// https://godbolt.org/z/d7ea6n1x3
struct a {
  bool b();
};
template <typename c> struct d : c {
  c::e;
  using f = d;
  constexpr f g(decltype(e.b())) { this; }
};
struct h {
  a e;
};
using i = d<h>;
auto j = i{}.g(1);