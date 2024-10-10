#include <iostream>

#if !DEFINE_AT_MAIN_CPP
__attribute((weak)) void foo(void) {
  std::cout << "Message from WEAK foo()" << std::endl;
}

#endif