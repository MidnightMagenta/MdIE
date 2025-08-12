#include <file.hpp>
#include <fstream>
#include <sstream>

mdie::Result mdie::read_plaintext(const std::string &path, std::string *data) {
	std::ifstream file(path);
	if (!file.is_open()) { return mdie::Result::COULD_NOT_OPEN_FILE; }
	std::stringstream buff;
	buff << file.rdbuf();
	*data = buff.str();
	return mdie::Result::SUCCESS;
}

std::string mdie::read_plaintext(const std::string &path) {
	std::string dat;
	if (read_plaintext(path, &dat) != mdie::Result::SUCCESS) { throw std::runtime_error("Could not load file"); }
	return dat;
}
