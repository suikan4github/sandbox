#include <iostream>

#include "foo.hpp"

#if DEFINE_AT_MAIN_CPP
__attribute((weak)) void foo(void) {
  std::cout << "Message from WEAK foo()" << std::endl;
}
#endif

int main() {
  foo();
  return 0;
}