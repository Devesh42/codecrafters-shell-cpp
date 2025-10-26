#include <iostream>
#include <string>
#include <sstream>
#include <vector>
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

void echo()
{
  std::string line;
  std::string arg;
  std::getline(std::cin, line);
  std::stringstream ss(line);
  while(ss >> arg)
  {
    std::cout << arg << " ";
  }
  std::cout << "\n";
  return;
}

int exit()
{
  int exit_code = 0;
  std::cin >> exit_code;
  return exit_code;
}

void type()
{
  std::string command;
  std::cin >> command;
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

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage

  while(true)
  {
    std::cout << "$ ";
    std::string input_cmd = "";
    std::cin >> input_cmd;
    if(input_cmd == "exit")
    {
      return exit();
    }
    else if(input_cmd == "echo")
    {
      echo();
    }
    else if(input_cmd == "type")
    {
      type();
    }
    else
    {
      std::cout << input_cmd << ": command not found\n";
    }
  }
}
