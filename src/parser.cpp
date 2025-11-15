#include "parser.h"
std::vector<std::string> handle_input()
{
  char line[BUF_SIZE];
  fgets(line, BUF_SIZE,stdin);
  int command_len = std::strlen(line)-1;
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