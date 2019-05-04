#pragma once

class LoggingEvent;

class LoggingEventVisitor
{
public:
	virtual void Visit(LoggingEvent* aLoggingEvent) = 0;
};