#include "Compiler.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/Parser.h"

#include "llvm/IR/GlobalValue.h"

#include <iostream>
#include <sstream>
#include <vector>

Compiler::Compiler()
    : context(std::make_unique<llvm::LLVMContext>()),
      module(std::make_unique<llvm::Module>("EventerLLVM", *context)),
      builder(std::make_unique<llvm::IRBuilder<>>(*context)) {
  setupVariables();
}

void Compiler::compile(const std::string &program) const {

  // ---------------------------------------------------------------
  // Show the program to be compiled.
  std::cout << "Running code:\n--" << program << "--\n" << std::endl;

  // ---------------------------------------------------------------
  // Create IR entry point.

  // First create main function where the program will start.
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(builder->getInt32Ty(), false);

  llvm::Function *mainFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", module.get());

  // Create the basic block.
  llvm::BasicBlock *entry =
      llvm::BasicBlock::Create(*context, "entry", mainFunc);

  // Insert the basic block to the builder.
  builder->SetInsertPoint(entry);

  // ---------------------------------------------------------------
  // Lexical analysis (Lexer).
  auto lexer = Lexer(program);
  std::vector<Token> const tokens = lexer.tokenize();

  // Show the generated tokens.
  std::cout << "Tokens:" << std::endl;
  for (const Token &t : tokens)
    std::cout << t.toString() << std::endl;
  std::cout << std::endl;

  // ---------------------------------------------------------------
  // Parse the tokens (Parser).
  auto parser = Parser(tokens, *context, *module, *builder);
  parser.parse();

  // ---------------------------------------------------------------
  // Get global variable.
  llvm::GlobalVariable *systemStatus = module->getNamedGlobal("systemStatus");
  llvm::Value *systemStatusValue = builder->CreateLoad(
      systemStatus->getValueType(), systemStatus, "systemStatusValue");

  builder->CreateRet(systemStatusValue);
  std::cout << "Generation LLVM IR:" << std::endl;
  module->print(llvm::outs(), nullptr);

  saveModuleToFile("output.ll");
}

void Compiler::saveModuleToFile(const std::string &fileName) const {
  std::error_code errorCode;
  llvm::raw_fd_ostream outLL(fileName, errorCode);
  module->print(outLL, nullptr);
  std::cout << "File exported: " << fileName << std::endl;
}

void Compiler::setupVariables() const {
  // Install "systemStatus" variable.
  new llvm::GlobalVariable(
      *module, llvm::Type::getInt32Ty(*context), false,
      llvm::GlobalVariable::ExternalLinkage,
      llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0),
      "systemStatus");
}
