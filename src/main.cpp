#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>

std::unordered_set<std::string> builtins = {"exit","echo","type"};

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
