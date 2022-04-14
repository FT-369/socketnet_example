#include "utils.hpp"

int	error_handling(std::string error, int ret)
{
	std::cerr << error << std::endl;
	return(ret);
}

std::vector<std::string> split(std::string const line, std::string const delimiter)
{
	std::vector<std::string> words;
	std::string new_line = line;
	size_t delimiter_len = delimiter.length();

	size_t pos;
	while ((pos = new_line.find(delimiter)) != std::string::npos) {
		words.push_back(new_line.substr(0, pos));
		new_line = new_line.substr(pos + delimiter_len);
	}
	words.push_back(new_line);

	return words;
}