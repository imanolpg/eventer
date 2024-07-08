#include "Parser/Parser.h"

Parser::Parser(const std::vector<Token> &tokens, llvm::LLVMContext &context,
               llvm::Module &module, llvm::IRBuilder<> &builder)
    : tokens_(tokens), context_(context), module_(module), builder_(builder),
      position_(0) {}

void Parser::parse() {
  // Parse the program.
  while (currentToken().type != TokenType::EndOfFile)
    parseAssignment();
}

Token Parser::currentToken() {
  if (position_ < tokens_.size())
    return tokens_[position_];
  return Token(TokenType::EndOfFile, "");
}

void Parser::parseAssignment() {
  llvm::AllocaInst *lhs = parseIdentifier();
  if (!lhs)
    return;

  Token token = advance();
  if (token.type != TokenType::Equals)
    return;

  llvm::Value *rhs = parseValue();
  if (!rhs)
    return;

  builder_.CreateStore(rhs, lhs);
}

Token Parser::advance() {
  if (position_ < tokens_.size())
    return tokens_[position_++];
  return Token(TokenType::EndOfFile, "");
}

llvm::AllocaInst *Parser::parseIdentifier() {
  Token token = currentToken();
  if (token.type != TokenType::Identifier)
    return nullptr;

  advance();
  llvm::AllocaInst *alloca = builder_.CreateAlloca(
      llvm::Type::getInt32Ty(context_), nullptr, token.value.c_str());
  return alloca;
}

llvm::Value *Parser::parseValue() {
  Token token = currentToken();
  if (token.type != TokenType::Value)
    return nullptr;

  advance();
  llvm::Value *value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context_),
                                              std::stoi(token.value));

  return value;
}