#include <stdint.h>

class foo {
 public:
  uint32_t bar(unsigned int a);
  int baz(int a);
};

// When 1, fail to compiler by Clang 18.1.3 x86_64-pc-linux-gnu
#if 1
uint32_t ::foo::bar(unsigned int a) { return a; }
#else
uint32_t foo::bar(unsigned int a) { return a; }
#endif

int ::foo::baz(int a) { return a; }

int main() {
  foo my_foo;

  return (my_foo.bar(0));
}
