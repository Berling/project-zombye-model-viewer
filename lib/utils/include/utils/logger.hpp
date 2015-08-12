#ifndef __UTILS_LOGGER_HPP__
#define __UTILS_LOGGER_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace utils {
	enum log_level {
		LOG_INFO,
		LOG_DEBUG,
		LOG_WARNING,
		LOG_ERROR,
		LOG_FATAL
	};

	class logger {
	public:
		logger();
		~logger();

		logger(const logger& rhs) = delete;
		logger& operator=(const logger& rhs) = delete;

		logger(logger&& rhs) = delete;
		logger& operator=(logger&& rhs) = delete;

		std::string format_time(const std::string& format);

		logger& operator()(log_level level) noexcept;
		logger& operator<<(std::ostream& (*ostream_function)(std::ostream&));

		template <typename type>
		logger& operator<<(const type& rhs) {
			std::stringstream ss;
			
			if (new_line_) {
				ss << format_time("[%Y-%m-%d %X]");

				switch(level_) {
					case LOG_INFO:
						ss << "[INFO] ";
						break;
					case LOG_DEBUG:
						ss << "[DEBUG] ";
						break;
					case LOG_WARNING:
						ss << "[WARN] ";
						break;
					case LOG_ERROR:
						ss << "[ERROR] ";
						break;
					case LOG_FATAL:
						ss << "[FATAL] ";
						break;
				}

				new_line_ = false;
			}

			ss << rhs;

			os_ << ss.str();
			std::cout << ss.str();

			return *this;
		}

	private:
		std::ofstream os_;
		log_level level_;
		bool new_line_;
	};


	extern logger log;
}

#endif