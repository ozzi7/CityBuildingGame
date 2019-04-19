#pragma once
#include <concurrentqueue.h>
#include <logging_event.h>
#include <map>
#include <ctime>
#include <fstream>

class Grid;

class LoggingEventHandler : public LoggingEventVisitor {
public:
	LoggingEventHandler::LoggingEventHandler(std::string filename, LoggingLevel fileLoggingLevel, LoggingLevel consoleLoggingLevel);
	void AddEvent(LoggingEvent*);
	LoggingEvent * GetEvent();
	bool ProcessEvent();
	void Visit(LoggingEvent * aLoggingEvent);

private:
	moodycamel::ConcurrentQueue<LoggingEvent*> cq;
	std::string logFileName;

	LoggingLevel fileLoggingLevel;
	LoggingLevel consoleLoggingLevel;

	std::fstream file;
};
