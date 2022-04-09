#include <random>
#include <string>

#include "GF.h"

using E = GF<5, 40, {2, 3, 0, 1, 1, 2, 4, 0, 3, 3, 3, 3, 0, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 4, 4, 3, 1, 0, 2, 2, 2, 2, 0, 4, 4}>;
constexpr E p{{2, 4, 3, 1, 4, 4, 3, 3, 3, 2, 1, 1, 3, 3, 2, 2, 0, 4, 1, 0, 1, 2, 4, 0, 1, 1, 1, 3, 3, 0, 3, 0, 1, 4, 4, 1, 4, 4, 3, 2}};
constexpr E q{{1, 1, 4, 3, 2, 4, 1, 1, 1, 0, 3, 2, 1, 1, 0, 3, 0, 1, 1, 2, 4, 3, 1, 0, 3, 3, 1, 4, 2, 2, 0, 3, 0, 3, 1, 0, 2, 3, 0, 1}};

namespace internal {

  constexpr const char* abc = "abcdefghijklmnoprstuvwxyz";

  std::string to_str(const E& elm) {
    std::string res{};
    res.reserve(20);
    for(auto i = 0u; i < 40; i += 2) {
      int x(elm[i]), y(elm[i + 1]);
      res += abc[y * 5 + x];
    }
    return res;
  }

  bool precheck_str(const std::string& str) {
    if(str.size() != 20)
      return false;
    const auto check_chr = [](char c) { return c >= 'a' && c <= 'z' && c != 'q'; };
    for(auto c : str)
      if(!check_chr(c))
        return false;
    return true;
  }

  E from_str(const std::string& str) {
    // assume check_str passed
    std::array<Mod<5>, 40> arr;
    const auto val = [](char c) -> int { return c < 'q' ? c - 97 : c - 98; };
    for(auto i = 0u; i < 20; i++) {
      auto v = val(str[i]);
      arr[2*i] = v % 5;
      arr[2*i + 1] = v / 5;
    }
    return {arr};
  }

} // namespace internal

std::string generate() {
  std::minstd_rand gen{std::random_device{}()};
  std::uniform_int_distribution<> uid(0, 4);
  std::array<Mod<5>, 40> id{};
  for(auto i = 0u; i < 20; i++)
    id[i] = uid(gen);
  return internal::to_str(p * E{id});
}

bool check(const std::string& str) {
  if(!internal::precheck_str(str))
    return false;
  auto e = q * internal::from_str(str);
  for(auto i = 21u; i < 40; i++)
    if(e[i] != 0)
      return false;
  return true;
}
