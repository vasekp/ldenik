#include <iostream>
#include "ids.h"

int main() {
  {
    int succ = 0;
    for(int i = 0; i < 100000; i++)
      succ += check(generate()) ? 1 : 0;
    std::cout << (int)succ << '\n';
  }

  {
    int succ = 0;
    for(int i = 0; i < 100000; i++) {
      auto str = generate();
      str[0] = 'a';
      succ += check(str) ? 1 : 0;
    }
    std::cout << (int)succ << '\n';
  }
}
