#include <iostream>

int main(int, char **) {
  int v[] = {1, 2, 3, 4, 5};
  int sum = 0;

  for (auto &&i : v) {
    sum += i;
  }
  std::cout << "Hello, from my_program!\n";
}
