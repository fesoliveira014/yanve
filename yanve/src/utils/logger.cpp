#include <utils/logger.h>

namespace yanve
{
Logger* Logger::_instance = NULL;

Logger::Logger(const std::string & fileName, bool console) : _numErrors(0U), _numWarning(0U), _console(console)
{
	_file.open(fileName.c_str());

	if (_file.is_open()) {
		updateTimeString();

		_file << "<" + _timeString + "> - Session Initiated." << std::endl;
		_file << std::endl;

		if (_console) {
			std::cout << "<" + _timeString + "> - Session Initiated." << std::endl;
			std::cout << std::endl;
		}
	}

  //_guiConsolePtr = nullptr;
}

Logger::~Logger()
{
	if (_file.is_open()) {
		updateTimeString();
		_file << std::endl;
		_file << "<" + _timeString + "> - There were " << _numWarning << " warnings and " << _numErrors << " errors." << std::endl;
		_file << std::endl << "---------------------------------------" << std::endl;
		_file << std::endl;

		if (_console) {
			std::cout << std::endl;
			std::cout << "<" + _timeString + "> - There were " << _numWarning << " warnings and " << _numErrors << " errors." << std::endl;
			_file << std::endl << "---------------------------------------" << std::endl;
			std::cout << std::endl;
		}

		_file.close();
	}

	//if (_guiConsolePtr)
	//	_guiConsolePtr = nullptr;
}

//void Logger::SetGuiConsole(GuiConsole* guiConsolePtr)
//{
//	_guiConsolePtr = guiConsolePtr;
//}

void Logger::updateTimeString() {
	_currentTime = time(0);
	_now = localtime(&_currentTime);

	char buff[10];
	snprintf(buff, sizeof(buff), "%02d:%02d:%02d", _now->tm_hour, _now->tm_min, _now->tm_sec);

	_timeString = buff;
}

void Logger::LogI(const std::string& tag, const std::string& message)
{
	updateTimeString();
	std::string formated = "<" + _timeString + "> " + "[Info] " + tag + ": " + message;
	logMessage(formated);
}

void Logger::LogW(const std::string& tag, const std::string& message)
{
	updateTimeString();
	std::string formated = "<" + _timeString + "> " + "[Warning] " + tag + ": "+ message;
	logMessage(formated);
}

void Logger::LogE(const std::string& tag, const std::string& message)
{
	updateTimeString();
	std::string formated = "<" + _timeString + "> " + "[Error] " + tag + ": " + message;
	logMessage(formated);
}

void Logger::logMessage(const std::string& formated_message)
{
	_file << formated_message << std::endl;
	if (_console) std::cout << formated_message << std::endl;
	//if (_guiConsolePtr) _guiConsolePtr->AddLog(formated_message.c_str());
}
}