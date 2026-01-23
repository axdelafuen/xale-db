#include "Core/ExceptionHandler.h"

namespace Xale::Core
{
	DbException::DbException(
		ExceptionCode code,
		std::string msg, 
		const char* file, 
		int line):  code(code), 
					msg(std::move(msg)), 
					file(file), 
					line(line)
	{
		fullMessage = "DbException: " + msg + " (File: " + file + ", Line: " + std::to_string(line) + ")";
	}

	const char* DbException::what() const noexcept 
	{
		return fullMessage.c_str();
	}

	ExceptionCode DbException::getCode() const
	{
		return code;
	}
}
