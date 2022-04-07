#include <iostream>
#include "ids.h"

constexpr int PASS = 0;
constexpr int FAIL = 1;

int main(int argc, const char* argv[]) {
  if(argc == 2 && check(argv[1]))
    return PASS;
  else
    return FAIL;
}
