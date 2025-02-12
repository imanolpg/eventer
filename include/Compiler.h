#ifndef COMPILER_H
#define COMPILER_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"


#include <string>


class Compiler {
public:

  Compiler();
  void compile(const std::string &program) const;

private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;

  void saveModuleToFile(const std::string &fileName = "compiled_program.ll") const;
  void setupVariables() const;
  void setupGlobalVariable(std::string const &varName, llvm::Type *varType) const;
};

#endif // COMPILER_H