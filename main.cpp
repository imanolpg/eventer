#include "Compiler.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  // Read program code.
  std::ifstream inputFile("main.ev");

  // Check if the file has been read correctly.
  if (!inputFile.is_open()) {
    std::cerr << "File can not be open." << std::endl;
    return -1;
  }

  std::stringstream buffer;
  buffer << inputFile.rdbuf();
  const std::string program = buffer.str();

  Compiler compiler;

  // Compile the program and export it.
  compiler.compile(program);

  return 0;
}
