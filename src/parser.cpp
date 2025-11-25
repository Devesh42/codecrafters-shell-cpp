#include "parser.h"
#include <unistd.h>
#include <iostream>

std::vector<std::string> handle_input(Trie& autoCompleter)
{
  std::string line;
  int index = 0;
  int command_len = 0;
  char c;

  while(1)
  {
    if(read(0,&c, 1) == -1)
    {
      perror("read error");
      exit(EXIT_FAILURE);
    }
    if(c == 3) //Handling Ctrl + C
    {
      exit(EXIT_SUCCESS);
    }else if( c == '\t')
    {
      line = autoCompleter.autoComplete(line);
      std::cout << " ";
      line += " ";
    }else if(c == '\r' || c == 4)
    {
      std::cout << '\n';
      break;
    }
    else
    {
      std::cout << c;
      line += c;
    }
  }

  command_len = line.size();
  ParseState current_state = ParseState::NORMAL;
  ParseState previous_state = ParseState::NORMAL;

  std::vector<std::string> args;
  std::string current_arg = "";
  for(int i=0; i < command_len; i++)
  {
    char c = line[i];
    if(current_state == ParseState::ESCAPE)
    {
      if(previous_state == ParseState::DOUBLE_QUOTE && (c == '\"' || c == '\\'))
        current_arg.pop_back();
      current_arg += c;
      current_state = previous_state;
    }
    else if(current_state == ParseState::NORMAL)
    {
      if(c == '\\')
      {
        previous_state = ParseState::NORMAL;
        current_state = ParseState::ESCAPE;
        continue;
      }
      else if(c == ' ')
      {
        if(!current_arg.empty())
          args.push_back(current_arg);
        current_arg.clear() ;
      }else if( c == '\'')
      {
        current_state = ParseState::QUOTE;
      }else if( c == '\"')
      {
        current_state = ParseState::DOUBLE_QUOTE;
      }else
      {
        current_arg += c;
      }
    } 
    else if(current_state == ParseState::QUOTE )
    {
      if(c == '\'')
      {
        current_state = ParseState::NORMAL;
      }else
      current_arg += c;
    }else if(current_state == ParseState::DOUBLE_QUOTE)
    {
      if(c == '\"')
      {
        current_state = ParseState::NORMAL;
      }else if(c == '\\')
      {
        previous_state = ParseState::DOUBLE_QUOTE;
        current_state = ParseState::ESCAPE;
        current_arg += c;
      }
      else
        current_arg += c;
    }
  }
  if(current_arg != "")
    args.push_back(current_arg);

  return args;
}