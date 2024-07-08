#ifndef COMPILER_H
#define COMPILER_H

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Compiler {
public:

  Compiler();
  void compile(const std::string &program);

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;

  void saveModuleToFile(const std::string &fileName = "compiled_program.ll") const;
  void setupVariables();
  void setupGlobalVariable(std::string const &varName, llvm::Type *varType);
};

#endif // COMPILER_H