#include "Compiler.h"

int main(int argc, char **argv) {
  Compiler compiler;
  std::string program = R"(x     = 5
                         y=3)";

  // Compile the program and export it.
  compiler.compile(program);

  return 0;
}