#pragma once
#include <iostream>

namespace tt {
namespace attr {

class Nocopyable {
public:
	Nocopyable() { }
	virtual ~Nocopyable() { }

private:
	Nocopyable& operator=(const Nocopyable&) { }
	Nocopyable operator=(const Nocopyable&&) noexcept { }

	Nocopyable(const Nocopyable&) { }
	Nocopyable(const Nocopyable&&) noexcept { }

//	Nocopyable& operator=(const Nocopyable&) = delete;
	
}; // class Nocopyable

class ErrorHandler
{
public:
	ErrorHandler() { }
	virtual ~ErrorHandler() { }
protected:
	virtual void deal(void* args) = 0;
private:

}; // class HandleError

struct LogErrorArgSt {
	const char*		_msg;
	std::ostream&	_out;
}; // struct LogErrorArgSt

class LogError : public ErrorHandler {
public:
	LogError() { }
	virtual ~LogError() { }
	virtual void deal(void* args) override {
		LogErrorArgSt leas = *(LogErrorArgSt*)args;
		leas._out << leas._msg << std::endl;
	}
private:

}; // class LogError

} // namespace attr;
} // namespace tt