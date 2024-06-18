#include "Compiler.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include <iostream>
#include <string>

Compiler::Compiler()
    : ctx(std::make_unique<llvm::LLVMContext>()),
      module(std::make_unique<llvm::Module>("EventerLLVM", *ctx)),
      builder(std::make_unique<llvm::IRBuilder<>>(*ctx)) {}

void Compiler::compile(const std::string &program) {

  std::cout << "Running code:\n--" << program << "--" << std::endl;

  // First create main function where the program will start.
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(builder->getInt32Ty(), false);

  llvm::Function *mainFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", module.get());

  // Create the basic block.
  llvm::BasicBlock *entry = llvm::BasicBlock::Create(*ctx, "entry", mainFunc);

  // Insert the basic block to the builder.
  builder->SetInsertPoint(entry);

  // Allocate space for a variable.
  llvm::AllocaInst *variable =
      builder->CreateAlloca(llvm::Type::getInt32Ty(*ctx), nullptr, "var1");

  // Store value into the variable.
  builder->CreateStore(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*ctx), 42),
                       variable);

  // Load the value from the variable
  llvm::Value *loadedValue =
      builder->CreateLoad(llvm::Type::getInt32Ty(*ctx), variable, "loadedVar");

  // Return the variable.
  builder->CreateRet(loadedValue);

  saveModuleToFile("output.ll");
}

void Compiler::saveModuleToFile(const std::string &fileName) {
  std::error_code errorCode;
  llvm::raw_fd_ostream outLL(fileName, errorCode);
  module->print(outLL, nullptr);
  std::cout << "File exported: " << fileName << std::endl;
}
