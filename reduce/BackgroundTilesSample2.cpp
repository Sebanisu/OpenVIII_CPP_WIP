// https://godbolt.org/z/K4Ma5qbYo
struct a {
  bool b();
};
template <typename c>
struct d : c {
  using c::e;
  using f = d;
  constexpr f g(decltype(e.b())) {
    auto r = *this;
    const auto a = r.e; //-Wunused-but-set-variable
    return r;}
};
struct h {
  a e;
};
using i = d<h>;
auto j = i{}.g(1);