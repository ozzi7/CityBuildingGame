#include "stdafx.h"
#pragma once
#include <logging_event_handler.h>
#include <grid.h>

LoggingEventHandler::LoggingEventHandler(std::string filename, LoggingLevel aFileLoggingLevel, LoggingLevel aConsoleLoggingLevel)
{
	time_t *rawtime = new time_t; // we'll get the time here by time() function.
	struct tm * timeinfo; /* we'll get the time info. (sec, min, hour, etc...) here from rawtime by the localtime() function */
	time(rawtime); // Get time into rawtime
	timeinfo = localtime(rawtime); // Get time info into timeinfo

	logFileName = filename + "_" + std::to_string(timeinfo->tm_year+1900) + "-" + std::to_string(timeinfo->tm_mon+1) + "-" + 
		std::to_string(timeinfo->tm_mday) + "_" + std::to_string(timeinfo->tm_hour) + "h" + std::to_string(timeinfo->tm_min) + "m" + 
		std::to_string(timeinfo->tm_sec) +"s";

	file.open(Path + "/../log/" + logFileName, std::ios::out);
	
	if (!file)
	{
		std::cout << "Could not create log file!" << std::endl;
	}

	fileLoggingLevel = aFileLoggingLevel;
	consoleLoggingLevel = aConsoleLoggingLevel;
}
void LoggingEventHandler::AddEvent(LoggingEvent * e)
{
	cq.enqueue(e);
}
LoggingEvent * LoggingEventHandler::GetEvent()
{
	LoggingEvent * e;
	if (cq.try_dequeue(e))
	{
		return e;
	}
	else
	{
		return NULL;
	}
}
bool LoggingEventHandler::ProcessEvent()
{
	LoggingEvent * logging_event = GetEvent();
	if (logging_event != NULL)
	{
		logging_event->Accept(this);
		return true;
	}
	return false;
}
void LoggingEventHandler::Visit(LoggingEvent * aLoggingEvent)
{
	std::stringstream ss;
	ss << aLoggingEvent->thread_id;
	uint64_t id = std::stoull(ss.str());

	std::string output = "Thread " + std::to_string(id) + ": " + aLoggingEvent->log_text;
	if (aLoggingEvent->loggingLevel <= fileLoggingLevel) {
		file << output << std::endl;
	}
	if (aLoggingEvent->loggingLevel <= consoleLoggingLevel) {
		std::cout << output << std::endl;
	}
}
