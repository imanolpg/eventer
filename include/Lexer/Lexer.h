#ifndef LEXER_H
#define LEXER_H

#include <string>

enum class TokenType { Identifier, Equals, Value, EndOfFile, Unknown };

struct Token {
  TokenType type;
  std::string value;

  Token() : type(TokenType::Unknown), value("") {}
  Token(TokenType type, const std::string &value) : type(type), value(value) {}
  std::string toString() const;
};

class Lexer {
public:
  Lexer(const std::string &input);
  std::vector<Token> tokenize();

private:
  const std::string &input_;
  size_t position_;

  bool isSpace(char c);
  bool isNewLine(char c);
  bool isAlphaNumOrUnderscore(char c);
  std::string extractWord(size_t initialPosition);
};
#endif // LEXER_H