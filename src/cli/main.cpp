/*
 * Copyright (c) 2023-2024 Mohammed Abdifatah. All rights reserved.
 * Distributed Under The MIT License
 *
 * src/cli/main:
 *   main compiler
 */

#include "../generator/generator.h"
#include "../shared/common.h"

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }
  std::string source = readFile(argv[1]);
  Parser parser(source);

  Bytecode bytecode;
  bool hadError = parser.compile(&bytecode);
  if (hadError) {
    std::cerr << "Compilling Failed!" << std::endl;
    return 1;
  }
  Generator gen("saynaa.asm");

  gen.main(bytecode);


  std::puts(""); // new line

  if (std::system("nasm -g -f elf64 saynaa.asm && ld saynaa.o -o app"))
    return 1;
  return 0;
}