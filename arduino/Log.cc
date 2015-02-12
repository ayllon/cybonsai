#include "../arduino/Log.h"

#include <HardwareSerial.h>
#include <stdlib.h>


Log::LogStream Log::debug("DEBUG   ");
Log::LogStream Log::info ("INFO    ");
Log::LogStream Log::warn ("WARNING ");
Log::LogStream Log::error("ERROR   ");
Log::LogStream Log::data ("DATA    ");
Log::LogStreamEndl Log::endl;


template <class T>
static void _internalLog(bool& newLine, const char* prefix, const T& value)
{
	if (newLine) {
		Serial.print(prefix);
		newLine = false;
	}
	Serial.print(value);
}


Log::LogStream::LogStream(const char* prefix): prefix(prefix), newLine(true)
{
}


Log::LogStream& Log::LogStream::operator << (Log::LogStreamEndl&)
{
	newLine = true;
	Serial.println();
	return	*this;
}


Log::LogStream& Log::LogStream::operator << (const char* str)
{
	_internalLog(newLine, prefix, str);
	return *this;
}


Log::LogStream& Log::LogStream::operator << (int value)
{
	_internalLog(newLine, prefix, value);
	return *this;
}


Log::LogStream& Log::LogStream::operator << (double value)
{
	_internalLog(newLine, prefix, value);
	return	*this;
}


Log::LogStream& Log::LogStream::operator << (const Printable& obj)
{
	_internalLog(newLine, prefix, obj);
	return *this;
}
