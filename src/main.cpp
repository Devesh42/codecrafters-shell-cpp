#include <iostream>
#include <string>
#include <sstream>

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
      int exit_code = 0;
      std::cin >> exit_code;
      return exit_code;
    }
    else if(input_cmd == "echo")
    {
      echo();
    }
    else
    {
      std::cout << input_cmd << ": command not found\n";
    }
  }
}
