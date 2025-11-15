#ifndef PARSER_H
#define PARSER_H

#include <cstring>
#include <vector>
#include <string>

#define BUF_SIZE 1000

enum class ParseState {
  NORMAL,
  QUOTE,
  DOUBLE_QUOTE,
  ESCAPE
};

std::vector<std::string> handle_input();

#endif