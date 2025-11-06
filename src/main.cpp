#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stdlib.h>
#include <unistd.h>

std::unordered_set<std::string> builtins = {"exit","echo","type"};

std::string get_env_var(const std::string & key )
{
  char * val;
  val = std::getenv(key.c_str());
  std::string retval = "";
  if(val != NULL)
  {
    retval = val;
  }
  return retval;
}

std::vector<std::string> split_string(std::string input, char delimiter)
{
  std::stringstream ss(input);
  std::string token;
  std::vector<std::string> tokens;
  while(std::getline(ss,token,delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

std::string remove_quotes(std::string& s)
{
  std::string clean_string = s;
  if(s[0] == '\'')
  {
    clean_string.erase(std::remove(clean_string.begin(),clean_string.end(), '\''),clean_string.end());
  }
  else if(s[0] == '\"')
  {
    clean_string.erase(std::remove(clean_string.begin(),clean_string.end(), '\"'),clean_string.end());
  }
  return clean_string;
}

std::string check_in_env(std::string command)
{
  std::vector<std::string> paths = split_string(get_env_var("PATH"), ':');
  for(auto& path: paths)
  {
    std::string full_path = path + "/" + command;
    if(access(full_path.c_str(), F_OK) == 0 && access(full_path.c_str(), X_OK) == 0 )
    {
      return full_path;
    }
  }
  return "";
}

void echo(std::vector<std::string>& args)
{
  for(std::string& arg: args)
  {
    std::cout << remove_quotes(arg) << " ";
  }
  std::cout << std::endl;
  return;
}

int exit(std::vector<std::string> args)
{
  if(!args.empty())
  {
    int exit_code = std::stoi(args[0]);
    return exit_code;
  }
  return 0;
}

void type(std::vector<std::string> args)
{
  std::string command = args[0];
  if(builtins.find(command) != builtins.end())
  {
    std::cout << command << " is a shell builtin\n";
  }
  else{
    std::string checkEnv = check_in_env(command);
    if( checkEnv != "")
      std::cout << command << " is " << checkEnv << "\n";
    else
      std::cout << command << ": not found\n";
  }
  return;
}

enum class ParseState {
  NORMAL,
  QUOTE,
  DOUBLE_QUOTE
};

void handle_input(std::string& command, std::vector<std::string>& args)
{
  args.clear();
  std::string line;
  std::string arg;
  std::cin >> command;
  std::getline(std::cin, line);
  line.erase(0,1);

  std::string current_arg = "";
  ParseState current_state = ParseState::NORMAL;
  for(char c: line)
  {
    if(current_state == ParseState::NORMAL)
    {
      if(c == ' ')
      {
        if(!current_arg.empty())
          args.push_back(current_arg);
        current_arg.clear() ;
      }else if( c == '\'')
      {
        current_arg += c;
        current_state = ParseState::QUOTE;
      }else if( c == '\"')
      {
        current_arg += c;
        current_state = ParseState::DOUBLE_QUOTE;
      }else
        current_arg += c;
    } 
    else if(current_state == ParseState::QUOTE )
    {
      if(c == '\'')
      {
        current_state = ParseState::NORMAL;
      }
      current_arg += c;
    }else if(current_state == ParseState::DOUBLE_QUOTE)
    {
      if(c == '\"')
      {
        current_state = ParseState::NORMAL;
      }
      current_arg += c;
    }
  }
  if(current_arg != "")
    args.push_back(current_arg);
  
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  while(true)
  {
    std::cout << "$ ";
    std::string input_cmd;
    std::vector<std::string> args;

    handle_input(input_cmd, args);
    if(input_cmd == "exit")
    {
      return exit(args);
    }
    else if(input_cmd == "echo")
    {
      echo(args);
    }
    else if(input_cmd == "type")
    {
      type(args);
    }
    else
    {
      std::string execPath = check_in_env(input_cmd);
      if(execPath != "")
      {
        std::string full_cmd = input_cmd;
        for(auto arg: args)
        {
          full_cmd = full_cmd + " " + arg;
        }
        // std::cout << full_cmd << std::endl;
        std::system(full_cmd.c_str());
      }
      else
        std::cout << input_cmd << ": command not found\n";
    }
  }
}
