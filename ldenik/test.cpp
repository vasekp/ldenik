#include <iostream>
#include "ids.h"

int main() {
  bool res = true;
  for(int i = 0; i < 100000; i++)
    res &= check(generate());
  std::cout << (int)res << '\n';
}
