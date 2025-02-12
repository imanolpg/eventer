#include "Lexer/TokenType.h"
#include <string>

/**
 * @brief Map TokenType values to string.
 *
 * @param type TokenType to converto to string.
 * @return std::string value.
 */
std::string tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::Save:
    return "Save";

  case TokenType::In:
    return "In";

  case TokenType::Create:
    return "Create";

  case TokenType::Done:
    return "Done";

  case TokenType::Addition:
    return "Addition";

  case TokenType::Subtraction:
    return "Subtraction";

  case TokenType::VariableIdentifier:
    return "VariableIdentifier";

  case TokenType::ProcedureIdentifier:
    return "ProcedureIdentifier";

  case TokenType::Number:
    return "Number";

  case TokenType::Word:
    return "Word";

  case TokenType::EndOfInstruction:
    return "EndOfInstruction";

  case TokenType::EndOfFile:
    return "EndOfFile";

  default:
    return "Unknown";
  }
}

std::map<std::string, TokenType> keywords = {{"save", TokenType::Save},
                                             {"in", TokenType::In},
                                             {"create", TokenType::Create},
                                             {"done", TokenType::Done}};

std::map<std::string, TokenType> operations = {{"+", TokenType::Addition},
                                               {"-", TokenType::Subtraction}};

bool isKeyword(std::string s) {
  try {
    keywords.at(s);
    return true;
  } catch (std::out_of_range &e) {
    return false;
  }
}