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
#include "helper.h"

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

void redirect_output(std::string fileName)
{
  int f_d = open(fileName.c_str(), O_CREAT|O_WRONLY, S_IRWXU);
  dup2(f_d,1);
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  while(true)
  {
    std::cout << "$ ";
    std::vector<std::string> args = handle_input();
    std::string fileName = "";
    std::string redirectType = "";
    if(std::find_if(args.begin(),args.end(),[](std::string s){
      return s == ">" || s == "1>";
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
            redirect_output(fileName);
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
