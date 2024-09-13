#include <stdint.h>

class foo {
 public:
  uint32_t bar(unsigned int a);
  unsigned int baz(unsigned int a);
  uint32_t qux(uint32_t a);
};

// Fail to compiler by Clang 18.1.3 x86_64-pc-linux-gnu
uint32_t ::foo::bar(unsigned int a) { return a; }

// No problem if not using typdef.
unsigned int ::foo::baz(unsigned int a) { return a; }

// No problem if not using global scope resolution operator.
uint32_t foo::qux(uint32_t a) { return a; }

int main() {
  foo my_foo;

  return (my_foo.bar(0));
}
