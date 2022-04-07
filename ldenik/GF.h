#include <sstream>
#include <array>
#include <cstdint>

template<unsigned M>
struct Mod {
  std::uint8_t val;

  constexpr Mod(int i = 0) : val(((i % M) + M) % M) { }
  constexpr Mod(unsigned i) : val(i % M) { }
  constexpr Mod(const Mod& other) = default;

  constexpr Mod operator+(Mod other) const {
    return {(val + other.val) % M};
  }

  constexpr Mod operator-(Mod other) const {
    return {(val + M - other.val) % M};
  }

  constexpr Mod operator*(Mod other) const {
    return {(val * other.val) % M};
  }

  constexpr Mod operator-() const {
    return {(M - val) % M};
  }

  Mod& operator+=(Mod in) {
    val = (val + in.val) % M;
    return *this;
  }

  Mod& operator-=(Mod in) {
    val = (val + M - in.val) % M;
    return *this;
  }

  Mod& operator*=(Mod in) {
    val = (val * in.val) % M;
    return *this;
  }

  constexpr explicit operator int() const {
    return val;
  }

  constexpr bool operator==(Mod<M> other) const {
    return val == other.val;
  }

  constexpr bool operator!=(Mod<M> other) const {
    return val != other.val;
  }
};

template<unsigned M>
std::ostream& operator<<(std::ostream& os, Mod<M> m) {
  return os << (unsigned int)(m.val);
}

template<unsigned M, unsigned N, std::array<Mod<M>, N> rem>
class GF {
  using arrType = std::array<Mod<M>, N>;
  arrType data;

  constexpr static auto muls = [](){
    std::array<std::array<Mod<M>, N>, M> ret{};
    for(auto i = 0u; i < M; i++)
      for(auto j = 0u ; j < N; j++)
        ret[i][j] = rem[j] * i;
    return ret;
  }();

  public:
  constexpr GF() : data() { }
  constexpr GF(const arrType& in) : data(in) { }
  constexpr GF(arrType&& in) : data(std::move(in)) { }
  constexpr GF(const GF& other) = default;
  constexpr GF(GF&& other) = default;
  GF& operator=(const GF& other) = default;
  GF& operator=(GF&& other) = default;

  constexpr static GF zero{};
  constexpr static GF one{{1}};
  constexpr static GF unit{{0,1}};

  auto operator[](std::size_t ix) const {
    return data[ix];
  }

  auto begin() const {
    return data.cbegin();
  }

  auto end() const {
    return data.cend();
  }

  GF operator+(const GF& other) const {
    arrType out{};
    for(auto i = 0u; i < N; i++)
      out[i] = data[i] + other[i];
    return {out};
  }

  GF operator+=(const GF& other) {
    for(auto i = 0u; i < N; i++)
      data[i] += other[i];
    return *this;
  }

  GF operator-(const GF& other) const {
    arrType out{};
    for(auto i = 0u; i < N; i++)
      out[i] = data[i] - other[i];
    return {out};
  }

  GF operator-=(const GF& other) {
    for(auto i = 0u; i < N; i++)
      data[i] -= other[i];
    return *this;
  }

  GF operator*(Mod<M> s) const {
    arrType out{};
    for(auto i = 0u; i < N; i++)
      out[i] = s * data[i];
    return {out};
  }

  friend GF operator*(Mod<M> s, const GF& elm) {
    return elm * s;
  }

  friend GF operator*(int s, const GF& elm) {
    return elm * s;
  }

  GF operator*(const GF& other) const {
    GF out{};
    GF copy = other;
    for(auto i = 0u; i < N; i++) {
      out += data[i] * copy;
      copy.shift();
    }
    return out;
  }

  GF& operator*=(const GF& other) {
    *this = (*this * other);
    return *this;
  }

  private:
  void shift() {
    int top = data[N - 1];
    for(auto i = N-1; i > 0; i--)
      data[i] = data[i-1];
    data[0] = 0;
    for(auto i = 0u; i < N; i++)
      data[i] -= muls[top][i];
  }
};

template<unsigned M, unsigned N, std::array<Mod<M>, N> rem>
std::ostream& operator<<(std::ostream& os, const GF<M,N,rem>& elm) {
  std::ostringstream oss{};
  oss << '{';
  for(auto i = 0u; i < N; i++) {
    oss << elm[i];
    oss << (i < N - 1 ? ',' : '}');
  }
  return os << oss.str();
}
