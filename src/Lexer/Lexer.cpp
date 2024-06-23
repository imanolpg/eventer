#include "Lexer/Lexer.h"

#include <iostream>
#include <string>
#include <vector>

std::string tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::Identifier:
    return "Identifier";

  case TokenType::Equals:
    return "Equals";

  case TokenType::Value:
    return "Value";

  case TokenType::EndOfFile:
    return "EndOfFile";

  case TokenType::Unknown:
    return "Unknown";

  default:
    return "Unknown";
  }
}

std::string Token::toString() const {
  return "Token(Type: " + tokenTypeToString(type) + ";\t" + "Value: " + value +
         ")";
}

// Constructor
Lexer::Lexer(const std::string &input) : input_(input) {}

std::vector<Token> Lexer::tokenize() {

  std::vector<Token> tokens;
  size_t tokenStartIndex;
  Token t;

  const size_t inputLength = input_.length();
  position_ = 0;

  while (position_ < inputLength) {
    // Consume spaces and new lines.
    if (isSpace(input_[position_]) || isNewLine(input_[position_])) {
    }
    // Consume expression: var = number
    else if (isAlphaNumOrUnderscore(input_[position_])) {
      std::string identifier = extractWord(position_);
      t = Token(TokenType::Identifier, identifier);
      tokens.push_back(t);

      while (isSpace(input_[position_]))
        ++position_;

      if (input_[position_] == '=') {
        t = Token(TokenType::Equals, "=");
        tokens.push_back(t);
      }
      ++position_;

      while (isSpace(input_[position_]))
        ++position_;

      std::string value = extractWord(position_);
      t = Token(TokenType::Value, value);
      tokens.push_back(t);
    }
    // Syntax error
    else {
    }

    ++position_;
  }

  return tokens;
}

bool Lexer::isSpace(char c) {
  if (c == ' ' || c == '\t')
    return true;
  return false;
}

bool Lexer::isNewLine(char c) {
  if (c == '\n')
    return true;
  return false;
}

bool Lexer::isAlphaNumOrUnderscore(char c) {
  if (c == '_' || std::isalpha(c) || std::isdigit(c))
    return true;
  return false;
}

std::string Lexer::extractWord(size_t initialPosition) {
  // Search for the word of length n starting in i.
  size_t n = 0;
  const size_t inputLength = input_.length();
  while (initialPosition + n < inputLength) {
    if (isAlphaNumOrUnderscore(input_[initialPosition + n]))
      ++n;
    else
      break;
  }

  // Extract the word.
  std::string word = input_.substr(initialPosition, n);

  // Update the position until which the tokens have been extracted.
  position_ = initialPosition + n;

  return word;
}