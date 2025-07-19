#include "Logger.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>

using namespace kod;

class LoggerTest : public ::testing::Test {
protected:
	Logger logger;

	void SetUp() override {
		logger.setLogLevel(Logger::LogSeverity::LDEBUG);
		logger.setFileNameMaxLenght(50);
		logger.shouldLogToConsole(false);
		logger.clearLogFile();
	}

	std::string readLogFile() {
		std::ifstream file("logger.log");
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
};

TEST_F(LoggerTest, LogSingleMessageToFile) {
	logger.log(Logger::LogSeverity::LINFO, "Test message");
	logger.stop();
	logger.join();

	std::string content = readLogFile();
	EXPECT_NE(content.find("Test message"), std::string::npos);
	EXPECT_NE(content.find("INFO"), std::string::npos);
}

TEST_F(LoggerTest, LogLevelFiltering) {
	logger.setLogLevel(Logger::LogSeverity::LERROR);
	logger.log(Logger::LogSeverity::LINFO, "Should NOT appear");
	logger.log(Logger::LogSeverity::LERROR, "Should appear");
	logger.stop();
	logger.join();

	std::string content = readLogFile();
	EXPECT_EQ(content.find("Should NOT appear"), std::string::npos);
	EXPECT_NE(content.find("Should appear"), std::string::npos);
}

TEST_F(LoggerTest, ClearLogFile) {
	logger.log(Logger::LogSeverity::LINFO, "Message 1");
	logger.stop();
	logger.join();

	logger.clearLogFile();

	std::ifstream file("logger.log");
	EXPECT_TRUE(file.peek() == std::ifstream::traits_type::eof());
}

TEST_F(LoggerTest, SetFileNameMaxLength) {
	logger.setFileNameMaxLenght(15);
	logger.log(Logger::LogSeverity::LINFO, "Test");
	logger.stop();
	logger.join();

	std::string content = readLogFile();
	EXPECT_NE(content.find("...t_logger.cpp:62"), std::string::npos); // accoriding to "Test" message line in this file
	EXPECT_NE(content.find("Test"), std::string::npos);
}
