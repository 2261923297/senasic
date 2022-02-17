#pragma once
#include <iostream>

class Nocopyable {
public:
	Nocopyable() { }
	virtual ~Nocopyable() { }

private:
	Nocopyable& operator=(const Nocopyable&) { }
	Nocopyable operator=(const Nocopyable&&) noexcept { }

	Nocopyable(const Nocopyable&) { }
	Nocopyable(const Nocopyable&&) noexcept { }
}; // class Nocopyable


class HandleError
{
public:
	HandleError() { }
	virtual ~HandleError() { }
protected:
	virtual void handle() = 0;
private:

}; // class HandleError

class LogError : public HandleError {
public:
	LogError() { }
	void log(const char* msg, std::ostream& out = std::cout) { 
		out << msg; 
		handle();
	}
protected:
	virtual void handle() = 0;
private:

}; // class LogError