#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Lexer/TokenType.h"

/**
 * @brief Construct a new Lexer:: Lexer object.
 *
 * @param input code to tokenize.
 */
Lexer::Lexer(const std::string &input)
    : input_(input), position_(0), inputLength_(input.length()) {}

/**
 * @brief Tokenize the code.
 *
 * @return std::vector<Token> result tokens.
 */
std::vector<Token> Lexer::tokenize() {

  std::vector<Token> tokens;

  Token t;
  bool insertToken;     // Used to control if token should be registered or not.
  bool inProcedureCall; // Used to control if in a procedure call tokens should
  // be treated as words or variable identifiers.

  // Tokenize.
  while (t.type_ != TokenType::EndOfFile) {

    insertToken = true;

    t = consumeNextToken();

    // New identifiers are preceeded by keywords 'in' or 'create'.
    // Check if this condition applies to change the token type and save the
    // new string as an identifier.
    if (isPreviousTokenIn(tokens)) {
      t.type_ = TokenType::VariableIdentifier;
      variableIdentifiers_.insert(t.value_);
    }

    if (isPreviousTokenCreate(tokens)) {
      t.type_ = TokenType::ProcedureIdentifier;
      procedureIdentifiers_.insert(t.value_);
    }

    // Avoid inserting more than one EndOfInstruction token followed up.
    if (t.type_ == TokenType::EndOfInstruction &&
        isPreviousTokenEndOfInstruction(tokens))
      insertToken = false;

    // When calling a procedure, only tokens starting with '->' will be treated
    // as identifiers. The rest of the words will be treated as words even if
    // there is a variableIdentifier with that same name.
    if (t.type_ == TokenType::ProcedureIdentifier &&
        !isPreviousTokenCreate(tokens))
      inProcedureCall = true;
    if (t.type_ == TokenType::EndOfInstruction)
      inProcedureCall = false;

    // Change token types in procedure calls.
    if (inProcedureCall) {
      if (isIdentifierInProcedureCall(t.value_)) {
        t.value_ = t.value_.erase(0, 2);
        t.type_ = TokenType::VariableIdentifier;
      } else if (t.type_ == TokenType::VariableIdentifier) {
        t.type_ = TokenType::Word;
      }
    }

    // Insert token into the token array.
    if (insertToken)
      tokens.push_back(t);
  }

  return tokens;
}

/**
 * @brief Consume the incoming token.
 *
 * @return Token next token.
 */
Token Lexer::consumeNextToken() {
  // Skip whitespaces.
  skipWhitespaces();

  // Check if current character is a new-line to insert a EndOfInstruction
  // token.
  if (isCurrentCharANewLineChar()) {
    position_++; // Increase position here because there is not a function to
                 // consume new-line characters. The general function to read
                 // tokens (readNextToken) skips whitespaces.
    return Token(TokenType::EndOfInstruction, "");
  }

  // Check if the whole program has been tokenized.
  if (isAtEndOfFile())
    return Token(TokenType::EndOfFile, "");

  // Read the next token.
  std::string tokenValue = readNextToken();
  Token t = createToken(tokenValue);

  return t;
}

/**
 * @brief Check if character in position_ is space or tabulation.
 *
 * @return true if character is space or tabulation.
 * @return false if character is not space of tabulation.
 */
bool Lexer::isSpace() {
  char c = input_[position_];
  if (c == ' ' || c == '\t')
    return true;
  return false;
}

/**
 * @brief Check if character in position_ is a new line character.
 *
 * @return true if character is '\n'.
 * @return false if character is not '\n'.
 */
bool Lexer::isNewLine() {
  char c = input_[position_];
  if (c == '\n')
    return true;
  return false;
}

/**
 * @brief Get the token beginning from position_.
 *
 * @return std::string token value.
 */
std::string Lexer::readNextToken() {
  std::string phrase = "";
  while (!isSpace() && !isNewLine() && !isAtEndOfFile()) {
    phrase.push_back(input_[position_]);
    position_++;
  }
  return phrase;
}

/**
 * @brief Checks if the whole code has been analized.
 *
 * @return true if the code has been analyzed.
 * @return false if there is still more code to be analyzed.
 */
bool Lexer::isAtEndOfFile() {
  if (position_ >= inputLength_)
    return true;
  return false;
}

/**
 * @brief Check if current character is a new-line character.
 *  Typically used to insert a EndOfInstruction token.
 *
 * @return true if current character is '\n'.
 * @return false if current character is not '\n'.
 */
