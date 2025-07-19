#pragma once
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <source_location>
#include <string>
#include <thread>

namespace kod {

	class Logger
	{
	public:
		enum class ThreadState
		{
			RUNNING,
			STOPPED
		};

		enum class LogSeverity
		{
			LDEBUG = 0L,
			LINFO = 1,
			LWARNING = 2,
			LERROR = 3,
			LFATAL = 4,
		};

		Logger();
		~Logger();
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(Logger&&) = delete;

		void log(kod::Logger::LogSeverity lvl, const char* message,
			const std::source_location loc = std::source_location::current());

		void setLogLevel(kod::Logger::LogSeverity lvl);
		void setFileNameMaxLenght(size_t fileNameMaxCharacter);
		void shouldLogToConsole(bool shouldLoogToConsole);
		void clearLogFile();

		void join();
		void stop();

	private:
		void logHandler();
		std::string getSeverityString(kod::Logger::LogSeverity lvl);
		std::string getCurrentTime();
		std::string getCurrentThreadID();
		std::string formatFileName(const std::string& file);
		std::string prepareLogMessage(kod::Logger::LogSeverity lvl, const std::string& message, const std::string& file,
			uint_least32_t line);

		kod::Logger::LogSeverity m_minLogLevel = kod::Logger::LogSeverity::LFATAL;
		std::atomic<kod::Logger::ThreadState> m_state = kod::Logger::ThreadState::RUNNING;

		std::string m_filename;
		std::ofstream m_file;
		std::thread m_thread;
		std::condition_variable_any m_cv;
		std::queue<std::string> m_queue;

		size_t m_fileNameMaxCharacter = 45;

		std::recursive_mutex m_mutex;
		bool m_consolLog = true;
		bool m_shouldCleanFileOnStartup = false;

	};

} // namespace kod

extern kod::Logger g_logger;

#define LOG_D(message) g_logger.log(kod::Logger::LogSeverity::LDEBUG, message)
#define LOG_I(message) g_logger.log(kod::Logger::LogSeverity::LINFO, message)
#define LOG_W(message) g_logger.log(kod::Logger::LogSeverity::LWARNING, message)
#define LOG_E(message) g_logger.log(kod::Logger::LogSeverity::LERROR, message)
#define LOG_F(message) g_logger.log(kod::Logger::LogSeverity::LFATAL, message)
