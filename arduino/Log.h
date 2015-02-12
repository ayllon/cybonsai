#ifndef __LOG_H__
#define __LOG_H__

#include <Printable.h>


namespace Log {

class LogStreamEndl {};

class LogStream {
private:
	const char* prefix;
	bool newLine;

public:
	LogStream(const char* prefix);

	LogStream& operator << (LogStreamEndl&);
	LogStream& operator << (const char*);
	LogStream& operator << (int);
	LogStream& operator << (double);
	LogStream& operator << (const Printable&);
};

extern LogStreamEndl endl;

extern LogStream debug;
extern LogStream info;
extern LogStream warn;
extern LogStream error;
extern LogStream data;

};

#endif // __LOG_H__
