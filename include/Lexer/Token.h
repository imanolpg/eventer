#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"
#include <string>

struct Token {
  TokenType type_;
  std::string value_;

  Token() : type_(TokenType::Unknown), value_("") {}
  Token(TokenType type, const std::string &value)
      : type_(type), value_(value) {}
  Token(std::string value);
  std::string toString() const;
};

std::string padLeft(std::string &str, size_t spaces);

#endif // TOKEN_H