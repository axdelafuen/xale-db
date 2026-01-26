#include "Core/ExceptionHandler.h"

namespace Xale::Core
{
	DbException::DbException(
		ExceptionCode code,
		std::string msg, 
		const char* file, 
		int line):  _code(code), 
					_msg(std::move(msg)), 
					_file(file), 
					_line(line)
	{
		_fullMessage = "DbException: " + _msg + " (File: " + _file + ", Line: " + std::to_string(_line) + ")";
	}

	const char* DbException::what() const noexcept 
	{
		return _fullMessage.c_str();
	}

	ExceptionCode DbException::getCode() const
	{
		return _code;
	}
}
