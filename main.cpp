#include "Compiler.h"

int main() {
  Compiler compiler;
  const std::string program = R"(x=5
  y=3
  m=8)";

  // Compile the program and export it.
  compiler.compile(program);

  return 0;
}