#pragma once
#include <logging_event_visitor.h>
#include <iostream>
#include <windows.h>

enum LoggingLevel { CRITICAL = 0, ERROR_L = 1, WARNING = 2, INFO = 3, DEBUG = 4, NOTSET = 5 };

class LoggingEvent
{
public:
	LoggingEvent(LoggingLevel aLoggingLevel, std::thread::id aThread_id, DWORD aTimestamp, std::string aLog_text)
	{
		loggingLevel = aLoggingLevel;
		thread_id = aThread_id;
		timestamp = aTimestamp;
		log_text = aLog_text;
	}
	LoggingEvent(LoggingLevel aLoggingLevel, std::string aLog_text)
	{
		loggingLevel = aLoggingLevel;
		thread_id = std::this_thread::get_id();
		timestamp = GetTickCount();
		log_text = aLog_text;
	}
	void LoggingEvent::Accept(LoggingEventVisitor* eventVisitor)
	{
		eventVisitor->Visit(this);
	}

	LoggingLevel loggingLevel;
	std::thread::id thread_id;
	DWORD timestamp;
	std::string log_text;
};