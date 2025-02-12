#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "Lexer/Token.h"

class Lexer {
public:
  Lexer(const std::string &input);
  std::vector<Token> tokenize();

private:
  const std::string &input_; // Code to tokenize.
  size_t position_;          // Position of tokenizing character.
  const size_t inputLength_; // Length of code to tokenize.
  std::set<std::string> variableIdentifiers_;  // Variables identifier list.
  std::set<std::string> procedureIdentifiers_; // Procedures identifier list.

  bool isSpace();
  bool isNewLine();
  std::string readNextToken();
  Token consumeNextToken();
  bool isCurrentCharANewLineChar();
  bool isAtEndOfFile();
  void skipWhitespaces();
  Token createToken(TokenType type, std::string value);
  Token createToken(std::string value);
  bool isVariableIdentifier(std::string s);
  bool isProcedureIdentifier(std::string s);
  bool isIdentifierInProcedureCall(std::string s);
  bool isPreviousTokenIn(std::vector<Token> tokens);
  bool isPreviousTokenCreate(std::vector<Token> tokens);
  bool isPreviousTokenEndOfInstruction(std::vector<Token> tokens);
};
#endif // LEXER_H