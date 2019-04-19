#include "stdafx.h"
#pragma once
#include <logging_event_handler.h>
#include <grid.h>

LoggingEventHandler::LoggingEventHandler(LoggingLevel aFileLoggingLevel, LoggingLevel aConsoleLoggingLevel)
{
	time_t *rawtime = new time_t; // we'll get the time here by time() function.
	struct tm * timeinfo; /* we'll get the time info. (sec, min, hour, etc...) here from rawtime by the localtime() function */
	time(rawtime); // Get time into rawtime
	timeinfo = localtime(rawtime); // Get time info into timeinfo

	logFileName = "log_" + std::to_string(timeinfo->tm_year+1900) + "-" + std::to_string(timeinfo->tm_mon+1) + "-" + 
		std::to_string(timeinfo->tm_mday) + "_" + std::to_string(timeinfo->tm_hour) + "h" + std::to_string(timeinfo->tm_min) + "m" + 
		std::to_string(timeinfo->tm_sec) +"s" + ".log";

	file.open(Path + "/../log/" + logFileName, std::ios::out);
	
	if (!file)
	{
		std::cout << "Could not create log file!" << std::endl;
	}
	else
		std::cout << "Log file located at " << Path + "/../log/" + logFileName << std::endl;

	fileLoggingLevel = aFileLoggingLevel;
	consoleLoggingLevel = aConsoleLoggingLevel;

	logging_levels[0] = "CRITICAL";
	logging_levels[1] = "ERROR";
	logging_levels[2] = "WARNING";
	logging_levels[3] = "INFO";
	logging_levels[4] = "DEBUG";
	logging_levels[5] = "NOTSET";
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

	//DWORD milliseconds = aLoggingEvent->timestamp % 1000;
	//aLoggingEvent->timestamp /= 1000;
	//DWORD seconds = aLoggingEvent->timestamp % 60;
	//aLoggingEvent->timestamp /= 60;
	//DWORD minutes = aLoggingEvent->timestamp % 60;
	//aLoggingEvent->timestamp /= 60;
	//DWORD hours = aLoggingEvent->timestamp; // may exceed 24 hours.

	/*std::string output = "[" + std::to_string(hours)+":"+ std::to_string(minutes)+":"+ std::to_string(seconds)+"."+
		std::to_string(milliseconds)+"] " + "Thread " + std::to_string(id) + ": " + aLoggingEvent->log_text;*/
	std::stringstream output;
	output << "[" << std::to_string(aLoggingEvent->timestamp) << "] " << "Thread " << std::to_string(id) << " " <<
		logging_levels[aLoggingEvent->loggingLevel] << std::setw(10) << ": " + aLoggingEvent->log_text << std::endl;
	std::string output_s = output.str();
	if (aLoggingEvent->loggingLevel <= fileLoggingLevel) {
		file << output_s;
	}
	if (aLoggingEvent->loggingLevel <= consoleLoggingLevel) {
		std::cout << output_s;
	}
}
