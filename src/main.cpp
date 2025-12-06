#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser.h"
#include "trie.h"
#include "helper.h"
#include <termios.h>

static struct termios old_termios, new_termios;
std::unordered_set<std::string> builtins = {"exit","echo","type"};

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

void redirect_output(int old_fd,std::string fileName,bool isAppend)
{
  int f_d;
  if(isAppend)
    f_d = open(fileName.c_str(), O_CREAT|O_WRONLY|O_APPEND, 0642);
  else
    f_d = open(fileName.c_str(), O_CREAT|O_WRONLY, 0642);
  dup2(f_d,old_fd);
}

void reset_terminal()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

void configure_terminal()
{
  tcgetattr(STDIN_FILENO, &old_termios);
  atexit(reset_terminal);
  new_termios = old_termios;
  new_termios.c_iflag &= ~(BRKINT| ICRNL | INPCK | ISTRIP | IXON);
  new_termios.c_lflag &= ~(ICANON | ECHO |  IEXTEN | ISIG);
  new_termios.c_cc[VMIN] = 1;
  new_termios.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH ,&new_termios);
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  configure_terminal();
  Trie autoCompleter = Trie();
  for(std::string command: builtins)
    autoCompleter.insert(command);
  std::vector<std::string> custom_execs = add_custom_execs();
  for(auto& exec: custom_execs)
  {
    autoCompleter.insert(exec);
  }
  while(true)
  {
    std::cout << "$ ";
    std::vector<std::string> args = handle_input(autoCompleter);
    std::string fileName = "";
    std::string redirectType = "";
    if(std::find_if(args.begin(),args.end(),[](std::string s){
      return s == ">" || s == "1>" || s == "2>" || s == ">>" || s == "2>>" || s == "1>>";
    }) != args.end()){
      if(args.size() >= 2)
      {
        fileName = args.back();
        args.pop_back();
        redirectType = args.back();
        args.pop_back();
      }
    }
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
          if(!redirectType.empty())
          {
            int old_fd = 1;
            bool isAppend = false;
            if(redirectType.find('2') != std::string::npos)
              old_fd = 2;
            if(redirectType.find(">>") != std::string::npos)
              isAppend = true;
            redirect_output(old_fd,fileName, isAppend);
          }
          execvp(arg_v[0],arg_v.data());   
          _exit(EXIT_FAILURE);
        }

      }
      else
        std::cout << input_cmd << ": command not found\n";
    }
  }
}
