#ifndef PARSER_H
#define PARSER_H

#include "Lexer/Lexer.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <vector>

class Parser {
public:
  Parser(const std::vector<Token> &tokens, llvm::LLVMContext &context,
         llvm::Module &module, llvm::IRBuilder<> &builder);
  void parse();

private:
  const std::vector<Token> &tokens_;
  size_t position_;
  llvm::LLVMContext &context_;
  llvm::Module &module_;
  llvm::IRBuilder<> &builder_;

  Token currentToken();
  Token advance();
  void parseAssignment();
  llvm::AllocaInst *parseIdentifier();
  llvm::Value *parseValue();
};

#endif // PARSER_H