/*
 * Copyright (c) 2023-2024 Mohammed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 *
 */

#ifndef lang_compiler_h
#define lang_compiler_h

#include "scanner.h"

#include "../shared/common.h"
#include "../shared/value.h"
#include "../utils/debug.h"

class Parser {
  Token current;
  Token previous;
  bool hadError;
  Scanner scanner;
  Bytecode *bytecode;

  typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
  } Precedence;

  typedef void (Parser::*ParseFn)();
  typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
  } ParseRule;

  void errorAt(Token *token, const std::string message);
  void error(const std::string message);
  void errorAtCurrent(const std::string message);
  void advance();
  bool check(TokenType type);
  void consume(TokenType type, const std::string message);
  bool match(TokenType type);
  void emitByte(uint32_t byte);
  void emitBytes(uint32_t byte1, uint32_t byte2);
  void emitReturn();
  uint32_t makeConstant(int value);
  uint32_t makeConstant(std::string value);
  void emitConstant(int value);
  void emitConstant(std::string value);
  void endCompiler();
  uint32_t identifierConstant(Token *name);
  uint32_t parseVariable(const char *errorMessage);
  void defineVariable(uint32_t global);
  void binary();
  void literal();
  void grouping();
  void number();
  void string();
  void namedVariable(Token name);
  void variable();
  void unary();
  void parsePrecedence(Precedence precedence);
  ParseRule getRule(TokenType type);
  void expression();
  void varDeclaration();
  void expressionStatement();
  void printStatement();
  void declaration();
  void statement();

public:
  Parser(const std::string &source);
  bool compile(Bytecode *bcode);
};

#endif