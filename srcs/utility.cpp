#include "utility.hpp"

std::queue<std::string> split(const std::string &toSplit, const std::string &delimiter) {
	std::queue<std::string> returned;
	unsigned long start = 0;
	unsigned long end = toSplit.find(delimiter);
	while (end != std::string::npos) {
		returned.push(toSplit.substr(start, end - start));
		start = end + delimiter.size();
		end = toSplit.find(delimiter, start);
	}
	returned.push(toSplit.substr(start));
	return returned;
}
