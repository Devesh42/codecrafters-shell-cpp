#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>
std::string get_env_var(const std::string & key );
std::vector<std::string> split_string(std::string input, char delimiter);
std::string remove_quotes(std::string& s);
std::string check_in_env(std::string command);

#endif //HELPER_H