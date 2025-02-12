#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <map>
#include <string>

enum class TokenType {
  // Keywords.
  Save,
  In,
  Create,
  Colon,
  Done,

  // Operations.
  Addition,
  Subtraction,

  // Numbers.
  Number,

  // String literals.
  Word,

  // Identifiers.
  VariableIdentifier,
  ProcedureIdentifier,

  // End of instruction (end of line).
  EndOfInstruction,

  // End of file.
  EndOfFile,

  // Unknown token.
  Unknown,
};

std::string tokenTypeToString(TokenType type);

extern std::map<std::string, TokenType> keywords;
extern std::map<std::string, TokenType> operations;

extern bool isKeyword(std::string s);

#endif // TOKENTYPE_H