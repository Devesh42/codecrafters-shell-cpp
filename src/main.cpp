#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage

  while(true)
  {
    std::cout << "\n$ ";
    std::string input_cmd = "";
    std::cin >> input_cmd;
    std::cout << input_cmd << ": command not found";
  }
}
