#pragma once
#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char delimiter);
std::vector<std::string> readDir(const std::string & path, const std::string & type);
