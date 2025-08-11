#ifndef MDIE_ERROR_H
#define MDIE_ERROR_H

#include <exception>
#include <mutex>
#include <singleton.hpp>
#include <string>

#define MDIE_LOG(msg) mdie::ErrorHandler::get_instance()->print_info(msg);
#define MDIE_WARN(msg) mdie::ErrorHandler::get_instance()->print_warning(msg);
#define MDIE_LOG_ERROR(msg, res) mdie::ErrorHandler::get_instance()->print_error(msg, res);

#ifdef _DEBUG
#define MDIE_LOG_DEBUG(msg) MDIE_LOG(msg)
#else
#define MDIE_LOG_DEBUG(msg) /* void */
#endif						// _DEBUG

namespace mdie {
enum Result { SUCCESS, TEST_ERROR, GL_ERROR };

class ErrorHandler : public Singleton<ErrorHandler> {
public:
	void set_log_path(const std::string &path) { m_logPath = path; }
	void print_info(const std::string &msg) { print_log("Info: " + msg + "\n"); }
	void print_warning(const std::string &msg);
	void print_error(const std::string &msg, const Result res) {
		std::string message = make_message(msg, res);
		print_log(message);
		show_message(message);
	}

private:
	std::string make_message(const std::string &msg, Result code) const noexcept;
	void show_message(const std::string &msg) const noexcept;
	void print_log(const std::string &msg) noexcept;

	std::string m_logPath = "./log.txt";
	std::mutex m_fileLock;
};
}// namespace mdie

#endif// !MDIE_ERROR_H
