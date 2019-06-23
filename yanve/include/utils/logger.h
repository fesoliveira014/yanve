#pragma once

#include <common.h>
#include <ctime>

#include "misc.h"

#define LogInstance yanve::Logger::instance()
#define LogInfo(tag, str, ...) LogInstance.LogI(tag, yanve::utils::stringFormat(str, __VA_ARGS__))
#define LogError(tag, str, ...) LogInstance.LogE(tag, yanve::utils::stringFormat(str, __VA_ARGS__))
#define LogWarning(tag, str, ...) LogInstance.LogW(tag, yanve::utils::stringFormat(str, __VA_ARGS__))

#ifdef YANVE_DEBUG
#define LogVerbose(tag, str, ...) LogInfo(tag, str, __VA_ARGS__)
#else
#define LogVerbose(tag, str, ...)
#endif

namespace yanve
{
class Logger
{
public:
	static YANVE_API Logger& instance() {
    static Logger logger{"log.txt", true};
		return logger;
	}

	//void SetGuiConsole(GuiConsole* guiConsolePtr);

	void YANVE_API LogI(const std::string& tag, const std::string& message);
	void YANVE_API LogW(const std::string& tag, const std::string& message);
	void YANVE_API LogE(const std::string& tag, const std::string& message);

	static void YANVE_API openglCallbackFunction(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	) {
		static bool nomore = false;
		(void)source; (void)type; (void)id;
		(void)severity; (void)length; (void)userParam;
		if (!nomore) {
			if (severity == GL_DEBUG_SEVERITY_HIGH) LogError("OpenGL", "%s", message);
			else LogInfo("OpenGL", "%s", message);

			if (severity == GL_DEBUG_SEVERITY_HIGH) {
				LogError("OpenGL", "This probably is breaking something.");
				//nomore = true;
			}
		}
	}
	
protected:
	Logger(const std::string & fileName, bool console = false);
  ~Logger();

  Logger(const Logger &) = delete;
  Logger(Logger&&) = delete;
  Logger &operator= (const Logger &) = delete;
  Logger &operator= (Logger&&) = delete;

  void updateTimeString();

	void logMessage(const std::string& formated_message);

	//static Logger* _instance;
	//GuiConsole* _guiConsolePtr;

	unsigned int _numErrors;
	unsigned int _numWarning;

	std::ofstream _file;

	time_t _currentTime;
	struct tm* _now;
	std::string _timeString;

	bool _console;
};
}