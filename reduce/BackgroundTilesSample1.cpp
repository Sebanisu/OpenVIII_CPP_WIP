// https://godbolt.org/z/3TxGsGbYr
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