bool Lexer::isCurrentCharANewLineChar() {
  if (input_[position_] == '\n')
    return true;
  return false;
}

/**
 * @brief Skips whitespaces characters like ' ' and '\t'.
 *
 */
void Lexer::skipWhitespaces() {
  while (input_[position_] == ' ' || input_[position_] == '\t') {
    position_++;

    if (isAtEndOfFile())
      break;
  }
}

/**
 * @brief Create a new token and set the correct token type.
 *
 * @param type token type. It will be redifined after constructing the token
 * object and checking if it is an identifier.
 * @param value token value.
 * @return Token generated token.
 */
Token Lexer::createToken(TokenType type, std::string value) {
  // Create the token object.
  Token t(value);

  // Identifiers will be set correctly after creation because the identifiers
  // set is in the lexer. The Token constructor has no way to know if a word is
  // an identifier. The Token constructor will return an identifier as a Word
  // type.
  if (isVariableIdentifier(t.value_))
    t.type_ = TokenType::VariableIdentifier;

  if (isProcedureIdentifier(t.value_))
    t.type_ = TokenType::ProcedureIdentifier;

  return t;
}

/**
 * @brief Craate a new token without specifing the token type.
 *
 * @param value token value.
 * @return Token generated token.
 */
Token Lexer::createToken(std::string value) {
  return createToken(TokenType::Unknown, value);
}

/**
 * @brief Check if a word has been declared as a variable identifier.
 *
 * @param s word to check.
 * @return true if word is a variable identifier.
 * @return false if word is not a variable identifier.
 */
bool Lexer::isVariableIdentifier(std::string s) {
  if (variableIdentifiers_.find(s) != variableIdentifiers_.end())
    return true;
  return false;
}

/**
 * @brief Check if a word has been initialized as a procedure identifier.
 *
 * @param s word to check.
 * @return true if word is a procedure identifier.
 * @return false if word is not a procedure identifier.
 */
bool Lexer::isProcedureIdentifier(std::string s) {
  if (procedureIdentifiers_.find(s) != procedureIdentifiers_.end())
    return true;
  return false;
}

/**
 * @brief Checks if token is a variable identifier in a procedure call.
 *
 * @param s token to check.
 * @return true if token is a variable identifier.
 * @return false if token is not a variable identifier.
 */
bool Lexer::isIdentifierInProcedureCall(std::string s) {
  if (s.starts_with("->"))
    if (isVariableIdentifier(s.erase(0, 2)))
      return true;
  return false;
}

/**
 * @brief Check if the last generated token is of type In.
 * Typically used to know if next token is a variable identifier or not.
 *
 * @param tokens tokens generated so far.
 * @return true if previous token is of type In.
 * @return false if previous token is not of type In.
 */
bool Lexer::isPreviousTokenIn(std::vector<Token> tokens) {
  // Check for empty vector.
  if (tokens.size() == 0)
    return false;

  // Get last registered token.
  TokenType lastTokenType = tokens[tokens.size() - 1].type_;
  if (lastTokenType == TokenType::In)
    return true;

  return false;
}

/**
 * @brief Check if the last generated token is of type Create.
 * Typically used to know if next token is a procedure identifier or not.
 *
 * @param tokens tokens generated so far.
 * @return true if previous token is of type Create.
 * @return false if previous token is not of type Create.
 */
bool Lexer::isPreviousTokenCreate(std::vector<Token> tokens) {
  // Check for empty vector.
  if (tokens.size() == 0)
    return false;

  // Get last registered token.
  TokenType lastTokenType = tokens[tokens.size() - 1].type_;
  if (lastTokenType == TokenType::Create)
    return true;

  return false;
}

/**
 * @brief Check if the last generated token is of type EndOfInstruction.
 * Typically used to track two followed EndOfInstructino tokens and ignore the
 * last one.
 *
 * @param tokens tokens generated so far.
 * @return true if previous token is of type EndOfInstruction.
 * @return false if previous token is not of type EndOfInstruction.
 */
bool Lexer::isPreviousTokenEndOfInstruction(std::vector<Token> tokens) {
  // Check for empty vector.
  if (tokens.size() == 0)
    return false;

  // Get last registered token.
  TokenType lastTokenType = tokens[tokens.size() - 1].type_;
  if (lastTokenType == TokenType::EndOfInstruction)
    return true;

  return false;
}