#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <error.hpp>
#include <fstream>
#include <iomanip>
#include <sstream>

void mdie::ErrorHandler::print_warning(const std::string &msg) {
	MessageBox(0, msg.c_str(), "Warning", MB_ICONWARNING | MB_OK);
	print_log("Warning: " + msg + "\n");
}

std::string mdie::ErrorHandler::make_message(const std::string &msg, mdie::Result code) const noexcept {
	std::stringstream logMsg;
	std::time_t timestamp = std::time(nullptr);

	logMsg << "Time: " << std::put_time(std::localtime(&timestamp), "%c");

	logMsg << "\nCode: " << code << "\nMessage: " << msg << "\n";
	return logMsg.str();
}

void mdie::ErrorHandler::show_message(const std::string &msg) const noexcept {
	MessageBox(nullptr, msg.c_str(), "Error", MB_ICONERROR | MB_OK);
}

void mdie::ErrorHandler::print_log(const std::string &msg) noexcept {
	std::lock_guard<std::mutex> lock(m_fileLock);
	std::ofstream log(m_logPath, std::ios::app);
	if (!log.is_open()) { return; }
	log << msg;
	log.close();
}
