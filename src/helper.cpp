#include <sstream>
#include <algorithm>
#include "helper.h"
#include <unistd.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

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

std::vector<std::string> add_custom_execs()
{
  std::vector<std::string> paths = split_string(get_env_var("PATH"), ':');
  std::vector<std::string> executables;
  for(const auto& path: paths)
  {
    for(const auto& entry: fs::recursive_directory_iterator(path))
    {
      fs::path entryPath = entry.path();
      executables.push_back(entryPath.filename());
    }
  }
  return executables;
}
