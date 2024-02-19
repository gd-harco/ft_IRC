#include "utility.hpp"

#include <client.hpp>

std::queue<std::string> split(const std::string &toSplit, const std::string &delimiter) {
	std::queue<std::string> returned;
	unsigned long start = 0;
	unsigned long end = toSplit.find(delimiter);
	while (end != std::string::npos) {
		returned.push(toSplit.substr(start, end - start) + delimiter);
		start = end + delimiter.size();
		end = toSplit.find(delimiter, start);
	}
	returned.push(toSplit.substr(start));
	return returned;
}

std::string	processedChannelName(const std::string &argument){
	std::string result = argument.substr(1);
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}
