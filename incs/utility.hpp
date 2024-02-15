#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <queue>

std::queue<std::string>	split(const std::string &toSplit, const std::string &delimiter);
std::string				processedChannelName(const std::string &argument);
#endif //UTILITY_HPP
