#include "Lexer/Token.h"
#include "Lexer/TokenType.h"
#include <string>

/**
 * @brief Construct a new Token:: Token object.
 * Generate a token and assing the TokenType automatically.
 *
 * @param value token value.
 */
Token::Token(std::string value) {
  // Check if the token is a keyword.
  try {
    TokenType type = keywords.at(value);
    value_ = value;
    type_ = type;
    return;
  } catch (const std::out_of_range &e) {
  };

  // Check if the token is an operation.
  try {
    TokenType type = operations.at(value);
    value_ = value;
    type_ = type;
    return;
  } catch (const std::out_of_range &e) {
  };

  // Check if token is a number.
  try {
    std::stod(value); // Convert value to double throws an erro if the token is
                      // not a number.
    value_ = value;
    type_ = TokenType::Number;
    return;
  } catch (std::invalid_argument) {
  } catch (std::out_of_range) {
  };

  // Else the token is a word.
  value_ = value;
  type_ = TokenType::Word;
}

/**
 * @brief Convert Token to string.
 *
 * @return std::string value.
 */
std::string Token::toString() const {
  const int spaceToLeave = 40;
  std::string tokenString = "Token(Type: ";
  std::string typeString = tokenTypeToString(type_);
  tokenString += padLeft(typeString, spaceToLeave);
  tokenString += "Value: '" + value_ + "')";
  return tokenString;
}

/**
 * @brief Adds chars at the beggining of the string to match the desire length.
 *
 * @param str padding string.
 * @param spaces size of resulting string.
 * @return std::string padded string.
 */
std::string padLeft(std::string &str, size_t spaces = 0) {
  for (size_t i = str.length(); i <= spaces - str.length(); ++i)
    str += " ";
  return str;
}
