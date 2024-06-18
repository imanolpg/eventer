#ifndef COMPILER_H
#define COMPILER_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

class Compiler {
public:
  Compiler();
  /**
   * @brief Compiles the source program and outputs a IR file
   *
   * @param program Program to compile.
   */
  void compile(const std::string &program);

private:
  std::unique_ptr<llvm::LLVMContext> ctx; ///< To
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>>
      builder; ///< To create and insert instructions into basic blocks.

  /**
   * @brief Saves the module to a .ll file.
   *
   * @param fileName Name of the output file.
   */
  void saveModuleToFile(const std::string &fileName = "compiled_program.ll");
};

#endif // COMPILER_H