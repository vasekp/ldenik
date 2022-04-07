#include <random>
#include <string>

#include "GF.h"

using E = GF<3,60,{2,0,1,2,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,1,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,2,0,1}>;
constexpr E p{{1,0,0,2,1,2,1,2,1,2,1,1,2,0,2,2,2,2,0,0,2,1,1,2,2,1,2,0,2,0,2,1,1,0,1,2,2,2,1,1,0,1,2,0,1,1,0,2,1,1,0,2,1,2,1,1,1,2,2,0}};
constexpr E q{{0,1,1,0,1,0,1,0,2,1,2,1,1,1,1,1,2,1,2,0,0,0,0,2,1,0,1,2,1,0,0,1,2,0,2,1,0,1,0,2,0,0,0,2,1,0,1,1,0,2,1,1,2,0,2,2,0,2,1,2}};

namespace internal {

  std::string to_str(const E& elm) {
    std::string res{};
    res.reserve(20);
    for(auto i = 0u; i < 60; i += 3) {
      int x(elm[i]), y(elm[i + 1]), z(elm[i + 2]);
      int c = z * 9 + y * 3 + x;
      res += c == 0 ? '_' : char(96 + c);
    }
    return res;
  }

  bool precheck_str(const std::string& str) {
    if(str.size() != 20)
      return false;
    const auto check_chr = [](char c) { return (c >= 'a' && c <= 'z') || c == '_'; };
    for(auto c : str)
      if(!check_chr(c))
        return false;
    return true;
  }

  E from_str(const std::string& str) {
    // assume check_str passed
    std::array<Mod<3>, 60> arr;
    const auto val = [](char c) -> int { return c == '_' ? 0 : c - 96; };
    for(auto i = 0u; i < 20; i++) {
      auto v = val(str[i]);
      arr[3*i] = v % 3;
      v /= 3;
      arr[3*i + 1] = v % 3;
      v /= 3;
      arr[3*i + 2] = v;
    }
    return {arr};
  }

} // namespace internal

std::string generate() {
  std::minstd_rand gen{std::random_device{}()};
  std::uniform_int_distribution<> uid(0,2);
  std::array<Mod<3>, 60> id{};
  for(auto i = 0u; i < 30; i++)
    id[i] = uid(gen);
  return internal::to_str(p * E{id});
}

bool check(const std::string& str) {
  if(!internal::precheck_str(str))
    return false;
  auto e = q * internal::from_str(str);
  for(auto i = 31u; i < 60; i++)
    if(e[i] != 0)
      return false;
  return true;
}
