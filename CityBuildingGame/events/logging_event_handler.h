#pragma once
#include <concurrentqueue.h>
#include <logging_event.h>
#include <map>
#include <ctime>
#include <fstream>
#include <iomanip>

class Grid;

class LoggingEventHandler : public LoggingEventVisitor
{
public:
	LoggingEventHandler::LoggingEventHandler();
	void AddEvent(LoggingEvent*);
	LoggingEvent* GetEvent();
	bool ProcessEvent();
	void Visit(LoggingEvent* aLoggingEvent) override;

private:
	moodycamel::ConcurrentQueue<LoggingEvent*> cq;
	std::string logFileName;

	LoggingLevel fileLoggingLevel;
	LoggingLevel consoleLoggingLevel;

	std::fstream file;
	std::map<int, std::string> logging_levels;
};
