#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1000

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
  }else if(clean_string.find("\"\"") != std::string::npos)
  {
    clean_string.erase(std::remove(clean_string.begin(),clean_string.end(), '\"'),clean_string.end());
  }else if(clean_string.find("\'\'") != std::string::npos)
  {
    clean_string.erase(std::remove(clean_string.begin(),clean_string.end(), '\''),clean_string.end());
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

int exit(std::vector<std::string>& args)
{
  if(args.size() >= 2)
  {
    int exit_code = std::stoi(args[1]);
    return exit_code;
  }
  return 0;
}

void type(std::string command)
{
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
  DOUBLE_QUOTE,
  ESCAPE
};

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
      }
      else
        current_arg += c;
    }
  }
  if(current_arg != "")
    args.push_back(current_arg);

  return args;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  while(true)
  {
    std::cout << "$ ";
    std::vector<std::string> args = handle_input();
    std::vector<char *> arg_v;
    for(const std::string&arg: args)
    {
      arg_v.push_back(const_cast<char *>(arg.c_str()));
    }
    arg_v.push_back(NULL);

    std::string input_cmd;
    if(!args.empty())
      input_cmd = args[0];
    if(input_cmd == "exit")
    {
      return exit(args);
    }
    else if(input_cmd == "type")
    {
      type(arg_v[1]);
    }
    else
    {
      std::string execPath = check_in_env(input_cmd);
      if(execPath != "")
      {
        pid_t pid = fork();
        if(pid == -1)
        {
          exit(-1);
        }else if(pid > 0)
        {
          int status;
          waitpid(pid, &status, 0);
        }else
        {
          execvp(arg_v[0],arg_v.data());   
          _exit(EXIT_FAILURE);
        }

      }
      else
        std::cout << input_cmd << ": command not found\n";
    }
  }
}
